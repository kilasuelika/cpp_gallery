#include <string>
#include <fstream>
#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/core/ostream.hpp>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <algorithm>
#include <boost/algorithm/string/find.hpp>
#include <regex>
#include <filesystem>

namespace fs = std::filesystem;

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::beast;


//Connection.
io_context ioc;
ssl::context ctx(ssl::context::tlsv12_client);

ip::tcp::resolver resolver{ ioc };
ip::basic_resolver_results<ip::tcp> results;
ip::tcp::socket sockets{ ioc };
http::request<http::string_body> req;
http::response<http::file_body> res;
http::response<http::file_body> pic_res;
flat_buffer buffer;

//Website.
string host = "https://query1.finance.yahoo.com";
string port = "443";
string header = "/v7/finance/download/";
string tail = "?period1=1278172800&period2=1562169600&interval=1d&events=history&crumb=stq3/vnL.gp";

//User data.
string folder = "files/";
vector<string> code_list = { "ADS.DE", "ALV.DE", "BAS.DE", "BAYN.DE", "BEI.DE", "BMW.DE",
		"CON.DE", "1COV.DE", "DAI.DE", "DBK.DE", "DB1.DE", "LHA.DE",
		"DPW.DE", "DTE.DE", "EOAN.DE", "FRE.DE", "FME.DE", "HEI.DE",
		"HEN3.DE", "IFX.DE", "LIN.DE", "MRK.DE", "MUV2.DE", "RWE.DE",
		"SAP.DE", "SIE.DE", "TKA.DE", "VOW3.DE", "VNA.DE", "WDI.DE" };

void setup_req(string target) {
	::req.method(http::verb::get);
	::req.target(target);
	::req.set(http::field::host, ::host);
	::req.set(http::field::content_type, "text/plain");
	::req.keep_alive(true);
};

bool connect() {
	//ssl
	//load_root_certificates(ctx);

	// Verify the remote server's certificate
	ctx.set_verify_mode(ssl::verify_peer);

	::results = ::resolver.resolve(::host, ::port);
	boost::asio::connect(::sockets, ::results.begin(), ::results.end());
	return true;
};

void write_and_read(string filename) {
	//send request.
	http::write(::sockets, ::req);
	system::error_code ec_file;
	//set up file body.
	::res.body().open(filename.c_str(), beast::file_mode::write, ec_file);
	//write file.
	system::error_code ec;
	//read returns bytes transferred.
	cout << http::read(::sockets, ::buffer, ::res, ec) << endl;
	//reset buffer.
	::buffer.consume(::buffer.size());
	//close file.
	::res.body().close();
};

int main()
{

	for (int i = 0; i < code_list.size(); i++) {

		string target = ::header + ::code_list[i] + ::tail;
		string file = ::folder + ::code_list[i] + ".csv";

		cout <<host+target << endl;

		//connect();
		//setup_req(target);
		//write_and_read(file);

		//cout << ::code_list[i] << " downloaded." << endl;
	};
}
