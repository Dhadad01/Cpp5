//
// Created by naqni on 04/01/2023.
//
#include "RSUsersLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
std::vector<RSUser> RSUsersLoader::create_users_from_file
(const string &users_file_path,std::unique_ptr<RecommenderSystem> rs)
noexcept (false)
{
    std::shared_ptr<RecommenderSystem> sp_rs =std::move(rs);
    std::ifstream file(users_file_path);
    string line;
    std::getline (file,line);
    string name;
    int year;
    std::vector<sp_movie> movies_vec;
    std::istringstream stream(line);
    int counter = 0;
    while (std::getline (stream,name,'-')){
      counter++;
      stream>>year;
      sp_movie cur_movie = std::make_shared<Movie>(name,year);
      movies_vec.push_back (cur_movie);
    }
    string user_name;
    std::vector<RSUser> users_vec;
    while (std::getline (file,line)){
      rank_map cur_rank_map;
      std::istringstream stream1(line);
      stream1>>user_name;
      char c = stream1.peek();
      for (int i = 0; i < counter; i++)
      {
        if (std::isdigit(c)){//add movie to who?
          cur_rank_map[movies_vec[i]] = c;
        }
        else{
          cur_rank_map[movies_vec[i]] = NAN;
        }

      }
      std::shared_ptr<rank_map> sp_cur_rank_map =
          std::make_shared<rank_map>(cur_rank_map);
      RSUser cur_user = RSUser (name, *sp_cur_rank_map, sp_rs);
      users_vec.push_back (cur_user);
    }
  return users_vec;
}


