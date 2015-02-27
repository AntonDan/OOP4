#ifndef INTERFACE_INCLUDE_GUARD
#define INTERFACE_INCLUDE_GUARD


#include <cstdio>
#include <fstream>
#include <vector>
#include <cstdarg>
#include "ForumManager.h"


void MainMenu(ForumManager & man, User * user);

void UserMenu(ForumManager &, User * user);

void ForumMenu(ForumManager &, User * user);

void ThreadMenu(ForumManager &, User * user);

#endif