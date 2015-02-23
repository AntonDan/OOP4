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

User::User(string name) : Visitor() {
	username = name;
	rights = 1;
}

void User::CreateThread(Forum * forum, string title, string content) {}

void User::CreatePost(Thread * thread, string content) {}

User::~User() {
	username = "";
}

#pragma endregion

#pragma region Moderator

Moderator::Moderator(string name) : User(name) {
	rights = 2;
}

void Moderator::DeleteThread(Thread * thread) {}

void Moderator::DeletePost(Post * post) {}

void Moderator::MoveThread(Thread * thread, Forum * destination) {}

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

Administrator::Administrator(string name) : Moderator(name) {
	rights = 3;
}

void Administrator::CreateForum(Forum * destination, string title) {}

void Administrator::CreateForum(System * destination, string title) {}

void Administrator::DeleteForum(Forum * forum) {}

void Administrator::MoveForum(Forum * forum, void * destination) {
	if (forum == NULL || destination == NULL) return;
}

void Administrator::RenameForum(Forum * forum, string title) {
	if (forum == NULL) return;
	forum->SetTitle(title);
}

void Administrator::ChangeUserRights(int rights) {}

void Administrator::DeleteUser(string username) {}

bool RenameUser(string username) { return false; }

void ChangeUserPassword(string new_password) {}


#pragma endregion