#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <cstring>
#include <stdlib.h>
// #include "bptree.h"
#include "students.h"
#include "dynamic_hash.h"
#include "DB.h"

#pragma pack(1)

using namespace std;

Students::Students()
{
	for (int i = 0; i < 20; i++)
		name[i] = '\0';
	studentID = 0;
	score = 0;
	advisorID = 0;
}

Professors::Professors()
{
	for (int i = 0; i < 20; i++)
		name[i] = '\0';
	profID = 0;
	Salary = 0;
}

Block::Block()
{
	Record_Count = 0;
	Bit_Num = 0;
}

Block_prof::Block_prof()
{
	Record_Count = 0;
	Bit_Num = 0;
}

Students to_student(ifstream& fin) {
	int i = 0, pos = 0;
	string tmp;
	Students student;
	getline(fin, tmp);
	pos = tmp.find(",", i);
	strcpy(student.name, tmp.substr(i, pos - i).c_str());
	i = pos + 1;
	pos = tmp.find(",", i);
	student.studentID = stoi(tmp.substr(i, pos - i));
	i = pos + 1;
	pos = tmp.find(",", i);
	student.score = stof(tmp.substr(i, pos - i));
	i = pos + 1;
	student.advisorID = stoi(tmp.substr(i));

	return student;
}

Professors to_prof(ifstream& fin){
	int i = 0, pos = 0;
	string tmp;
	Professors prof;
	getline(fin, tmp);
	pos = tmp.find(",", i);
	strcpy(prof.name, tmp.substr(i, pos - i).c_str());
	i = pos + 1;
	pos = tmp.find(",", i);
	prof.profID = stoi(tmp.substr(i, pos - i));
	i = pos + 1;
	prof.Salary = stoi(tmp.substr(i));

	return prof;
}

void ReadQuery() {
	int K;
	Students student;
	Professors professor;

	_DB studDB, profDB;
	ifstream fin;
	string tmp;
	fin.open("query.dat");

	ofstream fout;
	fout.open("query.res");

	getline(fin, tmp);
	K = stoi(tmp);

	for (int j = 0; j < K; j++) {
		int i = 0, pos = 0;
		getline(fin, tmp);
		pos = tmp.find(",", i);
		if (!strcmp(tmp.substr(i, pos - i).c_str(), "Search-Exact")) {
			i = pos + 1;
			pos = tmp.find(",", i);
			if (!strcmp(tmp.substr(i, pos - i).c_str(), "Students")) {
				i = pos + 1;
				pos = tmp.find(",", i);
				i = pos + 1;
				studDB.Open();
				student = studDB.ID_Search(stoi(tmp.substr(i)));
				
				if (student.studentID == 0)
					fout << "Not Found studentID" << endl;
			
				else {
					fout << 1 << endl;
					fout << student.name << ", " << student.studentID << ", " << student.score << ", " << student.advisorID << endl;
				}
				studDB.Close();
			}
			else if (!strcmp(tmp.substr(i, pos - i).c_str(), "Professors")) {
				i = pos + 1;
				pos = tmp.find(",", i);
				i = pos + 1;
				profDB.Open_prof();
				professor = profDB.ID_Search_prof(stoi(tmp.substr(i)));

				if (professor.profID == 0)
					fout << "Not Found profID" << endl;
				else {
					fout << 1 << endl;
					fout << professor.name << ", " << professor.profID << ", " << professor.Salary << endl;
				}
				profDB.Close();
			}
		}
		else if (tmp.substr(i, pos - i).c_str() == "Search-Range") {

		}
		else if (tmp.substr(i, pos - i).c_str() == "Join") {

		}
		
	}
	fin.close();
	fout.close();
}

void Exact_Prof(){
}

void Exact_Stud(){
}

void Range_Prof(){
}

void Range_Stud(){
}


int main() {
	/*
	int N, N2;
	int k =0 ;
	string tmp;
	Professors* professors;
	Students* students;
	_DB testDB;
	
	if(strcmp(argv[1],"Search-Exact")==0)
	{
		if(argc!= 5)
			cout << "error" << endl;
		else if(strcmp(argv[2],"Professors")==0)
			Exact_Prof(argv[3], argv[4]);
		else if(strcmp(argv[2],"Students")==0)
			Exact_Stud(argv[3], argv[4]);
		else
			cout<<"error" << endl;
	}
	else if(strcmp(argv[1],"Search-Range")==0){
		if(argc!= 6)
			cout << "error" << endl;
		else if(strcmp(argv[2],"Professors")==0)
			Range_Prof(argv[3], argv[4],argv[5]);
		else if(strcmp(argv[2],"Students")==0)
			Range_Stud(argv[3], argv[4],argv[5]);
		else
			cout<<"error" << endl;
	}
	else if(strcmp(argv[1],"Join")==0){
		if(argc!= 4)
			cout << "error" << endl;
	}
	else
		cout << "error" << endl;
	
	BPNode* root = new BPNode();

	ifstream fin1, fin2;
	fin1.open("sampleData.csv");
	fin2.open("prof_data.csv");

	
	fstream sco;
	ofstream fout1, fout2;
	fout1.open("Students_score.idx");
	fout2.open("Professor_Salary.idx");
	getline(fin1, tmp);
	N = stoi(tmp);
	getline(fin2, tmp);
	N2 = stoi(tmp);
	cout << N2 ;
	
	testDB.Open();
	
	students = new Students[N];
	
	for (int i = 0; i < N; i++) {
		students[i] = to_student(fin1);
		testDB.Insert(students[i]);
		root->Insert(students[i],testDB.BlockNum(students[i].studentID));
	}
	
	cout << sizeof(BPNode);
	testDB.Print();

	
	professors = new Professors[N2];
	//for (int i = 0; i < N2; i++) {
//		professors[i] = to_prof(fin2);
//	}
	cout << "Enter the leaf number" << endl;
	cin >> k;

	for (int i = 0; i < N; i++) {
		root->Insert(students[i],testDB.BlockNum(students[i].studentID));
		
	}
	root->Print(fout2);
	root->Print(k);
	fin1.close();
	fin2.close();
	fout1.close();
	fout2.close();
	testDB.Close();
	delete [] students;
	delete [] professors;
	return 0;
	*/

	int N;
	int k = 0;
	string tmp;
	Students* students;
	Professors* professors;
	_DB studDB, profDB;

	// BPNode* root = new BPNode();

	ifstream fin;
	fstream sco;

	fin.open("student_data.csv");

	getline(fin, tmp);
	N = stoi(tmp);
	N = 100000;

	studDB.Open();
	students = new Students[N];
	for (int i = 0; i < N; i++) {
		students[i] = to_student(fin);
		studDB.Insert(students[i]);
	}

	cout << "check" << endl;

	delete[] students;

	fin.close();

	fin.open("prof_data.csv");

	getline(fin, tmp);
	N = stoi(tmp);

	profDB.Open_prof();
	professors = new Professors[N];
	for (int i = 0; i < N; i++) {
		professors[i] = to_prof(fin);
		profDB.Insert_prof(professors[i]);
	}
	delete[] professors;

	fin.close();

	ReadQuery();

	return 0;


}