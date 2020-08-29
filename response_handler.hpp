#ifndef SRC_RESPONSE_HANDLER_HPP_
#define SRC_RESPONSE_HANDLER_HPP_
#include "http_config.hpp"
enum http_response_code {
	HTTP_200 = 200,
	HTTP_201,
	HTTP_404 = 404,
	HTTP_501 = 501,
	HTTP_502 = 502
};
class response_handler {
public:
	response_handler() = default;
	virtual ~response_handler() = default;
	virtual void handle_request(http_config &) = 0;
};

#endif /* SRC_RESPONSE_HANDLER_HPP_ */