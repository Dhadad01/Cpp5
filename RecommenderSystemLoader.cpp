//
// Created by naqni on 04/01/2023.
//
#include "RecommenderSystemLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
std::unique_ptr<RecommenderSystem>RecommenderSystemLoader::create_rs_from_movies_file
(const string &movies_file_path) noexcept (false){
  std::ifstream file(movies_file_path);
  RecommenderSystem rs;
  string line;
  while (std::getline (file,line))
  {
    string name;
    int year;
    int attribute;
    std::vector<double> attributes_vec = {};
    std::istringstream stream(line);
    std::getline (stream,name,'-');
    stream>>year;
    while (stream>>attribute){
      if(stream.fail()||attribute>10||attribute<1){
        throw std::invalid_argument ("expected int but got float instead");
      }
      attributes_vec.push_back (attribute);
    }
    rs.add_movie (name,year,attributes_vec);
  }
  std::unique_ptr<RecommenderSystem> recommender_system =
      std::make_unique<RecommenderSystem>(rs);
  return recommender_system;
}


