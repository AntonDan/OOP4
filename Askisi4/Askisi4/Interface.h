#ifndef INTERFACE_INCLUDE_GUARD
#define INTERFACE_INCLUDE_GUARD


#include <cstdio>
#include <fstream>
#include <vector>
#include <cstdarg>
#include "ForumManager.h"

string Trim(string &);

bool MainMenu(ForumManager &, User *);

bool UserMenu(ForumManager &, User *);

bool ForumMenu(ForumManager &, User *);

bool ThreadMenu(ForumManager &, User *);

#endif