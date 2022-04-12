#ifndef ICG_PUBLISHER_UDP_
#define ICG_PUBLISHER_UDP_

#include <icg/publisher.h>
#include <icg/body.h>

#include <boost/asio.hpp>
//#include <boost/array.hpp>


class PublisherUDP : public icg::Publisher
{
public:
    PublisherUDP(const std::string& name);

    bool SetUp();

    bool UpdatePublisher(int iteration);

    bool AddBody(const std::shared_ptr<icg::Body>& body_ptr);

    ~PublisherUDP();

private:

    boost::asio::io_service io_service_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint endpoint_;

    std::vector<std::shared_ptr<icg::Body>> body_ptrs_;

    std::string timeToString(std::chrono::system_clock::time_point& t);


};

#endif // ICG_PUBLISHER_UDP_