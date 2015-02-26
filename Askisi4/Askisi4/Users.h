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
	string password;
	string username;

public:
	User(string, string);

	void CreateThread(Forum *, int, int, string, string, string);

	void CreatePost(Thread * , int, string, string );

	void SetUsername(string newname);

	void SetPassword(string code);

	string GetUsername() const;

	string GetPassword() const;

	~User();
};

class Moderator : public User {
public:
	Moderator(string, string);

	void DeleteThread(Thread  *);

	void DeletePost(Post * post);

	void MoveThread(Thread * , Forum * );

	void MovePost(Post * , Thread * );

	void RenameThread(Thread *, string);

	void SetSticky(Thread * , bool );

	void SetLocked(Thread *, bool);
};

class Administrator : public Moderator {
public:
	Administrator(string, string);

	void CreateForum(SF * , string );

	void DeleteForum(SF *, int );

	void MoveForum(Forum * , SF * );

	void RenameForum(Forum * , string );

	void ChangeUserRights(int );

	void DeleteUser(string, oList<User> &);

	bool RenameUser(string, string, oList<User> &);

	bool ChangeUserPassword(string, string, oList<User> &);

};

#pragma endregion

#endif