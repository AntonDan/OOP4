#include "stdafx.h"
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

string Thread::GetUsername() const { return username; }

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

void Thread::Save(ofstream & postFile, string path) {
	/* Get a list of the posts inside the thread and save their contents/data  */
	for (int i = 0; i < posts.GetLength(); ++i) {
		Post * cPost = posts[i];
		postFile << path + "." << cPost->GetID() << " " << atoi(cPost->GetUser().c_str()) << " " << cPost->GetContent() << endl;
	}
}

// DESTRUCTORS
Thread::~Thread() {
	/* This destructor is unecessary and can be removed */
	title = "";
	username = "";
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
	Forum * temp = forums.Delete(index-1);
	if (temp != NULL) {
		delete temp;
	}
}

void SF::DeleteForum(Forum * forum) {
	/* Making sure forum exists before deleting it (not necessary but just in case) */
	Forum * temp = forums.Delete(forum);
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

void Forum::Save(ofstream & forumFile, ofstream & threadFile, ofstream & postFile, string path) {
	SaveSubforums(forumFile, threadFile, postFile, path);
	SaveThreads(threadFile, postFile, path);
}
 
void Forum::SaveSubforums(ofstream & forumFile, ofstream & threadFile, ofstream & postFile, string path) {
	/* Get the subforums of the given forum and save their state as well as their subforums state */
	for (int i = 0; i < forums.GetLength(); ++i) {
		Forum * cForum = forums[i];
		string temppath = path + "." + to_string(i + 1); // Update path
		forumFile << temppath << " " << cForum->GetTitle() << endl; // Save forum
		cForum->Save(forumFile, threadFile, postFile, temppath); // Save forum's subforums
	}
}

void Forum::SaveThreads(ofstream & threadFile, ofstream & postFile, string path) {
	/* Get the threads inside the given forum and save their data as well as the posts that they contain */
	for (int i = 0; i < threads.GetLength(); ++i) {
		Thread * cThread = threads[i];
		string tempath = path + "." + to_string(cThread->GetID()); // Update path
		threadFile << tempath << " " << ((cThread->isSticky()) ? ("S") : ("N")) << " " << ((cThread->isLocked()) ? ("L") : ("N")) << " " << atoi(cThread->GetUsername().c_str()) << " " << cThread->GetTitle() << endl; // Save thread
		cThread->Save(postFile, tempath); // Save posts inside thread
	}
}

// DESTRUCTOR
Forum::~Forum() {
	title = "";
}
#pragma endregion

#pragma region System
// CONSTRUCTOR
System::System() : title("D.I.T.Lists") , LastThreadID(0) , LastPostID(0) {} // Initializing class variables

// GETTERS 
string System::GetTitle() const { return title; }

// METHODS
void System::Save(ofstream & forumFile, ofstream & threadFile, ofstream & postFile) {
	/* Get the main forums and save the state of each one of them and their subforums
	* also initiate a path for every one of the above forums (a path should start a single integer and unfold like this: 2.1.5.3.1) */
	string path;
	for (int i = 0; i < forums.GetLength(); ++i) {
		path = to_string(i + 1);
		forumFile << path << " " << forums[i]->GetTitle() << endl;   // Save main forum
		forums[i]->Save(forumFile, threadFile, postFile, path); // Save subforums
	}

}
#pragma endregion


