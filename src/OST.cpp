#include <iostream>
#include <cstdlib>
#include "OSTNode.cpp"

using namespace std;

class OST {
public:
    OSTNode* root;


	OST () {
		root = NULL;
	}

	int insertNode(OSTNode* z) {
		OSTNode* y = NULL;
		OSTNode* x = root;
		while (x != NULL) {
			y = x;
			x->size++;
			if (z->key < x->key) {
				x = x->left;
			} else {
				x = x->right;
			}
		}
		z->parent = y;
		if (y == NULL) {
			root = z;
		} else if (z->key < y->key) {
			y->left = z;
		} else if (z->key > y->key) {
			y->right = z;
		} else {
			return 0;
		}
		z->left = NULL;
		z->right = NULL;
		z->size = 1;

		return 1;
	}

	OSTNode* select (OSTNode* x, int i) {
		int r = x->left->size + 1;

		if (i == r) {
			return x;
		} else if (i < r) {
			return select(x->left, i);
		} else {
			return select(x->right, i - r);
		}
	}

	int rank (OSTNode* x) {
		int r = x->left->size + 1;

		OSTNode* y = x;
		while (y != root) {
			if (y == y->parent->right) {
				r = r + y->parent->left->size + 1;
			}
			y = y->parent;
		}

		return r;
	}

	virtual ~OST() {};
};
