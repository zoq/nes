/**
 * @file parser.hpp
 * @author Marcus Edel
 *
 * Miscellaneous parser routines.
 */
#include <mlpack/core.hpp>

#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace parser {

using boost::property_tree::ptree;


class Parser {
 public:
  /**
   * Create the Parser object.
   */
  Parser() { /* Nothing to do here */ }

  /**
   * Create the Parser object using the specified json string and create the
   * tree to extract the attributes.
   *
   * @param data The data encoded as json string.
   */
  Parser(const std::string& data)
  {
    Parse(data);
  }

  /**
   * Parse the specified json string and create a tree to extract the
   * attributes.
   *
   * @param data The data encoded as json string.
   */
  void Parse(const std::string& data)
  {
    std::stringstream ss(data);
    boost::property_tree::read_json(ss, pt);
  }

  /**
   * Parse the postion of mario.
   *
   * @param x The x coordinate of mario.
   * @param y The y coordinate of mario.
   */
  void MarioPostion(int& x, int& y)
  {
    x = pt.get_child("mario").get<int>("x");
    y = pt.get_child("mario").get<int>("y");
  }

  /**
   * Parse the endpoint data.
   *
   * @param host The endpoint hostname.
   * @param port The endpoint port.
   */
  void Endpoint(std::string& host, std::string& port)
  {
    host = pt.get_child("endpoint").get<std::string>("host");
    port = pt.get_child("endpoint").get<std::string>("port");
  }

  /**
   * Parse the tiles data and return in matrix form.
   *
   * @param tiles The tiles as matrix.
   */
  void Tiles(arma::mat& tiles)
  {
    int radius = 0;
    ptree::const_iterator end = pt.get_child("tiles").end();

    // Get the number of tiles in one row.
    for (ptree::const_iterator it = pt.get_child("tiles").begin();
        it != end; ++it, radius++);

    // Create the tiles matrix.
    tiles = arma::zeros<arma::mat>(radius, radius);

    // Get the radius (view field).
    radius = std::floor(radius / 2);

    // Transform json string to armadillo matrix.
    for (ptree::const_iterator it = pt.get_child("tiles").begin();
        it != end; ++it)
    {
      int index = std::stoi(it->first);

      // Map the index to the right matrix postion.
      if (index > 1)
      {
        index = radius + index - 1;
      }
      else if (index < 1)
      {
        if (index != 0)
        {
          index = radius + index;
        }
        else
        {
          index = tiles.n_rows - 1;
        }
      }
      else
      {
        index = radius;
      }

      Vec(it->second, it->second.get_value<std::string>(), tiles, index);
    }
  }

  /**
   * Parse the number lives.
   *
   * @param lives The current lives.
   */
  void MarioLives(int& lives)
  {
    lives = pt.get<int>("lives");
  }

  /**
   * Parse the number coins.
   *
   * @param coins The current number of coins.
   */
  void MarioCoins(int& coins)
  {
    coins = pt.get<int>("coins");
  }

  /**
   * Parse the player state.
   *
   * @param state The current player state.
   */
  void PlayerState(int& state)
  {
    state = pt.get<int>("state");
  }

  /**
   * Parse the current game image.
   *
   * @param image The current game image as string.
   */
  void GameImage(const std::string& json, std::string& image)
  {
    image = json;
  }

 private:
  //! Store results of the given json string in the row'th of the given
  // matrix v.
  void Vec(const ptree& pt, const ptree::key_type& key, arma::mat& v, int row)
  {
    int col = 0;
    for (auto& item : pt.get_child(key))
    {
      v(row, col++) = item.second.get_value<int>();
    }
  }

  //! Locally stored property_tree to parse the json string.
  ptree pt;

}; // class Parser

} // namespace parser