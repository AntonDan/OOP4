#include "stdafx.h"
#include "ForumManager.h"

using namespace std;

#pragma region ForumManager
// CONSTRUCTORS 
ForumManager::ForumManager(System * system, oList<User> * users) {
	main = system;
	this->users = users;
	currentForum = NULL;
	currentThread = NULL;
}

// GETTERS
System * ForumManager::GetMain() const { return main; }

Forum * ForumManager::GetCurrentForum() const { return currentForum; }

Thread * ForumManager::GetCurrentThread() const { return currentThread; }

// METHODS
// VISITOR
void ForumManager::VisitForum(int index) {
	/* If we are not inside a forum visit a System subforum
	* else visit a forum subforum */
	if (currentForum == NULL) {
		currentForum = main->GetForum(index);
	} else {
		currentForum = currentForum->GetForum(index);
	}
}

void ForumManager::VisitForum(Forum * forum) { currentForum = forum; }

void ForumManager::VisitThread(int index) {
	/* If we are inside a forum (threads do not exist directly under system) visit the thread */
	if (currentForum != NULL) {
		currentThread = currentForum->GetThread(index);
	}
}

void ForumManager::Back() {
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

void ForumManager::PrintCurrent() const {
	/* Print suitable data depending on were we are (inside a thread , a forum or a the main System) */
	if (currentThread != NULL) {
		cout << "Thread: " << currentThread->GetTitle() << "  by: " << currentThread->GetUserName() << endl;
	} else if (currentForum != NULL) {
		cout << "Forum: " << currentForum->GetTitle() << endl;
	} else {
		cout << main->GetTitle() << endl;
	}
}

void ForumManager::PrintContents() {
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

void ForumManager::PrintContent(int index) const {
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

// USER
void ForumManager::CreateThread(Forum * forum, int TID, int PID, string title, string username, string content) {
	/* Create a thread and a post inside it */
	(forum->CreateThread(TID, title, username))->CreatePost(PID, username, content);
}

void ForumManager::CreatePost(Thread * thread, int PID, string username, string content) {
	/* Create a post with given data */
	thread->CreatePost(PID, username, content);
}


// MODERATOR
void ForumManager::DeleteThread(Thread * thread) {
	/* Accesing thread's parent and deleting the given thread that it contains */
	(thread->GetParent())->DeleteThread(thread);
}

void ForumManager::DeletePost(Post * post) {
	/* Accesing post's parent and deleting the given post that it contains */
	(post->GetParent())->DeletePost(post);
}

void ForumManager::MoveThread(Thread * thread, Forum * destination) {
	/* Accesing thread's parent and removing the given thread that it contains, after that we add it to the destination.
	Finally, we assign the new parent to the moved thread */
	destination->AddThread((thread->GetParent())->RemoveThread(thread));
	thread->SetParent(destination);
}

void ForumManager::MovePost(Post * pos, Thread * destination) {
	/* Accesing post's parent and removing the given post that it contains, after that we add it to the destination.
	Finally, we assign the new parent to the moved post */
	destination->AddPost((pos->GetParent())->RemovePost(pos));
	pos->SetParent(destination);
}

void ForumManager::RenameThread(Thread * thread, string title) {
	thread->SetTitle(title);
}

void ForumManager::ChangeSticky(Thread * thread) {
	if (thread == NULL) return;
	thread->SetSticky(!thread->isSticky());
}

void ForumManager::ChangeLocked(Thread * thread) {
	if (thread == NULL) return;
	thread->SetLocked(!thread->isLocked());
}

// ADMINISTRATOR
void ForumManager::CreateForum(SF * destination, string title) {
	destination->CreateForum(title);
}

void ForumManager::DeleteForum(SF * forum, int index) {
	forum->DeleteForum(index);
}

void ForumManager::MoveForum(Forum * forum, SF * destination) {
	/* Accesing forum's parent and removing the given forum that it contains, after that we add it to the destination.
	Finally, we assign the new parent to the moved forum */ 
	if (forum == NULL || destination == NULL) return;
	destination->AddForum((forum->GetParent())->RemoveForum(forum));
	forum->SetParent(destination);
}

void ForumManager::RenameForum(Forum * forum, string title) {
	if (forum == NULL) return;
	forum->SetTitle(title);
}

void ForumManager::ChangeUserRights(string username , int rights) {
	for (int i = 0; i < users->GetLength(); ++i){
		if ((*users)[i]->GetUsername() == username){
			(*users)[i]->SetRights(rights);
		}
	}
}

void ForumManager::DeleteUser(string username) {
	/* Searching the user list until the given username is found, then deleting it */
	for (int i = 0; i < users->GetLength(); ++i){
		if ((*users)[i]->GetUsername() == username){
			delete users->Delete(i);
		}
	}
}

bool ForumManager::RenameUser(string username, string newname) {
	/* Searching the user list until the given username is found, then rename it or return false if new username already exists */
	for (int i = 0; i < users->GetLength(); ++i){
		if ((*users)[i]->GetUsername() == newname) return false;
	}

	for (int i = 0; i < users->GetLength(); ++i){
		if ((*users)[i]->GetUsername() == username){
			(*users)[i]->SetUsername(newname);
			return true;
		}
	}
	return false;
}

bool ForumManager::ChangeUserPassword(string username, string code) {				// username: name of the user's account		code: new passord to be assigned
	/* Searching the user list until the given username is found, then assign the new password it */
	for (int i = 0; i < users->GetLength(); ++i){
		if ((*users)[i]->GetUsername() == username){
			(*users)[i]->SetUsername(code);
			return true;
		}
	}
	return false;
}
#pragma endregion