#include <cstdlib>

using namespace std;

class OSTNode {
public:
	int key;
	int size;
	OSTNode* parent;
	OSTNode* left;
	OSTNode* right;
	int util;

	OSTNode (int key) {
		this->key = key;
		parent = NULL;
		left = NULL;
		right = NULL;
		size = 0;
		util = 0;
	}

	virtual ~OSTNode() {};
};



