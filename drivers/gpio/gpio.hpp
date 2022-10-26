#pragma once

#include "driver/gpio.h"

namespace peripherals {
typedef gpio_config_t config_t;
typedef gpio_num_t pin_t;
typedef gpio_pullup_t pullup_t;
typedef gpio_pulldown_t pulldown_t;
typedef gpio_int_type_t int_type_t;

class gpio {
public:
    //! Initialise \a gpio base class with \a pin \a config and \a invert state
    gpio(const pin_t pin, const config_t& config, const bool invert = false);

    //! \returns state
    //! Implement in derived class
    virtual bool state() const = 0;

protected:
    const gpio_num_t _pin;
    const config_t _config;
    const bool _invert { false };
    esp_err_t _status { ESP_OK };
};

class gpio_output : public gpio {
public:
    //! Initialise \a gpio_output with \a pin and \a invert state
    gpio_output(const pin_t pin, const bool invert = false);

    //! set gpio to \a state
    esp_err_t set(const bool state);

    //! toggles the gpio
    esp_err_t toggle();

    //! \returns _state of the gpio
    bool state() const final { return _state; }

private:
    bool _state { false };
};

class gpio_input : public gpio {
public:
    //! Initialise \a gpio_input with \a pin and \a invert state
    gpio_input(pin_t pin, int_type_t int_type, const bool invert = false);

    //! \returns _state of the gpio
    bool state() const final;
};

} // namespace gpio
