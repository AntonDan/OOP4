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
// GENERAL
User * ForumManager::Validate(string username, string password) {
	for (int i = 0; i < users->GetLength(); ++i) {
		if ((*users)[i]->GetUsername() == username) {
			if ((*users)[i]->GetPassword() == password) {
				return (*users)[i];
			}
		}
	}
	return NULL;
}

User * ForumManager::IDtoUser(int id) {
	for (int i = 0; i < users->GetLength(); ++i){
		if ((*users)[i]->GetId() == id) return (*users)[i];
	}
	return NULL;
}

User * ForumManager::FindUserbyName(string username) {
	for (int i = 0; i < users->GetLength(); ++i){
		if ((*users)[i]->GetUsername() == username) return (*users)[i];
	}
	return NULL;
}

int ForumManager::UsertoID(string username) {
	for (int i = 0; i < users->GetLength(); ++i){
		if ((*users)[i]->GetUsername() == username) return (*users)[i]->GetId();
	}
	return -1;
}

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

void ForumManager::VisitThread(int id) {
	/* If we are inside a forum (threads do not exist directly under system) visit the thread */
	if (currentForum != NULL) {
		currentThread = currentForum->GetThreadByID(id);
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
		if (currentThread != NULL) {
			currentForum = (Forum *)currentThread->GetParent(); // type conversion in this case is considered safe since we made sure that cForums parent is not the System
			currentThread = NULL;
		} else {
			currentForum = (Forum *)currentForum->GetParent(); // type conversion in this case is considered safe since we made sure that cForums parent is not the System
		}
	}
}

void ForumManager::PrintCurrent() const {
	/* Print suitable data depending on were we are (inside a thread , a forum or a the main System) */
	if (currentThread != NULL) {
		cout << "Thread: " << currentThread->GetTitle() << "  by: " << currentThread->GetUsername() << endl;
	} else if (currentForum != NULL) {
		cout << "Forum: " << currentForum->GetTitle() << endl;
	} else {
		cout << main->GetTitle() << endl;
	}
}

void ForumManager::PrintContents(int type) {
	/* Depending on case print either the posts inside the current Thread
	*  or the subforums and threads inside the forum
	*  or the main forums under the System */
	oList<Forum> * forums;
	if (currentThread != NULL) {
		oList<Post> * posts = currentThread->GetPosts();
		for (int i = 0; i < posts->GetLength(); ++i) {
			cout << "User: " << (*posts)[i]->GetUser() << "  content: " << (*posts)[i]->GetContent() << "  [ID: " << (*posts)[i]->GetID() << "] " << endl;
		}
	} else if (currentForum != NULL) {
		if (type % 2 == 0) {
			forums = currentForum->GetForums();
			for (int i = 0; i < forums->GetLength(); ++i) {
				cout << (*forums)[i]->GetTitle() << "[ID: " << i + 1 << "] " << endl;

			}
		} 
		if (type > 0) {
			oList<Thread> * threads = currentForum->GetThreads();
			for (int i = 0; i < threads->GetLength(); ++i) {
				cout << "Thread " << i + 1 << ": " << (*threads)[i]->GetTitle() << "  by: " << (*threads)[i]->GetUsername() << "  [ID: " << (*threads)[i]->GetID() << "] " << endl;
			}
		}
	} else {
		forums = main->GetForums();
		for (int i = 0; i < forums->GetLength(); ++i) {
			cout << (*forums)[i]->GetTitle() << "  [ID: " << i + 1 << "] " << endl;
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
		if (currentForum->GetThread(index) != NULL) cout << "Thread " << index + 1 << ": " << currentForum->GetThread(index)->GetTitle() << "  by: " << currentForum->GetThread(index)->GetUsername() << endl;
	} else {
		cout << main->GetTitle() << endl;
	}
}

bool ForumManager::Register(string username, string password) {
	for (int i = 0; i < users->GetLength(); ++i){
		/* If you want to check if a smaller ID is available (for example when a user is deleted) all you have to do is run a while loop until you find an id gab in the user list
		 * Note: For this to work the list has to be sorted */
		if ((*users)[i]->GetUsername() == username) return false; 
	}
	users->Add(new User(users->GetLength() + 1, username, password, 1));
	return true;
}

// USER
void ForumManager::CreateThread(Forum * forum, string title, string username, string content) {
	/* Create a thread and a post inside it */
	main->LastPostID += 1;
	main->LastThreadID += 1;
	(forum->CreateThread(main->LastThreadID, title, username))->CreatePost(main->LastPostID, username, content);
}

void ForumManager::CreatePost(Thread * thread, string username, string content) {
	/* Create a post with given data */
	main->LastPostID += 1;
	thread->CreatePost(main->LastPostID, username, content);
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

void ForumManager::DeleteForum(Forum * forum) {
	(forum->GetParent())->DeleteForum(forum);
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

void ForumManager::PrintUsers() const {
	for (int i = 0; i < users->GetLength(); ++i) {
		cout << "ID: " << (*users)[i]->GetId() << "  | Rights: " << (*users)[i]->GetRights() << "  | Username: " << (*users)[i]->GetUsername() << endl;
	}
}

bool ForumManager::ChangeUserRights(string username , int rights) {
	for (int i = 0; i < users->GetLength(); ++i){
		if ((*users)[i]->GetUsername() == username){
			(*users)[i]->SetRights(rights);
			return true;
		}
	}
	return false;
}

bool ForumManager::DeleteUser(string username) {
	/* Searching the user list until the given username is found, then deleting it */
	for (int i = 0; i < users->GetLength(); ++i){
		if ((*users)[i]->GetUsername() == username){
			delete users->Delete(i);
			return true;
		}
	}
	return false;
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
			(*users)[i]->SetPassword(code);
			return true;
		}
	}
	return false;
}

void ForumManager::Save(ofstream & forumfile, ofstream & threadfile, ofstream & postfile, ofstream & userfile) {
	SaveSystem(forumfile, threadfile, postfile);
	SaveUsers(userfile);
}

void ForumManager::SaveSystem(ofstream & forumfile, ofstream & threadfile, ofstream &postfile) {
	main->Save(forumfile, threadfile, postfile);
}

void ForumManager::SaveUsers(ofstream & userfile) {
	for (int i = 0; i < users->GetLength(); ++i){
		userfile << (*users)[i]->GetId() << " " << (*users)[i]->GetUsername() << " " << (*users)[i]->GetPassword() << " " << (*users)[i]->GetRights() << endl;
	}
}
#pragma endregion