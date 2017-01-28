/**
 *event counter
 *
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>

//#define BLACK 1
//#define RED 0

using namespace std;

//tree node for the red black tree
struct treeNode{
	int ID;		//ID
	int count;	//count
	bool color;		//tree color
	treeNode *leftTree, *rightTree, *parent;	//left child,right child and parent pointer

	//constructer
	treeNode(int ID1, int count1, bool color1, treeNode *p, treeNode *l, treeNode *r){
		ID = ID1;
		count = count1;
		color = color1;
		parent = p;
		leftTree = l;
		rightTree = r;
	}
};

//the main structure for the event counter
class counter{
public:
	treeNode *cRoot;	//root node
	
	//search for the node whose ID is theId
	treeNode* Find(int theId);

	//insert the node into tree ,the node's id is theId and count is m
	void Insert(int theId, int m);

	//after insert process the tree may be unbalanced,this function is to help the tree get balanced
	void fixTree(treeNode *p);

	//the left rotate for the tree
	void rotateLeft(treeNode *p);

	//the right rotate for the tree
	void rotateRight(treeNode *p);

	//remove the node whose ID is theId
	void Remove(int theId);

	//after remove process, the tree may be unbalanced,this function is to rebalance the tree
	void fixTree2(treeNode *p1, treeNode *p2);

	//this function is to delete the tree
	void RemoveTree(treeNode *p){
		if (p == NULL)
			return;
		if (p->leftTree != NULL)
			return RemoveTree(p->leftTree);
		if (p->rightTree != NULL)
			return RemoveTree(p->rightTree);
		delete p;
		p = NULL;
	}

	//this function is to find the node which is the successor node of the node whose ID is theId
	treeNode* findNext(int theId);

	//this function is to find the node which is the predecessor node of the node whose ID is theId
	treeNode* findPrevious(int theId);

	counter():cRoot(NULL){
		cRoot = NULL;
	}

	~counter(){
		RemoveTree(cRoot);
	}

	treeNode* getRoot(){
		return cRoot;
	}

	//functions that are required
	void Increase(int theId, int m);
	void Reduce(int theId, int m);
	void Count(int theId);
	void InRange(int ID1, int ID2);
	void Next(int theId);
	void Previous(int theId);
};

//search for the node whose ID is theId
treeNode* counter::Find(int theId){
	treeNode* tempNode;
	tempNode = cRoot;
	if (tempNode == NULL)
		return NULL;
	else
		while (tempNode->ID != theId){
			if (theId<tempNode->ID)		//if theId is smaller than the ID of parent,it is on the left of the parent
				tempNode = tempNode->leftTree;
			else
				tempNode = tempNode->rightTree;
			if (tempNode == NULL)
				return NULL;
		}
	return tempNode;
}

//insert the node into tree ,the node's id is theId and count is m
void counter::Insert(int theId, int m){
	treeNode *tempNode, *tempParent;
	treeNode *insertNode = new treeNode(theId, m, 0, NULL, NULL, NULL);
	tempNode = cRoot;
	tempParent = NULL;
	if (tempNode == NULL){
		cRoot = insertNode;
		return;
	}

	while (tempNode != NULL){
		tempParent = tempNode;
		if (insertNode->ID<tempNode->ID)
			tempNode = tempNode->leftTree;
		else
			tempNode = tempNode->rightTree;
	}
	insertNode->parent = tempParent;
	if (insertNode->ID<tempParent->ID)
		tempParent->leftTree = insertNode;
	else
		tempParent->rightTree = insertNode;

	fixTree(insertNode);
}

//after insert process the tree may be unbalanced,this function is to help the tree get balanced
void counter::fixTree(treeNode *p){
	treeNode *tempNode, *tempParent, *tempGrandParent, *tempUncle;
	tempNode = p;
	tempParent = tempNode->parent;
	while ((tempParent != NULL) && (tempParent->color == 0)){//5cases when it is unbalanced
		tempGrandParent = tempParent->parent;
		if ((tempGrandParent != NULL)&&(tempParent == tempGrandParent->leftTree)){
			tempUncle = tempGrandParent->rightTree;
			if ((tempUncle != NULL) && (tempUncle->color == 0)){
				tempUncle->color = 1;
				tempParent->color = 1;
				tempGrandParent->color = 0;
				tempNode = tempGrandParent;
				continue;
			}
			if (tempNode == tempParent->rightTree){
				treeNode *q;
				rotateLeft(tempParent);
				q = tempParent;
				tempParent = tempNode;
				tempNode = q;
			}
			tempParent->color = 1;
			tempGrandParent->color = 0;
			rotateRight(tempGrandParent);
		}
		else{
			if (tempGrandParent == NULL)
				break;
			tempUncle = tempGrandParent->leftTree;
			if ((tempUncle != NULL) && (tempUncle->color == 0)){
				tempUncle->color = 1;
				tempParent->color = 1;
				tempGrandParent->color = 0;
				tempNode = tempGrandParent;
				continue;
			}
			if (tempNode == tempParent->leftTree){
				treeNode *q;
				rotateRight(tempParent);
				q = tempParent;
				tempParent = tempNode;
				tempNode = q;
			}
			tempParent->color = 1;
			tempGrandParent->color = 0;
			rotateLeft(tempGrandParent);
		}
		tempParent = tempNode->parent;
	}
	cRoot->color = 1;
}

//the left rotate for the tree
void counter::rotateLeft(treeNode *p){
	treeNode *tempNode, *tempRight;
	tempNode = p;
	tempRight = p->rightTree;
	tempNode->rightTree = tempRight->leftTree;
	if (tempRight->leftTree != NULL)
		tempRight->leftTree->parent = tempNode;
	tempRight->parent = tempNode->parent;

	if (tempNode->parent == NULL)
		cRoot = tempRight;
	else if (tempNode->parent->leftTree == tempNode)
		tempNode->parent->leftTree = tempRight;
	else
		tempNode->parent->rightTree = tempRight;
	tempRight->leftTree = tempNode;
	tempNode->parent = tempRight;
}

//the right rotate for the tree
void counter::rotateRight(treeNode *p){
	treeNode *tempNode, *tempLeft;
	tempNode = p;
	tempLeft = p->leftTree;
	tempNode->leftTree = tempLeft->rightTree;
	if (tempLeft->rightTree != NULL)
		tempLeft->rightTree->parent = tempNode;
	tempLeft->parent = tempNode->parent;

	if (tempNode->parent == NULL)
		cRoot = tempLeft;
	else if (tempNode->parent->rightTree == tempNode)
		tempNode->parent->rightTree = tempLeft;
	else
		tempNode->parent->leftTree = tempLeft;
	tempLeft->rightTree = tempNode;
	tempNode->parent = tempLeft;
}

//remove the node whose ID is theId
void counter::Remove(int theId){
	treeNode *tempNode, *tempChild, *tempParent;
	bool tempColor;
	tempNode = Find(theId);
	if ((tempNode->leftTree != NULL) && (tempNode->rightTree != NULL)){
		treeNode *tempFollow;
		tempFollow = tempNode->rightTree;
		while (tempFollow->leftTree != NULL)
			tempFollow = tempFollow->leftTree;
		if (tempNode != cRoot){
			if (tempNode->parent->leftTree == tempNode)
				tempNode->parent->leftTree = tempFollow;
			else
				tempNode->parent->rightTree = tempFollow;
		}
		else
			cRoot = tempFollow;

		tempChild = tempFollow->rightTree;
		tempParent = tempFollow->parent;
		tempColor = tempFollow->color;
		if (tempParent == tempNode)
			tempParent = tempFollow;
		else{
			if (tempChild != NULL)
				tempChild->parent = tempParent;
			tempParent->leftTree = tempChild;
			tempFollow->rightTree = tempNode->rightTree;
			tempNode->rightTree->parent = tempFollow;
		}
		tempFollow->parent = tempNode->parent;
		tempFollow->color = tempNode->color;
		tempFollow->leftTree = tempNode->leftTree;
		tempNode->leftTree->parent = tempFollow;
		if (tempColor == 1)
			fixTree2(tempChild, tempParent);
		delete tempNode;
		return;
	}
	if (tempNode->leftTree != NULL)
		tempChild = tempNode->leftTree;
	else
		tempChild = tempNode->rightTree;

	tempParent = tempNode->rightTree;
	tempColor = tempNode->color;
	if (tempChild != NULL)
		tempChild->parent = tempParent;

	if (tempParent != NULL){
		if (tempParent->leftTree == tempNode)
			tempParent->leftTree = tempChild;
		else
			tempParent->rightTree = tempChild;
	}
	else
		cRoot = tempChild;

	if (tempColor == 1)
		fixTree2(tempChild, tempParent);
	delete tempNode;
}

//after remove process, the tree may be unbalanced,this function is to rebalance the tree
void counter::fixTree2(treeNode *p1, treeNode *p2){
	treeNode *tempNode, *tempParent, *temp;
	tempNode = p1;
	tempParent = p2;
	if (tempNode == NULL)
		return;
	while (((tempNode != NULL) || (tempNode->color == 1)) && (tempNode != cRoot)){
		if (tempParent->leftTree == tempNode){
			temp = tempParent->rightTree;
			if (temp->color == 0){
				temp->color = 1;
				tempParent->color = 0;
				rotateLeft(tempParent);
				temp = tempParent->rightTree;
			}
			if (((temp->leftTree != NULL) || (temp->leftTree->color == 1)) && ((temp->rightTree != NULL) || (temp->rightTree->color == 1))){
				temp->color = 0;
				tempNode = tempParent;
				tempParent = tempNode->parent;
			}
			else{
				if ((temp->rightTree != NULL) || (temp->rightTree->color == 1)){
					temp->leftTree->color = 1;
					temp->color = 0;
					rotateRight(temp);
					temp = tempParent->rightTree;
				}
				temp->color = tempParent->color;
				tempParent->color = 1;
				temp->rightTree->color = 1;
				rotateLeft(tempParent);
				tempNode = cRoot;
				break;
			}
		}
		else{
			temp = tempParent->leftTree;
			if (temp->color == 0){
				temp->color = 1;
				tempParent->color = 0;
				rotateRight(tempParent);
				temp = tempParent->leftTree;
			}
			if (((temp->leftTree != NULL) || (temp->leftTree->color == 1)) && ((temp->rightTree != NULL) || (temp->rightTree->color == 1))){
				temp->color = 0;
				tempNode = tempParent;
				tempParent = tempNode->parent;
			}
			else{
				if ((temp->leftTree != NULL) || (temp->leftTree->color == 1)){
					temp->rightTree->color = 1;
					temp->color = 0;
					rotateLeft(temp);
					temp = tempParent->rightTree;
				}
				temp->color = tempParent->color;
				tempParent->color = 1;
				temp->leftTree->color = 1;
				rotateRight(tempParent);
				tempNode = cRoot;
				break;
			}
		}
	}
	if (tempNode != NULL)
		tempNode->color = 1;
}

//this function is to find the node which is the successor node of the node whose ID is theId
treeNode* counter::findNext(int theId){
	treeNode *pr, *pp;
	treeNode *p = Find(theId);
	if (p == NULL)
		return p;
	pr = p->rightTree;
	if (pr != NULL){
		while (pr->leftTree != NULL)
			pr = pr->leftTree;
		return pr;
	}
	pp = p->parent;
	while ((pp != NULL) && (p == pp->rightTree)){
		p = pp;
		pp = pp->parent;
	}
	return pp;
}

//this function is to find the node which is the predecessor node of the node whose ID is theId
treeNode* counter::findPrevious(int theId){
	treeNode *pl, *pp;
	treeNode *p = Find(theId);
	if (p == NULL)
		return p;
	pl = p->leftTree;
	if (pl != NULL){
		while (pl->rightTree != NULL)
			pl = pl->rightTree;
		return pl;
	}
	pp = p->parent;
	while ((pp != NULL) && (p == pp->leftTree)){
		p = pp;
		pp = pp->parent;
	}
	return pp;
}

//Increase the count of the event theID by m
void counter::Increase(int theId, int m){
	treeNode *node;
	node = Find(theId);
	if (node == NULL)	//when the node of theId does not exist,insert it into the tree
		Insert(theId, m);
	else
		node->count = node->count + m;
	node = Find(theId);
	cout << node->count << endl;
}

//Decrease the count of theID by m
void counter::Reduce(int theId, int m){
	treeNode *node;
	node = Find(theId);
	if (node == NULL)
		cout << "0" << endl;
	else if (node->count <= m){
		Remove(theId);
		cout << "0" << endl;
	}
	else{
		node->count = node->count - m;
		cout << node->count << endl;
	}
}

//Print the count of theID
void counter::Count(int theId){
	treeNode *node;
	node = Find(theId);
	if (node == NULL)
		cout << "0" << endl;
	else
		cout << node->count << endl;
}

//Print the total count for IDs between ID1 and ID2
void counter::InRange(int ID1, int ID2){
	int total,tempid;
	treeNode *currentNode,*endNode;
	if(Find(ID1)==NULL){
		tempid = ID1 + 1;
		while(Find(tempid)==NULL){
			tempid++;
			if(tempid>ID2){
				cout << "0" << endl;
				return;
			}
		}
	}
	else
		tempid = ID1;
	total = Find(tempid)->count;
	currentNode = Find(tempid);
	if(Find(ID2)==NULL){
		tempid = ID2 - 1;
		while(Find(tempid)==NULL)
			tempid--;
	}
	endNode = Find(tempid);
	while (currentNode != endNode){
		currentNode = findNext(currentNode->ID);
		total = total + currentNode->count;
	}
	cout << total << endl;
}

//Print the ID and the count of the event with the lowest ID that is greater that theID
void counter::Next(int theId){
	treeNode *node;
	if(Find(theId)==NULL){
		int tempid = theId;
		while(Find(tempid)==NULL){
			tempid++;
			if(tempid>100000){
				cout << "0 0" << endl;
				return;
			}
		}
		node = Find(tempid);
		cout << node->ID << " " << node->count << endl;
	}
	else{
		node = findNext(theId);
		if (node != NULL)
			cout << node->ID << " " << node->count << endl;
		else
			cout << "0 0" << endl;
	}
}

//Print the ID and the count of the event with the greatest key that is less that theID
void counter::Previous(int theId){
	treeNode *node;
	if(theId<=0){
		cout << "0 0" << endl;
		return;
	}
	if(Find(theId)==NULL){
		int tempid=theId;
		while(Find(tempid)==NULL){
			tempid--;
			if(tempid==0){
				cout << "0 0" << endl;
				return;
			}
		}
		node = Find(tempid);
		cout << node->ID << " " << node->count << endl;
	}
	else{
		node = findPrevious(theId);
		if (node != NULL)
			cout << node->ID << " " << node->count << endl;
		else
			cout << "0 0" << endl;
	}	
}


int main(int argc, char *argv[]){
	int sum,i;
	int id,count;
	string temp;
	counter *eventCounter = new counter();	//
	ifstream infile;  
	infile.open(argv[1]);	//load input data from the file
	if(!infile){
		cout << "Unable to open infile";
		return 0;
	}
	infile >> temp;		//first read the sum of nodes
	sum = atoi(temp.c_str());
	string a;
	for(i=0;i<sum;i++){		//read each node's ID and count
		infile >> a;
		id = atoi(a.c_str());
		infile >> a;
		count = atoi(a.c_str());
		eventCounter->Insert(id, count);	//insert every line in the file to the tree
	}
	int num = 2;
	if(argc>2){//if the input is from stdin
		while(strcmp(argv[num],"quit")!=0){		//continue reading and executing until there is a 'quit'
			if(strcmp(argv[num],"increase")==0){
			eventCounter->Increase(atoi(argv[num+1]),atoi(argv[num+2]));
			num = num +3;
			}
			else if(strcmp(argv[num],"reduce")==0){
				eventCounter->Reduce(atoi(argv[num+1]),atoi(argv[num+2]));
				num = num +3;
			}
			else if(strcmp(argv[num],"count")==0){
				eventCounter->Count(atoi(argv[num+1]));
				num = num + 1;
			}
			else if(strcmp(argv[num],"inrange")==0){
				eventCounter->InRange(atoi(argv[num+1]),atoi(argv[num+2]));
				num = num +3;
			}
			else if(strcmp(argv[num],"next")==0){
				eventCounter->Next(atoi(argv[num+1]));
				num = num + 2;
			}
			else if(strcmp(argv[num],"previous")==0){
				eventCounter->Previous(atoi(argv[num+1]));
				num = num + 2;
			}
			else{
				num = num + 1;
			}
			if(num>=argc)
				break;
		}
	}
	else{//if the input is from the redirection
		string opt,opt1,opt2;
		cin >> opt;		//read the input
		while(strcmp(opt.c_str(),"quit")!=0){		//continue reading and executing until there is a 'quit'
			if(strcmp(opt.c_str(),"increase")==0){
				cin >> opt1 >> opt2;
				eventCounter->Increase(atoi(opt1.c_str()),atoi(opt2.c_str()));
			}
			else if(strcmp(opt.c_str(),"reduce")==0){
				cin >> opt1 >> opt2;
				eventCounter->Reduce(atoi(opt1.c_str()),atoi(opt2.c_str()));
			}
			else if(strcmp(opt.c_str(),"count")==0){
				cin >> opt1;
				eventCounter->Count(atoi(opt1.c_str()));
			}
			else if(strcmp(opt.c_str(),"inrange")==0){
				cin >> opt1 >> opt2;
				eventCounter->InRange(atoi(opt1.c_str()),atoi(opt2.c_str()));
			}
			else if(strcmp(opt.c_str(),"next")==0){
				cin >> opt1;
				eventCounter->Next(atoi(opt1.c_str()));
			}
			else if(strcmp(opt.c_str(),"previous")==0){
				cin >> opt1;
				eventCounter->Previous(atoi(opt1.c_str()));
			}
			cin >> opt;
		}
	}
	eventCounter->RemoveTree(eventCounter->getRoot());	//delete the tree
	return 0;
}