#ifndef SRC_HTTP_CONFIG_HPP_
#define SRC_HTTP_CONFIG_HPP_
#include "http_key.hpp"
class http_config {
public:
	http_config() {
		url_port = 0;
		response_code = 200;
		rest_type_ = UNKNOWN_REST_TYPE;
		url_type_ = UNKNOWN_SENSOR_SYSTEM_TYPE;
	}
	virtual ~http_config() = default;
public:
	rest_type rest_type_;
	url_type url_type_;
	int url_port;
	long response_code;
	std::string url;
	std::string url_ip;
	std::string url_path;
	std::string request_body;
	std::string response_body;
};

#endif /* SRC_HTTP_CONFIG_HPP_ */