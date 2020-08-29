#ifndef SRC_HTTP_UTILITY_HPP_
#define SRC_HTTP_UTILITY_HPP_
#include <map>
#include "event.h"
#include "evhttp.h"
#include "event2/listener.h"
#include "event2/bufferevent.h"
#include "event2/bufferevent_ssl.h"
#include "response_handler.hpp"
class http_utility {
public:
	static inline http_utility &get() {
		static http_utility obj;
		return obj;
	}
	inline void set_http_utility(struct evhttp_request *req, http_config &config) {
		http_req_ = req;
		if (nullptr == http_req_) {
			return;
		}
		get_rest_type(config);
		get_url(config);
		get_url_type(config);
		get_request_body(config);
	}
	inline void registe_handler(const http_key &key, response_handler *handler) {
		mapper_.insert(std::make_pair(key, handler));
	}
	void get_rest_type(http_config &config) {
		switch (evhttp_request_get_command(http_req_)) {
		case EVHTTP_REQ_GET:
			config.rest_type_ = GET;
			break;
		case EVHTTP_REQ_POST:
			config.rest_type_ = POST;
			break;
		case EVHTTP_REQ_PUT:
			config.rest_type_ = PUT;
			break;
		case EVHTTP_REQ_DELETE:
			config.rest_type_ = DELETE;
			break;
		default:
			config.rest_type_ = UNKNOWN_REST_TYPE;
		}
	}
	void get_url(http_config &config) {
		const char *p = evhttp_request_uri(http_req_);
		if (nullptr == p) {
			return;
		}
		config.url = p;
		struct evhttp_uri *decoded = evhttp_uri_parse(p);
		if (nullptr == decoded) {
			return;
		}
		const char *path = evhttp_uri_get_path(decoded);
		if (nullptr == path || 0 == path[0]) {
			config.url_path = "/";
		}
		else {
			config.url_path = path;
		}
		evhttp_uri_free(decoded);
	}
	inline void get_url_type(http_config &config) {
		config.url_type_ = url_mapper::get().get_type(config.url);
	}
	inline void get_request_body(http_config &config) {
		const char *body = (const char *)EVBUFFER_DATA(http_req_->input_buffer);
		if (body != nullptr) {
			config.request_body = body;
		}
	}
	void handle_request(http_config &config) {
		http_key httpKey(config.rest_type_, config.url_type_);
		auto it = mapper_.find(httpKey);
		if (end(mapper_) == it) {
			return;
		}
		it->second->handle_request(config);
	}
	void reply_client(http_config &config) {
		struct evbuffer *buf = evbuffer_new();
		evhttp_add_header(http_req_->output_headers, "Content-Type", "application/json;charset=UTF-8");
		evbuffer_add_printf(buf, "%s", config.response_body.c_str());
		evhttp_send_reply(http_req_, config.response_code, "return", buf);
		evbuffer_free(buf);
	}
	void handle_http(http_config &config) {
		handle_request(config);
		reply_client(config);
	}
private:
	http_utility() {
		http_req_ = nullptr;
	}
	virtual ~http_utility() {
		http_req_ = nullptr;
	}
private:
	struct evhttp_request *http_req_;
	std::map<http_key, response_handler *, http_key_cmp>mapper_;
};

#endif /* SRC_HTTP_UTILITY_HPP_ */