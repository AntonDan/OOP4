#include "stdafx.h"
#include "Interface.h"

using namespace std;

void MainMenu(ForumManager & man,  User * user) {
	cout << "Select: \n "
		<< "The ID of the Forum, you wish to visit. \n"
		<< "L, to Save the current system state and Exit. \n"
		<< "X, to Exit without Saving. " << endl;

	if (user->GetRights() == 3) {
		cout << "N, to Create a new Forum \n"
	   		<< "U, to Manage the User Database. " << endl;
	}

	char selection;
	cin.clear();
	cin.sync();
	cin >> selection;

	switch (selection) {
	case 'L':
		break;
	case  'X':
		break;
	case 'N':
		if (user->GetRights() < 3) break;
		cout << "Enter forum name: ";
		string name;
		cin >> name;
		man.CreateForum(man.GetMain(), name);
		break;
	}

}

void UserMenu(User * user) {
	cout << "Select: \n"
		<< "C, to view Users Catalogue. \n"
		<< "M, to Change Rights. \n"
		<< "D, to Delete User. \n"
		<< "U, to Change Username. \n"
		<< "P, to Change Password. \n"
		<< "L, to Save current System and Exit. \n"
		<< "X, to Exit without Saving. " << endl;
}

void ForumMenu(User * user){
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
}

void ThreadMenu(User * user){
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
}