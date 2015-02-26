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

void Post::SetParent(Thread * parent){
	this->parent = parent;
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
	/* info = 0 : sticky = false - locked = false 
	 * info = 1 : sticky = true  - locked = false 
	 * info = 2 : sticky = false - locked = true 
	 * info = 3 : sticky = true  - locked = true   
	 */
	sticky = (info % 2 == 1);
	locked = (info >= 2);
	this->parent = parent;
}

// SETTERS 
void Thread::SetTitle(string new_title) { title = new_title; }

void Thread::SetSticky(bool value) { sticky = value; }

void Thread::SetLocked(bool value) { locked = value; }

void Thread::SetParent(Forum * forum) { parent = forum; }

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
	/* Pass this (pointer to this Thread) as the posts parent */
	posts.Add(new Post(id, user, content, this));
}

void Thread::DeletePost(int index) {
	delete posts.Delete(index);
}

void Thread::DeletePost(Post * post) {
	delete posts.Delete(post);
}

Post * Thread::RemovePost(Post * post) {
	/* The difference between delete and remove is that remove just substracts the Post from the forum and returns it
	 * Delete destroys the post permanently */
	return posts.Delete(post);
}

void Thread::AddPost(Post * post) {
	posts.Add(post);
}

// DESTRUCTORS
Thread::~Thread() {
	/* This destructor is unecessary and can be removed */
	title = "";
	username = "";

	posts.~oList();
}

#pragma endregion

#pragma region SF
// GETTERS
Forum * SF::GetForum(int index) const { return forums[index - 1]; }

oList<Forum> * SF::GetForums() { return &forums; }

// METHODS
Forum * SF::CreateForum(string title) {
	/* The reason why we create the forum into a seperate variable before adding it to the subforum list is because 
	 * we want to return it afterwards */
	Forum * newforum = new Forum(title, this);
	forums.Add(newforum);
	return newforum;
}

void SF::DeleteForum(int index) {
	/* Making sure forum exists before deleting it (not necessary but just in case) */
	Forum * temp = forums.Delete(index);
	if (temp != NULL) {
		delete temp;
	}
}

Forum * SF::RemoveForum(Forum * forum) {
	return forums.Delete(forum);
}

void SF::AddForum(Forum * forum) {
	forums.Add(forum);
}
#pragma endregion

#pragma region Forum
// CONSTRUCTOR
Forum::Forum(string title, SF * parent ) {
	this->title = title;
	this->parent = parent;
}

// SETTERS
void Forum::SetTitle(string new_title) { title = new_title; }

void Forum::SetParent(SF * parent) { this->parent = parent; }

// GETTERS 
string Forum::GetTitle() const { return title; }

Thread * Forum::GetThread(int index) const { return threads[index-1]; } // The reason for the index - 1 is because within the files (databases) the threads start with an id of 1 instead of 0

Thread * Forum::GetThreadByID(int ID) const {
	/* Scan in all threads and if you find one that has an ID same with the given one , return it , else return NULL  */
	for (int i = 0; i < threads.GetLength(); ++i) {
		if (threads[i]->GetID() == ID) {
			return threads[i];
		}
	}
	return NULL;
}

oList<Thread> * Forum::GetThreads() { return &threads; } // Cannot be a constant (const) function since we use &threads

SF * Forum::GetParent() const { return parent; }


// METHODS
Thread * Forum::CreateThread(int id, string title, string username, int info) {
	/* Thread is stored into a temporary variable so it can be returned after added in the list */
	Thread * newthread = new Thread(id, title, username, this, info);
	threads.Add(newthread);
	return newthread;
}

void Forum::DeleteThread(int index) {
	/* Making sure thread exists before deleting it */
	Thread * temp = threads.Delete(index);
	if (temp != NULL) {
		delete temp;
	}
}

void Forum::DeleteThread(Thread * thread) {
	/* Making sure thread exists before deleting it */
	Thread * temp = threads.Delete(thread);
	if (temp != NULL) {
		delete temp;
	}
}

Thread * Forum::RemoveThread(Thread * thread) {
	/* Remove and return thread without deleting it */
	return threads.Delete(thread);
}

void Forum::AddThread(Thread * thread) {
	threads.Add(thread);
}
 
// DESTRUCTOR
Forum::~Forum() {
	/* This destructor is unecessary and can be removed */
	forums.~oList();
	threads.~threads();
	title = "";
}
#pragma endregion

#pragma region System
// CONSTRUCTOR
System::System() : title("D.I.T.Lists") , LastThreadID(0) , LastPostID(0) {} // Initializing class variables

// GETTERS 
string System::GetTitle() const { return title; }

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
void ForumNavigator::VisitForum(int index) {
	/* If we are not inside a forum visit a System subforum 
	 * else visit a forum subforum */
	if (currentForum == NULL) {
		currentForum = main->GetForum(index);
	} else {
		currentForum = currentForum->GetForum(index);
	}
}

void ForumNavigator::VisitForum(Forum * forum) { currentForum = forum; }

void ForumNavigator::VisitThread(int index) {
	/* If we are inside a forum (threads do not exist directly under system) visit the thread */
	if (currentForum != NULL) {
		currentThread = currentForum->GetThread(index);
	}
}

void ForumNavigator::Back() {
	/* If by going back you return to the main system both set both cForum and cThread to NULL 
	 * else just set cThread = NULL (since a thread doesnt exist inside a thread) and move up a forum */
	if (currentForum == NULL) return; // GUARD
	if (System * par = dynamic_cast<System *>(currentForum->GetParent())) {
		currentForum = NULL;
		currentThread = NULL;
	} else {
		currentThread = NULL;
		currentForum = (Forum *)currentForum->GetParent(); // type conversion in this case is considered safe since we made sure that cForums parent is not the System
	}
}

void ForumNavigator::PrintCurrent() const {
	/* Print suitable data depending on were we are (inside a thread , a forum or a the main System) */
	if (currentThread != NULL) {
		cout << "Thread: " << currentThread->GetTitle() << "  by: " << currentThread->GetUserName() << endl;
	} else if (currentForum != NULL) {
		cout << "Forum: " << currentForum->GetTitle() << endl;
	} else {
		cout << main->GetTitle() << endl;
	}
}

void ForumNavigator::PrintContents() {
	/* Depending on case print either the posts inside the current Thread 
	*  or the subforums and threads inside the forum 
	*  or the main forums under the System */
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
	/* If we are under a thread print the content of the post
	 * If we are under a post print the data of the thread and subforum 
	 * If we are under the system print the data of the main forum 
	 */
	if (currentThread != NULL) {
		cout << "Post: " << index + 1 << ": " << "User: " << currentThread->GetPost(index)->GetUser() << "  content: " << currentThread->GetPost(index)->GetContent() << endl;
	} else if (currentForum != NULL) {
		if (currentForum->GetForum(index) != NULL) 	cout << "Forum " << index + 1 << ": " << currentForum->GetForum(index)->GetTitle() << endl;
		if (currentForum->GetThread(index) != NULL) cout << "Thread " << index + 1 << ": " << currentForum->GetThread(index)->GetTitle() << "  by: " << currentForum->GetThread(index)->GetUserName() << endl;
	} else {
		cout << main->GetTitle() << endl;
	}
}

#pragma endregion