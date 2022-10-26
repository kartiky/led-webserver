set(SOURCES ./webserver/webserver.cpp)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS . bsp webserver
                        REQUIRES drivers esp_https_server)
