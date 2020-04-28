// Created by moisrex on 11/1/19.

#ifndef WEBPP_VALVE_H
#define WEBPP_VALVE_H

#include "../http/request.h"

#include <type_traits>
#include <utility>

namespace webpp::valves {

    struct empty_condition {
        template <typename RequestType>
        [[nodiscard]] constexpr bool
        operator()(RequestType const& /* req */) const noexcept {
            return true;
        }
    };

    enum class logical_operators { AND, OR, XOR };

    template <typename ValveType, typename NextValve>
    struct basic_valve : public ValveType {
        using next_valve_type =
          std::remove_reference_t<std::remove_cv_t<NextValve>>;

        next_valve_type   next;
        logical_operators op;

        constexpr basic_valve(ValveType&& super, next_valve_type&& _next,
                              logical_operators op) noexcept
          : ValveType(std::move(super)),
            next(std::move(_next)),
            op(op) {
        }

        constexpr basic_valve(ValveType const&       super,
                              const next_valve_type& _next,
                              logical_operators      op) noexcept
          : ValveType(super),
            next(_next),
            op(op) {
        }

        constexpr basic_valve(ValveType const& super, next_valve_type&& _next,
                              logical_operators op) noexcept
          : ValveType(super),
            next(std::move(_next)),
            op(op) {
        }

        constexpr basic_valve(ValveType&& super, next_valve_type const& _next,
                              logical_operators op) noexcept
          : ValveType(std::move(super)),
            next(_next),
            op(op) {
        }

        constexpr basic_valve(basic_valve const& v) noexcept = default;
        constexpr basic_valve(basic_valve&& v) noexcept      = default;

        constexpr basic_valve&
                               operator=(basic_valve const& v) noexcept = default;
        constexpr basic_valve& operator=(basic_valve&&) noexcept = default;
    };

    template <typename ValveType>
    struct basic_valve<ValveType, void> : public ValveType {
        using ValveType::ValveType;
    };

    template <typename ValveType, typename NextValve = void>
    class valve : public basic_valve<ValveType, NextValve> {
      public:
        using type            = ValveType;
        using next_valve_type = NextValve;

        using basic_valve<ValveType, NextValve>::basic_valve;
        constexpr valve() noexcept = default;

        /**
         * @tparam NewValveType
         * @param valve
         */
        template <typename NewValve>
        [[nodiscard]] constexpr auto
        set_next(NewValve&& v, logical_operators the_op) const noexcept {
            if constexpr (std::is_void_v<next_valve_type>) {
                // this part will only execute when the "next_valve_type" is
                // void

                // the first way (A<X, void> and B<Y, void> === A<X, B<Y, void>>
                return valve<ValveType, NewValve>(
                  *this, std::forward<NewValve>(v), std::move(the_op));
            } else {
                // this means this function has a "next" valve already,
                // so it goes to the next's next valve
                // this way we recursively create a valve type and return it.
                auto n = basic_valve<ValveType, NextValve>::next.set_next(
                  std::forward<NewValve>(v), the_op);
                return valve<ValveType, decltype(n)>{*this, n, this->op};
            }
        }

        template <typename NewValve>
        [[nodiscard]] constexpr auto operator&&(NewValve&& v) const noexcept {
            if constexpr (std::is_convertible_v<
                            typename std::decay_t<NewValve>::type,
                            empty_condition>) {
                // AnyValve && EmptyValve == AnyValve
                return *this;
            } else {
                return set_next(std::forward<NewValve>(v),
                                logical_operators::AND);
            }
        }

        template <typename NewValve>
        [[nodiscard]] constexpr auto operator&(NewValve&& v) const noexcept {
            if constexpr (std::is_convertible_v<
                            typename std::decay_t<NewValve>::type,
                            empty_condition>) {
                // AnyValve && EmptyValve == AnyValve
                return *this;
            } else {
                return set_next(std::forward<NewValve>(v),
                                logical_operators::AND);
            }
        }

        template <typename NewValve>
        [[nodiscard]] constexpr auto operator||(NewValve&& v) const noexcept {
            if constexpr (std::is_convertible_v<
                            typename std::decay_t<NewValve>::type,
                            empty_condition>) {
                // AnyValve || EmptyValve == EmptyValve
                return valve<empty_condition>{};
            } else {
                return set_next(std::forward<NewValve>(v),
                                logical_operators::OR);
            }
        }

        template <typename NewValve>
        [[nodiscard]] constexpr auto operator|(NewValve&& v) const noexcept {
            if constexpr (std::is_convertible_v<
                            typename std::decay_t<NewValve>::type,
                            empty_condition>) {
                // AnyValve || EmptyValve == EmptyValve
                return valve<empty_condition>{};
            } else {
                return set_next(std::forward<NewValve>(v),
                                logical_operators::OR);
            }
        }

        template <typename NewValve>
        [[nodiscard]] constexpr auto operator^(NewValve&& v) const noexcept {
            return set_next(std::forward<NewValve>(v), logical_operators::XOR);
        }

        template <typename RequestType>
        [[nodiscard]] bool operator()(RequestType const& req) const noexcept {
            if constexpr (std::is_void_v<NextValve>) {
                return ValveType::operator()(req);
            } else {
                switch (basic_valve<ValveType, NextValve>::op) {
                    case logical_operators::AND:
                        return ValveType::operator()(req) &&
                               basic_valve<ValveType, NextValve>::next.
                               operator()(req);
                    case logical_operators::OR:
                        return ValveType::operator()(req) ||
                               basic_valve<ValveType, NextValve>::next.
                               operator()(req);
                    case logical_operators::XOR:
                        return ValveType::operator()(req) ^
                               basic_valve<ValveType, NextValve>::next.
                               operator()(req);
                    default: return false;
                }
            }
        }
    };

    /**
     * Dynamic version of the above valve class.
     * todo: use a better name maybe?
     */
    template <typename Traits, typename Interface>
    struct dynamic_valve {
        static_assert(
          is_traits_v<Traits>,
          "The specified template parameter is not a valid traits type.");

        using traits    = Traits;
        using interface = Interface;
        using req_t     = request_t<traits, interface> const&;

      protected:
        std::function<bool(req_t)> func;

      public:
        dynamic_valve() noexcept                = default;
        dynamic_valve(dynamic_valve const&)     = default;
        dynamic_valve(dynamic_valve&&) noexcept = default;
        dynamic_valve& operator=(dynamic_valve&&) noexcept = default;
        dynamic_valve& operator=(dynamic_valve const&) noexcept = default;

        template <typename NewValve>
        dynamic_valve&
        set_next(NewValve&& v, logical_operators const& op) noexcept(
          std::is_nothrow_invocable_v<NewValve, req_t>) {
            switch (op) {
                case logical_operators::AND:
                    return operator&&(std::forward<NewValve>(v));
                case logical_operators::OR:
                    return operator||(std::forward<NewValve>(v));
                case logical_operators::XOR:
                    return operator^(std::forward<NewValve>(v));
            }
            return *this;
        }

        template <typename NewValve>
        dynamic_valve& operator&&(NewValve&& v) noexcept(
          std::is_nothrow_invocable_v<NewValve, req_t>) {
            func = [=, *this](req_t req) {
                if (func)
                    return func(req) && v(req);
                return v(req);
            };
            return *this;
        }

        template <typename NewValve>
        dynamic_valve& operator||(NewValve&& v) noexcept(
          std::is_nothrow_invocable_v<NewValve, req_t>) {
            func = [=, *this](req_t req) {
                if (func)
                    return func(req) || v(req);
                return v(req);
            };
            return *this;
        }

        template <typename NewValve>
        dynamic_valve& operator&(NewValve&& v) noexcept(
          std::is_nothrow_invocable_v<NewValve, req_t>) {
            return operator&&(v);
        }

        template <typename NewValve>
        dynamic_valve& operator|(NewValve&& v) noexcept(
          std::is_nothrow_invocable_v<NewValve, req_t>) {
            return operator||(v);
        }

        template <typename NewValve>
        dynamic_valve& operator^(NewValve&& v) noexcept(
          std::is_nothrow_invocable_v<NewValve, req_t>) {
            func = [=, *this](req_t req) {
                bool one = func ? func(req) : true;
                bool two = v(req);
                return (one && !two) || (!one && two);
            };
            return *this;
        }

        [[nodiscard]] bool operator()(req_t req) const
          noexcept(std::is_nothrow_invocable_v<decltype(func), req_t>) {
            return func(req);
        }
    };

    struct empty_t : public valve<empty_condition> {
        using valve<empty_condition>::valve;

        /**
         * EmptyValve || AnyValve == EmptyValve
         */
        template <typename NewValve>
        [[nodiscard]] constexpr auto
        operator||(NewValve&& /* v */) const noexcept {
            return empty_t{};
        }

        /**
         * EmptyValve || AnyValve == EmptyValve
         */
        template <typename NewValve>
        [[nodiscard]] constexpr auto
        operator|(NewValve&& /* v */) const noexcept {
            return empty_t{};
        }

        /**
         *  EmptyValve && AnyValve == AnyValve
         */
        template <typename NewValve>
        [[nodiscard]] constexpr auto operator&&(NewValve&& v) const noexcept {
            return std::forward<NewValve>(v);
        }

        /**
         * EmptyValve && AnyValve == AnyValve
         */
        template <typename NewValve>
        [[nodiscard]] constexpr auto operator&(NewValve&& v) const noexcept {
            return std::forward<NewValve>(v);
        }
    };
    constexpr empty_t empty;

} // namespace webpp::valves

#endif // WEBPP_VALVE_H
