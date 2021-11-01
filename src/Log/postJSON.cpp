#include "Log/postJSON.hpp"
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <iostream>
#include <optional>
#include <string>

namespace Log {
std::optional<std::string> postJSON(std::string const& host,
                                    std::string const& port,
                                    std::string const& target,
                                    std::string const& json) {
	std::string result;
	try {
		namespace beast = boost::beast;    // from <boost/beast.hpp>
		namespace http = beast::http;    // from <boost/beast/http.hpp>
		namespace net = boost::asio;    // from <boost/asio.hpp>
		using tcp = net::ip::tcp;    // from <boost/asio/ip/tcp.hpp>

		// The io_context is required for all I/O
		net::io_context ioc;

		// These objects perform our I/O
		tcp::resolver resolver(ioc);
		beast::tcp_stream stream(ioc);

		// Look up the domain name
		auto const results = resolver.resolve(host, port);

		// Make the connection on the IP address we get from a lookup
		stream.connect(results);

		// Set up an HTTP POST request message
		// HTTP 1.1
		constexpr int httpVersion = 11;
		http::request<http::string_body> req {
		    http::verb::post, target, httpVersion};
		req.set(http::field::host, host);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
		req.body() = json;
		req.set(http::field::content_type, "application/json");
		req.prepare_payload();

		// Send the HTTP request to the remote host
		http::write(stream, req);

		// This buffer is used for reading and must be persisted
		beast::flat_buffer buffer;

		// Declare a container to hold the response
		http::response<http::dynamic_body> res;

		// Receive the HTTP response
		http::read(stream, buffer, res);

		// Save the result
		result = beast::buffers_to_string(res.body().data());

		// Write the message to standard out
		std::cout << res << std::endl;

		// Gracefully close the stream
		beast::error_code ec;
		stream.socket().shutdown(tcp::socket::shutdown_both, ec);
		if (ec == net::error::eof || ec == net::ssl::error::stream_truncated) {
			// Rationale eof:
			// http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
			// Rationale stream_truncated:
			// https://github.com/boostorg/beast/issues/824
			ec = {};
		}
		if (ec) {
			throw beast::system_error {ec};
		}

		// If we get here then the connection is closed gracefully
	} catch (std::exception const& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return std::nullopt;
	}
	return result;
}

}    // namespace Log

