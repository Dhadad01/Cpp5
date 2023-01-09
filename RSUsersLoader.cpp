//
// Created by naqni on 04/01/2023.
//
#include "RSUsersLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#define LENGTH_OF_DIGIT 1
#define HYPHEN '-'
#define TEN 10
int
get_first_line (string &name, std::vector<sp_movie> &movies_vec, std::istringstream &stream, int counter)
{
  int year;
  while (std::getline (stream, name, HYPHEN))
  {
    size_t start = name.find_first_not_of (" \t");
    if (start != std::string::npos)
    {
      name.erase (0, start);
    }
    counter++;
    stream >> year;
    sp_movie cur_movie = std::make_shared<Movie> (name, year);
    movies_vec.push_back (cur_movie);
  }
  return counter;
}
std::vector<RSUser> RSUsersLoader::create_users_from_file
    (const string &users_file_path, std::unique_ptr<RecommenderSystem> rs)
noexcept (false)
{
  std::shared_ptr<RecommenderSystem> sp_rs = std::move (rs);
  std::ifstream file (users_file_path);
  if(!file.is_open()){
    throw std::ios_base::failure("Unable to open file");
  }
  string line;
  std::getline (file, line);
  string name;
  int year;
  std::vector<sp_movie> movies_vec;
  std::istringstream stream (line);
  int counter = 0;
  counter = get_first_line (name, movies_vec, stream, counter);
  string user_name;
  std::vector<RSUser> users_vec;
  string rank;
  while (std::getline (file, line))
  {
    rank_map cur_rank_map (TEN, sp_movie_hash, sp_movie_equal);
    std::istringstream stream1 (line);
    stream1 >> user_name;
    for (int i = 0; i < counter; i++)
    {
      stream1 >> rank;
      if(rank=="10")
      {
        cur_rank_map[movies_vec[i]] = std::stoi (rank);
        continue;
      }
      char c = rank[0];
      if (isdigit (c) && rank.length () == LENGTH_OF_DIGIT)
      {
        cur_rank_map[movies_vec[i]] = std::stoi (rank);
      }
      else if(rank=="NA")
      {
        cur_rank_map[movies_vec[i]] = NAN;
      }
      else
      {
        throw std::invalid_argument("invalid arguments from file");
      }
    }
    std::shared_ptr<rank_map> sp_cur_rank_map =
        std::make_shared<rank_map> (cur_rank_map);
    RSUser cur_user = RSUser (user_name, *sp_cur_rank_map, sp_rs);
    users_vec.push_back (cur_user);
  }
  return users_vec;
}

