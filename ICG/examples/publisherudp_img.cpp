#include "publisherudp_img.h"
#include <boost/asio.hpp>

PublisherUDP_img::PublisherUDP_img(
	const std::string& name,
	const short port,
	const std::shared_ptr<icg::NormalColorViewer>& color_viewer_ptr
	) :
	io_service_(boost::asio::io_service()),
	socket_(boost::asio::ip::udp::socket(io_service_)),
	endpoint_(boost::asio::ip::udp::endpoint()),
	port_(port),
	color_viewer_ptr_(color_viewer_ptr),
	Publisher(name) {}

bool PublisherUDP_img::SetUp() {
	set_up_ = false;

	std::string host = "127.0.0.1";

	endpoint_.address(boost::asio::ip::address::from_string(host));
	endpoint_.port(port_);


	boost::system::error_code lErrorCode;
	socket_.open(boost::asio::ip::udp::v4(),lErrorCode);
	std::cout << "opening socket: " <<  lErrorCode.message() << std::endl;
	

	set_up_ = true;
	return true;
}

bool PublisherUDP_img::AddBody(const std::shared_ptr<icg::Body>& body_ptr) {
	set_up_ = false;
	// Check if renderer geometry for body already exists
	for (auto& p : body_ptrs_) {
		if (body_ptr->name() == p->name()) {
			std::cerr << "Body data " << body_ptr->name() << " already exists"
				<< std::endl;
			return false;
		}
	}
	// Add body ptr and body data
	body_ptrs_.push_back(body_ptr);
	return true;
}

std::string PublisherUDP_img::timeToString(std::chrono::system_clock::time_point& t){
	std::time_t time = std::chrono::system_clock::to_time_t(t);
	std::string time_str = std::ctime(&time);
	time_str.resize(time_str.size() - 1);
	return time_str;
}

bool PublisherUDP_img::UpdatePublisher(int iteration) {
	std::stringstream ss;
	for (auto& p : body_ptrs_) {
		const icg::Transform3fA t = p->geometry2world_pose();
		ss << " " + p->name() + ": ";
		ss << t.matrix();
		//auto time_p = std::chrono::system_clock::now();
	}
	boost::shared_ptr<std::string> message(new std::string(ss.str())); //timeToString(time_p)


	cv::imshow("imshow_from_publisher",color_viewer_ptr_->currentImage());

	socket_.send_to(boost::asio::buffer(*message, message->size()), endpoint_);

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

PublisherUDP_img::~PublisherUDP_img() {
	boost::system::error_code lErrorCode;
	socket_.close(lErrorCode);
	std::cout << "closing socket: " <<  lErrorCode.message() << std::endl;
}



