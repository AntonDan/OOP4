#include "stdafx.h"
#include "Functions.h"

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

int myatoi(string & str){
	int value = 0;
	for (unsigned int i = 0; i < Trim(str).size(); ++i){
		if (isdigit(Trim(str)[i])){
			value *= 10;
			value += Trim(str)[i] - '0';
		}
		else{
			return value;
		}
	}
	return value;
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

/* The Split function splits a string that is seperated by the given character and returns the substrings within a vector data structure
* ex. : "This.is a.test" => {"This","is a","test"}
*/
vector<string> Split(string str, const char split) {
	string temp;
	vector<string> tokens;
	int pos;

	while ((pos = str.find(split, 0)) > -1) {
		temp = str.substr(0, pos);
		str	= Trim(str.substr(pos + 1));
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
void Parse(const string line, const string mask, const bool string_term, const char split, const unsigned int argn, ...) {
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
			}
			else {
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