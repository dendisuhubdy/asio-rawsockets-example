#include <iostream>
#include <istream>
#include <ostream>
#include <iterator>
#include <algorithm>
#include <stdio.h>
#include <exception>

#include <asio.hpp>

#include "udp_header.hpp"
#include "raw.hpp"

int main() {
	udp_header udph;
	udph.source_port(23);
	udph.destination_port(23);
	udph.length(11);
	udph.checksum(0x00); //optional for IPv4

	udph.to_string();

	std::string body("---"); //create a small data payload

	asio::io_service io_service;

	asio::basic_raw_socket<asio::ip::raw> socket_(io_service);

	asio::streambuf request_buffer;
	std::ostream os(&request_buffer);
	os << udph << body;

	asio::ip::address_v4::bytes_type b = {{127, 0, 0, 1}};

	asio::ip::raw::endpoint ep(asio::ip::address_v4(b), 23);

	try {
		socket_.open();
		socket_.send_to(request_buffer.data(), ep);
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}
