#include <cstdlib>

using namespace std;

class Node {
public:
	int key;
	Node* parent;
	Node* left;
	Node* right;
	int meta;

	Node (int key) {
		this->key = key;
		parent = NULL;
		left = NULL;
		right = NULL;
		meta = 0;
	}

	virtual ~Node() {};
};



