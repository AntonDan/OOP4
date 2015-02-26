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
	//CONSTRUCTOR
	User(string, string);

	//METHODS
	void CreateThread(Forum *, int, int, string, string, string);

	void CreatePost(Thread * , int, string, string );

	//SETTERS
	void SetUsername(string newname);

	void SetPassword(string code);

	void SetRights(int value);

	//GETTERS
	string GetUsername() const;

	string GetPassword() const;
		
	int GetRights() const;

	//DESTRUCTOR
	~User();
};

class Moderator : public User {
public:
	//CONSTRUCTOR
	Moderator(string, string);

	//METHODS
	void DeleteThread(Thread  *);

	void DeletePost(Post * post);

	void MoveThread(Thread * , Forum *);

	void MovePost(Post * , Thread * );

	void RenameThread(Thread *, string);

	//SETTERS
	void SetSticky(Thread * , bool );

	void SetLocked(Thread *, bool);
};

class Administrator : public Moderator {
public:
	//CONSTRUCTOR
	Administrator(string, string);

	//METHODS
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