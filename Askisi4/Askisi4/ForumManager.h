#ifndef FM_INCLUDE_GUARD
#define FM_INCLUDE_GUARD
#pragma region Forum Navigator

#include "System.h"
#include "Users.h"

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
	void VisitForum(int);

	void VisitForum(Forum *);

	void VisitThread(int);

	void Back();

	void PrintCurrent() const;

	void PrintContents();

	void PrintContent(int) const;

	void CreateThread(Forum *, int, int, string, string, string);

	void CreatePost(Thread *, int, string, string);

	void DeleteThread(Thread *);

	void DeletePost(Post *);

	void MoveThread(Thread *, Forum *);

	void MovePost(Post *, Thread *);

	void RenameThread(Thread *, string);

	void ChangeSticky(Thread *);

	void ChangeLocked(Thread *);

	void CreateForum(SF *, string);

	void DeleteForum(SF *, int);

	void MoveForum(Forum *, SF *);

	void RenameForum(Forum *, string);

	void ChangeUserRights(string, int);

	void DeleteUser(string);

	bool RenameUser(string, string);

	bool ChangeUserPassword(string, string);
};

#pragma endregion
#endif