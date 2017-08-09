/*
Jessica Barnitz
CSCI 2270
Final Project
*/

#include "HashTable.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

//constructor
HashTable::HashTable()
{
	collisionChaining = 0;
	collisionOpenAddr = 0;

	searchOpsChaining = 0;
	searchOpsOpenAddr = 0;
}

HashTable::~HashTable()
{
	//dtor
	for (int i = 0; i < tableSize; i++)
	{
		table[i] = NULL;
		tableOpenAddr[i] = NULL;
	}
	delete table;
	delete tableOpenAddr;
}

//create both table and tableOpenAddr, used for chaining and open addressing operations respectfully
void HashTable::createTable()
{
	
	table = new PlayerInfo*[tableSize];
	tableOpenAddr = new PlayerInfo*[tableSize];
	for (int i = 0; i < tableSize; i++)
	{
		table[i] = NULL;
		tableOpenAddr[i] = NULL;
	}
}

void HashTable::emptySpace()
{
	int emptyChaining = 0;
	int emptyOpenAddr = 0; 
	for(int i = 0; i < tableSize; i++)
	{
		if(tableOpenAddr[i] == NULL)
		{
			emptyOpenAddr++;
		}
		if(table[i] == NULL)
		{
			emptyChaining++;
		}
	}
	cout << "Number of empty indexes with open addressing is: " << emptyOpenAddr << ", with table size: " << tableSize << endl;
	cout << "Number of empty indexes with chaining is: " << emptyChaining << ", with table size: " << tableSize << endl;
}

//hash sum to provide index number to store the key in the hash table using ascii table
int HashTable::hashSum(string key)
{
	int hash = 0;
	int index = 0;
	//loop through the string and convert the individual char into an ascii value
	for (int i = 0; i < key.length(); i++)
	{
		hash = hash + ((int)key[i]*pow(2,i));
	}
	//create the index where the key will be stored in the hash table; % tableSize ensures it is within bounds of the array
	index = (hash % tableSize);
	return index;
}
/*
//bernstein's hash
int HashTable::hashSum(string key)
{
	int hash = 5381;
	int index = 0;

	for(int i = 0; i < key.length(); i++)
	{
		hash = 33*hash+key.at(i);
	}
	hash = hash ^ (hash >> 16);
	index = hash % tableSize;
	return index;
}
*/
//concatenate two strings to pass into hash sum as the key
string HashTable::keyConcatenate(string s1, string s2)
{
	string key = "";
	key = s1 + s2;
	return key;
}

//concatenate the player team and salary information
string HashTable::teamInfoConcatenate(string s1, string s2, string s3, string s4)
{
	string teamsInfo = "";
	teamsInfo = (s1 + ", " + s2 + ", " + s3 + ", " + s4);
	return teamsInfo;
}

void HashTable::insertChaining(string in_firstName, string in_lastName, int in_yearBorn, string in_countryBorn, int in_weight, int in_height, string in_bats, string in_throws, string in_teamsAndSalary)
{
	PlayerInfo *newPlayer = new PlayerInfo(in_firstName, in_lastName, in_yearBorn, in_countryBorn, in_weight, in_height, in_bats, in_throws, in_teamsAndSalary);
	//concatenate the string
	string key = keyConcatenate(in_firstName, in_lastName);
	//identify the index position
	int index = hashSum(key);
	//if no elements at that position of the hashtable
	if(table[index] == NULL)
	{
		table[index] = newPlayer;
		return;
	}
	//else there are elements: collision resolution for chaining
	else
	{
		//increment collision counter once
		collisionChaining++;
		PlayerInfo *temp = table[index];
		//search for the player in the linked list
		while(temp != NULL)
		{
			//a match is found (based on name)
			if ((temp->firstName == in_firstName) && (temp->lastName == in_lastName))
			{
				//verify uniqueness of player
				if ((temp->yearBorn == in_yearBorn) && (temp->height == in_height))
				{
					updatePlayerInfo(temp, in_teamsAndSalary);
					//no additional collision as the player is already in the table
					collisionChaining--;
					return;
				}
			}
			temp = temp->next;
		}
		//player was not found in the table, therefore add the player to the table
		//reset temp pointer to the ideal index location to find the correct, alphabetized location for insertion
		temp = table[index];
		while((temp->next != NULL) && (temp->lastName < in_lastName))
		{
			temp = temp->next;
			//increment search counter for each traversal in searching for the player
			searchOpsChaining++;
		}

		//if temp is not the tail
		if(temp->next != NULL)
		{
			newPlayer->next = temp;
			newPlayer->previous = temp->previous;
			//if temp is not the head of the linked list
			if(temp->previous != NULL)
			{
				temp->previous->next = newPlayer;
			}
			//else update the head of the linked list
			else
			{
				table[index] = newPlayer;
			}
			temp->previous = newPlayer;
		}
		//else temp is the tail
		else
		{
			temp->next = newPlayer;
			newPlayer->previous = temp;
		}
	}
}

void HashTable::insertOpenAddr(string in_firstName, string in_lastName, int in_yearBorn, string in_countryBorn, int in_weight, int in_height, string in_bats, string in_throws, string in_teamsAndSalary)
{
	PlayerInfo *newPlayer = new PlayerInfo(in_firstName, in_lastName, in_yearBorn, in_countryBorn, in_weight, in_height, in_bats, in_throws, in_teamsAndSalary);
	//concatenate the string
	string key = keyConcatenate(in_firstName, in_lastName);
	//identify the index position
	int index = hashSum(key);
	//no elements at that position of the hashtable
	if(tableOpenAddr[index] == NULL)
	{
		tableOpenAddr[index] = newPlayer;
	}

	//collision resolution for open addressing
	else
	{
		if ((tableOpenAddr[index]->firstName == in_firstName) && (tableOpenAddr[index]->lastName == in_lastName))
		{
			//verify uniqueness of player
			if ((tableOpenAddr[index]->yearBorn == in_yearBorn) && (tableOpenAddr[index]->height == in_height))
			{
				updatePlayerInfo(tableOpenAddr[index], in_teamsAndSalary);
				return;
			}
		}
		//set i equal to index, to break the traversal if the array is full and player not found
		int i = index;
		if((index+1) == tableSize)
		{
			index = 0;
			searchOpsOpenAddr++;
		}
		else
		{
			index++;
			searchOpsOpenAddr++;
		}
		while((tableOpenAddr[index] != NULL) && (i != index))
		{
			//possibility that player we are inserting was not at ideal location 
			if((tableOpenAddr[index]->firstName == in_firstName) && (tableOpenAddr[index]->lastName == in_lastName))
			{
				//verify uniqueness of player
				if ((tableOpenAddr[index]->yearBorn == in_yearBorn) && (tableOpenAddr[index]->height == in_height))
				{
					updatePlayerInfo(tableOpenAddr[index], in_teamsAndSalary);
					return;
				}
			}
			if ((index+1) == tableSize)
			{
				index = 0;
				searchOpsOpenAddr++;
			}
			else
			{
				index++;
				searchOpsOpenAddr++;
			}
		}
		tableOpenAddr[index] = newPlayer;
		collisionOpenAddr++;
	}
}

void HashTable::searchChaining(string in_firstName, string in_lastName)
{
	searchOpsChaining = 0;
	//concatenate the string
	string key = keyConcatenate(in_firstName, in_lastName);
	//identify the index position
	int index = hashSum(key);
	PlayerInfo *temp = table[index];
	//there is at least one element at the index position
	while(temp != NULL)
	{
		//a match is found (based on name)
		if ((temp->firstName == in_firstName) && (temp->lastName == in_lastName))
		{
			//display player information & number of search operations
			cout << "First name: " << temp->firstName << endl;
			cout << "Last name: " << temp->lastName << endl;
			cout << "Year born: " << temp->yearBorn << endl;
			cout << "Country born: " << temp->countryBorn << endl;
			cout << "Weight: " << temp->weight << endl;
			cout << "Height: " << temp->height << endl;
			cout << "Bats: " << temp->bats << endl;
			cout << "Throws: " << temp->throws << endl;
			cout << "Teams and Salary: " << endl;
			for (int i = 0; i < temp->teamsAndSalary.size(); i++)
			{
				cout << temp->teamsAndSalary[i] << endl;
			}
				cout << "Search operations using chaining: " << searchOpsChaining << endl;
				return;
			}
		temp = temp->next;
		searchOpsChaining++;
	}
	cout << "player not found" << endl;
	cout << "Search operations using chaining: " << searchOpsChaining << endl;	
}

//search function to display player information
void HashTable::searchOpenAddr(string in_firstName, string in_lastName)
{
	searchOpsOpenAddr = 0;
	//concatenate the string
	string key = keyConcatenate(in_firstName, in_lastName);
	//identify the index position
	int index = hashSum(key);

	//when the key hashes to an index position that is empty in trying to search for a player
	if(tableOpenAddr[index] == NULL)
	{
		cout << "Player not found" << endl;
		cout << "Search operations using open addressing: " << searchOpsOpenAddr << endl;
		return;
	}

	//when the key hashs to the ideal index position and that player is there
	if((tableOpenAddr[index]->firstName == in_firstName) && (tableOpenAddr[index]->lastName == in_lastName))
	{
		//displaying the palyer information from chaining instead
		displayEntryOpenAddr(index);
		cout << "Search operations using open addressing: " << searchOpsOpenAddr << endl;
		return;
	}

	//the player is not at the ideal index location, traverse the array until either player is found or NULL is reached
	else if((tableOpenAddr[index]->firstName != in_firstName) && (tableOpenAddr[index]->lastName != in_lastName) && (tableOpenAddr[index] != NULL))
	{
		//set i equal to index, to break the traversal if the array is full and player not found
		int i = index;
		if((index+1) == tableSize)
		{
			index = 0;
			searchOpsOpenAddr++;
		}
		else
		{
			index++;
			searchOpsOpenAddr++;
		}
		while((tableOpenAddr[index] != NULL) && (i != index))
		{
			if((tableOpenAddr[index]->firstName == in_firstName) && (tableOpenAddr[index]->lastName == in_lastName))
			{
				//displaying the player information from chaining instead
				displayEntryOpenAddr(index);
				cout << "Search operations using open addressing: " << searchOpsOpenAddr << endl;
				return;
			}
			if ((index+1) == tableSize)
			{
				index = 0;
				searchOpsOpenAddr++;
			}
			else
			{
				index++;
				searchOpsOpenAddr++;
			}
		}
	}
	cout << "Player not found" << endl;
	cout << "Search operations using open addressing: " << searchOpsOpenAddr << endl;
}

//if player is found in the table update the player stats
void HashTable::updatePlayerInfo(PlayerInfo* player, string in_teamsAndSalary)
{
	player->teamsAndSalary.push_back(in_teamsAndSalary);
}

void HashTable::displayEntryOpenAddr(int index)
{
	cout << "First name: " << tableOpenAddr[index]->firstName << endl;
	cout << "Last name: " << tableOpenAddr[index]->lastName << endl;
	cout << "Year born: " << tableOpenAddr[index]->yearBorn << endl;
	cout << "Country born: " << tableOpenAddr[index]->countryBorn << endl;
	cout << "Weight: " << tableOpenAddr[index]->weight << endl;
	cout << "Height: " << tableOpenAddr[index]->height << endl;
	cout << "Bats: " << tableOpenAddr[index]->bats << endl;
	cout << "Throws: " << tableOpenAddr[index]->throws << endl;
	cout << "Teams and Salary: " << endl;
	for (int i = 0; i < tableOpenAddr[index]->teamsAndSalary.size(); i++)
	{
		cout << tableOpenAddr[index]->teamsAndSalary[i] << endl;
	}
}
//parse the text file and create two tables for open addressing and chaining resolution
void HashTable::parseFile()
{
	ifstream file;
	string line;
	string temp;
	int counter = 0;

	file.open("PlayerData.txt");
	//failed to open the file correctly
	if (file.fail())
	{
		cout << "File opening error" << endl;
		return;
	}
	//to remove the first line of information from the file
	getline(file, line);

	//create a hash table as the user opens the program
	while(getline(file, line))
	{
		//parse each line
		stringstream ss(line);
		//set default values for all variables to be parsed
		string firstName = "";
		string lastName = "";
		int yearBorn = -1;
		string countryBorn = "";
		int weight = -1;
		int height = -1;
		string bats = "";
		string throws = "";
		string yearID = "";
		string teamID = "";
		string leagueID = "";
		string salary = "";
		string teamInfo = "";
		int indexPos;

		//iterate through each line by parsing the string on ',', and assigning temp to each piece of parsed data
		while(getline(ss,temp,','))
		{
			//yearID
			if (counter % 13 == 0){
				yearID = temp;
				counter++;
			}
			//teamID
			else if (counter % 13 == 1){
				teamID = temp;
				counter++;
			}
			//leagueID
			else if (counter % 13 == 2){
				leagueID = temp;
				counter++;
			}
			//playerID
			else if (counter % 13 == 3){
				//dont need this info
				counter++;
			}
			//salary
			else if (counter % 13 == 4){
				salary = temp;
				teamInfo = teamInfoConcatenate(yearID, teamID, leagueID, salary);
				counter++;
			}
			//firstName
			else if (counter % 13 == 5){
				firstName = temp;
				counter++;
			}
			//lastName
			else if (counter % 13 == 6){
				lastName = temp;
				counter++;
			}
			//birthYear
			else if (counter % 13 == 7){
				yearBorn = stoi(temp);
				counter++;
			}
			//birthCountry
			else if (counter % 13 == 8){
				countryBorn = temp;
				counter++;
			}
			//weight
			else if (counter % 13 == 9){
				weight = stoi(temp);
				counter++;
			}
			//height
			else if (counter % 13 == 10){
				height = stoi(temp);
				counter++;
			}
			//bats
			else if (counter % 13 == 11){
				bats = temp;
				counter++;
			}
			//throws
			else if (counter % 13 == 12){
				throws = temp;
				//add player to both hash tables 
				insertChaining(firstName, lastName, yearBorn, countryBorn, weight, height, bats, throws, teamInfo);
				insertOpenAddr(firstName, lastName, yearBorn, countryBorn, weight, height, bats, throws, teamInfo);
				counter++;
			}
		}
	}
}