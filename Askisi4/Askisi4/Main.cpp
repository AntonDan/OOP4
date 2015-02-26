#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdarg>
#include "DS.h"
#include "Users.h"

using namespace std;

#pragma region Functions
// Forward declarations
void SaveSubforums(ofstream &, ofstream &, ofstream &, Forum *, string);

void SaveThreads(ofstream &, ofstream &, Forum *, string);

void SavePosts(ofstream &, Thread *, string);

/* The Save functions take care of (guess what?) saving the given system's condition in the given files (forum file , thread file , post file) */
void SaveSystem(ofstream & forumFile , ofstream & threadFile , ofstream & postFile , System & system) {
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
	SaveThreads(threadFile , postFile , forum, path);  // Save threads of file (this command an also be blased at the end of the function)
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

void SavePosts(ofstream & postFile , Thread * thread , string path ) {
	/* Get a list of the posts inside the thread and save their contents/data  */
	oList<Post> * posts;
	posts = thread->GetPosts();
	for (int i = 0; i < posts->GetLength(); ++i) {
		Post * cPost = (*posts)[i];
		postFile << path + "." << cPost->GetID() << " " << atoi(cPost->GetUser().c_str()) << " " << cPost->GetContent() << endl;
	}
}

/* The Trim function removes all whitespaces (char(32)) from the end and start of the given string */
string Trim(string str) {
	if (str.length() == 0) return "";
	string new_str = "";
	
	unsigned int i, j;
	for (i = 0; i < str.length(); ++i)
		if (str[i] != ' ')	break;

	for (j = str.length() - 1; j > 0; --j)
		if (str[j] != ' ') 	break;

	for (unsigned int k = i; k <= j; ++k)
		new_str += str[k];
	
	return new_str;
}

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

User * Validate() {

}
#pragma endregion

#pragma region main
int main(void) {
	/* 1) Create main system 
	 * 2) Open to read database files
	 * 3) Declare needed variables
	 */
	System mainSystem;
	ifstream iforumfile("C:/Users/Antonis/Desktop/OOP4/Askisi4/Debug/Databases/forum.txt");
	ifstream ithreadfile("C:/Users/Antonis/Desktop/OOP4/Askisi4/Debug/Databases/thread.txt");
	ifstream ipostfile("C:/Users/Antonis/Desktop/OOP4/Askisi4/Debug/Databases/post.txt");

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

	oList<User> users;

	string username, password;
	cout << "Καλώς ήλθατε στο D.I.T. Lists \n"
		<< "Παρακαλώ δώστε όνομα χρήση και κωδικό πρόσβασης \n"
		<< "Όνομα χρήστη: ";
	cin >> username;
	cout << "Κωδικός πρόσβασης: ";
	cin >> password;
	
	
	

	/* close input files */
	iforumfile.close();
	ithreadfile.close();
	ipostfile.close();

	/* Open files for writting */
	ofstream oforumfile("C:/Users/Antonis/Desktop/OOP4/Askisi4/Debug/Databases/forum.txt");
	ofstream othreadfile("C:/Users/Antonis/Desktop/OOP4/Askisi4/Debug/Databases/thread.txt");
	ofstream opostfile("C:/Users/Antonis/Desktop/OOP4/Askisi4/Debug/Databases/post.txt");

	SaveSystem(oforumfile, othreadfile, opostfile, mainSystem); // Save system in files 

	/* close outpute files */
	oforumfile.close();
	othreadfile.close();
	opostfile.close();
	
	cin.clear();
	cin.sync();
	cin.get();
	return 0;
}

#pragma endregion


/* DEBUGGING CODE */
#pragma region navigator
/*
System mySystem;

ForumNavigator navigator(&mySystem);
Forum * forum = NULL;
Thread * thread = NULL;
int a, b;
string s1, s2, s3;

while (true) {
	cout <<
		"Forum Navigator and Actions Guide \n" <<
		"> Visit forum    | Syntax: 1 i     \n" <<
		"> Visit thread   | Syntax: 2 i     \n" <<
		"> Back           | Syntax: 3       \n" <<
		"> Print Current  | Syntax: 4       \n" <<
		"> Print Contents | Syntax: 5       \n" <<
		"> Print Content  | Syntax: 6 i     \n" <<
		"> Create Forum   | Syntax: 7 t     \n" <<
		"> Create Thread  | Syntax: 8 t u c \n" <<
		"> Create Post    | Syntax: 9 u c   \n" <<
		"> Delete Forum   | Syntax: 10 i    \n" <<
		"> Delete Thread  | Syntax: 11 i    \n" <<
		"> Delete Post    | Syntax: 12 i    \n" <<
		"> Exit           | Syntax: 0       \n" <<
		"Info: i = index , t = title , u = username , c = content \n" << endl;
	cout << "\n>";
	cin >> a;

	cin.clear();
	cin.sync();

	switch (a) {
	case 1:
		cout << "Enter index number: " << endl;
		cin >> b;
		navigator.VisitForum(b);
		break;
	case 2:
		cout << "Enter index number: " << endl;
		cin >> b;
		navigator.VisitThread(b);
		break;
	case 3:
		navigator.Back();
		break;
	case 4:
		navigator.PrintCurrent();
		break;
	case 5:
		navigator.PrintContents();
		break;
	case 6:
		cout << "Enter index number: " << endl;
		cin >> b;
		navigator.PrintContent(b);
		break;
	case 7:
		cout << "Enter forum title" << endl;
		getline(cin, s1);
		navigator.CreateForum(s1);
		break;
	case 8:
		cout << "Enter threads title , username and content of first post" << endl;
		getline(cin, s1); getline(cin, s2);	getline(cin, s3);
		navigator.CreateThread(s1, s2, s3);
		break;
	case 9:
		cout << "Enter writer's username and content" << endl;
		getline(cin, s1);
		getline(cin, s2);
		navigator.CreatePost(s1, s2);
		break;
	case 10:
		cout << "Enter index number: " << endl;
		cin >> b;
		navigator.DeleteForum(b);
		break;
	case 11:
		cout << "Enter index number: " << endl;
		cin >> b;
		navigator.DeleteThread(b);
		break;
	case 12:
		cout << "Enter index number: " << endl;
		cin >> b;
		navigator.DeletePost(b);
		break;
	case 0:
		break;
	default:
		break;
	}
	if (a == 0) break;
	cout << "\n" << endl;
}
cout << "Exitting" << endl;
*/
#pragma endregion

