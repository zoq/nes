/**
 * @file client.hpp
 * @author Marcus Edel
 *
 * Miscellaneous client routines.
 */
#include <mlpack/core.hpp>

#include <string>
#include <boost/asio.hpp>

namespace client {

using boost::asio::ip::tcp;

/**
 * Implementation of the Client.
 */
class Client {
 public:
  /**
   * Create the Client object using the given host and port.
   *
   * @param host The hostname to connect.
   * @param port The port used for the connection.
   */
  Client(const std::string& host, const std::string& port) : s(io_service)
  {
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), host, port);
    tcp::resolver::iterator iterator = resolver.resolve(query);

    boost::asio::connect(s, iterator);
  }

  /**
   * Receive a message using the currently open socket.
   *
   * @param data The received data.
   */
  void Receive(std::string& data)
  {
    boost::asio::streambuf response;
    size_t reply_length =  boost::asio::read_until(s, response, "\r\n\r\n\r\n");

    data = std::string(
        boost::asio::buffers_begin(response.data()),
        boost::asio::buffers_begin(response.data()) + reply_length);
  }

  /**
   * Send a message using the currently open socket.
   *
   * @param data The data to be send.
   */
  void Send(const std::string& data)
  {
    boost::system::error_code ignored_error;
    boost::asio::write(s, boost::asio::buffer(data + "\r\n"),
        boost::asio::transfer_all(), ignored_error);
  }

 private:
  //! Locally stored io service.
  boost::asio::io_service io_service;

  //! Locally stored socket object.
  tcp::socket s;
}; // class Client

} // namespace client