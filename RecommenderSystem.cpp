//
// Created by naqni on 02/01/2023.
//
#include "RecommenderSystem.h"
#include "cmath"
#include <set>
RecommenderSystem::RecommenderSystem ()
{}
double RecommenderSystem::find_relative(std::vector<double>& pref_vec,
                                        std::vector<double>& other){
  double n1=0;
  double n2=0;
  double s=0 ;
  for(int i=0;i<_size_of_recommend_vec;i++){
    n1+=pref_vec[i]*pref_vec[i];
    n2+=other[i]*other[i];
    s+=pref_vec[i]*other[i];
  }
  n1 = sqrt (n1);
  n2 = sqrt (n2);
  s=s/(n1*n2);
  return s;
}
sp_movie RecommenderSystem::recommend_by_content (const RSUser &user)
{
  rank_map normalized_rm =user.get_ranks();
  noramlize (normalized_rm);
  std::vector<double> pref_vec;
  for(int i=0;i<_size_of_recommend_vec;i++){
    pref_vec.push_back (0);
  }
  double c;
  for(auto &it:normalized_rm){
    if(!std::isnan (it.second)){
      c = it.second;
      for(int i=0;i<_size_of_recommend_vec;i++){
        pref_vec[i]+=_movie_map[it.first][i]*c;
      }
    }
  }
  sp_movie movie = normalized_rm.begin()->first;
  double cur_relate = 0;
  for(auto &it:normalized_rm){
    if(std::isnan (it.second)){
      if(find_relative (pref_vec,_movie_map[it.first])>cur_relate){
        movie = it.first;
        cur_relate = find_relative (pref_vec,_movie_map[it.first]);
      }
    }
  }
  return movie;
}
void RecommenderSystem::noramlize (rank_map &normalized_rm)
{
  double sum = 0;
  int counter =0;
  for(auto &it:normalized_rm)
  {
    if(!std::isnan(it.second)){
      counter++;
      sum+=it.second;
    }
  }
  for(auto &it:normalized_rm){
    if(!std::isnan(it.second)){
      it.second-=it.second;
    }
  }
}
sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
{
  for (auto &it: _movie_map)
  {
    if (it.first->get_name () == name && it.first->get_year ())
    {
      return it.first;
    }
  }
  return sp_movie (nullptr);
}
sp_movie RecommenderSystem::add_movie (const std::string &name, int year,
                                       const std::vector<double> &features)
{
  _size_of_recommend_vec = features.size();
  sp_movie movie = std::make_shared<Movie> (name, year);
  _movie_map[movie] = features;
  return movie;
}
std::ostream &operator<< (std::ostream &os, RecommenderSystem &rs)
{
  for (auto &it: rs._movie_map)
  {
    os << it.first;
  }
  return os;
}
sp_movie RecommenderSystem::recommend_by_cf(const RSUser& user, int k){
  std::vector<sp_movie> unseen_movies;
  std::vector<sp_movie> seen_movies;
  for(auto &it:user.get_ranks()){
    if(std::isnan(it.second))
    {
      unseen_movies.push_back (it.first);
    }
    else{
      seen_movies.push_back (it.first);//TODO change
    }
  }
  double max = 0;
  sp_movie best_movie = unseen_movies.front();
  for (auto &it:unseen_movies)
  {
    if(predict_movie_score(user, it,k)>max){
      best_movie = it;
      max = predict_movie_score(user, it,k);
    }
  }
  return best_movie;
}
double RecommenderSystem::predict_movie_score(const RSUser &user, const
sp_movie &movie,
                           int k){
  std::map<sp_movie,double> map_of_seen_movies;
  for (auto &it:user.get_ranks())
  {
    if(!std::isnan (it.second)){
      map_of_seen_movies[it.first] = find_relative (_movie_map[movie],
                                                    _movie_map[it.first]);
    }
  }
  std::set<sp_movie> related_set;
  for (int i=0;i<k;i++)
  {
    sp_movie cur_max= map_of_seen_movies.rbegin()->first;
    related_set.insert(cur_max);
    map_of_seen_movies.erase(cur_max);
  }
  double sum = 0;
  double divide = 0;
  for(auto &it:related_set){
    divide+=map_of_seen_movies[it];
    sum+=map_of_seen_movies[it]*user.get_ranks().find (it)->second;
  }
  return (sum/divide);
}