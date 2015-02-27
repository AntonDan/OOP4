#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include "Users.h"

#pragma region Visitor

//CONSTRUCTOR
Visitor::Visitor() {
	rights = 0;
}

#pragma endregion



#pragma region User 

//CONSTRUCTOR
User::User(int id, string name, string code) : Visitor() {
	this->id = id;
	username = name;
	password = code;
	rights = 1;
}

//METHODS
void User::CreateThread(Forum * forum, int TID, int PID, string title, string username, string content) {
	/* Create a thread and a post inside it */
	(forum->CreateThread(TID, title, username))->CreatePost(PID, username, content);
}

void User::CreatePost(Thread * thread, int PID, string username, string content) {
	/* Create a post with given data */
	thread->CreatePost(PID, username, content);
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

