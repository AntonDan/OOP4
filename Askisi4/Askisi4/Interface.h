#ifndef INTERFACE_INCLUDE_GUARD
#define INTERFACE_INCLUDE_GUARD


#include <cstdio>
#include <fstream>
#include <vector>
#include <cstdarg>
#include "ForumManager.h"


void MainMenu(ForumManager & man, User * user);

void UserMenu(User * user);

void ForumMenu(User * user);

void ThreadMenu(User * user);

#endif