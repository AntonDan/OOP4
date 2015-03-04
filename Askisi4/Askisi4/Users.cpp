#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include "Users.h"


#pragma region User 

//CONSTRUCTOR
User::User(int id, string name, string code, int rights) {
	this->id = id;
	username = name;
	password = code;
	this->rights = rights;
}


User::User(string name, string code, int rights) {
	this->id = -1;
	username = name;
	password = code;
	this->rights = rights;
}

//SETTERS
void User::SetUsername(string newname){
	this->username = newname;
}

void User::SetPassword(string code){
	this->password = code;
}

void User::SetRights(int value){
	this->rights = value;
}

//GETTERS
string User::GetUsername() const{
	return this->username;
}

int User::GetRights() const{
	return this->rights;
}

string User::GetPassword() const{
	return this->password;
}

int User::GetId() const{
	return this->id;
}

//DESTRUCTOR
User::~User() {
	username = "";
}

#pragma endregion

