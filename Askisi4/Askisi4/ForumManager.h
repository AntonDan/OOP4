#ifndef FM_INCLUDE_GUARD
#define FM_INCLUDE_GUARD

#include "System.h"
#include "Users.h"

#pragma region Forum Navigator

class ForumManager {
private:
	System * main;
	Forum  * currentForum;
	Thread * currentThread;
	oList<User> * users;

public:
	// CONSTRUCTORS 
	ForumManager(System *, oList<User> *);

	// GETTERS
	System * GetMain() const;
	Forum * GetCurrentForum() const;
	Thread * GetCurrentThread() const;

	// METHODS 
	/* so tidy , much OCD , wow */
	User * Validate(string, string);

	string IDtoUser(int) const;

	User * FindUserbyName(string);

	int UsertoID(string);

	void VisitForum(int);

	void VisitForum(Forum *);

	void VisitThread(int);

	void Back();

	void PrintCurrent() const;

	void PrintContents(int = 0);

	void PrintContent(int) const;

	bool Register(string user, string password);

	void CreateThread(Forum *, string, string, string);

	void CreatePost(Thread *, string, string);

	void DeleteThread(Thread *);

	void DeletePost(Post *);

	void MoveThread(Thread *, Forum *);

	void MovePost(Post *, Thread *);

	void RenameThread(Thread *, string);

	void ChangeSticky(Thread *);

	void ChangeLocked(Thread *);

	void CreateForum(SF *, string);

	void DeleteForum(Forum *);

	void MoveForum(Forum *, SF *);

	void RenameForum(Forum *, string);

	void PrintUsers() const;

	bool ChangeUserRights(string, int);

	bool DeleteUser(string);

	bool RenameUser(string, string);

	bool ChangeUserPassword(string, string);
	
	void Save(ofstream &, ofstream &, ofstream &, ofstream &);
	
	void SaveSystem(ofstream &, ofstream &, ofstream &);

	void SaveUsers(ofstream &);

};
#pragma endregion

#endif