#ifndef SYSTEM_INCLUDE_GUARD
#define SYSTEM_INCLUDE_GUARD

#include <iostream>
#include <cstdlib>
#include <string>
#include "DS.h"

using namespace std;

#pragma region FD
// forward declarations
class Thread;
class Forum;
class System;
class SF;
#pragma endregion

#pragma region SystemComponents

#pragma region Post

class Post {
private:
	int id;
	string username;
	string content;

	Thread * parent;

public:
	Post(int, string , string , Thread *);
	void SetUser(string );
	void SetContent(string );
	int GetID() const;
	string GetUser() const;
	string GetContent() const;
	Thread * GetParent() const;
	~Post();
};

#pragma endregion

#pragma region Thread 
class Thread {
private:
	int id;
	string title;
	string username;
	oList<Post> posts;

	bool sticky;
	bool locked;

	Forum * parent;

public:
	// CONSTRUCTORS
	Thread(int, string, string, Forum * , int = 0);

	// SETTERS 
	void SetTitle(string);
	void SetSticky(bool);
	void SetLocked(bool);
	void SetParent(Forum *);

	// GETTERS
	int GetID() const;
	string GetTitle() const;
	string GetUserName() const;
	oList<Post> * GetPosts();
	Post * GetPost(int) const;
	Post * GetPostByID(int) const;
	Forum * GetParent() const;
	bool isSticky() const;
	bool isLocked() const;

	// METHODS
	void CreatePost(int, string , string);
	void DeletePost(int index);
	void DeletePost(Post *);
	Post * RemovePost(Post *);
	void AddPost(Post *);

	// DESTRUCTORS
	~Thread();
};
#pragma endregion

#pragma region SF
class SF {
protected:
	oList<Forum> forums;

public:
	// GETTERS
	virtual Forum * GetForum(int) const;
	virtual oList<Forum> * GetForums();

	// METHODS
	Forum * CreateForum(string);
	void AddForum(Forum *);
	void DeleteForum(int);
	void DeleteForum(Forum *);
	Forum * RemoveForum(Forum *);
	
};
#pragma endregion

#pragma region Forum
class Forum : public SF {
private:
	string title;
	oList<Thread> threads;

	SF * parent;

public:
	// CONSTRUCTOR
	Forum(string , SF *);

	// SETTERS
	void SetTitle(string);
	void SetParent(SF *);

	// GETTERS 
	string GetTitle() const;
	SF * GetParent() const;
	Thread * GetThread(int ) const;
	Thread * GetThreadByID(int) const;
	oList<Thread> * GetThreads();

	// METHODS
	Thread * CreateThread(int, string , string , int = 0);
	void DeleteThread(int);
	void DeleteThread(Thread *);
	Thread * RemoveThread(Thread *);
	void AddThread(Thread *);
	
	// DESTRUCTOR
	~Forum();
};
#pragma endregion

#pragma region System
class System : public SF {
private:
	const string title; 
	oList<Forum> forums; // 

public:
	// VARS
	int LastThreadID;
	int LastPostID;

	// CONSTRUCTOR
	System();
	
	// GETTERS 
	string GetTitle() const;

	// DESTRUCTOR
	~System();
};
#pragma endregion

#pragma region Forum Navigator

class ForumNavigator {
private:
	System * main;
	Forum  * currentForum;
	Thread * currentThread;

public:
	// CONSTRUCTORS 
	ForumNavigator(System *);

	// GETTERS
	System * GetMain() const;

	Forum * GetCurrentForum() const;

	Thread * GetCurrentThread() const;

	// METHODS
	void VisitForum(int);

	void VisitForum(Forum * );

	void VisitThread(int );

	void Back();

	void PrintCurrent() const;

	void PrintContents();

	void PrintContent(int ) const;
};

#pragma endregion

#pragma endregion

#endif