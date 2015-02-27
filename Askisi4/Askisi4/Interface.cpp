#include "stdafx.h"
#include <cstdio>
#include <fstream>
#include <vector>
#include <cstdarg>
#include "DS.h"
#include "Users.h"

using namespace std;

void MainMenu(User * user) {
	cout << "Select: \n "
		<< "The ID of the Forum, you wish to visit. \n"
		<< "L, to Save the current system state and Exit. \n"
		<< "X, to Exit without Saving. " << endl;

	if (user->GetRights() == 3) {
		cout << "N, to create a new forum \n"
			<< "U, to Manage the User Database. " << endl;
	}

}

void UserMenu(User * user){
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
		<< "H, to return to Main Menu. \n"
		<< "L, to Save the current system state and Exit. \n"
		<< "X, to Exit without Saving. " << endl;

	if (user->GetRights() == 3){
		cout << "N, to Create Forum. \n"
			<< "E, to Rename Forum. \n"
			<< "D, to Felete Forum. \n"
			<< "M, to Move Forum. " << endl;
	}
}

void ThreadMenu(User * user){
	cout << "Select: \n"
		<< "B, to go Back. \n"
		<< "R, to Reply to a Post. \n"
		<< "H, to return to Main Menu. \n"
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