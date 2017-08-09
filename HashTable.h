/*
Jessica Barnitz
CSCI 2270
Final Project
*/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct PlayerInfo
{
	string firstName;
	string lastName;
	int yearBorn;
	string countryBorn;
	int weight;
	int height;
	string bats;
	string throws;
	PlayerInfo *next;
	PlayerInfo *previous;

	vector<string> teamsAndSalary;

	PlayerInfo(){};

	PlayerInfo(string in_firstName, string in_lastName, int in_yearBorn, string in_countryBorn, int in_weight, int in_height, string in_bats, string in_throws, string in_teamsAndSalary)
	{
		firstName = in_firstName;
		lastName = in_lastName;
		yearBorn = in_yearBorn;
		countryBorn = in_countryBorn;
		weight = in_weight;
		height = in_height;
		bats = in_bats;
		throws = in_throws;
		next = NULL;
		previous = NULL;

		teamsAndSalary.push_back(in_teamsAndSalary);
	}
};

//class HashEntry
class HashTable
{
	public:
		int tableSize = -1;
		int collisionChaining;
		int collisionOpenAddr;
		int searchOpsChaining;
		int searchOpsOpenAddr;
		//used for chaining
		PlayerInfo **table;
		PlayerInfo **tableOpenAddr;

		HashTable();
		~HashTable();
		void createTable();
		void emptySpace();
		int hashSum(string key);
		string keyConcatenate(string s1, string s2);
		string teamInfoConcatenate(string s1, string s2, string s3, string s4);
		void insertChaining(string in_firstName, string in_lastName, int in_yearBorn, string in_countryBorn, int in_weight, int in_height, string in_bats, string in_throws, string in_teamsAndSalary);
		void insertOpenAddr(string in_firstName, string in_lastName, int in_yearBorn, string in_countryBorn, int in_weight, int in_height, string in_bats, string in_throws, string in_teamsAndSalary);
		void searchChaining(string in_firstName, string in_lastName);
		void searchOpenAddr(string in_firstName, string in_lastName);
		void displayEntryOpenAddr(int index);
		void parseFile();

	protected:

	private:
		void updatePlayerInfo(PlayerInfo* player, string in_teamsAndSalary);
};

#endif //HASHTABLE_H