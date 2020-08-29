#ifndef SRC_SOCKET_UTILITY_HPP_
#define SRC_SOCKET_UTILITY_HPP_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include "socket_config.hpp"
class sockect_utility {
public:
	inline static sockect_utility &get_instance() {
		static sockect_utility obj;
		return obj;
	}
private:
	sockect_utility() = default;
	virtual ~sockect_utility() = default;
public:
	int bind_socket(const socket_config &config) {
		int sock_fd = init_socket(config.type);
		if (-1 == sock_fd) {
			return -1;
		}
		struct sockaddr_in addr = {0};
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(config.port);
		if (bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			std::cerr << "socket bind failed on port = " << config.port << std::endl;
			return -1;
		}
		if (TCP == config.type) {
			if (listen(sock_fd, config.backlog) < 0) {
				std::cerr << "socket listen failed on backlog = " << config.backlog << std::endl;
				return -1;
			}
		}
		int flag = 0;
		flag = fcntl(sock_fd, F_GETFL, 0);
		if (flag < 0) {
			std::cerr << "fcntl F_GETFL on socket = " << sock_fd << " failed...!" << std::endl;
			return -1;
		}
		if (fcntl(sock_fd, F_SETFL, flag | O_NONBLOCK) < 0) {
			std::cerr << "fcntl F_SETFL non block on socket = " << sock_fd << " failed...!" << std::endl;
			return -1;
		}
		return sock_fd;
	}
	inline bool set_sock_broadcast(int sock_fd) {
		if (sock_fd < 0) {
			return false;
		}
		int so_broadcast = 1;
		return setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, (char *)&so_broadcast, sizeof(so_broadcast)) >= 0;
	}
	bool close_socket(int &sock_fd) {
		if (sock_fd >= 0) {
			close(sock_fd);
			sock_fd = -1;
			return true;
		}
		return false;
	}
private:
	int init_socket(unsigned char type) {
		int sock_fd = -1;
		switch (type) {
		case UDP:
			sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
			if (false == set_sock_broadcast(sock_fd)) {
				return -1;
			}
			return sock_fd;
			break;
		case TCP:
			sock_fd = socket(AF_INET, SOCK_STREAM, 0);
			break;
		default:
			std::cerr << "unknown socket type...!" << std::endl;
			return -1;
		}
		if (sock_fd < 0) {
			std::cerr << "socket fd create failed...!" << std::endl;
			return -1;
		}
		int on = 1;
		if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
			std::cerr << "set socket to resue failed...!" << std::endl;
			return -1;
		}
		if (setsockopt(sock_fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof(on)) < 0) {
			std::cerr << "set socket to keep alive failed...!" << std::endl;
			return -1;
		}
		return sock_fd;
	}
};

#endif /* SRC_SOCKET_UTILITY_HPP_ */