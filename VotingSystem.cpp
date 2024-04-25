/*
	Name: Voting System for Cartoon Characters 
	Copyright: 2023
	Author: Brian Cerda
	Date: 09/11/23 11:30
	Description: voting system that asks a voter to enter in their ID number and let's them vote on their favorite cartoon character 
	The program then updates the tally in a seperate file as output, outputs to an audit trail, and checks to see if the voters are registered.
	If so, then it proceeds to let the user vote.
*/
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdio>
using namespace std;
void mainMenu();
void getID(string&, bool&);
bool checkID(string);
void printTally();
void vote(int);
void updateVote(string);
void updateAuditLogStr(string);
void updateAuditLogInt(int);

//Gets voterID from user and verifies it.
void getID(string& voterID, bool& verified)
{
	cout << "Please enter your voter ID.\n";
	cin >> voterID;
	updateAuditLogStr(voterID);
	cout << "Verifying voter ID.\n";
	system("pause");
	system("CLS");
	verified = checkID(voterID);
}

//Validates user ID with VoterDataBase.txt.
bool checkID(string ID)
{
	fstream file;
	string temp, storedID, voterStatus;
	bool foundID = false;
	file.open("VoterDataBase.txt");
	while(getline(file, temp))
	{
		istringstream fileLine(temp);
		fileLine >> storedID >> voterStatus;
		if(voterStatus == "No" && storedID == ID)
		{	
			cout << "ID has been Verified\n";
			foundID = true;
			file.close();
			return foundID;
		}
		else if(voterStatus == "Yes" && storedID == ID)
		{
			cout << "The ID you entered has already voted.\n"
				 << "Please contact adminsitration if you feel there is an issue.\n";
			file.close();
			return foundID;
		}			
	}
	cout << "The ID you entered was not found.\n"
		 << "Please enter in a new ID.\n";
	file.close();
	return foundID;
}

//Menu function that allows the user to cast a vote, print the tally, or close the election.
void mainMenu()
{
	int choice = 0;
	string voterID;
	bool validVoter = false;
	do{
		getID(voterID, validVoter);
		if(validVoter == true)
		{
			cout << "EVOTE\n"
			<< "__________________\n"
			<< "1)Mickey Mouse\n"
			<< "2)Donald Duck\n"
			<< "3)Bugs Bunny\n";
			cin >> choice;
			updateAuditLogInt(choice);
			if(choice > 0 && choice < 4)
			{	
				vote(choice);
				updateVote(voterID);
				cout << "Thank you for your vote.\n";
			}
			else if(choice == 4)
			{	
				cout << "Printing current tally.\n";
				printTally();
			}
			else if(choice != 5)
			{
				cout << "You have entered an invalid value.\n"
					 << "Please try again.\n";	 
			}
		}
		system("pause");
		system("CLS");
	}while(choice <= 0 || choice < 5);
	if(choice == 5)
	{	
		cout << "Closing polls and printing results.\n";
		printTally();
	}	
}

//Updates Tally.txt with the new vote casted by the user.
void vote(int choice)
{
	ifstream inFile("Tally.txt");
	ofstream outFile("TallyUpdated.txt");
	string temp, candidateFirstName, candidateLastName;
	string border = "-------------------------------";
	int voteCount = 0;
	while(getline(inFile, temp))
	{
		istringstream fileLine(temp);
		fileLine >> candidateFirstName;
		if(choice == 1 && candidateFirstName == "Mickey")
		{
			fileLine >> candidateLastName >> voteCount;
			voteCount++;
			outFile << candidateFirstName << " " << candidateLastName << "\t" << voteCount << endl; 
		}
		else if(choice == 2 && candidateFirstName == "Donald")
		{
			fileLine >> candidateLastName >> voteCount;
			voteCount++;
			outFile << candidateFirstName << " " << candidateLastName << "\t" << voteCount << endl;
		}
		else if(choice == 3 && candidateFirstName == "Bugs")
		{
			fileLine >> candidateLastName >> voteCount;
			voteCount++;
			outFile << candidateFirstName << " " << candidateLastName << "\t" << voteCount << endl;
		}
		else if(candidateFirstName == "TOTAL")
		{
			fileLine >> candidateLastName >> voteCount;
			voteCount++;
			outFile << candidateFirstName << "\t\t" << voteCount << endl;
		}
		else if(candidateFirstName == "Candidates" || candidateFirstName == border)
		{
			outFile << temp << endl;
		}
		else
		{
			fileLine >> candidateLastName >> voteCount;
			outFile << candidateFirstName << " " << candidateLastName << "\t" << voteCount << endl;
		}
	}
	inFile.close();
	outFile.close();
	remove("Tally.txt");
	rename("TallyUpdated.txt", "Tally.txt");
}

//Prints total tally of votes from Tally.txt
void printTally()
{
	ifstream inFile("Tally.txt");
	string line;
	while(getline(inFile, line))
	{	cout << line << endl;	}
	system("pause");
	system("CLS");
	inFile.close();
}

//Updates voterDatabase.txt when a valid user votes.
void updateVote(string voterID)
{
	ifstream inFile("VoterDataBase.txt");
	ofstream outFile("UpdatedDataBase.txt");
	string temp, ID, voterStatus, firstName, lastName;
	while(getline(inFile, temp))
	{
		istringstream fileLine(temp);
		fileLine >> ID >> voterStatus >> firstName >> lastName;
		if(voterID == ID)
		{
			voterStatus = "Yes";
			outFile << ID << "\t" << voterStatus << "\t\t" << firstName << " " << lastName << endl; 	
		}
		else
		{	outFile << temp << endl;	}	
	} 
	outFile.close();
	inFile.close();
	remove("VoterDataBase.txt");
	rename("UpdatedDataBase.txt", "VoterDataBase.txt");
}

//appends string input to AuditTrail.txt
void updateAuditLogStr(string input)
{
	ofstream outFile("AuditTrail.txt", ios_base::app);
	outFile << input << endl;
	outFile.close();
}

//appends int input to AuditTrail.txt
void updateAuditLogInt(int input)
{
	ofstream outFile("AuditTrail.txt", ios_base::app);
	outFile << input << endl;
	outFile.close();
}
int main()
{
	mainMenu();
	return 0;
}