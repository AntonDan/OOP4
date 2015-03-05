#include "stdafx.h"
#include <cstdio>
#include "ForumManager.h"
#include "Interface.h"


using namespace std;

#pragma region main
int main(int argc, char * argv[] ) {
	/* 1) Create main system and user list
	 * 2) Open to read database files
	 * 3) Declare needed variables
	 */
	System mainSystem;
	oList<User> users;

	/*
	ifstream iforumfile ("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/forum.txt");
	ifstream ithreadfile("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/thread.txt");
	ifstream ipostfile  ("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/post.txt");
	ifstream iuserfile  ("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/user.txt");
	*/
	ifstream iforumfile("C:/Users/Antonis/Desktop/OOP4/OOP4/Askisi4/Debug/Databases/forum.txt");
	ifstream ithreadfile("C:/Users/Antonis/Desktop/OOP4/OOP4/Askisi4/Debug/Databases/thread.txt");
	ifstream ipostfile("C:/Users/Antonis/Desktop/OOP4/OOP4/Askisi4/Debug/Databases/post.txt");
	ifstream iuserfile("C:/Users/Antonis/Desktop/OOP4/OOP4/Askisi4/Debug/Databases/user.txt");

	string line , path , name;
	Forum * tempForum = NULL;
	int ID;
	unsigned int i = 0;

	ForumManager fma(&mainSystem, &users);

#pragma region User Creator
	string username, password;
	int id, rights;

	while (getline(iuserfile, line)){
		Parse(line, "I S S I", false, ' ', 4, &id, &username, &password, &rights); // Parse: Integer String String Integer (id , username , password and user rights)
		users.Add(new User(id, username, password, rights)); // Create User and add him to the list
	}
#pragma endregion

#pragma region Forum creator
	/* Parsing forum file and creating forums */
	while (getline(iforumfile, line)) {
		Parse(line, "S S", true , ' ', 2, &path, &name); // Parse: two strings (path and forum title)
		vector<string> vpath = Split(path, '.');
		if (vpath.size() > 1) { // If we aren't under the system
			/* Navigating to the forum parent */
			Forum * tempForum = mainSystem.GetForum(atoi(vpath[0].c_str()));
			for (i = 1; i < vpath.size() - 1; ++i) {
				tempForum = tempForum->GetForum(atoi(vpath[i].c_str()));
			}
			tempForum->CreateForum(name); // Create forum
		} else {
			mainSystem.CreateForum(name); // Create forum
		}
	}
#pragma endregion

#pragma region Thread creator	
	char sticky, locked;
	/* Parsing thread file and creating threads */
	while (getline(ithreadfile, line)) {
		Parse(line, "S C C I S", true, ' ', 5, &path, &sticky, &locked, &ID , &name); // Parse: String Char Char Int String (path , sticky , locked , user ID and thread title)
		vector<string> vpath = Split(path, '.');
		/* Navigate to thread's parent forum */
		Forum * tempForum = mainSystem.GetForum(atoi(vpath[0].c_str()));
		for (i = 1; i < vpath.size() - 1; ++i) {
			tempForum = tempForum->GetForum(atoi(vpath[i].c_str()));
		}
		tempForum->CreateThread(atoi(vpath[vpath.size() - 1].c_str()), name, (fma.IDtoUser(ID) != NULL) ? (fma.IDtoUser(ID)->GetUsername()):("Unknown"), ((sticky == 'S') + (locked == 'L') * 2));		// Create Thread 
	}
#pragma endregion

#pragma region Post creator
	string content;
	/* Parsing post file and creating threads */
	while (getline(ipostfile, line)) {
		Parse(line, "S I S", true, ' ', 3, &path, &ID, &content);  // Parse: String int String (path , user ID and comment content)
		vector<string> vpath = Split(path, '.');
		Forum * tempForum = mainSystem.GetForum(atoi(vpath[0].c_str()));
		/* Navigate to parent forum */
		for (i = 1; i < vpath.size() - 2; ++i) {
			tempForum = tempForum->GetForum(atoi(vpath[i].c_str()));
		}
		/* Navigate to parent thread */
		Thread * tempThread = tempForum->GetThreadByID(atoi(vpath[i++].c_str()));
		tempThread->CreatePost(atoi(vpath[i].c_str()), (fma.IDtoUser(ID) != NULL) ? (fma.IDtoUser(ID)->GetUsername()) : ("Unknown"), content); // Create post
	}
#pragma endregion


	if (argc > 1) {
		if (argv[1] = "-R") {
			if (!RegistrationMenu(fma)) return 0;
		}
	} else if (!WelcomeMenu(fma)) return 0;

	/* close input files */
	iforumfile.close();
	ithreadfile.close();
	ipostfile.close();
	iuserfile.close();

	/* Open files for writting */
	/*	
	ofstream oforumfile("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/forum.txt");
	ofstream othreadfile("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/thread.txt");
	ofstream opostfile("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/post.txt");
	ofstream ouserfile("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/user.txt");
	*/
	ofstream oforumfile("C:/Users/Antonis/Desktop/OOP4/OOP4/Askisi4/Debug/Databases/forum.txt");
	ofstream othreadfile("C:/Users/Antonis/Desktop/OOP4/OOP4/Askisi4/Debug/Databases/thread.txt");
	ofstream opostfile("C:/Users/Antonis/Desktop/OOP4/OOP4/Askisi4/Debug/Databases/post.txt");
	ofstream ouserfile("C:/Users/Antonis/Desktop/OOP4/OOP4/Askisi4/Debug/Databases/user.txt");
	fma.Save(oforumfile, othreadfile, opostfile, ouserfile);

	/* close output files */
	oforumfile.close();
	othreadfile.close();
	opostfile.close();
	ouserfile.close();

	return 0;
}
#pragma endregion
