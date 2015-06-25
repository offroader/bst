#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
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
			if (z->key == x->key) {
				return 0;
			}
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

	int insert (int key) {
		return insertNode(new OSTNode(key));
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

	inline int size (OSTNode* x) {
		if (x == NULL) return 0;
		return x->size;
	}

	OSTNode* balanceR (OSTNode* h) {
	   if (h == NULL || h->size < 2) return h;

	   h = partR (h, h->size/2);

	   h->left  = balanceR (h->left);
	   h->right = balanceR (h->right);

	   return h;
	}

	OSTNode* balanceM (OSTNode* x) {
	   if (x == NULL || x->size < 2) return x;

	   int h = (int)log2(x->size);

	   if (x->size <= 3 * (int)pow(2, h-1) - 1 ) {

		   x = partR(x, x->size - (int)pow(2, h-1) + 1 - 1);

		   x-> left = balanceR(x->left);
		   x->right = balanceM(x->right);
	   } else {

		   x = partR(x, (int)pow(2, h) - 1);

		   x-> left = balanceM(x->left);
		   x->right = balanceR(x->right);
	   }

	   return x;
	}

	OSTNode* rotR (OSTNode* h) {
	    OSTNode* x = h->left;
		h->left = x->right;
		x->right = h;

		h->size = size(h->left) + size(h->right) + 1;
		x->size = size(x->left) + size(x->right) + 1;

		return x;
	}

	OSTNode* rotL (OSTNode* h) {
	    OSTNode* x = h->right;
	    h->right = x->left;
	    x->left = h;

	    h->size = size(h->left) + size(h->right) + 1;
		x->size = size(x->left) + size(x->right) + 1;

	    return x;
	}

	OSTNode* partR (OSTNode* h, int k) {

	   int t = size(h->left);

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

	void balanceM () {
		root = balanceM(root);
	}

	void balance () {
		root = balanceR(root);
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

			delete node;
		}
	}

	virtual ~OST() {};

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
		if (root != NULL) {
			cout << "OST root is: " << root->key << endl;
		}
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

	int tree_to_vine (OSTNode* r) {
	   OSTNode* vineTail = r;
	   OSTNode* remainder = vineTail->right;

	   int size = 0;
	   OSTNode* tempPtr;
	   while (remainder != NULL) {
		   // if no leftward subtree, move rightward
		  if (remainder->left == NULL) {
			 vineTail = remainder;
			 remainder = remainder->right;
			 size++;
		  }
		  // else eliminate the leftward subtree by rotations
		  else {
			  /* rightward rotation */
			 tempPtr = remainder->left;
			 remainder->left = tempPtr->right;
			 tempPtr->right = remainder;
			 remainder = tempPtr;
			 vineTail->right = tempPtr;
		  }
	   }

	   return size;
	}

	inline int Max (int l, int r) {
		return l > r ? l : r;
	}

	int fullSize(int size) {
		int n = 1;
		while (n <= size) {
			n = n + n + 1;
		}
		return n / 2;
	}

	void compression(OSTNode* root, int count) {
		OSTNode* scanner = root;
		// leftward rotation
		for (int i = 0; i < count; i++) {
			OSTNode* child = scanner->right;
			scanner->right = child->right;
			scanner = scanner->right;
			child->right = scanner->left;
			scanner->left = child;
		}
	}

	void vine_to_tree(OSTNode* root, int size) {
		int fullCount = fullSize(size);
		compression(root, size - fullCount);
		for (size = fullCount; size > 1; size /= 2) {
			compression(root, size / 2);
		}
	}

	void balanceDSW () {
		OSTNode* pseudo_root = new OSTNode(-1);
		pseudo_root->right = root;

		int size = tree_to_vine(pseudo_root);

//		drawTree(pseudo_root->right);
//		cout << endl << endl;

		vine_to_tree(pseudo_root, size);

//		drawTree(pseudo_root->right);

		root = pseudo_root->right;
	}
};
