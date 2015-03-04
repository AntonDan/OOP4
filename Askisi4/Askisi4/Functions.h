#ifndef FUNCTIONS_INCLUDE_GUARD
#define FUNCTIONS_INCLUDE_GUARD

#include <cctype>
#include <string>
#include <vector>
#include <cstdarg>
#include <iostream>

using namespace std;

string Trim(string &);

bool isNumber(const string &);

vector<string> Split(string, const char);

void Parse(const string, const string, const bool, const char, const unsigned int , ...);

#endif 