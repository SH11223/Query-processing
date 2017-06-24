#ifndef STUDENT_H_INCLUDED
#define STUDENT_H_INCLUDED
#define BLOCKSIZE 4096
class Student{
friend class BPNode;
private:
public:
	char *name;
	unsigned studentID;
	float score;
	unsigned advisorID;
	int Blocknum;
    Student(char name[20], unsigned studentID, float score, unsigned advisorID){
        strcpy(this->name, name);
        this->studentID = studentID;
        this->score = score;
        this->advisorID = advisorID;
    }
    Student(){
        char* a = NULL;
        this->name = a;
        this->score = 0;
        this->studentID = 0;
        this->advisorID =0;
    }
    char* getname(){
        return name;}
    float getscore(){
        return score;}
    void toPrint(ofstream& of){
        of << this->name << "," << this->studentID << "," << this->score << "," << this->advisorID << endl;}
};


Student to_student(ifstream& fin) {
    int i = 0, pos = 0;
    string tmp;
    Student student;
    getline(fin, tmp);
    pos = tmp.find(",", i);
    strcpy(student.name,tmp.substr(i, pos -i).c_str());
//	strncpy_s(student.name, tmp.substr(i, pos - i).c_str(), _TRUNCATE);
    i = pos + 1;
    pos = tmp.find(",", i);
//	student.studentID = stoi(tmp.substr(i, pos - i));
    i = pos + 1;
    pos = tmp.find(",", i);
//	student.score = stof(tmp.substr(i, pos - i));
    i = pos + 1;
//	student.advisorID = stoi(tmp.substr(i));

    return student;
}

#endif // STUDENT_H_INCLUDED
