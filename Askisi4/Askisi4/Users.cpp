#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include "Users.h"

#pragma region Visitor

Visitor::Visitor() {
	rights = 0;
}

#pragma endregion

#pragma region User 

User::User(string name, string code) : Visitor() {
	username = name;
	rights = 1;
	password = code;
}

void User::CreateThread(Forum * forum, int TID, int PID, string title, string username, string content) {
	(forum->CreateThread(TID, title, username))->CreatePost(PID, username, content);
}

void User::CreatePost(Thread * thread, int PID, string username, string content) {
	thread->CreatePost(PID, username, content);
}

void User::SetUsername(string newname){
	this->username = newname;
}

void User::SetPassword(string code){
	this->password = code;
}

string User::GetUsername() const{
	return this->username;
}

string User::GetPassword() const{
	return this->password;
}



User::~User() {
	username = "";
}

#pragma endregion

#pragma region Moderator

Moderator::Moderator(string name, string code) : User(name, code) {
	rights = 2;
}

void Moderator::DeleteThread(Thread * thread) {
	Forum * parent = thread->GetParent();
	parent->DeleteThread(thread);
}

void Moderator::DeletePost(Post * post) {
	Thread * parent = post->GetParent();
	parent->DeletePost(post);
}

void Moderator::MoveThread(Thread * thread, Forum * destination) {
	Forum * parent = thread->GetParent();

}

void Moderator::MovePost(Post * pos, Thread * destinationt) {}

void Moderator::RenameThread(Thread * thread, string title) {
	thread->SetTitle(title);
}

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

Administrator::Administrator(string name, string code) : Moderator(name, code){ 
	rights = 3;
}

void Administrator::CreateForum(SF * destination, string title) {
	destination->CreateForum(title);
}

void Administrator::DeleteForum(SF * forum, int index) {
	forum->DeleteForum(index);
}

void Administrator::MoveForum(Forum * forum, SF * destination) {
	if (forum == NULL || destination == NULL) return;
	destination->AddForum((forum->GetParent())->RemoveForum(forum));
	forum->SetParent(destination);
}

void Administrator::RenameForum(Forum * forum, string title) {
	if (forum == NULL) return;
	forum->SetTitle(title);
}

void Administrator::ChangeUserRights(int rights) {
	rights = rights;
}

void Administrator::DeleteUser(string username, oList<User> & users) {
	for(int i = 0; i < users.GetLength(); ++i){
		if (users[i]->GetUsername() == username){
			delete users.Delete(i);
		}
	}
}

bool Administrator::RenameUser(string username, string newname, oList<User> & users) {
	for (int i = 0; i < users.GetLength(); ++i){
		if (users[i]->GetUsername() == username){
			users[i]->SetUsername(newname);
			return true;
		}
	}
	return false;
}

bool Administrator::ChangeUserPassword(string username, string code, oList<User> & users) {
	for (int i = 0; i < users.GetLength(); ++i){
		if (users[i]->GetUsername() == username){
			users[i]->SetUsername(code);
			return true;
		}
	}
	return false;
}

#pragma endregion