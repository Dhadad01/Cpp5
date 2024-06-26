
#include "Movie.h"

#include <utility>
#define HASH_START 17
#define RES_MULT 31

/**
 * hash function used for a unordered_map (implemented for you)
 * @param movie shared pointer to movie
 * @return an integer for the hash map
 */
Movie::Movie (string name, int year) : _year (year), _name (std::move(name))
{}
const string &Movie::get_name () const
{
  return _name;
}
int Movie::get_year () const
{
  return _year;
}
bool Movie::operator< (const Movie &rhs) const
{
  if (get_year () != rhs.get_year ())
  {
    return get_year () < rhs.get_year ();
  }
  //else
  return get_name () < rhs.get_name ();
}
std::ostream &operator<< (std::ostream &os, const Movie &movie)
{
  os << movie.get_name () << " (" << movie.get_year () << ")" << std::endl;
  return os;
}
std::size_t sp_movie_hash (const sp_movie &movie)
{
  std::size_t res = HASH_START;
  res = res * RES_MULT + std::hash<std::string> () (movie->get_name ());
  res = res * RES_MULT + std::hash<int> () (movie->get_year ());
  return res;
}

/**
 * equal function used for an unordered_map (implemented for you)
 * @param m1
 * @param m2
 * @return true if the year and name are equal else false
 */
bool sp_movie_equal (const sp_movie &m1, const sp_movie &m2)
{
  return !(*m1 < *m2) && !(*m2 < *m1);
}
