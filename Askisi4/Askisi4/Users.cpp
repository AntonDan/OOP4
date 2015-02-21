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

void User::CreateThread(Forum * forum, string title, string content) {
	forum->CreateThread(title, username, content);
}

void User::CreatePost(Thread * thread, string content) {
	thread->CreatePost(username, content);
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
	if (thread != NULL){
		delete thread;
		thread = NULL;
	}
}

void Moderator::DeletePost(Post * post) {
	if (post != NULL) {
		delete post;
		post = NULL;
	}
}

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

void CreateForum(Forum * destination, string title) {
	if (destination == NULL) return;
	destination->CreateSubforum(title);
}

void CreateForum(System * destination, string title) {
	if (destination == NULL) return;
	destination->CreateForum(title);
}

void DeleteForum(Forum * forum) {
	if (forum != NULL) {
		delete forum;
		forum = NULL;
	}
}

void MoveForum(Forum * forum, void * destination) {
	if (forum == NULL || destination == NULL) return;

}

void RenameForum(Forum * forum, string title) {
	if (forum == NULL) return;
	forum->SetTitle(title);
}

void ChangeUserRights(int rights) {}

void DeleteUser(string username) {}

bool RenameUser(string username) { return false; }

void ChangeUserPassword(string new_password) {}


#pragma endregion