#pragma once

#include "../../YeetCore/include/controllers.hpp"
#include "../../YeetCore/include/repository.hpp"
#include "../../YeetCore/include/commit.hpp"
#include "../../YeetCore/include/db.hpp"
#include "../../YeetCore/include/branch.hpp"
#include "../../YeetCore/include/history.hpp"
#include "../../YeetUtils/include/file_utils.hpp"
#include "../../YeetCore/include/revert.hpp"
#include <filesystem>

// @return the exit code
// @def Has all the commands of this project
int commands(int num_of_arg, char* argv[]);