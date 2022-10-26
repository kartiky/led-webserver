set(SOURCES ./gpio/gpio.cpp ./wifi/access_point.cpp)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS . gpio wifi
                        REQUIRES driver esp_wifi nvs_flash esp_event esp_netif lwip)
