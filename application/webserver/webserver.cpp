#include "webserver.hpp"
#include "esp_err.h"
#include "esp_log.h"

namespace application {

std::function<void(void)> webserver::_on;
std::function<void(void)> webserver::_off;

webserver::webserver(std::function<void(void)> on, std::function<void(void)> off)
{
    _on = on;
    _off = off;
}

httpd_handle_t webserver::start(void)
{
    httpd_handle_t _server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    ESP_LOGI(__func__, "Starting server");

    esp_err_t ret = httpd_start(&_server, &config);
    if (ESP_OK != ret) {
        ESP_LOGI(__func__, "Error starting server!");
        return NULL;
    }
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_AP_STAIPASSIGNED, &connect_handler, &_server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &_server));

    ESP_LOGI(__func__, "Registering URI handlers");
    httpd_register_uri_handler(_server, &_on_uri);
    httpd_register_uri_handler(_server, &_off_uri);

    return _server;
}

esp_err_t webserver::stop(httpd_handle_t server)
{
    // Stop the httpd _server
    return httpd_stop(server);
}

void webserver::disconnect_handler(void* arg, esp_event_base_t event_base,
    int32_t event_id, void* event_data)
{
    httpd_handle_t* _server = static_cast<httpd_handle_t*>(arg);
    if (*_server) {
        if (stop(*_server) == ESP_OK) {
            *_server = NULL;
        } else {
            ESP_LOGE(__func__, "Failed to stop https server");
        }
    }
}

void webserver::connect_handler(void* arg, esp_event_base_t event_base,
    int32_t event_id, void* event_data)
{
    httpd_handle_t* _server = static_cast<httpd_handle_t*>(arg);
    if (*_server == NULL) {
        *_server = start();
    }
}

esp_err_t webserver::on_get_handler(httpd_req_t* req)
{
    _on();
    httpd_resp_set_type(req, "text/html");
    esp_err_t error = httpd_resp_send(req, "<!DOCTYPE html><html><head><style>.button {background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer;}</style></head><body><h2>LED Webserver</h2><p>LED OFF... Click button to turn on.</p><button class=\"button\" onclick=\"window.location.href='/on'\"> ON </button></body></html>", HTTPD_RESP_USE_STRLEN);

    if (ESP_OK != error) {
        ESP_LOGI(__func__, "Error sending response: %d", error);
    } else {
        ESP_LOGI(__func__, "Response sent successfully");
    }
    return error;
}

esp_err_t webserver::off_get_handler(httpd_req_t* req)
{
    _off();
    httpd_resp_set_type(req, "text/html");
    esp_err_t error = httpd_resp_send(req, "<!DOCTYPE html><html><head><style>.button {background-color: #000000; border: none; color: white; padding: 15px 32px; text-align: center;  text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer;}</style></head><body><h2>LED Webserver</h2><p>LED ON... Click button to turn off.</p><button class=\"button\"onclick=\"window.location.href='/off'\"> OFF </button></body></html>", HTTPD_RESP_USE_STRLEN);

    if (ESP_OK != error) {
        ESP_LOGI(__func__, "Error sending response: %d", error);
    } else {
        ESP_LOGI(__func__, "Response sent successfully");
    }
    return error;
}

esp_err_t webserver::http_404_error_handler(httpd_req_t* req, httpd_err_code_t err)
{
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Not found!");
    return ESP_FAIL;
}
}
