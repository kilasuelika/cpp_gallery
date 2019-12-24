// BeastFileBody.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <fstream>
#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/ostream.hpp>
#include <algorithm>
#include <boost/algorithm/string/find.hpp>
#include <regex>
#include <filesystem>

namespace fs = std::filesystem;

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::beast;

int main()
{
	string main_host = "www.mm131.com";
	string sub_host = "img1.mm131.me";
	string port = "80";
	io_context ioc;
	ip::tcp::resolver resolver{ ioc };
	ip::basic_resolver_results<ip::tcp> results;
	ip::tcp::socket sockets{ ioc };
	http::request<http::string_body> req;
	http::response<http::dynamic_body> res;
	http::response<http::file_body> pic_res;
	flat_buffer buffer;

	req.method(http::verb::get);
	req.target("/pic/2194/1.jpg");
	req.set(http::field::host, sub_host);
	req.set(http::field::content_type, "text/plain");
	req.set(http::field::referer, "http://www.mm131.com/qingchun/2194.html");
	req.set(http::field::user_agent, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/76.0.3779.0 Safari/537.36");
	req.keep_alive(true);

	results = resolver.resolve(sub_host, port);
	boost::asio::connect(sockets, results.begin(), results.end());

	http::write(sockets, req);
	system::error_code ec_file;
	string filename = "test.jpeg";
	pic_res.body().open(filename.c_str(), beast::file_mode::write, ec_file);
	http::read(sockets, buffer, pic_res);
	buffer.consume(buffer.size());
	pic_res.body().close();

}
