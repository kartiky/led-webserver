set(SOURCES ./gpio/gpio.cpp)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS . gpio
                        REQUIRES driver)
