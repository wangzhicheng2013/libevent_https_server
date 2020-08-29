#include "https_server.hpp"
#include "response_handler_info.hpp"
using namespace sensor_https_server;
int main() {
    response_handler_info::get().init();
    https_server server;
    socket_config config;
    std::cout << server.init(config) << std::endl;

    return 0;
}