#ifndef SRC_HTTPS_SERVER_HPP_
#define SRC_HTTPS_SERVER_HPP_
#include <signal.h>
#include <vector>
#include <thread>
#include <functional>
#include "https_thread.hpp"
class https_server {
public:
	https_server() = default;
	~https_server() {
		for (auto &it : threads_) {
			if (it.joinable()) {
				it.join();
			}
		}
	}
	bool init(const socket_config &config, int network_size = std::thread::hardware_concurrency()) {
		signal(SIGPIPE, SIG_IGN);
		load_ssl();
		if (network_size <= 0 || network_size > NETWORKCAPACITY) {
			return false;
		}
		int sock_fd = sockect_utility::get_instance().bind_socket(config);
		if (sock_fd < 0) {
			return false;
		}
		https_threads_.resize(network_size);
		for (auto &it : https_threads_) {
			if (!it.init(sock_fd)) {
				return false;
			}
		}
		threads_.resize(network_size);
		for (int i = 0;i < network_size;i++) {
			try {
				threads_[i] = std::thread(std::bind(&https_server::start_https_thread, this, i));
			}
			catch (...) {
				return false;
			}
		}
		return true;
	}
	void load_ssl() {
		SSL_library_init();
		SSL_load_error_strings();
		OpenSSL_add_all_algorithms();
	}
private:
	inline void start_https_thread(int index) {
		https_threads_[index].http_dispath_event();
	}
private:
	static const int NETWORKCAPACITY = 128;
private:
	std::vector<https_thread>https_threads_;
	std::vector<std::thread>threads_;
};




#endif /* SRC_HTTPS_SERVER_HPP_ */