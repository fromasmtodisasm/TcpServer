#pragma once
#include <exception>
#include <cassert>

namespace net {
namespace sockets {

class SocketException : public virtual std::exception
{
public:
	typedef enum {
		init_error,
		invalid_socket
	}exception_code;
	exception_code code;
	SocketException(exception_code c) :
		code(c)
	{}
	virtual const char *what() const throw()
	{
		const char *msg = "programming error";
		switch (code) {
		case init_error:
			msg = "sockets init error";
			break;
		case invalid_socket:
			msg = "invalid socket";
			break;
		default:
			assert(false);
		}
		return msg;
	}
};
}
}

