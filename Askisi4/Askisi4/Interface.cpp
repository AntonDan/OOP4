#include "stdafx.h"
#include <cctype>
#include "Interface.h"

using namespace std;

/* The Trim function removes all whitespaces (char(32)) from the end and start of the given string */
string Trim(string & str) {
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

bool isNumber(const string & str) {
	/* Can be implemented without iterators, but we like to keep a variety on our 
	 * solutions. (Alternative solution would be using the function overload []) 
	 * 1) Start (using a pointer) fro, the beginning of the string 
	 * 2) For every pointer to a character inside the given string , check if it is a digit 
	 *    if yes, move to the next character. If not, break out of the loop 
	 * 3) if we have reached the end of the string without breaking out of the loop (and the string isn't empty) return true 
	 *    else return false
	 */
	string::const_iterator ptr = str.begin();
	while (ptr != str.end() && isdigit(*ptr)) ++ptr;
	return !str.empty() && ptr == str.end();
}

void MainMenu(ForumManager & nav, User * user) {
	cout << endl;
	nav.PrintContents();
	cout << "\n" << endl;
	cout << "Select: \n "
		<< "The ID of the Forum, you wish to visit. \n"
		<< "L, to Save the current system state and Exit. \n"
		<< "X, to Exit without Saving. " << endl;

	cout << endl;

	if (user->GetRights() == 3) {
		cout << "N, to Create a new Forum \n"
	   		<< "U, to Manage the User Database. " << endl;
	}
	cout << "\n\n" << ">";
	string selection;
	cin.clear();
	cin.sync();
	cin >> selection;

	selection = Trim(selection);

	if (isNumber(selection)) {
		nav.VisitForum(atoi(selection.c_str()));
		ForumMenu(nav, user);
	} else {
		switch (selection[0]) {
		case 'L':
			break;
		case  'X':
			break;
		case 'N':
			if (user->GetRights() < 3) break;
			cout << "Enter forum name: ";
			string name;
			cin.clear();
			cin.sync();
			getline(cin ,  name);
			nav.CreateForum(nav.GetMain(), name);
			break;
		}
	}



}

void UserMenu(ForumManager & nav, User * user) {
	cout << "Select: \n"
		<< "C, to view Users Catalogue. \n"
		<< "M, to Change Rights. \n"
		<< "D, to Delete User. \n"
		<< "U, to Change Username. \n"
		<< "P, to Change Password. \n"
		<< "L, to Save current System and Exit. \n"
		<< "X, to Exit without Saving. " << endl;

	char selection;
	cin.clear();
	cin.sync();
	cin >> selection;

	switch (selection){
	case 'C':
		break;
	case 'M':
		break;
	case 'D':
		break;
	case 'U':
		break;
	case 'P':
		break;
	case 'L':
		break;
	case 'X':
		break;
	default:
		break;
	}
}

void ForumMenu(ForumManager & nav, User * user){
	cout << endl;
	cout << "Forums: " << endl;
	nav.PrintContents(0);
	cout << endl;
	cout << "Threads: " << endl;
	nav.PrintContents(1);
	cout << "\n" << endl;

	cout << "Select: \n"
		<< "F, Visit Forum. \n"
		<< "T, to Visit Thread menu. \n"
		<< "B, to go Back. \n"
		<< "C, to Create Thread. \n"
		<< "H, to Return to Main Menu. \n"
		<< "L, to Save the current system state and Exit. \n"
		<< "X, to Exit without Saving. " << endl;

	if (user->GetRights() == 3){
		cout << "N, to Create Forum. \n"
			<< "E, to Rename Forum. \n"
			<< "D, to Delete Forum. \n"
			<< "M, to Move Forum. " << endl;
	}

	char selection;
	cin.clear();
	cin.sync();
	cin >> selection;

	switch (selection){
	case 'F':
		break;
	case 'T':
		break;
	case 'B':
		break;
	case 'C':
		break;
	case 'H':
		break;
	case 'L':
		break;
	case 'X':
		break;
	case 'N':
		if (user->GetRights() < 3) break;
		break;
	case 'E':
		if (user->GetRights() < 3) break;
		break;
	case 'D':
		if (user->GetRights() < 3) break;
		break;
	case 'M':
		if (user->GetRights() < 3) break;
		break;
	default:
		break;
	}
	
}

void ThreadMenu(ForumManager & nav, User * user){
	cout << "Select: \n"
		<< "B, to go Back. \n"
		<< "R, to Reply to a Post. \n"
		<< "H, to Return to Main Menu. \n"
		<< "L, to Save the current system state and Exit. \n"
		<< "X, to Exit without Saving." << endl;

	if (user->GetRights() >= 2){
		cout << "D, to Delete Thread. \n"
			<< "M, to Move Thread. \n"
			<< "E, to Rename Thread. \n"
			<< "Y, to Set Sticky. \n"
			<< "K, to Set Locked. \n"
			<< "A, to Delete Post. \n"
			<< "S, to Move Post." << endl;
	}

	char selection;
	cin.clear();
	cin.sync();
	cin >> selection;
	string content;

	switch (selection){
	case 'B':
		nav.Back();
		break;
	case 'R':
		if (user->GetRights() < 1) break;
		
		cin >> content;
		nav.GetMain()->LastPostID += 1;
		nav.CreatePost(nav.GetCurrentThread(), nav.GetMain()->LastPostID, user->GetUsername(), content);
		break;
	case 'H':
		
		break;
	case 'L':

		break;
	case 'X':
		break;
	case 'D':
		if (user->GetRights() < 2) break;
		break;
	case 'M':
		if (user->GetRights() < 2) break;
		break;
	case 'E':
		if (user->GetRights() < 2) break;
		break;
	case 'Y':
		if (user->GetRights() < 2) break;
		break;
	case 'K':
		if (user->GetRights() < 2) break;
		break;
	case 'A':
		if (user->GetRights() < 2) break;
		break;
	case 'S':
		if (user->GetRights() < 2) break;
		break;
	default:
		break;
	}
}