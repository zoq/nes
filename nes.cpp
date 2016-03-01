/**
 * @file nes.hpp
 * @author Marcus Edel
 *
 * Simple test programm to communicate with the host, that runs nes game.
 */

#include <mlpack/core.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>

#include "parser.hpp"
#include "client.hpp"
#include "messages.hpp"

using namespace mlpack;

int main(int argc, char* argv[])
{
  if (argc > 2)
  {
    std::string host(argv[1]);
    std::string port(argv[2]);

    // Connect to the specified host using the given port.
    client::Client client(host, port);

    parser::Parser parser;
    std::string command;
    for(;;)
    {
      std::string json;

      std::cout << "command: ";
      std::getline(std::cin, command);

      if (command.find("a") != std::string::npos)
      {
        messages::Append(json, messages::PressLeft());
      }
      else if (command.find("d") != std::string::npos)
      {
        messages::Append(json, messages::PressRight());
      }
      else if (command.find("s") != std::string::npos)
      {
        messages::Append(json, messages::PressDown());
      }
      else if (command.find("f") != std::string::npos)
      {
        messages::Append(json, messages::PressA());
      }
      else if (command.find("j") != std::string::npos)
      {
        messages::Append(json, messages::PressB());
      }
      else if (command.find("w") != std::string::npos)
      {
        messages::Append(json, messages::PressUp());
      }
      else if (command.find("k") != std::string::npos)
      {
        messages::Append(json, messages::PressStart());
      }
      else if (command.find("r") != std::string::npos)
      {
        messages::Append(json, messages::GameReset());
        client.Send(messages::JSONMessage(json));

        // There is only one game value which is Reset when the command 'r' is
        // executed.
        continue;
      }
      else if (command.find("i") != std::string::npos)
      {
        messages::Append(json, messages::GameImage());
        client.Send(messages::JSONMessage(json));

        std::string imageStr;
        client.Receive(imageStr);

        parser.GameImage(imageStr, imageStr);
        std::vector<char> vectordata(imageStr.begin(), imageStr.end());

        cv::Mat data_mat(vectordata, true);
        cv::Mat image(cv::imdecode(data_mat, 1));

        imshow("image", image);        
        if(cv::waitKey(1) >= 0) break;

        continue;
      }
      else if (command.find("c") != std::string::npos)
      {
        messages::Append(json, messages::ConfigFrame(30));
      }

      messages::Append(json, messages::GameInfo());

      client.Send(messages::JSONMessage(json));
      client.Receive(json);

      Log::Info << json << std::endl;
    }

    return 0;
  }
  else
  {
    Log::Fatal << "No host and port specified. Please provide the hostname and"
               << " the port." << std::endl;
    return 1;
  }  
}