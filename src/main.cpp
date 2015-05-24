#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#include "Tree.cpp"

using namespace std;

void orderedInsert();
void randomInsert();
void randomInsert(Tree*, int);
void printInOrder(Node*);
void destroyTree(Node*);
Node* treeMinimum(Node*);
int tree_to_vine(Node*);
int fullSize(int);
void vine_to_tree(Node*, int);
void compression(Node*, int);

int main(int argc, char** argv) {
	Tree* tree = new Tree();
	int N = 0;

	for (int i = 7; i > 0; i--) {
		Node* node = new Node(i);
		tree->insertNode(node);
		N++;
	}

//	randomInsert(tree, N);

//	tree->insertNode(new Node(3));
//	tree->insertNode(new Node(5));
//	tree->insertNode(new Node(7));
//	tree->insertNode(new Node(6));
//	tree->insertNode(new Node(8));
//	tree->insertNode(new Node(1));
//	tree->insertNode(new Node(4));

	cout<< "tree root: " << tree->root->key <<endl;
	printInOrder(tree->root);

	cout<< "tree to vine"<<endl;
	int size=0;
	size = tree_to_vine(tree->root);
	cout<<"size:"<<size<<endl;

	cout<< "tree root: " << tree->root->key <<endl;
	printInOrder(tree->root);

	cout<< "vine to tree"<<endl;
	vine_to_tree(tree->root, size);

	cout<<"fullsize(size) = "<<fullSize(size)<< endl;
	cout<<"fullsize("<<N<<") = "<<fullSize(N)<< endl;

	cout<< "tree root: " << tree->root->key <<endl;
	printInOrder(tree->root);


	cout<< "destroying tree";
	destroyTree(tree->root);
	cout<< " ended.";

	return 0;
}

void orderedInsert() {
	Tree* tree = new Tree();
	int N = 10;
	cout << "ordered list (size: " << N << ") using insert method:" << endl;
	clock_t start, finish;
	start = clock();

	for (int i = 0; i < N; i++) {
		Node* node = new Node(i+1);
		tree->insertNode(node);
	}

	finish = clock();
	cout << "Time: " << ((double) (finish - start)) / 1000 << " ms" << endl;


}


void randomInsert(Tree* tree, int n) {
	for (int i = 0; i < n; i++) {
		Node* node = new Node(std::rand() % 100);
		int inserted = tree->insertNode(node);
		cout<< (inserted == 1 ? "inserted " : "skipped ")<<node->key<< endl;
		if (inserted == 0) {
			n++;
			delete node;
		}
	}
}

void randomInsert() {
	Tree* tree = new Tree();

	int N = 1000;

	cout << "unordered list (size: " << N << ") using insert method:" << endl;
	clock_t start, finish;
	start = clock();

	int size = 10000;
	int arr[size];
	int i = 0;
	string line;

	ifstream myfile("data.in");
	if (myfile.is_open()) {
		while (getline(myfile, line) && i < size - 1) {
			arr[i] = atoi(line.c_str());
			i++;
		}
		myfile.close();
	}

	for (int i = 0; i < (N / size); i++) {
		for (int j = 0; j < size; j++) {
			int r = std::rand() % 10 + 1;
			int number = arr[j] + r;
			Node* node = new Node(number);
			tree->insertNode(node);
		}
	}

	finish = clock();
	cout << "Time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	destroyTree(tree->root);
}

void printInOrder(Node* node) {
	if (node != NULL) {
		if (node->left != NULL) {
			//cout << " key: " << node->key << " left: " << node->left->key << endl;
			cout << node->left->key << " <--- " << node->key << endl;
			printInOrder(node->left);
		}
//		int p = -1;
//		if (node->parent != NULL) {
//			p = node->parent->key;
//		}
		//cout << " key: " << node->key << " parent:" << p << endl;

		if (node->right != NULL) {
			//cout << " key: " << node->key << " right: " << node->right->key << endl;
			cout << node->key << " ---> " << node->right->key << endl;
			printInOrder(node->right);
		}
	}
	cout << endl;
}

//void printInOrder(Node* node) {
//	if (node != NULL) {
//		if (node->left != NULL) {
//			//cout << " key: " << node->key << " left: " << node->left->key << endl;
//			cout << node->left->key << " <--- " << node->key << endl;
//			printInOrder(node->left);
//		}
////		int p = -1;
////		if (node->parent != NULL) {
////			p = node->parent->key;
////		}
//		//cout << " key: " << node->key << " parent:" << p << endl;
//
//		if (node->right != NULL) {
//			//cout << " key: " << node->key << " right: " << node->right->key << endl;
//			cout << node->key << " ---> " << node->right->key << endl;
//			printInOrder(node->right);
//		}
//	}
//	cout << endl;
//}

void destroyTree(Node* node) {
	if (node != NULL) {
		if (node->left != NULL) {
			destroyTree(node->left);
		}
		if (node->right != NULL) {
			destroyTree(node->right);
		}
//		cout << "deleted node:" << node->key << endl;
		delete node;
	}
}

Node* treeMinimum (Node* node) {
	if (node->left != NULL) {
		return treeMinimum(node->left);
	}
	return node;
}

int tree_to_vine(Node* root) {
    Node* vineTail = new Node(-1);
    vineTail->right = root;
    Node* remainder = vineTail->right;

    root = vineTail;

    Node* tempPtr;
    int size = 0;
    while (remainder != NULL) {
        if (remainder->left == NULL) {
            vineTail = remainder;
            remainder = remainder->right;
            size++;
        } else {
            // Rightward rotation
            tempPtr = remainder->left;
            remainder->left = tempPtr->right;
            tempPtr->right = remainder;
            remainder = tempPtr;
            vineTail->right = tempPtr;
        }
    }

    root = root->right;

    return size;
}

int fullSize(int size) {
	int n = 1;
	while (n <= size) {
		n = n + n + 1;
	}
	return n / 2;
}

void compression(Node* root, int count) {
	Node* scanner = root;
	//Leftward rotation
	for (int i = 0; i < count; i++) {
		Node* child = scanner->right;
		scanner->right = child->right;
		scanner = scanner->right;
		child->right = scanner->left;
		scanner->left = child;
	}
}

void vine_to_tree(Node* root, int size) {
	int fullCount = fullSize(size);
	compression(root, size - fullCount);
	for (size = fullCount; size > 1; size /= 2) {
		compression(root, size / 2);
	}
}
