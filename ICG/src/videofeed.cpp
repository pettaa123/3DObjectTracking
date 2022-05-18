#include "icg/videofeed.h"
#include <iostream>
#include "icg/common.h"

namespace icg {

	Videofeed::Videofeed(const short port) :
		port_(port),
		io_service_(boost::asio::io_service()),
		socket_(boost::asio::ip::udp::socket(io_service_)),
		endpoint_(boost::asio::ip::udp::endpoint())
	{
		set_up_ = false;	
	}


	bool Videofeed::SetUp() {
		set_up_ = false;

		std::string host = "127.0.0.1";
		endpoint_.address(boost::asio::ip::address::from_string(host));
		endpoint_.port(port_);


		boost::system::error_code lErrorCode;
		socket_.open(boost::asio::ip::udp::v4(), lErrorCode);
		std::cout << "opening videofeed socket on port " << port_ << ": " << lErrorCode.message() << std::endl;


		set_up_ = true;
		return true;
	}

	bool Videofeed::UpdateVideofeed(cv::Mat& mat) {
		if (!set_up_) {
			SetUp();
		}
		// Assuming you already have a cv::Mat mat object
		std::vector<uchar> buf;
		std::vector<int> params;
		params.push_back(cv::IMWRITE_JPEG_QUALITY);
		params.push_back(50);

		if (!cv::imencode(".jpg", mat, buf, params)) {
			std::cout << "JPEG compression failed.\n";
			return false;
		}
		
		//boost::shared_ptr<std::string> message(new std::string((const uchar*)buf.data()));
		socket_.send_to(boost::asio::buffer(&buf[0], buf.size()), endpoint_);
		//async probably computational slower than sync
		//socket_->async_send_to(boost::asio::buffer(*message), *endpoint_,
		//	[](const boost::system::error_code& ec,
		//		std::size_t bytes_transferred)
		//	{
		//		std::cout << ec << std::endl;
		//		std::cout << bytes_transferred << std::endl;
		//		return;
		//	});
		return true;
	}

	Videofeed::~Videofeed() {
		boost::system::error_code lErrorCode;
		socket_.close(lErrorCode);
		std::cout << "closing videofeed socket: " << lErrorCode.message() << std::endl;
	}
}
