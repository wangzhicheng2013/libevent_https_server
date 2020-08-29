#ifndef SRC_URL_MAPPER_HPP_
#define SRC_URL_MAPPER_HPP_
#include <string>
#include <unordered_map>
enum  url_type {
	V1_SENSOR_SYSTEM_INIT,
	UNKNOWN_SENSOR_SYSTEM_TYPE
};
class url_mapper {
public:
	inline static url_mapper &get() {
		static url_mapper mapper;
		return mapper;
	}
	url_type get_type(const std::string &url) {
		std::string tmp;
		auto pos = url.find('?');
		if (pos != std::string::npos) {
			tmp.assign(url, 0, pos);
		}
		else {
			tmp = url;
		}
		auto it = mapper_.find(tmp);
		if (end(mapper_) == it) {
			return UNKNOWN_SENSOR_SYSTEM_TYPE;
		}
		return it->second;
	}
private:
	url_mapper() {
		mapper_.insert(std::make_pair("/v1/sensor/system/init", V1_SENSOR_SYSTEM_INIT));
	}
	virtual ~url_mapper() = default;
private:
	std::unordered_map<std::string, url_type>mapper_;			// key - url
};

#endif /* SRC_URL_MAPPER_HPP_ */