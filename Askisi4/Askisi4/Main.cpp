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

void PrintSubForums(Forum * , string );

void PrintThreads(Forum * , string );

void PrintSystem(System * root) {
	oList<Forum> * forums;
	forums = root->GetForums();
	for (int i = 0; i < forums->GetLength(); ++i ) {
		PrintSubForums((*forums)[i] , to_string(i+1) );
		cout << "Forum " << i << " :  " <<  (*forums)[i]->GetTitle() << endl;
	}
}

void PrintSubForums(Forum * forum , string curPath ) {

	oList<Forum> * forums;
	forums = forum->GetForums();
	for (int i = 0; i < forums->GetLength(); ++i) {
		curPath += "." + to_string(i + 1);
		cout << "Forum " << curPath << " :  " << (*forums)[i]->GetTitle() << endl;
		PrintSubForums((*forums)[i], curPath );
	}

	PrintThreads(forum, curPath);

}

void PrintThreads(Forum * forum, string curPath ) {

	oList<Thread> * threads;
	threads = forum->GetThreads();
	for (int i = 0; i < threads->GetLength(); ++i) {
		cout << "Thread " << curPath + "." << (*threads)[i]->GetID() << " :  " << (*threads)[i]->GetTitle() << endl;
	}
}

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

void Parse(const string line, const string mask, const bool string_term , const char split, const unsigned int argn, ...) {
	va_list ap;
	va_start(ap, argn);

	vector<string> tokens = Split(line, split);
	vector<string> datatypes = Split(mask, ' ');

	for (unsigned int i = 0; i < argn; ++i) {
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

#pragma endregion

#pragma region main

int main(void) {
	
	System mainSystem;
	ForumNavigator nav(&mainSystem);
	ifstream forumFile("C:/Users/Antonis/Desktop/file.txt");
	ifstream threadFile("C:/Users/Antonis/Desktop/file2.txt");
	string line , path , name;
	Forum * tempForum = NULL;


#pragma region Forum creator
	int pamount = -1, camount = 0;

	while (getline(forumFile, line)) {
		// Parse line
		Parse(line, "S S", true , ' ', 2, &path, &name);
		camount = count(path.begin(), path.end(), '.');
		if (camount > pamount) {
			nav.VisitForum(tempForum);
		} else if (camount < pamount) {
			for (int i = 0; i < pamount - camount; ++i) {
				nav.Back();
			}
		}
		tempForum = nav.CreateForum(name);
		pamount = camount;
	}
#pragma endregion


#pragma region Thread creator	
	char sticky, locked;
	
	while (getline(threadFile, line)) {
		Parse(line, "S C C S", true, ' ', 4, &path, &sticky, &locked, &name);
		vector<string> vpath = Split(path, '.');
		// Navigate to thread's parent forum
		Forum * tempForum = mainSystem.GetForum(atoi(vpath[0].c_str()));
		for (unsigned int i = 1; i < vpath.size() - 1; ++i) {
			tempForum = tempForum->GetForum(atoi(vpath[i].c_str()));
		}
		// Create Thread
		tempForum->CreateThread(atoi(vpath[vpath.size() - 1].c_str()), name , "Username" , ((sticky=='S') + (locked=='L')*2) );
	}
#pragma endregion

#pragma region Post creator

#pragma endregion

	PrintSystem(&mainSystem);

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

