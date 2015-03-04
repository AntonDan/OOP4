#include "stdafx.h"
#include <cstdio>
#include "ForumManager.h"
#include "Interface.h"


using namespace std;

#pragma region Function

User * Validate(string username, string password, oList<User> const & users) {
	for (int i = 0; i < users.GetLength(); ++i) {
		if (users[i]->GetUsername() == username) {
			if (users[i]->GetPassword() == password) {
				return users[i];
			}
		}
	}
	return NULL;
}
#pragma endregion

#pragma region main
int main(void) {
	/* 1) Create main system 
	 * 2) Open to read database files
	 * 3) Declare needed variables
	 */
	System mainSystem;
	ifstream iforumfile ("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/forum.txt");
	ifstream ithreadfile("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/thread.txt");
	ifstream ipostfile  ("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/post.txt");
	ifstream iuserfile  ("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/user.txt");

	string line , path , name;
	Forum * tempForum = NULL;
	int ID;
	unsigned int i = 0;

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
		// Create Thread
		// Find username based by ID
		tempForum->CreateThread(atoi(vpath[vpath.size() - 1].c_str()), name , to_string(ID) , ((sticky=='S') + (locked=='L')*2) );
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
		tempThread->CreatePost(atoi(vpath[i].c_str()) , to_string(ID), content ); // Create post
	}
#pragma endregion

#pragma region User Creator
	oList<User> users;
	string username, password;
	int id, rights;

	while (getline(iuserfile, line)){
		Parse(line, "I S S I", false, ' ',  4, &id, &username, &password, &rights);
		users.Add(new User(id, username, password, rights));
	}
#pragma endregion

	User * currentUser = NULL;
	
	cout << "\t===========================\n\t||Welcome to D.I.T Lists!|| \n\t===========================\n\n"
		<< "Please type in username and password \n";
	do {
		cout << "Username: ";
		cin >> username;
		cout << "Password: ";
		cin >> password;

		currentUser = Validate(username, password, users);

		if (currentUser == NULL) {
			cout << "Incorrect username or password. \n Please try again." << endl;
		}
	} while (currentUser == NULL);

	cout << "Welcome " << currentUser->GetUsername() << "!" << endl;
	cout << "You belong in the category: " << ((currentUser->GetRights()>0) ? ((currentUser->GetRights() > 1) ? ((currentUser->GetRights() > 2) ? ("Administrator") : ("Moderator")) : ("User")) : ("Visitor")) << endl; // ONE LINE CODING FTW ! \\(* O *)//

	
	
	ForumManager fma(&mainSystem, &users);

	if (!MainMenu(fma, currentUser)) return 0;

	/* close input files */
	iforumfile.close();
	ithreadfile.close();
	ipostfile.close();
	iuserfile.close();

	/* Open files for writting */
	ofstream oforumfile("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/forum.txt");
	ofstream othreadfile("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/thread.txt");
	ofstream opostfile("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/post.txt");
	ofstream ouserfile("C:/Users/Mertiko/Desktop/OOP4/Askisi4/Debug/Databases/user.txt");

	fma.Save(oforumfile, othreadfile, opostfile, ouserfile);

	/* close output files */
	oforumfile.close();
	othreadfile.close();
	opostfile.close();
	ouserfile.close();

	return 0;
}
#pragma endregion
