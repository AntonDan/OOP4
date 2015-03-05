#include "stdafx.h"
#include "Interface.h"


using namespace std;

bool RegistrationMenu(ForumManager & nav) {
	User * currentUser;
	string usm, pass;
	cout << endl;
	cout << "\t===========================\n\t||Welcome to D.I.T Lists!|| \n\t===========================\n\n" << endl;

	while (true) {
		cout << "Registration Form:" << endl;
		cout << "Username:";
		cin >> usm;
		cout << "Password";
		cin.clear();
		cin.sync();
		cin >> pass;
		if (pass.size() < 6) {
			cout << "Your password has to be at least 6 characters long" << endl; // FEEL THE PAIN! 
		} else if (!nav.Register(usm, pass)) {
			cout << "Registration failed! Given username is not available." << endl;
		} else {
			cout << "Registration complete!" << endl;
			cout << "Account created with: \n" <<
				"Username: " << usm << "\n" <<
				"Password: " << pass << endl;
			break;
		}
	}
	currentUser = nav.FindUserbyName(usm);
	cout << "Welcome " << currentUser->GetUsername() << "!" << endl;
	cout << "You belong in the category: " << ((currentUser->GetRights()>0) ? ((currentUser->GetRights() > 1) ? ((currentUser->GetRights() > 2) ? ("Administrator") : ("Moderator")) : ("User")) : ("Visitor")) << endl; // ONE LINER CODING FTW ! \\(* O *)//

	return MainMenu(nav, currentUser);
}

bool WelcomeMenu(ForumManager & nav) {
	User * currentUser = NULL;
	string username, password;
	cout << endl;
	cout << "\t===========================\n\t||Welcome to D.I.T Lists!|| \n\t===========================\n\n"
		<< "Please type in username and password \n";

	do {
		cout << "Username: ";
		getline(cin, username);
		cout << "Password: ";
		getline(cin, password);

		currentUser = nav.Validate(username, password);

		if (password == "" && username == "") {
			currentUser = new User("Guest", "", 0);
		}

		if (currentUser == NULL) {
			cout << "Incorrect username or password. \n Please try again." << endl;
		}
	} while (currentUser == NULL);

	cout << "Welcome " << currentUser->GetUsername() << "!" << endl;
	cout << "You belong in the category: " << ((currentUser->GetRights()>0) ? ((currentUser->GetRights() > 1) ? ((currentUser->GetRights() > 2) ? ("Administrator") : ("Moderator")) : ("User")) : ("Visitor")) << endl; // WARNING: This line looks terrible at sub 23 inch displays

	return MainMenu(nav, currentUser);

}

bool MainMenu(ForumManager & nav, User * user) {
	cout << endl;
	nav.PrintContents();
	cout << "\n\n" << endl;
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
		int id = atoi(selection.c_str());
		if (id > nav.GetMain()->GetForums()->GetLength() || id <= 0) {
			cout << "Invalid ID entered" << endl;
			return MainMenu(nav, user);
		} else {
			nav.VisitForum(id);
			return ForumMenu(nav, user);
		}
	} else {
		switch (toupper(selection[0])) {
		case 'L':
			cout << "Saving state and exiting..." << endl;
			return true;
		case  'X':
			cout << "Exiting..." << endl;
			return false;
		case 'N': {
					  if (user->GetRights() < 3) {
						  cout << "Invalid command given" << endl;
						  return MainMenu(nav, user);
					  }
					  cout << "Enter forum name: ";
					  string name;
					  cin.clear();
					  cin.sync();
					  getline(cin, name);
					  nav.CreateForum(nav.GetMain(), name);
					  cout << "Forum created!" << endl;
					  return MainMenu(nav, user);
		}
		case 'U':
			if (user->GetRights() < 3) {
				cout << "Invalid command given" << endl;
				return MainMenu(nav, user);
			}
			return UserMenu(nav , user);
		default:
			cout << "Invalid command given" << endl;
			return MainMenu(nav, user);
		}
	}



}

bool UserMenu(ForumManager & nav, User * user) {
	cout << endl;
	if (user->GetRights() < 3) {
		cout << "You are not allowed to access this menu" << endl;
		return MainMenu(nav, user);
	}
	cout << "\n\nSelect: \n"
		<< "C, to view Users Catalogue. \n"
		<< "M, to Change Rights. \n"
		<< "D, to Delete User. \n"
		<< "U, to Change Username. \n"
		<< "P, to Change Password. \n"
		<< "H, to return to the Main Menu. \n"
		<< "L, to Save current System and Exit. \n"
		<< "X, to Exit without Saving. " << endl;
	cout << "\n\n" << ">";

	char selection;
	cin.clear();
	cin.sync();
	cin >> selection;
	cin.clear();
	cin.sync();

	switch (toupper(selection)){
	case 'C':
		nav.PrintUsers();
		return UserMenu(nav, user);
	case 'M':{
		int rights;
		string username;

		cout << "Enter username: ";
		cin >> username;
		cout << "\nEnter rights: ";
		cin.clear();
		cin.sync();
		cin >> rights;

		if (user->GetRights() > 3 || user->GetRights() < 1) {
			cout << "Invalid rights given" << endl;
		} else if (nav.ChangeUserRights(username, rights)) {
			cout << "Rights updated" << endl;
		} else {
			cout << "Invalid username given" << endl;
		}
		return UserMenu(nav, user);
	}
	case 'D':{
		string username;
		cout << "Enter username: ";
		cin >> username;

		if (nav.DeleteUser(username)){
			cout << "User deleted" << endl;
		} else {
			cout << "Invalid username" << endl;
		}
		return UserMenu(nav, user);
	}
		
	case 'U':{
		string curr_username, new_username;
		cout << "Enter current username: ";
		cin >> curr_username;
		cout << "Enter new username: ";
		cin.clear();
		cin.sync();
		cin >> new_username;

		if (nav.RenameUser(curr_username, new_username)){
			cout << "User renamed" << endl; 
		} else {
			cout << "Invalid new username given or user does not exist" << endl;
		}
		return UserMenu(nav, user);
	}		
	case 'P':{
		string username;
		string newcode;
		cout << "Enter  username: ";
		cin >> username;
		cout << "Enter new upassword: ";
		cin.clear();
		cin.sync();
		cin >> newcode;

		if (nav.ChangeUserPassword(username, newcode)){
			cout << "Password changed" << endl;
		} else {
			cout << "Invalid username given" << endl;
		}
		return UserMenu(nav, user);
	}		
	case 'H':
		return MainMenu(nav, user);
	case 'L':
		cout << "Saving state and exiting..." << endl;
		return true;
	case 'X':
		cout << "Exiting..." << endl;
		return false;
	default:
		cout << "Invalid command given" << endl;
		return UserMenu(nav, user);
	}

	return true;
}

bool ForumMenu(ForumManager & nav, User * user){
	cout << endl;
	cout << "Forums: " << endl;
	nav.PrintContents(0);
	cout << endl;
	cout << "Threads: " << endl;
	nav.PrintContents(1);
	cout << "\n" << endl;

	cout << "\n\nSelect: \n"
		<< "F, to Visit Forum. \n"
		<< "T, to Visit Thread. \n"
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

	cout << "\n\n" << ">";

	char selection;
	cin.clear();
	cin.sync();
	cin >> selection;
	cin.clear();
	cin.sync();

	switch (toupper(selection)){
	case 'F': {
		int id;

		cout << "Enter forum ID" << endl;
		cin >> id;
		nav.VisitForum(id);
		return ForumMenu(nav, user);
	}
	case 'T':{
		int id;

		cout << "Enter Thread ID" << endl;
		cin >> id;
		nav.VisitThread(id);
		return ThreadMenu(nav, user);
	}		
	case 'B':{
		nav.Back();
		if (nav.GetCurrentForum() == NULL) {
			return MainMenu(nav, user);
		} else {
			return ForumMenu(nav, user);
		}
	}
	case 'C':{
		string title, content;

		cout << "Enter title: ";
		getline(cin, title);
		cout << "Enter content: ";
		getline(cin, content);
		
		nav.CreateThread(nav.GetCurrentForum(),title, user->GetUsername(), content);

		return ForumMenu(nav, user);
	}		
	case 'H':{
		return MainMenu(nav, user);
	}		
	case 'L':
		cout << "Saving state and exiting..." << endl;
		return true;	
	case 'X':
		cout << "Exiting..." << endl;
		return false;
	case 'N':
		if (user->GetRights() < 3){
			cout << "Invalid command given" << endl;
			return ForumMenu(nav, user);
		} else {
			string title;

			cout << "Enter title: ";
			getline(cin, title);
			nav.CreateForum(nav.GetCurrentForum(), title);
			return ForumMenu(nav, user);
		}
	case 'E':
		if (user->GetRights() < 3){
			cout << "Invalid command given" << endl;
			return ForumMenu(nav, user);
		} else {
			string title;

			cout << "Enter title:";
			getline(cin, title);
			nav.RenameForum(nav.GetCurrentForum(), title);
			return ForumMenu(nav, user);
		}
	case 'D':
		if (user->GetRights() < 3){
			cout << "Invalid command given" << endl;
			return ForumMenu(nav, user);
		} else {
			int index;

			cout << "Type ID of Forum to be deleted: ";
			cin >> index;
			nav.DeleteForum(nav.GetCurrentForum(), index);

			return ForumMenu(nav, user);
		}
	case 'M':
		if (user->GetRights() < 3){
			cout << "Invalid command given" << endl;
			return ForumMenu(nav, user);
		} else {
			int id;
			string path;

			cout << "Type ID of Forum to be moved: ";
			cin >> id;
			cout << "Type path: ";
			cin.clear();
			cin.sync();
			cin >> path;

			vector<string> tokens = Split(path,'.');
			SF * tempForum = nav.GetMain();
			for (unsigned int i = 0; i < tokens.size(); ++i){
				tempForum = tempForum->GetForum(atoi(tokens[i].c_str()));
			}

			nav.MoveForum(nav.GetCurrentForum()->GetForum(id), tempForum);
			return ForumMenu(nav, user);
		}
	default:
		cout << "Invalid command given" << endl;
		return ForumMenu(nav, user);
	}
	return true;
	
}

bool ThreadMenu(ForumManager & nav, User * user){
	cout << endl;
	nav.PrintContents();

	cout << "\n\nSelect: \n"
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

	cout << "\n\n" << ">";

	char selection;
	cin.clear();
	cin.sync();
	cin >> selection;
	cin.clear();
	cin.sync();

	switch (toupper(selection)){
	case 'B':
		nav.Back();
		return ForumMenu(nav, user);
	case 'R': {
		if (user->GetRights() < 1) {
			cout << "You need to be a user to create a post" << endl;
		} else {
			string content;
			cin >> content;
			nav.CreatePost(nav.GetCurrentThread(), user->GetUsername(), content);
		}
		return ThreadMenu(nav, user);
	}
	case 'H':{
		cout << "Returning to Main Menu" << endl;
		return MainMenu(nav, user);
	}		
	case 'L':{
		cout << "Saving state and exiting..." << endl;
		return true;
	}
	case 'X':{
		cout << "Exiting..." << endl;
		return false;
	}		
	case 'D':{
		if (user->GetRights() < 2) {
			cout << "Invalid command given" << endl;
			return ThreadMenu(nav, user);
		} else {
			Thread * temp = nav.GetCurrentThread();
			nav.Back();
			nav.DeleteThread(temp);
			return ForumMenu(nav, user);
		}
	}
	case 'M':{
		if (user->GetRights() < 2) {
			cout << "Invalid command given " << endl;
			return ThreadMenu(nav, user);
		} else {
			string path;

			cout << "Type path: ";
			cin >> path;

			vector<string> tokens = Split(path, '.');
			Forum * tempForum = nav.GetMain()->GetForum((atoi(tokens[0].c_str())));
			for (unsigned int i = 1; i < tokens.size(); ++i){
				tempForum = tempForum->GetForum(atoi(tokens[i].c_str()));
			}

			Thread * temp = nav.GetCurrentThread();
			nav.Back();
			nav.MoveThread(temp, tempForum);
			return ForumMenu(nav, user);
		}
	}
	case 'E':{
		if (user->GetRights() < 2) {
			cout << "Invalid command given " << endl;
		} else {
			string name;
			cout << "Enter new thread name: ";
			getline(cin, name);
			nav.RenameThread(nav.GetCurrentThread(), name);
		}
		return ThreadMenu(nav, user);
	}
	case 'Y':{
		if (user->GetRights() < 2) {
			cout << "Invalid command given " << endl;
		} else {
			nav.ChangeSticky(nav.GetCurrentThread());
			cout << "Thread is set to " << ((nav.GetCurrentThread()->isSticky())? (""):("not")) << " sticky" << endl;
		}
		return ThreadMenu(nav, user);
	}
	case 'K':{
		if (user->GetRights() < 2) {
			cout << "Invalid command given " << endl;
		} else {
			nav.ChangeLocked(nav.GetCurrentThread());
			cout << "Thread is set to " << ((nav.GetCurrentThread()->isLocked()) ? ("") : ("not")) << " locked" << endl;
		}
		return ThreadMenu(nav, user);
	}
	case 'A':{
		if (user->GetRights() < 2) {
			cout << "Invalid command given " << endl;
		} else {
			int id;
			cout << "Enter  in the ID of the Post to be deleted: ";
			cin >> id;
			nav.DeletePost(nav.GetCurrentThread()->GetPostByID(id));
		}
		return ThreadMenu(nav, user);
	}
	case 'S':{
		if (user->GetRights() < 2) {
			cout << "Invalid command given " << endl;
		} else {
			string path;
			int id;

			cout << "Type path: ";
			cin >> path;
			cout << "Type in the ID of the Post to be moved: ";
			cin.clear();
			cin.sync();
			cin >> id;
			
			vector<string> tokens = Split(path, '.');
			Forum * tempForum = nav.GetMain()->GetForum((atoi(tokens[0].c_str())));
			for (unsigned int i = 1; i < tokens.size() - 1; ++i){
				tempForum = tempForum->GetForum(atoi(tokens[i].c_str()));
			}
			Thread * tempThread = tempForum->GetThreadByID(atoi(tokens[tokens.size()-1].c_str()));
			Post * temp = nav.GetCurrentThread()->GetPost(id);
			nav.Back();
			nav.MovePost(temp, tempThread);
		}
		return ThreadMenu(nav, user);
	}		
	default:
		cout << "Invalid command given" << endl;
		return ThreadMenu(nav, user);
	}
	return true;
}