#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include "bptree.h"
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

Professors to_prof(ifstream& fin) {
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

void JoinPS(ofstream& fout, int count) {
	int k = 0;
	_DB studDB, profDB;
	profDB.Open_prof();
	for (int q = 0; q <= count; q++) {
		_DB studDB;
		studDB.Open("Student" + to_string(count));
		for (int i = 0; i < 1024; i++) {
			if (i > 0 && studDB.H->Hash_Table.Table_Block_Offset[i] == studDB.H->Hash_Table.Table_Block_Offset[i - 1])
				continue;
			if (studDB.H->Hash_Table.Table_Block_Offset[i] == -1)
				break;
			studDB.DBFile.seekg(studDB.H->Hash_Table.Table_Block_Offset[i], ios::beg);
			studDB.DBFile.read((char*)&studDB.DB_Buffer, sizeof(Block));
			for (int j = 0; j < 1024; j++) {
				if ((j > 0 && profDB.H->Hash_Table.Table_Block_Offset[j] == profDB.H->Hash_Table.Table_Block_Offset[j - 1]) || profDB.H->Hash_Table.Table_Block_Offset[j] == -1)
					continue;
				profDB.DBFile.seekg(profDB.H->Hash_Table.Table_Block_Offset[j], ios::beg);
				profDB.DBFile.read((char*)&profDB.DB_Buffer_prof, sizeof(Block_prof));
				for (int a = 0; a < IN_BLOCK_MAX; a++) {
					for (int b = 0; b < IN_BLOCK_MAX_PRO; b++) {
						if ((studDB.DB_Buffer.Record[a].advisorID == profDB.DB_Buffer_prof.Record[b].profID) && studDB.DB_Buffer.Record[a].advisorID != 0) {
							fout << studDB.DB_Buffer.Record[a].name << ", " << studDB.DB_Buffer.Record[a].studentID << ", " << studDB.DB_Buffer.Record[a].score << ", " << studDB.DB_Buffer.Record[a].advisorID << endl;
							k++;
						}
					}
				}
			}
		}
		studDB.Close();
	}
	fout << "Join Result : " << k << endl;
	profDB.Close();
}

void ReadQuery(BPNode* stu, BPNode_P* prof, int count) {
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
		int i = 0, pos = 0, check = 0;
		getline(fin, tmp);
		pos = tmp.find(",", i);
		if (!strcmp(tmp.substr(i, pos - i).c_str(), "Search-Exact")) {
			i = pos + 1;
			pos = tmp.find(",", i);
			if (!strcmp(tmp.substr(i, pos - i).c_str(), "Students")) {
				i = pos + 1;
				pos = tmp.find(",", i);
				i = pos + 1;
				for (int q = 0; q <= count; q++) {
					_DB studDB;
					studDB.Open("Student" + to_string(q));
					student = studDB.ID_Search(stoi(tmp.substr(i)));

					if (student.studentID == 0) {}

					else {
						fout << 1 << endl;
						fout << student.name << ", " << student.studentID << ", " << student.score << ", " << student.advisorID << endl;
						check = 1;
						continue;
					}
					studDB.Close();
				}
				if (check == 0)
					fout << "Not found studentID" << endl;
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
		else if (!strcmp(tmp.substr(i, pos - i).c_str(), "Search-Range")) {
			Students temp;
			i = pos + 1;
			pos = tmp.find(",", i);
			if (!strcmp(tmp.substr(i, pos - i).c_str(), "Students")) {
				i = pos + 1;
				pos = tmp.find(",", i);
				i = pos + 1;
				float j = atof(tmp.substr(i, pos - i).c_str());
				cout << j << endl;
				pos = tmp.find(",", i);
				i = pos + 1;
				cout << tmp.substr(i).c_str() << endl;
				stu->Search(j, atof(tmp.substr(i).c_str()), fout);
			}
			else if (!strcmp(tmp.substr(i, pos - i).c_str(), "Professors")) {
				i = pos + 1;
				pos = tmp.find(",", i);
				i = pos + 1;
				int j = stoi(tmp.substr(i, pos - i));
				pos = tmp.find(",", i);
				i = pos + 1;
				prof->Search(j, stoi(tmp.substr(i)), fout);
			}
		}
		else if (!strcmp(tmp.substr(i, pos - i).c_str(), "Join")) {
			fout << "Join Start" << endl;
			JoinPS(fout, count);
		}

	}
	fin.close();
	fout.close();
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
	//      professors[i] = to_prof(fin2);
	//   }
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
	int count = 0;
	string tmp;
	// Students* students;
	Students student;
	Professors* professors;
	_DB studDB, profDB;

	BPNode* root = new BPNode();
	BPNode_P *root_P = new BPNode_P();

	ifstream fin;
	fstream sco;
	ofstream of;
	fin.open("student_data.csv");
	of.open("Students_score.idx");
	getline(fin, tmp);
	N = stoi(tmp);
	while (N > 100000) {
		N -= 100000;
		count++;
	}

	cout << count << endl;

	for (int q = 0; q < count; q++) {
		_DB studDB;
		studDB.Open("Student" + to_string(q));
		// students = new Students[100000];
		for (int i = 0; i < 100000; i++) {

			// students[i] = to_student(fin);
			student = to_student(fin);
			studDB.Insert(student);

			// root->Insert(students[i], i);

		}

		// root->Print(sco);

		// delete[] students;
		studDB.Close();
		cout << "check" << endl;
	}
	_DB studDB1;
	studDB1.Open("Student" + to_string(count));
	// students = new Students[N];

	for (int i = 0; i < N; i++) {

		// students[i] = to_student(fin);
		student = to_student(fin);
		studDB1.Insert(student);

		// root->Insert(students[i], i);

	}

	// root->Print(sco);

	// delete[] students;

	fin.close();

	fin.open("prof_data.csv");

	getline(fin, tmp);
	N = stoi(tmp);
	// N = 8000;
	profDB.Open_prof();

	professors = new Professors[N];
	for (int i = 0; i < N; i++) {
		professors[i] = to_prof(fin);
		profDB.Insert_prof(professors[i]);
		// root_P->Insert(professors[i], 0);
	}
	delete[] professors;
	
	// root_P->Print(sco);
	fin.close();
	studDB1.Close();
	profDB.Close();
	of.close();
	ReadQuery(root, root_P, count);

	return 0;


}