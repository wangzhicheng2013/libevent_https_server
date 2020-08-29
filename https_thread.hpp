#ifndef SRC_HTTPS_THREAD_HPP_
#define SRC_HTTPS_THREAD_HPP_
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "socket_utility.hpp"
#include "http_utility.hpp"
class https_thread {
public:
	https_thread() {
		sock_fd = -1;
		event_base_ptr_ = nullptr;
		event_http_ptr_ = nullptr;
		ssl_ctx_ = nullptr;
	}
	https_thread(const https_thread &other) {
		if (&other != this) {
			sock_fd = other.sock_fd;
			event_base_ptr_ = other.event_base_ptr_;
			event_http_ptr_ = other.event_http_ptr_;
		}
	}
	https_thread & operator = (const https_thread &other) {
		if (&other != this) {
			sock_fd = other.sock_fd;
			event_base_ptr_ = other.event_base_ptr_;
			event_http_ptr_ = other.event_http_ptr_;
		}
		return *this;
	}
	~https_thread() {
		if (nullptr != event_base_ptr_) {
			event_base_loopexit(event_base_ptr_, nullptr);
		}
		if (nullptr != event_http_ptr_)
		{
			evhttp_free(event_http_ptr_);
			event_http_ptr_ = nullptr;
		}
		if (nullptr != event_base_ptr_) {
			event_base_free(event_base_ptr_);
			event_base_ptr_ = nullptr;
		}
		if (nullptr != ssl_ctx_) {
			SSL_CTX_free(ssl_ctx_);
			ssl_ctx_ = nullptr;
		}
	}
	bool init(int fd) {
		if (false == init_ssl()) {
			return false;
		}
		if (false == init_certs()) {
			return false;
		}
		sock_fd = fd;
		if (sock_fd < 0) {
			return false;
		}
		event_base_ptr_ = event_base_new();
		if (nullptr == event_base_ptr_) {
			return false;
		}
		event_http_ptr_ = evhttp_new(event_base_ptr_);
		if (nullptr == event_http_ptr_) {
			return false;
		}
		if (evhttp_accept_socket(event_http_ptr_, sock_fd)) {
			return false;
		}
		evhttp_set_gencb(event_http_ptr_, &https_thread::http_call_back_fun, nullptr);
		evhttp_set_bevcb(event_http_ptr_, &https_thread::bevcb, ssl_ctx_);
		return true;
	}
	inline void http_dispath_event() {
		if (event_base_ptr_ != nullptr) {
			event_base_dispatch(event_base_ptr_);
		}
	}
private:
	bool init_ssl() {
		ssl_ctx_ = SSL_CTX_new(SSLv23_server_method());
		if (!ssl_ctx_) {
			return false;
		}
		SSL_CTX_set_options(ssl_ctx_, SSL_OP_SINGLE_DH_USE | 
									  SSL_OP_SINGLE_ECDH_USE |
									  SSL_OP_NO_SSLv2);
		EC_KEY *ecdh = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
		if (!ecdh) {
			ERR_print_errors_fp(stderr);
			return false;
		}
		if (1 != SSL_CTX_set_tmp_ecdh(ssl_ctx_, ecdh)) {
			ERR_print_errors_fp(stderr);
			EC_KEY_free(ecdh);
			return false;
		}
		EC_KEY_free(ecdh);
		return true;
	}
	bool init_certs() {
		if (1 != SSL_CTX_use_certificate_chain_file(ssl_ctx_, certificate_chain)) {
			ERR_print_errors_fp(stderr);
			return false;
		}
		if (1 != SSL_CTX_use_PrivateKey_file(ssl_ctx_, private_key, SSL_FILETYPE_PEM)) {
			ERR_print_errors_fp(stderr);
			return false;
		}
		if (1 != SSL_CTX_check_private_key(ssl_ctx_)) {
			ERR_print_errors_fp(stderr);
			return false;
		}
		return true;
	}
	static void http_call_back_fun(struct evhttp_request *req, void *arg) {
		http_config config;
		http_utility::get().set_http_utility(req, config);
		http_utility::get().handle_http(config);
	}
	static struct bufferevent *bevcb(struct event_base *base, void *arg) {
		auto *ctx = static_cast<SSL_CTX *>(arg);
		struct bufferevent *r = bufferevent_openssl_socket_new(base, 
															-1,
															SSL_new(ctx),
															BUFFEREVENT_SSL_ACCEPTING,
															BEV_OPT_CLOSE_ON_FREE);
		return r;
		
	}
private:
	int sock_fd;
	event_base *event_base_ptr_;
	evhttp *event_http_ptr_;
	SSL_CTX *ssl_ctx_;
	const char *certificate_chain = "server-certificate-chain.pem";
	const char *private_key = "server-private-key.pem";
};

#endif /* SRC_HTTPS_THREAD_HPP_ */