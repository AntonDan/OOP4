#include "stdafx.h"
#include "SystemSave.h"

using namespace std;

/* The Save functions take care of (guess what?) saving the given system's condition in the given files (forum file , thread file , post file) */
void SaveSystem(ofstream & forumFile, ofstream & threadFile, ofstream & postFile, System & system) {
	/* Get the main forums and save the state of each one of them and their subforums
	* also initiate a path for every one of the above forums (a path should start a single integer and unfold like this: 2.1.5.3.1) */
	oList<Forum> * forums;
	forums = system.GetForums();
	string path;
	for (int i = 0; i < forums->GetLength(); ++i) {
		path = to_string(i + 1);
		forumFile << path << " " << (*forums)[i]->GetTitle() << endl;   // Save main forum
		SaveSubforums(forumFile, threadFile, postFile, (*forums)[i], path); // Save subforums
	}

}

void SaveSubforums(ofstream & forumFile, ofstream & threadFile, ofstream & postFile, Forum * forum, string path) {
	SaveThreads(threadFile, postFile, forum, path);  // Save threads of file (this command an also be blased at the end of the function)
	/* Get the subforums of the given forum and save their state as well as their subforums state */
	oList<Forum> * forums;
	forums = forum->GetForums();
	for (int i = 0; i < forums->GetLength(); ++i) {
		Forum * cForum = (*forums)[i];
		string temppath = path + "." + to_string(i + 1); // Update path
		forumFile << temppath << " " << cForum->GetTitle() << endl; // Save forum
		SaveSubforums(forumFile, threadFile, postFile, cForum, temppath); // Save forum's subforums
	}
}

void SaveThreads(ofstream & threadFile, ofstream & postFile, Forum * forum, string path) {
	/* Get the threads inside the given forum and save their data as well as the posts that they contain */
	oList<Thread> * threads;
	threads = forum->GetThreads();
	for (int i = 0; i < threads->GetLength(); ++i) {
		Thread * cThread = (*threads)[i];
		string tempath = path + "." + to_string(cThread->GetID()); // Update path
		threadFile << tempath << " " << ((cThread->isSticky()) ? ("S") : ("N")) << " " << ((cThread->isLocked()) ? ("L") : ("N")) << " " << atoi(cThread->GetUserName().c_str()) << " " << cThread->GetTitle() << endl; // Save thread
		SavePosts(postFile, cThread, tempath); // Save posts inside thread
	}
}

void SavePosts(ofstream & postFile, Thread * thread, string path) {
	/* Get a list of the posts inside the thread and save their contents/data  */
	oList<Post> * posts;
	posts = thread->GetPosts();
	for (int i = 0; i < posts->GetLength(); ++i) {
		Post * cPost = (*posts)[i];
		postFile << path + "." << cPost->GetID() << " " << atoi(cPost->GetUser().c_str()) << " " << cPost->GetContent() << endl;
	}
}

void SaveUsers(ofstream & userfile, oList<User> & users){
	for (int i = 0; i < users.GetLength(); ++i){
		userfile << users[i]->GetId() << " " << users[i]->GetUsername() << " " << users[i]->GetPassword() << " " << users[i]->GetRights() << endl;
	}
}
