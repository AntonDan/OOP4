#ifndef FM_INCLUDE_GUARD
#define FM_INCLUDE_GUARD
#pragma region Forum Navigator

#include "System.h"
#include "Users.h"

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

	void VisitForum(Forum *);

	void VisitThread(int);

	void Back();

	void PrintCurrent() const;

	void PrintContents();

	void PrintContent(int) const;

	void DeleteThread(Thread *);

	void DeletePost(Post *);

	void MoveThread(Thread *, Forum *);

	void MovePost(Post *, Thread *);

	void RenameThread(Thread *, string);

	void SetSticky(Thread *, bool);

	void SetLocked(Thread *, bool);

	void CreateForum(SF *, string);

	void DeleteForum(SF *, int);

	void MoveForum(Forum *, SF *);

	void RenameForum(Forum *, string);

	void ChangeUserRights(User * , int);

	void DeleteUser(string, oList<User> &);

	bool RenameUser(string, string, oList<User> &);

	bool ChangeUserPassword(string, string, oList<User> &);
};

#pragma endregion
#endif