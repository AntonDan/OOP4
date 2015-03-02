#include "stdafx.h"
#include <cstdio>
#include <vector>
#include <cstdarg>
#include "ForumManager.h"
#include "Interface.h"

using namespace std;

#pragma region Functions
/* The Split function splits a string that is seperated by the given character and returns the substrings within a vector data structure
 * ex. : "This.is a.test" => {"This","is a","test"} 
 */
vector<string> Split(string str, const char split) {
	string temp;
	vector<string> tokens;
	int pos;

	while ((pos = str.find(split, 0)) > -1) {
		temp = str.substr(0, pos);
		str = Trim(str.substr(pos + 1));
		tokens.push_back(temp);
	}
	tokens.push_back(Trim(str));
	return tokens;
}

/* The parse function will take care of the parsing of a single given string according to the given mask and the sperator and it will store the 
 * data obtained in the given variables. 
 * ex. : Parse("58 John J 35 This is a description" , "I S C I S", true, ' ', &id , &name , &initial, &age, &description )
 * will assign the values : id = 58 , name = John , initial = J , age = 35 and description = "This is a description" 
 * It supports the following datatypes:
 * Integer 'I' , String 'S' , Character 'C' (more datatypes can be easily added but may cause conflicts -for example if there were to implemented the
 * float datatype, what would happen if the seperator was a '.' ? -)
 * Note: Safecasting has not been implemented since the use of this function within this project is assumed to always be correct. However you decide to 
 * use this function outside this project , we strongly recommend to change static casting to safe (dynamic) casting. 
 */
void Parse(const string line, const string mask, const bool string_term , const char split, const unsigned int argn, ...) {
	/* 1) Starting an argument list 
	 * 2) Splitting line according to the given character
	 * 3) Splitting given mask by spaces ("I S C S" => {"I","S","C","S"} 
	 * 4) For each argument given:
	 *    if it's supposed to be an integer , convert the string to integer and assign it to the given (int) variable *
	 *    if it's supposed to be a string , if it's the last argument given and the line is supposed to terminate with a string (the last string , if it's seperated with spaces  
	 *    and the seprator is ' ' , will now be in the token list stored as seperate words) store every word in the given string variable else just store the word in the given string variable
	 *    if it's supposed to be a character , store the first character of the token string in the given (char) variable 
	 *    if the datatype is not recognized print error message (can be also printed in the error stream) 
	 *  5) End argument list 
	 */
	va_list ap;
	va_start(ap, argn);

	vector<string> tokens = Split(line, split);
	vector<string> datatypes = Split(mask, ' ');

	for (unsigned int i = 0; i < argn; ++i) {
		if (i >= datatypes.size()) break;
		switch (datatypes[i][0]) {
		case 'I':
			*va_arg(ap, int *) = atoi(tokens[i].c_str());
			break;
		case 'S':
			if (i == argn - 1 && string_term) {
				string temp;
				temp += tokens[i];
				for (unsigned int j = i + 1; j < tokens.size(); ++j) {
					temp += (" " + tokens[j]);
				}
				*va_arg(ap, string *) = temp;
			} else {
				*va_arg(ap, string *) = tokens[i];
			}
			break;
		case 'C':
			*va_arg(ap, char *) = tokens[i][0];
			break;
		default:
			cout << "Unsupported datatype" << endl;
			break;
		}
	}

	va_end(ap);
}

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
	ifstream iforumfile ("C:/Users/Antonis/Desktop/OOP4/OOP4/Askisi4/Debug/Databases/forum.txt");
	ifstream ithreadfile("C:/Users/Antonis/Desktop/OOP4/OOP4/Askisi4/Debug/Databases/thread.txt");
	ifstream ipostfile  ("C:/Users/Antonis/Desktop/OOP4/OOP4/Askisi4/Debug/Databases/post.txt");
	ifstream iuserfile  ("C:/Users/Antonis/Desktop/OOP4/OOP4/Askisi4/Debug/Databases/user.txt");

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
