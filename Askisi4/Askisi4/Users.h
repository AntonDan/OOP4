#ifndef USERS_INCLUDE_GUARD
#define USERS_INCLUDE_GUARD

#include "System.h"

using namespace std;

#pragma region Accounts

class Visitor {
protected:
	int rights;
private:

public:
	Visitor();

};

class User : public Visitor {
protected:
	int id;
	string password;
	string username;

public:
	//CONSTRUCTOR
	User(int, string, string, int);

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