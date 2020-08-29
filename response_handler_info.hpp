#ifndef SRC_RESPONSE_HANDLER_INFO_HPP_
#define SRC_RESPONSE_HANDLER_INFO_HPP_
#include "http_utility.hpp"
#include "sensor_system_init_response_handler.hpp"
namespace sensor_https_server {
class response_handler_info {
public:
	static inline response_handler_info &get() {
		static response_handler_info obj;
		return obj;
	}
	void init() {
		system_init_handler();
	}
private:
	response_handler_info() = default;
	virtual ~response_handler_info() = default;
private:
	void system_init_handler() {
		http_key key(POST, V1_SENSOR_SYSTEM_INIT);
		http_utility::get().registe_handler(key, &system_init_handler_);
	}
private:
	sensor_system_init_response_handler system_init_handler_;
};
}


#endif /* SRC_RESPONSE_HANDLER_INFO_HPP_ */