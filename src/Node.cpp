#include <cstdlib>

using namespace std;

class Node {
public:
	int key;
	Node* parent;
	Node* left;
	Node* right;
	int color;
	int size;
	int blackQuota;

	Node (int key) {
		this->key = key;
		parent = NULL;
		left = NULL;
		right = NULL;
		color = 0;
		size = 0;
		blackQuota = 0;
	}

	virtual ~Node() {};
};



