#include "../../../include/webpp/interfaces/cgi.h"

#include "../../../include/webpp/utils/casts.h"
#include "../../../include/webpp/utils/strings.h"

#include <cctype>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <sstream>

using namespace webpp;

// fixme: implement these too:
//    AUTH_PASSWORD
//    AUTH_TYPE
//    AUTH_USER
//    CERT_COOKIE
//    CERT_FLAGS
//    CERT_ISSUER
//    CERT_KEYSIZE
//    CERT_SECRETKEYSIZE
//    CERT_SERIALNUMBER
//    CERT_SERVER_ISSUER
//    CERT_SERVER_SUBJECT
//    CERT_SUBJECT
//    CF_TEMPLATE_PATH
//    CONTENT_LENGTH
//    CONTENT_TYPE
//    CONTEXT_PATH
//    GATEWAY_INTERFACE
//    HTTPS
//    HTTPS_KEYSIZE
//    HTTPS_SECRETKEYSIZE
//    HTTPS_SERVER_ISSUER
//    HTTPS_SERVER_SUBJECT
//    HTTP_ACCEPT
//    HTTP_ACCEPT_ENCODING
//    HTTP_ACCEPT_LANGUAGE
//    HTTP_CONNECTION
//    HTTP_COOKIE
//    HTTP_HOST
//    HTTP_REFERER
//    HTTP_USER_AGENT
//    QUERY_STRING
//    REMOTE_ADDR
//    REMOTE_HOST
//    REMOTE_USER
//    REQUEST_METHOD
//    SCRIPT_NAME
//    SERVER_NAME
//    SERVER_PORT
//    SERVER_PORT_SECURE
//    SERVER_PROTOCOL
//    SERVER_SOFTWARE
//    WEB_SERVER_API

// TODO: use GetEnvironmentVariableA for Windows operating system
extern char** environ;

cgi::cgi() noexcept : basic_interface() {
    // I'm not using C here; so why should I pay for it!
    // And also the user should not use cin and cout. so ...
    std::ios::sync_with_stdio(false);
}

std::string_view cgi::header(std::string name) noexcept {
    // fixme: check if this is all we have to do or we have to do more too:
    std::transform(name.begin(), name.end(), name.begin(), [](auto const& c) {
        if (c == '-')
            return '_';
        return static_cast<char>(std::toupper(c));
    });

    name.insert(0, "HTTP_");
    return env(name.c_str());
}

// void cgi::run(router_t<cgi>& _router) noexcept {
//    auto self = std::make_shared<cgi>(this);
//    webpp::request<webpp::cgi> req(self);
//    auto res = _router.run(req);
//    std::ios_base::sync_with_stdio(false); // TODO: write tests for this
//    part for (auto const& [attr, value] : res.headers()) {
//        std::cout << attr << ": " << value << "\r\n";
//    }
//    std::cout << "\r\n" << res.body();
//}

std::streamsize cgi::read(char* data, std::streamsize length) noexcept {
    std::cin.read(data, length);
    return std::cin.gcount();
}

void cgi::write(std::ostream& stream) noexcept {
    // TODO: check if you need to ignore the input or not

    // I think o-stream is not readable so we cannot do this:
    // https://stackoverflow.com/questions/15629886/how-to-write-ostringstream-directly-to-cout
    std::cout << stream.rdbuf(); // TODO: test this, I don't trust myself :)
}

void cgi::write(char const* data, std::streamsize length) noexcept {
    std::cout.write(data, length);
}

std::string_view cgi::env(char const* const key) noexcept {
    if (auto value = getenv(key))
        return value;
    return {};
}

std::string_view cgi::headers() noexcept {
    // we can do this only in CGI, we have to come up with new ways for
    // long-running protocols:
    static std::string headers_cache;
    if (headers_cache.empty()) {
        // TODO: this code won't work on windows. Change when you are worried
        // about windows
        for (auto it = ::environ; *it; it++) {
            std::string_view h{*it};
            if (starts_with(h, "HTTP_")) {
                headers_cache.append(h.substr(5));
                // FIXME: decide if you need to convert _ to - or not.
            }
        }
    }
    return headers_cache;
}

std::string_view cgi::body() noexcept {
    // again, we can do this only in cgi protocol not in other interfaces:
    static std::string body_cache;
    if (body_cache.empty()) {
        if (auto content_length_str = env("CONTENT_LENGTH");
            !content_length_str.empty()) {
            // now we know how much content the user is going to send
            // so we just create a buffer with that size
            auto content_length = to_uint(content_length_str);

            char* buffer = new char[content_length];
            std::cin.rdbuf()->pubsetbuf(buffer, sizeof(buffer));
        } else {
            // we don't know how much the user is going to send. so we use a
            // small size buffer:

            // TODO: add something here
        }
    }
    return body_cache;
}

void cgi::operator()() noexcept {
    webpp::request_t<cgi> req;
    auto                  res = router(req);
    res.calculate_default_headers();
    auto header_str = res.header.str();
    auto str        = res.body.str();

    // From RFC: https://tools.ietf.org/html/rfc3875
    // Send status code:
    // Status         = "Status:" status-code SP reason-phrase NL
    // status-code    = "200" | "302" | "400" | "501" | extension-code
    // extension-code = 3digit
    // reason-phrase  = *TEXT

    std::stringstream status_line;
    status_line << "Status: " << res.header.status_code() << " "
                << status_reason_phrase(res.header.status_code()) << "\r\n";

    auto _status_line_str = status_line.str();
    write(_status_line_str.data(), _status_line_str.size());

    write(header_str.data(), header_str.size());
    write("\r\n", 2);
    write(str.data(), str.size());
}
