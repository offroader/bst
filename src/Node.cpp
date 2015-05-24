#include <cstdlib>

using namespace std;

class Node {
public:
	int key;
	int color;
	Node* parent;
	Node* left;
	Node* right;

	Node (int key) {
		this->key = key;
		parent = NULL;
		left = NULL;
		right = NULL;
		color = 0;
	}

	virtual ~Node() {};
};



