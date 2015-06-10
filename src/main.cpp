#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#include "Tree.cpp"
#include "OST.cpp"

using namespace std;


void osttest();
void rbtest(int);
void orderedInsert();
void randomInsert();
void randomInsert(Tree*, int);

int main(int argc, char** argv) {
	srand (time(NULL));

	rbtest(1000000);
	rbtest(100000);
	rbtest(10000);

	return 0;
}


void osttest () {
	OST* tree = new OST();

	tree->insertNode(new OSTNode(3));
	tree->insertNode(new OSTNode(5));
	tree->insertNode(new OSTNode(7));
	tree->insertNode(new OSTNode(6));
	tree->insertNode(new OSTNode(8));
	tree->insertNode(new OSTNode(1));
	tree->insertNode(new OSTNode(4));

	cout << "tree root: " << tree->root->key <<endl;
	tree->draw();

	tree->balance();

	cout << endl;
	cout << "balanceRed:" << endl;
	cout << "tree root: " << tree->root->key <<endl;
	tree->draw();

	cout<< "destroying tree";
	tree->destroy();
	cout<< " ended.";
}
void rbtest (int tree_size) {
	Tree* tree = new Tree();
	clock_t start, finish;

	cout<< endl<< "Tree size: " << tree_size << endl;
    tree->printSize();

	start = clock();
	randomInsert(tree, tree_size);
	finish = clock();
	cout << "Build time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	//tree->printInOrder();

	tree->printSize();
	tree->printHeight();
	tree->printRoot();

	start = clock();
	tree->balance();
	finish = clock();

	tree->printSize();
	tree->printHeight();
	tree->printRoot();

	cout << "Time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	tree->destroy();

	cout << "Tree is destroyed" << endl << endl;

}

void rbtest2 () {
	Tree* tree = new Tree();

	tree->insertNode(new Node(3));
	tree->insertNode(new Node(5));
	tree->insertNode(new Node(7));
	tree->insertNode(new Node(6));
	tree->insertNode(new Node(8));
	tree->insertNode(new Node(1));
	tree->insertNode(new Node(4));

	cout << "tree root: " << tree->root->key <<endl;
	tree->draw();

	tree->balance();

	cout << endl;
	cout << "balanced:" << endl;
	cout << "tree root: " << tree->root->key <<endl;
	tree->draw();

	cout<< "destroying tree";
	tree->destroy();
	cout<< " ended.";
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
		int k;
		if (std::rand() % 10 > 5) {
			k = std::rand() % 100000000;
		} else {
			k = std::rand() % 1000;
		}
		Node* node = new Node(k);
		int inserted = tree->insertNode(node);
		//cout<< i << (inserted == 1 ? "inserted " : "skipped ")<<node->key<< endl;
		if (inserted == 0) {
			delete node;
			i--;
			continue;
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

	tree->destroy();
}
