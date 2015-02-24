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

void User::CreateThread(Forum * forum, int TID, int PID, string title, string username, string content) {
	(forum->CreateThread(TID, title, username))->CreatePost(PID, username, content);
}

void User::CreatePost(Thread * thread, int PID, string username, string content) {
	thread->CreatePost(PID, username, content);
}

User::~User() {
	username = "";
}

#pragma endregion

#pragma region Moderator
Moderator::Moderator(string name) : User(name) {
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

Administrator::Administrator(string name) : Moderator(name){ 
	rights = 3;
}

void Administrator::CreateForum(SF * destination, string title) {
	destination->CreateForum(title);
}

void Administrator::DeleteForum(SF * forum, int index) {
	forum->DeleteForum(index);
}

void Administrator::MoveForum(Forum * forum, void * destination) {
	if (forum == NULL || destination == NULL) return;
}

void Administrator::RenameForum(Forum * forum, string title) {
	if (forum == NULL) return;
	forum->SetTitle(title);
}

void Administrator::ChangeUserRights(int rights) {
	rights = rights;
}

void Administrator::DeleteUser(string username) {

}

bool Administrator::RenameUser(string username) { return false; }

void Administrator::ChangeUserPassword(string new_password) {}


#pragma endregion