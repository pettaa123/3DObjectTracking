#include "publisherudp.h"
#include <boost/asio.hpp>

PublisherUDP::PublisherUDP(const std::string& name) :
	io_service_(boost::asio::io_service()),
	socket_(boost::asio::ip::udp::socket(io_service_)),
	endpoint_(boost::asio::ip::udp::endpoint()),

	Publisher(name) {}

bool PublisherUDP::SetUp() {
	set_up_ = false;

	std::string host = "127.0.0.1";
	short port = 53082;
	endpoint_.address(boost::asio::ip::address::from_string(host));
	endpoint_.port(port);

	socket_.open(boost::asio::ip::udp::v4());

	set_up_ = true;
	return true;
}

bool PublisherUDP::AddBody(const std::shared_ptr<icg::Body>& body_ptr) {
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

std::string PublisherUDP::timeToString(std::chrono::system_clock::time_point& t){
	std::time_t time = std::chrono::system_clock::to_time_t(t);
	std::string time_str = std::ctime(&time);
	time_str.resize(time_str.size() - 1);
	return time_str;
}

bool PublisherUDP::UpdatePublisher(int iteration) {
	for (auto& modality_ptr : body_ptrs_) {
		const icg::Transform3fA t = body_ptrs_[0]->geometry2world_pose();
		std::stringstream ss;
		ss << t.matrix();
		//auto time_p = std::chrono::system_clock::now();

		boost::shared_ptr<std::string> message(new std::string(ss.str())); //timeToString(time_p)
		socket_.send_to(boost::asio::buffer(*message,message->size()), endpoint_);
		//socket_->async_send_to(boost::asio::buffer(*message), *endpoint_,
		//	[](const boost::system::error_code& ec,
		//		std::size_t bytes_transferred)
		//	{
		//		std::cout << ec << std::endl;
		//		std::cout << bytes_transferred << std::endl;
		//		return;
		//	});
	}
	return true;
}

PublisherUDP::~PublisherUDP() {
	socket_.close();
}



