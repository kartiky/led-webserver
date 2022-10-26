#pragma once

#include "driver/gpio.h"
#include "gpio/gpio.hpp"

namespace application::pins {
constexpr static const peripherals::pin_t led { GPIO_NUM_2 };
}