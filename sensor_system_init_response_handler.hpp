#ifndef SRC_SENSOR_SYSTEM_INIT_RESPONSE_HANDLER_HPP_
#define SRC_SENSOR_SYSTEM_INIT_RESPONSE_HANDLER_HPP_
#include <iostream>
#include "response_handler.hpp"
namespace sensor_https_server {
class sensor_system_init_response_handler: public response_handler {
public:
	sensor_system_init_response_handler() = default;
	virtual ~sensor_system_init_response_handler() = default;
	virtual void handle_request(http_config &config) override {
		std::cout << "sensor_system_init_response_handler." << std::endl;
		std::cout << config.request_body << std::endl;
		config.response_body = "ok";
		config.response_code = HTTP_200;
	}
};
}

#endif /* SRC_SENSOR_SYSTEM_INIT_RESPONSE_HANDLER_HPP_ */