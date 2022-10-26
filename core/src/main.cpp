#include "main.hpp"
#include "bsp/bsp.hpp"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_log.h"
#include "gpio/gpio.hpp"
#include "webserver/webserver.hpp"
#include "wifi/access_point.hpp"

extern "C" void app_main(void)
{
    peripherals::gpio_output led { application::pins::led };
    connectivity::wifi::access_point wifi_ap {};

    auto on_click_on = [&]() { led.set(true); };
    auto on_click_off = [&]() { led.set(false); };

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    application::webserver webserver { on_click_on, on_click_off };

    while (1) {
        vTaskDelay(10);
    }
}