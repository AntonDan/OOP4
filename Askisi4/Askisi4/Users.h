#ifndef USERS_INCLUDE_GUARD
#define USERS_INCLUDE_GUARD

#include "System.h"

using namespace std;

#pragma region Accounts

class User {
protected:
	int id;
	string password;
	string username;
	int rights;

public:
	//CONSTRUCTOR
	User(int, string, string, int);

	User(string, string, int);

	//METHODS

	//SETTERS
	void SetUsername(string newname);

	void SetPassword(string code);

	void SetRights(int value);

	//GETTERS
	string GetUsername() const;

	string GetPassword() const;
		
	int GetRights() const;

	int User::GetId() const;

	//DESTRUCTOR
	~User();
};

#pragma endregion

#endif