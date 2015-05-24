#include <cstdlib>

using namespace std;

class OSTNode {
public:
	int key;
	int size;
	OSTNode* parent;
	OSTNode* left;
	OSTNode* right;

	OSTNode (int key) {
		this->key = key;
		parent = NULL;
		left = NULL;
		right = NULL;
		size = 0;
	}

	virtual ~OSTNode() {};
};



