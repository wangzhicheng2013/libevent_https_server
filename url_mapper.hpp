#ifndef SRC_URL_MAPPER_HPP_
#define SRC_URL_MAPPER_HPP_
#include <string>
#include <map>
enum rest_type {
	GET,
	POST,
	PUT,
	DELETE,
	UNKNOWN_REST_TYPE
};
enum  url_type {
	V1_SENSOR_SYSTEM_INIT,
	UNKNOWN_SENSOR_SYSTEM_TYPE
};

using url_mapper_key_type = std::pair<std::string, int>;	// first -- url second -- rest_type
class url_mapper {
public:
	inline static url_mapper &get() {
		static url_mapper mapper;
		return mapper;
	}
	url_type get_type(const std::string &url, int type) {
		std::string tmp;
		auto pos = url.find('?');
		if (pos != std::string::npos) {
			tmp.assign(url, 0, pos);
		}
		else {
			tmp = url;
		}
		auto it = mapper_.find(std::make_pair(tmp, type));
		if (end(mapper_) == it) {
			return UNKNOWN_SENSOR_SYSTEM_TYPE;
		}
		return it->second;
	}
private:
	url_mapper() {
		mapper_.insert(std::make_pair(std::make_pair("/v1/sensor/system/init", POST), V1_SENSOR_SYSTEM_INIT));
	}
	virtual ~url_mapper() = default;
private:
	std::map<url_mapper_key_type, url_type>mapper_;			
};

#endif /* SRC_URL_MAPPER_HPP_ */