//
// Created on 2/21/2022.
//




#ifndef USERFACTORY_H
#define USERFACTORY_H
#include "RecommenderSystemLoader.h"
class RSUsersLoader
{
private:


public:
    RSUsersLoader() = delete;
    /**
     *
     * loads users by the given format with their movie's ranks
     * @param users_file_path a path to the file
     * of the users and their movie ranks
     * @param rs RecommendingSystem for the Users
     * @return vector of the users created according to the file
     */
    static std::vector<RSUser> create_users_from_file
    (const std::string& users_file_path, std::unique_ptr<RecommenderSystem> rs)
    noexcept(false);
// private:
//  static int
//  get_first_line (string &name, int year, std::vector<sp_movie> &movies_vec, std::istringstream &stream, int counter);
};


#endif //USERFACTORY_H
