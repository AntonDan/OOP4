#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include "System.h"

#pragma region Post

// CONSTRUCTOR
Post::Post(int id, string user, string con, Thread * parent ) {
	this->id = id;
	username = user;
	content = con;
	this->parent = parent;
}

// SETTERS
void Post::SetUser(string user) {
	username = user;
}

void Post::SetContent(string con) {
	content = con;
}

// GETTERS
int Post::GetID() const { return id; }

string Post::GetUser() const {
	return username;
}

string Post::GetContent() const {
	return content;
}

Thread * Post::GetParent() const{ return parent; }

// DESTRUCTOR
Post::~Post() {
	username = "";
	content = "";
}

#pragma endregion

#pragma region Thread

// CONSTRUCTORS
Thread::Thread(int id ,string title, string user , Forum * parent , int info ) {
	this->id = id;
	this->title = title;
	username = user;
	sticky = (info % 2 == 1);
	locked = (info >= 2);
	this->parent = parent;
}

// SETTERS 
void Thread::SetTitle(string new_title) { title = new_title; }

void Thread::SetSticky(bool value) { sticky = value; }

void Thread::SetLocked(bool value) { locked = value; }

// GETTERS
int Thread::GetID() const { return id; }

string Thread::GetTitle() const { return title; }

string Thread::GetUserName() const { return username; }

oList<Post> * Thread::GetPosts() { return (&posts); }

Post * Thread::GetPost(int index) const { return  posts[index]; }

Post * Thread::GetPostByID(int ID) const {
	for (int i = 0; i < posts.GetLength(); ++i) {
		if (posts[i]->GetID() == ID) {
			return posts[i];
		}
	}
	return NULL;
}

Forum * Thread::GetParent() const{ return parent; }

bool Thread::isSticky() const { return sticky; }

bool Thread::isLocked() const { return locked; }

// METHODS
void Thread::CreatePost(int id, string user, string content) {
	posts.Add(new Post(id, user, content, this));
}

void Thread::DeletePost(int index) {
	delete posts.Delete(index);
}

// DESTRUCTORS
Thread::~Thread() {
	title = "";
	username = "";

	posts.~oList();
}

#pragma endregion

#pragma region Forum

// CONSTRUCTOR
Forum::Forum(string title, Forum * parent ) {
	this->title = title;
	this->parent = parent;
}

// SETTERS
void Forum::SetTitle(string new_title) { title = new_title; }

// GETTERS 
string Forum::GetTitle() const { return title; }

Forum * Forum::GetForum(int index) const { return subforums[index-1]; }

Thread * Forum::GetThread(int index) const { return threads[index-1]; }

Thread * Forum::GetThreadByID(int ID) const {
	for (int i = 0; i < threads.GetLength(); ++i) {
		if (threads[i]->GetID() == ID) {
			return threads[i];
		}
	}
	return NULL;
}

oList<Forum> * Forum::GetForums() { return &subforums; }

oList<Thread> * Forum::GetThreads() { return &threads; }

// METHODS
Forum * Forum::GetParent() const {
	return parent;
}

Forum * Forum::CreateSubforum(string title) {
	Forum * newforum = new Forum(title, this);
	subforums.Add(newforum);
	return newforum;
}

Thread * Forum::CreateThread(int id, string title, string username, int info) {
	Thread * newthread = new Thread(id, title, username, this, info);
	threads.Add(newthread);
	return newthread;
}

void Forum::DeleteSubforum(int index) {
	Forum * temp = subforums.Delete(index);
	if (temp != NULL) {
		delete temp;
	}
}

void Forum::DeleteSubforum(Forum * forum) {
	Forum * temp = subforums.Delete(forum);
	if (temp != NULL) {
		delete temp;
	}
}

Forum * Forum::RemoveSubforum(Forum * forum) {
	return subforums.Delete(forum);
}

void Forum::DeleteThread(int index) {
	Thread * temp = threads.Delete(index);
	if (temp != NULL) {
		delete temp;
	}
}

void Forum::DeleteThread(Thread * thread) {
	Thread * temp = threads.Delete(thread);
	if (temp != NULL) {
		delete temp;
	}
}

Thread * Forum::RemoveThread(Thread * thread) {
	return threads.Delete(thread);
}

// DESTRUCTOR
Forum::~Forum() {
	subforums.~oList();
	threads.~threads();
	title = "";
}

#pragma endregion

#pragma region System

// CONSTRUCTOR
System::System() : title("D.I.T.Lists") , LastThreadID(0) , LastPostID(0) {}

// GETTERS 
string System::GetTitle() const { return title; }

Forum * System::GetForum(int index) const { return forums[index-1]; }

oList<Forum> * System::GetForums() { return &forums; }

// METHODS
Forum * System::CreateForum(string title) {
	Forum * newforum = new Forum(title, NULL);
	forums.Add(newforum);
	return newforum;
}

void System::DeleteForum(int index) {
	Forum * temp = forums.Delete(index);
	if (temp != NULL) {
		delete temp;
	}
}

// DESTRUCTOR
System::~System() {
	forums.Destroy();
}

#pragma endregion

#pragma region Forum Navigator
// CONSTRUCTORS 
ForumNavigator::ForumNavigator(System * system) {
	main = system;
	currentForum = NULL;
	currentThread = NULL;
}

// GETTERS
System * ForumNavigator::GetMain() const { return main; }

Forum * ForumNavigator::GetCurrentForum() const { return currentForum; }

Thread * ForumNavigator::GetCurrentThread() const { return currentThread; }

// METHODS
/* Visitors+ */
void ForumNavigator::VisitForum(int index) {
	if (currentForum == NULL) {
		currentForum = main->GetForum(index);
	} else {
		Forum * temp = currentForum;
		currentForum = currentForum->GetForum(index);
		if (currentForum != NULL) {
			back.Add(temp);
		} else {
			currentForum = temp;
		}
	}
}

void ForumNavigator::VisitForum(Forum * forum) {
	if (currentForum == NULL) {
		currentForum = forum;
	} else {
		Forum * temp = currentForum;
		currentForum = forum;
		if (currentForum != NULL) {
			back.Add(temp);
		} else {
			currentForum = temp;
		}
	}
}

void ForumNavigator::VisitThread(int index) {
	if (currentForum != NULL) {
		currentThread = currentForum->GetThread(index);
		if (currentThread != NULL) {
			back.Add(currentForum);
		}
	}
}

void ForumNavigator::Back() {
	if (back.isEmpty()) {
		currentForum = NULL;
		currentThread = NULL;
	} else {
		currentThread = NULL;
		currentForum = back.Delete(back.GetLength() - 1);
	}
}

void ForumNavigator::PrintCurrent() const {
	if (currentThread != NULL) {
		cout << "Thread: " << currentThread->GetTitle() << "  by: " << currentThread->GetUserName() << endl;
	} else if (currentForum != NULL) {
		cout << "Forum: " << currentForum->GetTitle() << endl;
	} else {
		cout << main->GetTitle() << endl;
	}
}

void ForumNavigator::PrintContents() {
	oList<Forum> * forums;
	if (currentThread != NULL) {
		oList<Post> * posts = currentThread->GetPosts();
		for (int i = 0; i < posts->GetLength(); ++i) {
			cout << "Post: " << i + 1 << ": " << "User: " << (*posts)[i]->GetUser() << "  content: " << (*posts)[i]->GetContent() << endl;
		}
	} else if (currentForum != NULL) {
		forums = currentForum->GetForums();
		for (int i = 0; i < forums->GetLength(); ++i) {
			cout << "Forum " << i + 1 << ": " << (*forums)[i]->GetTitle() << endl;
		}


		oList<Thread> * threads = currentForum->GetThreads();
		for (int i = 0; i < threads->GetLength(); ++i) {
			cout << "Thread " << i + 1 << ": " << (*threads)[i]->GetTitle() << "  by: " << (*threads)[i]->GetUserName() << endl;
		}
	} else {
		forums = main->GetForums();
		for (int i = 0; i < forums->GetLength(); ++i) {
			cout << "Forum " << i + 1 << ": " << (*forums)[i]->GetTitle() << endl;
		}
	}
}

void ForumNavigator::PrintContent(int index) const {
	if (currentThread != NULL) {
		cout << "Post: " << index + 1 << ": " << "User: " << currentThread->GetPost(index)->GetUser() << "  content: " << currentThread->GetPost(index)->GetContent() << endl;
	} else if (currentForum != NULL) {
		if (currentForum->GetForum(index) != NULL) 	cout << "Forum " << index + 1 << ": " << currentForum->GetForum(index)->GetTitle() << endl;
		if (currentForum->GetThread(index) != NULL) cout << "Thread " << index + 1 << ": " << currentForum->GetThread(index)->GetTitle() << "  by: " << currentForum->GetThread(index)->GetUserName() << endl;
	} else {
		cout << main->GetTitle() << endl;
	}
}

/* User+ */
void ForumNavigator::CreateThread(string title, string unsername, string content) {
	if (currentForum != NULL) {
		main->LastThreadID += 1;
		currentForum->CreateThread(main->LastThreadID , title, unsername);
		// Create Post
	}
}

void ForumNavigator::CreatePost(string username, string content) {
	if (currentThread != NULL) {
		main->LastPostID += 1;
		currentThread->CreatePost(main->LastPostID , username, content);
	}
}

/* Mod+ */
void ForumNavigator::DeleteThread(int index) {
	if (currentThread == NULL && currentForum != NULL){
		currentForum->DeleteThread(index);
	}
}

void ForumNavigator::DeletePost(int index) {
	if (currentThread != NULL){
		currentThread->DeletePost(index);
	}
}

void ForumNavigator::MoveThread() {}

void ForumNavigator::MovePost() {}

void ForumNavigator::RenameThread(string new_title) {}

void ForumNavigator::SetSticky(int index) {}

void ForumNavigator::SetLocked(int index) {}

/* Admin_ */
Forum * ForumNavigator::CreateForum(string title) {
	if (currentForum != NULL) {
		return currentForum->CreateSubforum(title);
	} else {
		return main->CreateForum(title);
	}
}

void ForumNavigator::DeleteForum(int index) {
	if (currentThread == NULL && currentForum != NULL){
		currentForum->DeleteSubforum(index);
	} else if (currentThread == NULL && currentForum == NULL) {
		main->DeleteForum(index);
	}
}

void ForumNavigator::MoveForum() {}

void ForumNavigator::RenameForum(string new_title) {}

void ForumNavigator::ChangeUserRights(string username, int rights) {}

void ForumNavigator::DeleteUser(string username) {}

void ForumNavigator::RenameUser(string username, string new_name) {}

void ForumNavigator::ChangeUserPassword(string username, string password) {}

// DESTRUCTOR
ForumNavigator::~ForumNavigator() {
	back.Empty();
}

#pragma endregion