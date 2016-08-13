/**
 * @file client.hpp
 * @author Marcus Edel
 *
 * Miscellaneous client routines.
 */
#include <mlpack/core.hpp>

#include <string>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

namespace client {

using boost::asio::ip::tcp;
using boost::asio::deadline_timer;
using boost::lambda::_1;
using boost::lambda::var;
using boost::lambda::bind;

void async_read_handler(const boost::system::error_code& err,
                        boost::system::error_code* err_out,
                        std::size_t bytes_transferred,
                        std::size_t* bytes_out)
{
  *err_out = err;
  *bytes_out = bytes_transferred;
}

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
  Client() :
      s(io_service),
      deadline(io_service)
  {
    deadline.expires_at(boost::posix_time::pos_infin);

    // Start the persistent actor that checks for deadline expiry.
    check_deadline();
  }

  void Connect(const std::string& host, const std::string& port)
  {
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), host, port);
    tcp::resolver::iterator iterator = resolver.resolve(query);

    // Set a deadline for the asynchronous operation.
    deadline.expires_from_now(boost::posix_time::seconds(10));

    // Set up the variable that receives the result of the asynchronous
    // operation.
    boost::system::error_code ec = boost::asio::error::would_block;

    // Start the asynchronous operation.
    boost::asio::async_connect(s, iterator, var(ec) = _1);

    // Block until the asynchronous operation has completed.
    do io_service.run_one(); while (ec == boost::asio::error::would_block);

    // Determine whether a connection was successfully established.
    if (ec || !s.is_open())
    {
      throw boost::system::system_error(
          ec ? ec : boost::asio::error::operation_aborted);
    }
  }

  /**
   * Receive a message using the currently open socket.
   *
   * @param data The received data.
   */
  void Receive(std::string& data)
  {
    // Set a deadline for the asynchronous operation.
    deadline.expires_from_now(boost::posix_time::seconds(10));

    // Set up the variable that receives the result of the asynchronous
    // operation.
    boost::system::error_code ec = boost::asio::error::would_block;

    boost::asio::streambuf response;
    size_t reply_length;

    boost::asio::async_read_until(s, response, "\r\n\r\n\r\n",
      boost::bind(async_read_handler, boost::asio::placeholders::error, &ec,
        boost::asio::placeholders::bytes_transferred, &reply_length));

    // Block until the asynchronous operation has completed.
    do io_service.run_one(); while (ec == boost::asio::error::would_block);

    if (ec)
    {
      throw boost::system::system_error(ec);
    }

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
    // Set a deadline for the asynchronous operation.
    deadline.expires_from_now(boost::posix_time::seconds(1000));

    // Set up the variable that receives the result of the asynchronous
    // operation.
    boost::system::error_code ec = boost::asio::error::would_block;

    boost::asio::async_write(s, boost::asio::buffer(data  + "\r\n"),
        var(ec) = _1);

    // Block until the asynchronous operation has completed.
    do io_service.run_one(); while (ec == boost::asio::error::would_block);

    if (ec)
    {
      throw boost::system::system_error(ec);
    }
  }

 private:
  void check_deadline()
  {
    // Check whether the deadline has passed. We compare the deadline against
    // the current time since a new asynchronous operation may have moved the
    // deadline before this actor had a chance to run.
    if (deadline.expires_at() <= deadline_timer::traits_type::now())
    {
      // The deadline has passed. The socket is closed so that any outstanding
      // asynchronous operations are cancelled. This allows the blocked
      // connect(), read_line() or write_line() functions to return.
      boost::system::error_code ignored_ec;
      s.close(ignored_ec);

      // There is no longer an active deadline. The expiry is set to positive
      // infinity so that the actor takes no action until a new deadline is set.
      deadline.expires_at(boost::posix_time::pos_infin);
    }

    // Put the actor back to sleep.
    deadline.async_wait(bind(&Client::check_deadline, this));
  }

  //! Locally stored io service.
  boost::asio::io_service io_service;

  deadline_timer deadline;

  //! Locally stored socket object.
  tcp::socket s;
}; // class Client

} // namespace client