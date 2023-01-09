

// don't change those includes
#include "RSUser.h"

#include <utility>
#include "RecommenderSystem.h"
RSUser::RSUser (std::string &name, rank_map &rank_map,
                std::shared_ptr<RecommenderSystem>
                recommender_system) : _name (name), _rank_map (rank_map),
                _recommender_system (std::move (recommender_system))
{}

std::string RSUser::get_name () const
{
  return this->_name;
}
rank_map RSUser::get_ranks () const
{
  return _rank_map;
}
void RSUser::add_movie_to_rs (const std::string &name, int year,
                              const std::vector<double> &features, double rate)
{
  sp_movie movie = std::make_shared<Movie> (name, year);
  _rank_map[movie] = rate;
  *(_recommender_system)->add_movie (name, year, features);
}
sp_movie RSUser::get_recommendation_by_content () const
{
  return _recommender_system->recommend_by_content (*this);
}
sp_movie RSUser::get_recommendation_by_cf (int k) const
{
  return _recommender_system->recommend_by_cf (*this, k);
}
double RSUser::get_prediction_score_for_movie (const std::string &name, int
year, int k) const
{
  sp_movie movie = std::make_shared<Movie> (name, year);
  return _recommender_system->predict_movie_score (*this, movie, k);
}
std::ostream &operator<< (std::ostream &os, RSUser &user)
{
  return operator<< (os,user._recommender_system);
}