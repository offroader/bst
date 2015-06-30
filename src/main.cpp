#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <map>
#include <cmath>

#include "Tree.cpp"
#include "OST.cpp"
#include "RBT.cpp"

using namespace std;

void test_ostDSW_ostSED_ostMod(int);
void test_build_rbt_ost(int);
void run_demo(int);
void fill(Tree*);

int main (int argc, char** argv) {
//	for (int n = 1000; n <= 100000000; n *= 10) {
//		test_build_rbt_ost(n);
//		test_ostDSW_ostSED_ostMod(n);
//		run_demo(n);
//	}

	Tree* tree = new Tree();

fill(tree);

	tree->draw();

	tree->destroy();
}

void test_ostDSW_ostSED_ostMod (int N) {
	if (!N) {
		cout << "invalid tree size" << endl;
		return;
	}

	cout << "Test OST balancing using DSW and Sedgewick's algorithm (also our modification) for " << N << " elements" << endl;

	srand (time(NULL));

	cout << endl<< "N:" << N << endl;

	map<int, int> mymap;

	OST* ost1 = new OST();
	OST* ost2 = new OST();
	OST* ost3 = new OST();

	for (int i = 0; i < N; i++) {
		int k;
		if (rand() % 10 > 5) {
			k = rand() % 10000000000;
		} else {
			k = rand() % 1000;
		}

		if (mymap.find(k) == mymap.end()) {
			mymap.insert(pair<int,int>(k, 1));
			ost1->insert(k);
			ost2->insert(k);
			ost3->insert(k);
		} else {
			i--;
			continue;
		}
	}

	clock_t start, finish;

	cout << "Initial: " << endl;
	ost1->printHeight();
	ost2->printHeight();
	ost3->printHeight();

	cout << endl;

	start = clock();
	ost1->balanceDSW();
	finish = clock();
	cout << "DSW balancing time: " << ((double) (finish - start)) / 1000 << " ms" << endl;
	ost1->printHeight();

	start = clock();
	ost2->balance();
	finish = clock();
	cout << "Sed. balancing time: " << ((double) (finish - start)) / 1000 << " ms" << endl;
	ost2->printHeight();

	start = clock();
	ost3->balanceM();
	finish = clock();
	cout << "Modified Sed. balancing time: " << ((double) (finish - start)) / 1000 << " ms" << endl;
	ost3->printHeight();

	ost1->destroy();
	ost2->destroy();
	ost3->destroy();
}

void test_build_rbt_ost (int tree_size) {
	if (!tree_size) {
		cout << "invalid tree size" << endl;
		return;
	}

	cout << "Test RBT and OST building times for " << tree_size << " elements" << endl;

	map<int, int> mymap;
	//int arr[tree_size];
	int* arr = new int[tree_size];

	for (int i = 0; i < tree_size; i++) {
		int k;
		if (rand() % 10 > 5) {
			k = rand() % 10000000000;
		} else {
			k = rand() % 1000;
		}

		if (mymap.find(k) == mymap.end()) {
			mymap.insert(pair<int,int>(k, 1));
			arr[i] = k;
		} else {
			i--;
			continue;
		}
	}

	//for (map<int, int>::iterator it = mymap.begin(); it != mymap.end(); ++it) {}

	// check
	cout << "map size: " << mymap.size() << endl;

	RBT* rbt = new RBT();
	OST* ost = new OST();
	clock_t start, finish;

	start = clock();
	for (int i = 0; i < tree_size; i++) {
		rbt->insert(arr[i]);
	}
	finish = clock();
	cout << "RBT building time: " << ((double) (finish - start)) / 1000 << " ms" << endl;


	start = clock();
	for (int i = 0; i < tree_size; i++) {
		ost->insert(arr[i]);
	}
	finish = clock();
	cout << "OST building time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	rbt->printSize();
	rbt->printRoot();
	rbt->printHeight();

	ost->printSize();
	ost->printRoot();
	ost->printHeight();

	rbt->destroy();
	ost->destroy();

	free(arr);

	cout << "Test finished." << endl;
}

void run_demo (int tree_size) {
	if (!tree_size) {
		cout << "invalid tree size" << endl;
		return;
	}
	cout << "Test our tree balancing time for " << tree_size << " elements" << endl;

	map<int, int> mymap;

	Tree* t1 = new Tree();
	Tree* t2 = new Tree();
	clock_t start, finish;


	for (int i = 0; i < tree_size; i++) {
		int k;
		if (rand() % 10 > 5) {
			k = rand() % 10000000000;
		} else {
			k = rand() % 1000;
		}

		if (mymap.find(k) == mymap.end()) {
			mymap.insert(pair<int,int>(k, 1));
			t1->insert(k);
			t2->insert(k);
		} else {
			i--;
			continue;
		}
	}

	start = clock();
	t1->balanceDSW();
	finish = clock();
	cout << "DSW balancing time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	start = clock();
	t2->balance();
	finish = clock();
	cout << "Sedge balancing time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

	t1->printHeight();
	t2->printHeight();

	t1->destroy();
	t2->destroy();

	cout << "Test finished." << endl;
}


void fill (Tree* tree) {
	tree->insert(710);
	tree->insert(231);
	tree->insert(462);
	tree->insert(317);
	tree->insert(464);
	tree->insert(305);
	tree->insert(872);
	tree->insert(511);
	tree->insert(827);
	tree->insert(588);
	tree->insert(761);
	tree->insert(277);
	tree->insert(559);
	tree->insert(416);
	tree->insert(536);
	tree->insert(384);
	tree->insert(45);
	tree->insert(461);
	tree->insert(718);
	tree->insert(290);
	tree->insert(666);
	tree->insert(269);
	tree->insert(583);
	tree->insert(413);
	tree->insert(755);
	tree->insert(129);
	tree->insert(406);
	tree->insert(575);
	tree->insert(396);
	tree->insert(813);
	tree->insert(262);
	tree->insert(410);
	tree->insert(238);
	tree->insert(358);
	tree->insert(151);
	tree->insert(885);
	tree->insert(664);
	tree->insert(690);
	tree->insert(204);
	tree->insert(583);
	tree->insert(254);
	tree->insert(284);
	tree->insert(307);
	tree->insert(341);
	tree->insert(495);
	tree->insert(475);
	tree->insert(611);
	tree->insert(330);
	tree->insert(982);
	tree->insert(938);
	tree->insert(361);
	tree->insert(919);
	tree->insert(121);
	tree->insert(689);
	tree->insert(412);
	tree->insert(86);
	tree->insert(433);
	tree->insert(947);
	tree->insert(653);
	tree->insert(242);
	tree->insert(629);
	tree->insert(858);
	tree->insert(950);
	tree->insert(801);
	tree->insert(640);
	tree->insert(363);
	tree->insert(568);
	tree->insert(252);
	tree->insert(474);
	tree->insert(144);
	tree->insert(537);
	tree->insert(241);
	tree->insert(293);
	tree->insert(492);
	tree->insert(4);
	tree->insert(20);
	tree->insert(860);
	tree->insert(731);
	tree->insert(703);
	tree->insert(197);
	tree->insert(859);
	tree->insert(534);
	tree->insert(487);
	tree->insert(589);
	tree->insert(46);
	tree->insert(146);
	tree->insert(306);
	tree->insert(102);
	tree->insert(351);
	tree->insert(789);
	tree->insert(328);
	tree->insert(32);
	tree->insert(158);
	tree->insert(896);
	tree->insert(833);
	tree->insert(299);
	tree->insert(407);
	tree->insert(498);
	tree->insert(227);
	tree->insert(441);
}
