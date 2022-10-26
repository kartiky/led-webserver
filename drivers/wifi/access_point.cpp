#include "access_point.hpp"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "nvs.h"
#include "nvs_flash.h"

namespace connectivity::wifi {

access_point::state_e access_point::_state { access_point::state_e::NOT_INITIALISED };
wifi_init_config_t access_point::_init_config = WIFI_INIT_CONFIG_DEFAULT();
wifi_config_t access_point::_config {};

access_point::access_point()
{
    ESP_LOGI(__func__, "initialising");
    if (state_e::NOT_INITIALISED == _state) {

        esp_err_t ret = nvs_flash_init();
        if (ESP_ERR_NVS_NO_FREE_PAGES == ret || ESP_ERR_NVS_NEW_VERSION_FOUND == ret) {
            ESP_ERROR_CHECK(nvs_flash_erase());
            ESP_ERROR_CHECK(nvs_flash_init());
        }

        ESP_ERROR_CHECK(esp_netif_init());
        esp_netif_create_default_wifi_ap();

        ESP_ERROR_CHECK(esp_wifi_init(&_init_config));

        ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
            ESP_EVENT_ANY_ID,
            &event_handler,
            NULL,
            NULL));

        set_config();
        ESP_ERROR_CHECK(esp_wifi_start());

        ESP_LOGI(__func__, "initialised _ssid:%s password:%s channel:%d",
            _ssid, _password, _channel);

        _state = state_e::INITIALISED;
    } else if (state_e::ERROR == _state) {
        ESP_LOGE(__func__, "error!");
    }
}

void access_point::set_config()
{
    memcpy(_config.ap.ssid, _ssid, std::min(strlen(_ssid), sizeof(_config.ap.ssid)));
    memcpy(_config.ap.password, _password, std::min(strlen(_password), sizeof(_config.ap.password)));
    _config.ap.channel = _channel;
    _config.ap.authmode = strlen(_password) == 0 ? WIFI_AUTH_OPEN : WIFI_AUTH_WPA2_PSK;
    _config.ap.max_connection = _max_connections;
    _config.ap.pmf_cfg.required = false;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &_config));
}

void access_point::event_handler(void* arg, esp_event_base_t event_base,
    int32_t event_id, void* event_data)
{
    switch (event_id) {
    case WIFI_EVENT_AP_STACONNECTED: {
        using connected_t = wifi_event_ap_staconnected_t;
        connected_t* event = static_cast<connected_t*>(event_data);

        ESP_LOGI(__func__, MACSTR " connected AID= %d",
            MAC2STR(event->mac), event->aid);

        _state = state_e::CONNECTED;
    } break;
    case WIFI_EVENT_AP_STADISCONNECTED: {
        using disconnected_t = wifi_event_ap_stadisconnected_t;
        disconnected_t* event = static_cast<disconnected_t*>(event_data);

        ESP_LOGI(__func__, MACSTR " disconnected AID= %d",
            MAC2STR(event->mac), event->aid);

        _state = state_e::DISCONNECTED;
    } break;
    default:
        break;
    }
}

std::string access_point::mac() const
{
    return static_cast<std::string>(MACSTR);
}

}