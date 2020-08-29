#ifndef SRC_SOCKET_CONFIG_HPP_
#define SRC_SOCKET_CONFIG_HPP_
#include <string>
enum {
	TCP,
	UDP
};
struct socket_config {
	int port = 7783;
	int backlog = 1024;
	unsigned char type = TCP;
};

#endif /* SRC_SOCKET_CONFIG_HPP_ */