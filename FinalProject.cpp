/*
Jessica Barnitz
CSCI 2270: Data Structures
Final Project 
*/

#include "HashTable.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;


int main(int argc, char* argv[])
{
	string input = "";
	string in_firstName = "";
	string in_lastName = "";
	HashTable ht;
	
	//if the command line arguments are incorrect
	if (argc != 2)
	{
		cout << "Missing the hash table size" << endl;
		return -1;
	}

	//command line argument of hash table size
	ht.tableSize = stoi(argv[1]);
	ht.createTable();
	//parse the textfile and populate the two hash tables
	ht.parseFile();

	//display table building information for the user
	cout << "Hash table size: " << ht.tableSize << endl;
	cout << "Collisions using open addressing: " << ht.collisionOpenAddr << endl;
	cout << "Collisions using chaining: " << ht.collisionChaining << endl;
	cout << "Search operations using open addressing: " << ht.searchOpsOpenAddr << endl;
	cout << "Search operations using chaining: " << ht.searchOpsChaining << endl;
	ht.emptySpace();

	while(input != "2")
	{
		//Display menu
		cout << "\n";
        cout << "======Main Menu======" << endl;
        cout << "1. Query hash table" << endl;
        cout << "2. Quit Program" << endl;
        getline(cin, input);

        if (input == "1")
        {
        	cout << "Enter the first name of the player: ";
        	getline(cin, in_firstName);
        	cout << "Enter the last name of the player: ";
        	getline(cin, in_lastName);
        	cout << "======================================" << endl;
        	//search for the player
        	ht.searchChaining(in_firstName, in_lastName);
			cout << "======================================" << endl;
			ht.searchOpenAddr(in_firstName, in_lastName);
        }
	}
	cout << "Goodbye!" << endl;
	return 0;
}