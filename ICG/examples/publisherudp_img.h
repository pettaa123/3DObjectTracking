#ifndef ICG_PUBLISHER_UDP_IMG
#define ICG_PUBLISHER_UDP_IMG

#include <icg/publisher.h>
#include <icg/body.h>
#include <icg/normal_viewer.h>

#include <boost/asio.hpp>
//#include <boost/array.hpp>


class PublisherUDP_img : public icg::Publisher
{
public:
    PublisherUDP_img(
        const std::string& name,
        const short port,
        const std::shared_ptr<icg::NormalColorViewer>& color_viewer_ptr
    );

    bool SetUp();

    bool UpdatePublisher(int iteration);

    bool AddBody(const std::shared_ptr<icg::Body>& body_ptr);

    ~PublisherUDP_img();

private:

    boost::asio::io_service io_service_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint endpoint_;

    std::vector<std::shared_ptr<icg::Body>> body_ptrs_;

    std::string timeToString(std::chrono::system_clock::time_point& t);

    short port_;
    const std::shared_ptr<icg::NormalColorViewer>& color_viewer_ptr_;


};

#endif // ICG_PUBLISHER_UDP_IMG