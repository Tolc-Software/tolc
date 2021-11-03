#include <TestUtil/startServer.hpp>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;    // from <boost/beast.hpp>
namespace http = beast::http;    // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;    // from <boost/asio/ip/tcp.hpp>

class http_connection : public std::enable_shared_from_this<http_connection> {
public:
	explicit http_connection(tcp::socket socket, std::string& message)
	    : m_socket(std::move(socket)), m_message(message) {}

	// Initiate the asynchronous operations associated with the connection.
	void start() {
		read_request();
		check_deadline();
	}

private:
	// The socket for the currently connected client.
	tcp::socket m_socket;

	std::string& m_message;

	// The buffer for performing reads.
	beast::flat_buffer m_buffer {8192};

	// The request message.
	http::request<http::dynamic_body> m_request;

	// The response message.
	http::response<http::dynamic_body> m_response;

	// The timer for putting a deadline on connection processing.
	net::steady_timer m_deadline {m_socket.get_executor(),
	                              std::chrono::seconds(20)};

	// Asynchronously receive a complete request message.
	void read_request() {
		auto self = shared_from_this();

		http::async_read(
		    m_socket,
		    m_buffer,
		    m_request,
		    [self](beast::error_code ec, std::size_t bytes_transferred) {
			    boost::ignore_unused(bytes_transferred);
			    if (!ec)
				    self->process_request();
		    });
	}

	// Determine what needs to be done with the request message.
	void process_request() {
		m_response.version(m_request.version());
		m_response.keep_alive(false);

		switch (m_request.method()) {
			case http::verb::post:
				m_response.result(http::status::ok);
				m_response.set(http::field::server, "Beast");
				m_response.set(http::field::content_type, "text/plain");
				m_message = beast::buffers_to_string(m_request.body().data());
				beast::ostream(m_response.body())
				    << "Body recieved: " << m_message << "\r\n";
				break;

			default:
				// We return responses indicating an error if
				// we do not recognize the request method.
				m_response.result(http::status::bad_request);
				m_response.set(http::field::content_type, "text/plain");
				beast::ostream(m_response.body())
				    << "Invalid request-method '"
				    << std::string(m_request.method_string()) << "'";
				break;
		}

		write_response();
	}

	// Construct a response message based on the program state.
	void create_response() {
		if (m_request.target() == "/count") {
			m_response.set(http::field::content_type, "text/html");
			beast::ostream(m_response.body())
			    << "<html>\n"
			    << "<head><title>Request count</title></head>\n"
			    << "<body>\n"
			    << "<h1>Request count</h1>\n"
			    << "</body>\n"
			    << "</html>\n";
		} else if (m_request.target() == "/time") {
			m_response.set(http::field::content_type, "text/html");
			beast::ostream(m_response.body())
			    << "<html>\n"
			    << "<head><title>Current time</title></head>\n"
			    << "<body>\n"
			    << "<h1>Current time</h1>\n"
			    << "</body>\n"
			    << "</html>\n";
		} else {
			m_response.result(http::status::not_found);
			m_response.set(http::field::content_type, "text/plain");
			beast::ostream(m_response.body()) << "File not found\r\n";
		}
	}

	// Asynchronously transmit the response message.
	void write_response() {
		auto self = shared_from_this();

		m_response.content_length(m_response.body().size());

		http::async_write(
		    m_socket, m_response, [self](beast::error_code ec, std::size_t) {
			    self->m_socket.shutdown(tcp::socket::shutdown_send, ec);
			    self->m_deadline.cancel();
		    });
	}

	// Check whether we have spent enough time on this connection.
	void check_deadline() {
		auto self = shared_from_this();

		m_deadline.async_wait([self](beast::error_code ec) {
			if (!ec) {
				// Close socket to cancel any outstanding operation.
				self->m_socket.close(ec);
			}
		});
	}
};

// "Loop" forever accepting new connections.
void http_server(tcp::acceptor& acceptor,
                 tcp::socket& socket,
                 std::string& message) {
	acceptor.async_accept(socket, [&](beast::error_code ec) {
		if (!ec) {
			std::make_shared<http_connection>(std::move(socket), message)
			    ->start();
		}
		// Uncomment to loop forever
		// http_server(acceptor, socket, message);
	});
}

namespace TestUtil {
std::string startServer(std::string const& ipAddress, unsigned short port) {
	try {
		auto const address = net::ip::make_address(ipAddress.c_str());

		net::io_context ioc {1};

		tcp::acceptor acceptor {ioc, {address, port}};
		tcp::socket socket {ioc};
		std::string message = "";
		http_server(acceptor, socket, message);

		ioc.run();
		return message;
	} catch (std::exception const& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return "";
	}
}
}    // namespace TestUtil
