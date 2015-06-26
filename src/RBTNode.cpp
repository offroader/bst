#include <cstdlib>

using namespace std;

class RBTNode {
public:
	int key;
	int color;
	RBTNode* parent;
	RBTNode* left;
	RBTNode* right;
	int size;

	RBTNode (int key) {
		this->key = key;
		parent = NULL;
		left = NULL;
		right = NULL;
		color = 0;
		size = 0;
	}

	virtual ~RBTNode() {};
};



