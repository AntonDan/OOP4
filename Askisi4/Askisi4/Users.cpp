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



#pragma region Moderator

//CONSTRUCTOR
Moderator::Moderator(int id,string name, string code) : User(id, name, code) {
	rights = 2;
}

//METHODS
void Moderator::DeleteThread(Thread * thread) {
	/* Accesing thread's parent and deleting the given thread that it contains */
	(thread->GetParent())->DeleteThread(thread);
}

void Moderator::DeletePost(Post * post) {
	/* Accesing post's parent and deleting the given post that it contains */
	(post->GetParent())->DeletePost(post);
}

void Moderator::MoveThread(Thread * thread, Forum * destination) {
	/* Accesing thread's parent and removing the given thread that it contains, after that we add it to the destination.
	   Finally, we assign the new parent to the moved thread */
	destination->AddThread((thread->GetParent())->RemoveThread(thread));
	thread->SetParent(destination);
}

void Moderator::MovePost(Post * pos, Thread * destination) {
	/* Accesing post's parent and removing the given post that it contains, after that we add it to the destination.
	   Finally, we assign the new parent to the moved post */
	destination->AddPost((pos->GetParent())->RemovePost(pos));
	pos->SetParent(destination);
}

void Moderator::RenameThread(Thread * thread, string title) {
	thread->SetTitle(title);
}

//SETTERS
void Moderator::SetSticky(Thread * thread, bool value) {
	if (thread == NULL) return;
	thread->SetSticky(value);
}

void Moderator::SetLocked(Thread * thread, bool value) {
	if (thread == NULL) return;
	thread->SetLocked(value);
}

#pragma endregion




#pragma region Administrator 

//CONSTRUCTOR
Administrator::Administrator(int id, string name, string code) : Moderator(id, name, code){ 
	rights = 3;
}


//METHODS
void Administrator::CreateForum(SF * destination, string title) {
	destination->CreateForum(title);
}

void Administrator::DeleteForum(SF * forum, int index) {
	forum->DeleteForum(index);
}

void Administrator::MoveForum(Forum * forum, SF * destination) {
	/* Accesing forum's parent and removing the given forum that it contains, after that we add it to the destination.
	   Finally, we assign the new parent to the moved forum */
	if (forum == NULL || destination == NULL) return;
	destination->AddForum((forum->GetParent())->RemoveForum(forum));
	forum->SetParent(destination);
}

void Administrator::RenameForum(Forum * forum, string title) {
	if (forum == NULL) return;
	forum->SetTitle(title);
}

void Administrator::ChangeUserRights(int rights) {
	this->rights = rights;
}

void Administrator::DeleteUser(string username, oList<User> & users) {
	/* Searching the user list until the given username is found, then deleting it */
	for(int i = 0; i < users.GetLength(); ++i){
		if (users[i]->GetUsername() == username){
			delete users.Delete(i);
		}
	}
}

bool Administrator::RenameUser(string username, string newname, oList<User> & users) {
	/* Searching the user list until the given username is found, then rename it */
	for (int i = 0; i < users.GetLength(); ++i){
		if (users[i]->GetUsername() == username){
			users[i]->SetUsername(newname);
			return true;
		}
	}
	return false;
}

bool Administrator::ChangeUserPassword(string username, string code, oList<User> & users) {				// username: name of the user's account		code: new passord to be assigned
	/* Searching the user list until the given username is found, then assign the new password it */
	for (int i = 0; i < users.GetLength(); ++i){
		if (users[i]->GetUsername() == username){
			users[i]->SetUsername(code);
			return true;
		}
	}
	return false;
}

#pragma endregion