#ifndef WEBPP_VALVES_METHODS_H
#define WEBPP_VALVES_METHODS_H

#include "route.h"

namespace webpp::valves {

    struct method_condition {
      private:
        std::string_view method_string;

      public:
        constexpr method_condition(std::string_view str) noexcept
          : method_string(std::move(str)) {
        }

        constexpr method_condition() noexcept = default;

        template <typename RequestType>
        [[nodiscard]] bool operator()(RequestType const& req) const noexcept {
            return req.request_method() == method_string;
        }
    };

    //    struct method : public routes::route<method_condition> {
    //        using routes::route<method_condition>::valve;
    //    };
    //
    //    constexpr auto get  = method("GET");
    //    constexpr auto post = method("POST");
    //    constexpr auto head = method("HEAD");
    //    constexpr auto put  = method("PUT");
    // TODO: add more methods
} // namespace webpp::valves


#endif // WEBPP_VALVES_METHODS_H
