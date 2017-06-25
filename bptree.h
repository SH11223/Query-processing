#ifndef BPTREE_H_INCLUDED
#define BPTREE_H_INCLUDED
using namespace std;
#include "students.h"
#define IN_NODE_MAX ((BLOCKSIZE - sizeof(Students)) / (sizeof(Students)+sizeof(int)+sizeof(int*)))
#define IN_NODE_MAX_P ((BLOCKSIZE - 3*sizeof(int*) -2*sizeof(int) -sizeof(bool)) / (sizeof(Professors) + sizeof(int) + sizeof(int*))

class BPNode{
private:
    BPNode* nextleaf;
    BPNode* childnode[IN_NODE_MAX+1];
	BPNode* parentnode;
    Students students[IN_NODE_MAX];
	int BNum[IN_NODE_MAX];
    bool leaf;
    int in;
	char blank[BLOCKSIZE - 3*sizeof(int*) - 2*sizeof(int) - sizeof(bool)- IN_NODE_MAX*(sizeof(Students) + sizeof(int) + sizeof(int*))];
public:
    BPNode(){
        nextleaf = NULL;
		parentnode = NULL;
		for(int i = 0; i < IN_NODE_MAX+1 ; i++){
			childnode[i] = NULL;
		}
        leaf = true;
        in = 0;
    }
    bool isFull(){
        return (IN_NODE_MAX == in);
    }
    void Sort(){
        Students temp;
		int BNtemp;
		int a = this->in;
        BPNode* childtem = new BPNode();
        int i, j;
		temp = Copy(this->students[a-1]);
		BNtemp = BNum[a-1];
        for (i =0; i<a-1; i++){
            if(temp.score < this->students[i].score){
                break;
            }
        }
        for(j=a;j>i+1;j--){
            students[j-1] = Copy(students[j-2]);
			BNum[j-1] = BNum[j-2];
        }
        students[i] = Copy(temp);
		BNum[i] = BNtemp;
        if(!this->isLeaf()){
            childtem = this->childnode[a];
            for(j = a; j > i+1; j--){
				this->childnode[j] = this->childnode[j-1];
            }
            this->childnode[i+1] = childtem;
        }
    }
    BPNode* Brother(){
        BPNode* b = new BPNode();
		b->parentnode = this->parentnode;
        b->leaf = this->leaf;
        return b;
    }
    void Split(){
		//Students* mid;
		//int* mid;
		int k =0;
		int j =0;
		int s =0;
        BPNode* parenttmp;
        BPNode* childtem;
        if(this->parentnode == NULL && this->isLeaf()){			//When this node is a root.
			
            BPNode* new1 = new BPNode();
            BPNode* new2 = new BPNode();
			s =0;
            for(k = this->in/2+1; k<in; k++){
				
                new2->students[s] = Copy(this->students[k]);
				
				new2->BNum[s] = this->BNum[k];
                new2->in++;
				s++;
            }
            for(j = 0; j< this->in/2+1 ; j++){
                new1->students[j] = Copy(this->students[j]);
				new1->BNum[j] = this->BNum[j];
                new1->in++;
            }
			
            new1->nextleaf = new2;
            this->students[0] = Copy(new1->students[new1->in-1]);
			this->BNum[0] = new1->BNum[0];
            this->childnode[0] = new1;
            this->childnode[1] = new2;
            this->leaf = false;
            this->in = 1;
            new1->parentnode = this;
            new2->parentnode = this;
        }
		else if (this->parentnode == NULL){
            BPNode* new1 = new BPNode();
            BPNode* new2 = new BPNode();
			s =0;
            for(k = this->in/2+1; k<this->in; k++){
				
                new2->students[s] = Copy(this->students[k]);
				
				new2->childnode[s+1] = this->childnode[k];
				new2->BNum[s] = this->BNum[k];
                new2->in++;
				s++;
            }
            for(j = 0; j< this->in/2+1 ; j++){
                new1->students[j] = Copy(this->students[j]);
				new1->BNum[j] = this->BNum[j];
				new1->childnode[j] = this->childnode[j];
                new1->in++;
            }
            this->students[0] = Copy(new1->students[new1->in-1]);
			this->BNum[0] = new1->BNum[new1->in-1];
            this->childnode[0] = new1;
            this->childnode[1] = new2;
            new1->leaf = false;
			new2->leaf = false;
            this->in = 1;
            new1->parentnode = this;
            new2->parentnode = this;
		}
        else{ //When this node is not a root.
			s =0;
            BPNode* new1 = this->Copy();
			int k;
			//childtem = new BPNode();
			//childtem = 
            for(k = this->in/2+1; k<this->in+1; k++){
                new1->students[s] = Copy(this->students[k]);
				new1->BNum[s] = this->BNum[k];
                new1->in++;
				new1->childnode[s+1]=this->childnode[k];
				s++;
				
                //this->students[k] =0;
            }
            this->in = IN_NODE_MAX/2+1;
            if(new1->isLeaf()){
                new1->nextleaf = this->nextleaf;
                this->nextleaf = new1;
            }
			parenttmp = this->parentnode;
			parenttmp->students[parenttmp->in] = Copy(this->students[this->in-1]);
			parenttmp->BNum[parenttmp->in] = this->BNum[this->in-1];
			parenttmp->in++;
			parenttmp->childnode[parenttmp->in] = new1;
			parenttmp->Sort();
			if(parenttmp->isFull())
				parenttmp->Split();
            //insert parent에 scores[in/2]
            //childnode 정리

        }


    }
    bool isEmpty(){
        return (in == 0);}
    bool isLeaf(){
        return leaf;}
    void Insert(Students stu, int Bnum){ //insert 
		int t= 0;
        int i = 0;
        BPNode* tmp = new BPNode();
        tmp = this;

        if(tmp->in == 0){
			tmp->students[tmp->in] = Copy(stu);
			tmp->BNum[tmp->in] = Bnum;
			tmp->in++;
        }
        else{
            while(!tmp->isLeaf()){
                for(i = 0; i < tmp->in; i++){
                    if(stu.score < tmp->students[0].score){
                        break;
                    }
					else if (stu.score > tmp->students[tmp->in-1].score){
						i=tmp->in-1;
					}
					else if(stu.score > tmp->students[i].score && stu.score < tmp->students[i+1].score){
						i++;
						break;
					}
					else if(stu.score == tmp->students[i].score && stu.score < tmp->students[i+1].score){
						i++;
						break;
					}
                    else
                        continue;
                }
                tmp = tmp->childnode[i];
            }
            tmp->students[tmp->in] = Copy(stu);
			tmp->BNum[tmp->in] = Bnum;
			tmp->in++;
            tmp->Sort();
			if(tmp->isFull()){
				tmp->Split();                                                                                          
			}
        }

    }
    void Print(ofstream& of){
		int s = 0;
		int i =0;
        BPNode* tmp = this;
        while(!tmp->isLeaf()){
            tmp = tmp->childnode[0];
        }
        while(tmp!= NULL){
			of << "LEAF NODE No." << s+1 << endl;
            for(i = 0; i<tmp->in; i++){
				of << tmp->students[i].score << ", " << tmp->students[i].studentID << "," << tmp->BNum[i] <<endl;
            }
            tmp = tmp->nextleaf;
			of << "-----------------------------------------------------------------------" << endl;
			s++;
        }
		/*
  		int s = 0;
  		int i =0;
          BPNode* tmp = this;
          while(!tmp->isLeaf()){
              tmp = tmp->childnode[0];
          }
		of.open("Students_score.idx", ios::in | ios::out | ios::binary);
		if(!of)
			of.open("Students_score.idx", ios::in | ios::out | ios::binary | ios::trunc);
		while (tmp != NULL) {
			of.write((char*)tmp, sizeof(BPNode));
			tmp = tmp->nextleaf;
		}
		of.close();*/
	}
    void Print(int k){
		int j =0;
        BPNode* tmp = this;
        while(!tmp->isLeaf()){
            tmp = tmp->childnode[0];
        }
        int i = 0;
		for( i =0;i<k;i++){
			tmp = tmp->nextleaf;
			if(tmp == NULL){
				cout << k << "is too big." << endl;
				break;
			}
		}
		if(tmp!= NULL){
			for(j =0; j< tmp->in ;j++){
				cout << tmp->students[j].score << ", " << tmp->students[j].studentID << "," << tmp->BNum[j] <<endl;
			}
		}
    }
	Students Copy(Students& a){
		Students b;
		strcpy(b.name, a.name);
		b.studentID = a.studentID;
		b.score = a.score;
		b.advisorID = a.advisorID;
		return b;
	}
};

class BPNode_P{
private:
    BPNode* nextleaf;
    BPNode* childnode[IN_NODE_MAX_P+1];
	BPNode* parentnode;
    Professors professors[IN_NODE_MAX_P];
	int BNum[IN_NODE_MAX_P];
    bool leaf;
    int in;
	char blank[BLOCKSIZE - 3*sizeof(int*) - 2*sizeof(int) - sizeof(bool)- IN_NODE_MAX_P*(sizeof(Professors) + sizeof(int) + sizeof(int*))];
public:
    BPNode(){
        nextleaf = NULL;
		parentnode = NULL;
		for(int i = 0; i < IN_NODE_MAX+1 ; i++){
			childnode[i] = NULL;
		}
        leaf = true;
        in = 0;
    }
    bool isFull(){
        return (IN_NODE_MAX_P == in);
    }
    bool isEmpty(){
        return (in == 0);}
    bool isLeaf(){
        return leaf;}
    BPNode* Brother(){
        BPNode* b = new BPNode();
		b->parentnode = this->parentnode;
        b->leaf = this->leaf;
        return b;
    }
    void Sort(){
        Professors temp;
		int BNtemp;
		//int a = this->in;
       // BPNode* childtem = new BPNode();
        BPNode* childtem;
        int i, j;
		temp = Copy(this->professors[this->in-1]);
		BNtemp = BNum[a-1];
        for (i =0; i<a-1; i++){
            if(temp.Salary < this->professors[i].Salary){
                break;
            }
        }
        for(j = this->in;j>i+1;j--){
            professors[j-1] = Copy(professors[j-2]);
			BNum[j-1] = BNum[j-2];
        }
        professors[i] = Copy(temp);
		BNum[i] = BNtemp;
        if(!this->isLeaf()){
            childtem = this->childnode[a];
            for(j = a; j > i+1; j--){
				this->childnode[j] = this->childnode[j-1];
            }
            this->childnode[i+1] = childtem;
        }
    }
    void Split(){
		int k =0;
		int j =0;
		int s =0;
        BPNode* parenttmp;
        BPNode* childtem;
		
        if(this->parentnode == NULL && this->isLeaf()){			//When this node is a root.
            BPNode* new1 = new BPNode();
            BPNode* new2 = new BPNode();
			s =0;
            for(k = this->in/2+1; k<in; k++){
				
                new2->parentnode[s] = Copy(this->parentnode[k]);
				
				new2->BNum[s] = this->BNum[k];
                new2->in++;
				s++;
            }
            for(j = 0; j< this->in/2+1 ; j++){
                new1->parentnode[j] = Copy(this->parentnode[j]);
				new1->BNum[j] = this->BNum[j];
                new1->in++;
            }
			
            new1->nextleaf = new2;
            this->parentnode[0] = Copy(new1->parentnode[new1->in-1]);
			this->BNum[0] = new1->BNum[0];
            this->childnode[0] = new1;
            this->childnode[1] = new2;
            this->leaf = false;
            this->in = 1;
            new1->parentnode = this;
            new2->parentnode = this;
        }
		else if (this->parentnode == NULL){
            BPNode* new1 = new BPNode();
            BPNode* new2 = new BPNode();
			s =0;
            for(k = this->in/2+1; k<this->in; k++){
				
                new2->parentnode[s] = Copy(this->parentnode[k]);
				
				new2->childnode[s+1] = this->childnode[k];
				new2->BNum[s] = this->BNum[k];
                new2->in++;
				s++;
            }
            for(j = 0; j< this->in/2+1 ; j++){
                new1->parentnode[j] = Copy(this->parentnode[j]);
				new1->BNum[j] = this->BNum[j];
				new1->childnode[j] = this->childnode[j];
                new1->in++;
            }
            this->parentnode[0] = Copy(new1->parentnode[new1->in-1]);
			this->BNum[0] = new1->BNum[new1->in-1];
            this->childnode[0] = new1;
            this->childnode[1] = new2;
            new1->leaf = false;
			new2->leaf = false;
            this->in = 1;
            new1->parentnode = this;
            new2->parentnode = this;
		}
        else{ //When this node is not a root.
			s =0;
            BPNode* new1 = this->Copy();
			int k;
			//childtem = new BPNode();
			//childtem = 
            for(k = this->in/2+1; k<this->in+1; k++){
                new1->parentnode[s] = Copy(this->parentnode[k]);
				new1->BNum[s] = this->BNum[k];
                new1->in++;
				new1->childnode[s+1]=this->childnode[k];
				s++;
				
                //this->parentnode[k] =0;
            }
            this->in = IN_NODE_MAX/2+1;
            if(new1->isLeaf()){
                new1->nextleaf = this->nextleaf;
                this->nextleaf = new1;
            }
			parenttmp = this->parentnode;
			parenttmp->parentnode[parenttmp->in] = Copy(this->parentnode[this->in-1]);
			parenttmp->BNum[parenttmp->in] = this->BNum[this->in-1];
			parenttmp->in++;
			parenttmp->childnode[parenttmp->in] = new1;
			parenttmp->Sort();
			if(parenttmp->isFull())
				parenttmp->Split();
            //insert parent에 scores[in/2]
            //childnode 정리

        }


    }
    void Insert(Professors prof, int Bnum){ //insert 
		int t= 0;
        int i = 0;
        BPNode* tmp = this;
        if(tmp->in == 0){
			tmp->professors[0] = Copy(prof);
			tmp->BNum[0] = Bnum;
			tmp->in++;
        }
        else{
            while(!tmp->isLeaf()){
                for(i = 0; i < tmp->in; i++){
                    if(prof.Salary < tmp->professors[0].Salary){
                        break;
                    }
				/*	else if (prof.Salary > tmp->professors[tmp->in-1].Salary){
						i=tmp->in-1;
					}*/
					else if(prof.Salary > tmp->professors[i].Salary && prof.Salary < tmp->professors[i+1].Salary){
						i++;
						break;
					}
					else if(prof.Salary == tmp->professors[i].Salary && prof.Salary < tmp->professors[i+1].Salary){
						i++;
						break;
					}
                    else
                        continue;
                }
                tmp = tmp->childnode[i];
            }
            tmp->professors[tmp->in] = Copy(prof);
			tmp->BNum[tmp->in] = Bnum;
			tmp->in++;
            tmp->Sort();
			if(tmp->isFull()){
				tmp->Split();                                                                                          
			}
        }
    }
    void Print(ofstream& of){
		int s = 0;
		int i =0;
        BPNode* tmp = this;
        while(!tmp->isLeaf()){
            tmp = tmp->childnode[0];
        }
        while(tmp!= NULL){
			of << "LEAF NODE No." << s+1 << endl;
            for(i = 0; i<tmp->in; i++){
				of << tmp->parentnode[i].Salary << ", " << tmp->parentnode[i].prodID << "," << tmp->BNum[i] <<endl;
            }
            tmp = tmp->nextleaf;
			of << "-----------------------------------------------------------------------" << endl;
			s++;
        }
		/*
  		int s = 0;
  		int i =0;
          BPNode* tmp = this;
          while(!tmp->isLeaf()){
              tmp = tmp->childnode[0];
          }
		of.open("Students_score.idx", ios::in | ios::out | ios::binary);
		if(!of)
			of.open("Students_score.idx", ios::in | ios::out | ios::binary | ios::trunc);
		while (tmp != NULL) {
			of.write((char*)tmp, sizeof(BPNode));
			tmp = tmp->nextleaf;
		}
		of.close();*/
	}
    void Print(int k){
		int j =0;
        BPNode* tmp = this;
        while(!tmp->isLeaf()){
            tmp = tmp->childnode[0];
        }
        int i = 0;
		for( i =0;i<k;i++){
			tmp = tmp->nextleaf;
			if(tmp == NULL){
				cout << k << "is too big." << endl;
				break;
			}
		}
		if(tmp!= NULL){
			for(j =0; j< tmp->in ;j++){
				cout << tmp->parentnode[j].Salary << ", " << tmp->parentnode[j].prodID << "," << tmp->BNum[j] <<endl;
			}
		}
    }
	Professors Copy(Professors& a){
		Professors b;
		strcpy(b.name, a.name);
		b.profID = a.profID;
		b.Salary = a.Salary;
		return b;
	}
};

#endif // BPTREE_H_INCLUDED
