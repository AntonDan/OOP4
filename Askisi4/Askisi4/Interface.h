#ifndef INTERFACE_INCLUDE_GUARD
#define INTERFACE_INCLUDE_GUARD


#include <cstdio>
#include <fstream>
#include <vector>
#include <cstdarg>
#include "ForumManager.h"

string Trim(string &);

void MainMenu(ForumManager &, User *);

void UserMenu(ForumManager &, User *);

void ForumMenu(ForumManager &, User *);

void ThreadMenu(ForumManager &, User *);

#endif