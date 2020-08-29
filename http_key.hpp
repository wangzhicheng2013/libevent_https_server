#ifndef SRC_HTTP_KEY_HPP_
#define SRC_HTTP_KEY_HPP_
#include "url_mapper.hpp"
enum rest_type {
	GET,
	POST,
	PUT,
	DELETE,
	UNKNOWN_REST_TYPE
};
class http_key {
public:
	explicit http_key(rest_type a, url_type b) : rest_type_(a), url_type_(b) {
	}
	virtual ~http_key() = default;
public:
	rest_type rest_type_;
	url_type url_type_;
};
class http_key_cmp {
public:
	inline bool operator () (const http_key &key0, const http_key &key1) {
		if (key0.rest_type_ !=  key1.rest_type_) {
			return key0.rest_type_ < key1.rest_type_;
		}
		return key0.url_type_ < key1.url_type_;
	}
};

#endif /* SRC_HTTP_KEY_HPP_ */