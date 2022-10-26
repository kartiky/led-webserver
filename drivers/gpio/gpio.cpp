#include "gpio.hpp"

namespace peripherals {

gpio::gpio(const pin_t pin, const config_t& config, const bool invert)
    : _pin { pin }
    , _config { config }
    , _invert { invert }
{
    gpio_config(&_config);
    _status |= gpio_config(&_config);
}

gpio_output::gpio_output(const pin_t pin, const bool invert)
    : gpio {
        pin,
        config_t { .pin_bit_mask = static_cast<uint64_t>(1) << pin,
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_ENABLE,
            .intr_type = GPIO_INTR_DISABLE },
        invert
    }
{
    if (_status == ESP_OK) {
        // init gpio to "off" state
        _status |= set(_invert);
    }
}

gpio_input::gpio_input(pin_t pin, int_type_t int_type, const bool invert)
    : gpio {
        pin,
        config_t { .pin_bit_mask = static_cast<uint64_t>(1) << pin,
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = int_type },
        invert
    }
{
}

esp_err_t gpio_output::set(const bool state)
{
    _state = state;
    return gpio_set_level(_pin, _invert ? !_state : _state);
}

esp_err_t gpio_output::toggle()
{
    _state ^= _state;
    return set(_state);
}

bool gpio_input::state() const
{
    return static_cast<bool>(gpio_get_level(_pin));
}

}