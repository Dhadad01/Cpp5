//
// Created on 2/20/2022.
//

#ifndef RECOMMENDERSYSTEM_H
#define RECOMMENDERSYSTEM_H
#include "RSUser.h"
#include "Movie.h"
#include <vector>
#include <memory>
#include <map>
struct Comp
{
    bool operator() (const sp_movie &rhs, const sp_movie &lhs)
    {
      return *rhs < *lhs;
    }
};
typedef std::vector<double> rate_vec;
typedef std::map<sp_movie, rate_vec, Comp> movie_map;
typedef std::vector<sp_movie> all_movies_vec;

class RecommenderSystem
{
 private:
  movie_map _movie_map;//movie map is a dict that has shared pointer as a
  // key and vector of ranks as value
  static void noramlize (rank_map &normalized_rm);
  static double
  find_relative (std::vector<double> &pref_vec, std::vector<double>
  &other);

 public:

  explicit RecommenderSystem ();
  /**
   * adds a new movie to the system
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie
  add_movie (const std::string &name, int year,
             const std::vector<double> &features);

  /**
   * a function that calculates the movie with highest score based on movie features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_content (const RSUser &user);

  /**
   * a function that calculates the movie with highest predicted score based on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf (const RSUser &user, int k);

  /**
   * Predict a user rating for a movie given argument using item cf procedure with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score (const RSUser &user, const sp_movie &movie,
                              int k);

  /**
   * gets a shared pointer to movie in system
   * @param name name of movie
   * @param year year movie was made
   * @return shared pointer to movie in system
   */
  sp_movie get_movie (const std::string &name, int year) const;
  friend std::ostream &operator<< (std::ostream &os, RecommenderSystem &rs);

};

#endif //RECOMMENDERSYSTEM_H
