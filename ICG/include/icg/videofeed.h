#ifndef VIDEOFEED_H
#define VIDEOFEED_H

#include <opencv2/core/mat.hpp>
#include <boost/asio.hpp>

namespace icg {

	class Videofeed
	{
	public:
		Videofeed(const short port);

		bool SetUp();

		bool UpdateVideofeed(cv::Mat& img);

		~Videofeed();

	private:
		short port_;
		boost::asio::io_service io_service_;
		boost::asio::ip::udp::socket socket_;
		boost::asio::ip::udp::endpoint endpoint_;
		bool set_up_;
	};
}

#endif // VIDEOFEED_H
