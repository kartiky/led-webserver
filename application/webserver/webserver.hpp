#pragma once

#include "esp_err.h"
#include "esp_event.h"
#include "esp_event_base.h"
#include "esp_http_server.h"
#include "esp_netif.h"
#include "wifi/access_point.hpp"

namespace application {
class webserver {
public:
    //! Constructor takes in two functions to execute when in \a on and \a off states.
    webserver(std::function<void(void)> on, std::function<void(void)> off);

private:
    //! Start webserver
    static httpd_handle_t start();
    //! Stop webserver
    static esp_err_t stop(httpd_handle_t server);

    //! Handler for on state
    static esp_err_t on_get_handler(httpd_req_t* req);
    //! Handler for off state
    static esp_err_t off_get_handler(httpd_req_t* req);
    //! 404 error handler
    static esp_err_t http_404_error_handler(httpd_req_t* req, httpd_err_code_t err);

    //! Webserver connect handler
    static void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
    //! Webserver disconnect handler
    static void disconnect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

private:
    static std::function<void(void)> _on;
    static std::function<void(void)> _off;

    constexpr static const httpd_uri_t _on_uri {
        .uri = "/on",
        .method = HTTP_GET,
        .handler = on_get_handler,
        .user_ctx = NULL
    };

    constexpr static const httpd_uri_t _off_uri {
        .uri = "/off",
        .method = HTTP_GET,
        .handler = off_get_handler,
        .user_ctx = NULL
    };
};
}