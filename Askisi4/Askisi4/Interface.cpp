#include "stdafx.h"
#include <cstdio>
#include <fstream>
#include <vector>
#include <cstdarg>
#include "DS.h"
#include "Users.h"

using namespace std;

void MainMenu(User * user) {
	cout << "ÅðéëÝîôå: \n "
		<< "ôï ID ôïõ öüñïõì ðïõ åðéèõìåßôå íá åðéóêåöèåßôå \n"
		<< "L, ãéá óþóéìï ôçò ôñÝ÷ïõóáò êáôÜóôáóçò êáé Ýîïäï áðü ôï óýóôçìá"
		<< "X, ãéá Ýîïäï áðü ôï óýóôçìá ÷ùñßò íá óùèåß ç ôñÝ÷ïõóá êáôÜóôáóÞ ôïõ" << endl;

	if (user->GetRights() == 3) {
		cout << "N, ãéá äçìéïõñãßá íÝïõ öüñïõì \n"
			<< "U, ãéá äéá÷åßñéóç ÷ñçóôþí" << endl;
	}

}