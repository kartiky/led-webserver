#pragma once

#include "esp_event.h"
#include "esp_wifi.h"
#include <algorithm>
#include <string>

namespace connectivity::wifi {

class access_point {
public:
    //! States which the Wifi AP can be in
    enum class state_e {
        NOT_INITIALISED,
        INITIALISED,
        CONNECTED,
        DISCONNECTED,
        ERROR
    };

    //! Wifi AP constructor and initialisation
    access_point();

    //! \returns state of Wifi AP
    constexpr const state_e& get_state() { return _state; }

    //! \returns MAC address as a string
    std::string mac() const;

private:
    void set_config();

    static void event_handler(void* arg, esp_event_base_t event_base,
        int32_t event_id, void* event_data);

private:
    static wifi_init_config_t _init_config;
    static wifi_config_t _config;

    static state_e _state;

    constexpr static const char* _ssid { "esp32" };
    constexpr static const char* _password { "password" };
    constexpr static const char _channel { 11 };
    constexpr static const char _max_connections { 4 };
};
}