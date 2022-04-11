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

bool PublisherUDP::AddModality(const std::shared_ptr<icg::Modality>& modality_ptr) {
	set_up_ = false;
	if (!AddPtrIfNameNotExists(modality_ptr, &modality_ptrs_)) {
		std::cerr << "Modality " << modality_ptr->name() << " already exists"
			<< std::endl;
		return false;
	}
	return true;
}

std::string PublisherUDP::timeToString(std::chrono::system_clock::time_point& t){
	std::time_t time = std::chrono::system_clock::to_time_t(t);
	std::string time_str = std::ctime(&time);
	time_str.resize(time_str.size() - 1);
	return time_str;
}

bool PublisherUDP::UpdatePublisher(int iteration) {
	for (auto& modality_ptr : modality_ptrs_) {
		const icg::Transform3fA t = modality_ptrs_[0]->body_ptr()->body2world_pose();
		std::stringstream ss;
		ss << t.matrix();
		auto time_p = std::chrono::system_clock::now();

		boost::shared_ptr<std::string> message(new std::string(timeToString(time_p) + ss.str()));
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



