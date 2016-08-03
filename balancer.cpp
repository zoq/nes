/**
 * @file balancer.cpp
 * @author Marcus Edel
 *
 * Simple load balancer.
 */

#include "messages.hpp"

#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

using boost::asio::ip::tcp;

const int maxLength = 1024;

std::vector<std::string> host;
std::vector<std::string> port;
size_t backlog;

void session(tcp::socket socket)
{
  try
  {
    char data[maxLength];
    boost::system::error_code error;
    size_t length = socket.read_some(boost::asio::buffer(data), error);
    if (error == boost::asio::error::eof)
    {
      return;
    }
    else if (error)
    {
      throw boost::system::system_error(error);
    }

    std::string message(data);

    if (message.find("get") != std::string::npos)
    {
      // Send endpoint information.
      backlog = backlog >= (host.size() - 1) ? 0 : backlog + 1;

      std::string endpointMessage = messages::JSONMessage(messages::SendEndpoint(host[backlog],
          port[backlog])) + "\r\n\r\n\r\n";

      boost::system::error_code ignored_error;
      boost::asio::write(socket, boost::asio::buffer(endpointMessage),
          boost::asio::transfer_all(), ignored_error);
    }
    else if (message.find("add") != std::string::npos)
    {
      // Add endpoint.
      std::size_t hostStart = message.find("add ");
      std::size_t portStart = message.find(":", hostStart);

      std::string hostData = message.substr(hostStart + 4,
          portStart - hostStart - 4);

      std::string portData = message.substr(portStart + 1,
          message.length() - portStart - 1);
      boost::trim_right(portData);

      host.push_back(hostData);
      port.push_back(portData);

      std::cout << "Add endpoint: " << hostData << ":" << portData << std::endl;
    }
    else if (message.find("remove") != std::string::npos)
    {
      // Remove endpoint.
      std::size_t hostStart = message.find("remove ");
      std::size_t portStart = message.find(":", hostStart);

      std::string hostData = message.substr(hostStart + 7,
          portStart - hostStart - 7);

      std::string portData = message.substr(portStart + 1,
          message.length() - portStart - 1);
      boost::trim_right(portData);

      for(size_t i = 0; i < host.size(); ++i)
      {
        if (host[i] == hostData && port[i] == portData)
        {
          host.erase(host.begin() + i);
          port.erase(port.begin() + i);

          std::cout << "Remove endpoint: " << hostData << ":" << portData << std::endl;
          break;
        }
      }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_service& ioService, size_t port)
{
  tcp::acceptor a(ioService, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    tcp::socket sock(ioService);
    a.accept(sock);
    std::thread(session, std::move(sock)).detach();
  }
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc < 2 || ((argc - 2) % 2) != 0)
    {
      std::cout << "Usage: <port> <host> <port> ... <host> <port>\n";
      return 1;
    }

    for (size_t i = 2; i < (argc - 1); i += 2)
    {
      std::cout << "Add endpoint: " << argv[i] << ":" << argv[i + 1] << std::endl;

      host.push_back(argv[i]);
      port.push_back(argv[i + 1]);
    }

    backlog = 0;

    boost::asio::io_service ioService;
    server(ioService, std::atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}