#ifndef SYSTEMSAVE_INCLUDE_GUARD
#define SYSTEMSAVE_INCLUDE_GUARD

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include "System.h"
#include "Users.h"

void SaveSystem(ofstream &, ofstream &, ofstream &, System &);

void SaveSubforums(ofstream &, ofstream &, ofstream &, Forum *, string);

void SaveThreads(ofstream &, ofstream &, Forum *, string);

void SavePosts(ofstream &, Thread *, string);

void SaveUsers(ofstream &, oList<User> &);

#endif