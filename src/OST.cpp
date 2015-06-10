#include <iostream>
#include <cstdlib>
#include <fstream>
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

	OSTNode* balanceR (OSTNode* h) {
	   if (!h || h->size < 2) return h;
	   h = partR (h, h->size/2);
	   h->left  = balanceR (h->left);
	   h->right = balanceR (h->right);
	   return h;
	}

	OSTNode* rotR (OSTNode* h) {
		int rNr = h->right ? h->right->size : 0,
	       rNl = h->left->right ? h->left->right->size : 0,
	       lN  = h->left->left  ? h->left->left->size  : 0;
	   OSTNode* x = h->left;  h->left = x->right; x->right = h;
	   h->size = rNr + rNl;  x->size = lN + h->size;
	   return x;
	}

	OSTNode* rotL (OSTNode* h) {
		int lNl = h->left ? h->left->size : 0,
	       lNr = h->right->left ? h->right->left->size : 0,
	       rN  = h->right->right ? h->right->right->size : 0;
	   OSTNode* x = h->right; h->right = x->left; x->left = h;
	   h->size = lNl + lNr; x->size = rN + h->size;
	   return x;
	}

	OSTNode* partR (OSTNode* h, int k) {
	   int t = h->left ? h->left->size : 0;
	   if (t > k) {
		   h->left = partR(h->left, k);
		   h = rotR(h);
	   }
	   if (t < k) {
		   h->right = partR(h->right, k-t-1);
		   h = rotL(h);
	   }

	   return h;
	}

	int updateSizes (OSTNode* h) {
	   if ( h == NULL ) return 0;

	   int lN = updateSizes(h->left);
	   int rN = updateSizes(h->right);
	   h->size = 1 + lN + rN;

	   return h->size;
	}

	void drawTree (OSTNode* x) {
		static int node_level = -1;
	 if (x != NULL) {
	    node_level += 1;
	    drawTree(x->right);
	 	for (int i = 0; i < 7 * node_level; i++) {
	 		printf(" ");
	 	}
	    printf("%d (%d)\n" , x->key, x->size);
	    drawTree(x->left);
	    node_level -= 1;
	   }
	}

	void draw () {
		drawTree(root);
	}

	void balance () {
		updateSizes(root);
		root = balanceR(root);
	}

	void destroy () {
		destroyTree(root);
	}

	void destroyTree(OSTNode* node) {
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

	virtual ~OST() {};

	inline int Max (int l, int r) {
		return l > r ? l : r;
	}

	int calculateHeight (OSTNode* node) {
		if (node != NULL) {
			return 1 + Max (calculateHeight(node->left), calculateHeight(node->right));
		}
		return 0;
	}

	void printHeight () {
		cout << "Tree height: " << calculateHeight(root) << endl;
	}

	void printInOrder (OSTNode* node) {
		if (node != NULL) {
			printInOrder(node->left);
			cout << node->key << endl;
			printInOrder(node->right);
		}
	}


	void printInOrder () {
		printInOrder(root);
	}

	void printRoot () {
		cout << "root is: " << root->key << endl;
	}

	int count (OSTNode* node) {
		if (node == NULL) {
			return 0;
		} else {
			return 1 + count(node->left) + count(node->right);
		}
	}

	void printSize () {
		cout << "Tree size: " << count(root) << endl;
	}
};

