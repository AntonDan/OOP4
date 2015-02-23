#ifndef USERS_INCLUDE_GUARD
#define USERS_INCLUDE_GUARD

#include "System.h"

using namespace std;

#pragma region Accounts

class Visitor {
protected:
	int rights;
private:

public:
	Visitor();

};

class User : public Visitor {
protected:
	int id;
	string username;

public:
	User(string );

	void CreateThread(Forum * , string , string );

	void CreatePost(Thread * , string );

	~User();
};

class Moderator : public User {
public:
	Moderator(string );

	void DeleteThread(Thread *);

	void DeletePost(Post * );

	void MoveThread(Thread * , Forum * );

	void MovePost(Post * , Thread * );

	void RenameThread(Thread *, string);

	void SetSticky(Thread * , bool );

	void SetLocked(Thread *, bool);
};

class Administrator : public Moderator {
public:
	Administrator(string);

	void CreateForum(Forum * , string );

	void CreateForum(System * , string );

	void DeleteForum(Forum * );

	void MoveForum(Forum * , void * );

	void RenameForum(Forum * , string );

	void ChangeUserRights(int );

	void DeleteUser(string );

	bool RenameUser(string );

	void ChangeUserPassword(string );

};

#pragma endregion

#endif