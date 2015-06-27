#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Node.cpp"

using namespace std;

class Tree {
public:
    Node* root;
    string mode;

	Tree () {
		root = NULL;
		mode = "RBT";
	}

	// insertion methods

	int insertNode(Node* z) {
		Node* y = NULL;
		Node* x = root;

		while (x != NULL) {
			y = x;
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
		z->color = 1;

		insertFixup(z);

		return 1;
	}

	void insertFixup(Node* z) {
		while (z->parent != NULL && z->parent->color == 1) {
			if (z->parent == z->parent->parent->left) {
				Node* y = z->parent->parent->right;
				if (y != NULL && y->color == 1) {
					z->parent->color = 0;
					y->color = 0;
					z->parent->parent->color = 1;
					z = z->parent->parent;
				} else {
					if (z == z->parent->right) {
						z = z->parent;
						rotateLeft(z);
					}

					if (z->parent != NULL) {
						z->parent->color = 0;
						if (z->parent->parent != NULL) {
							z->parent->parent->color = 1;
							rotateRight(z->parent->parent);
						}
					}
				}
			} else {
				Node* y = z->parent->parent->left;
				if (y != NULL && y->color == 1) {
					z->parent->color = 0;
					y->color = 0;
					z->parent->parent->color = 1;
					z = z->parent->parent;
				} else {
					if (z == z->parent->left) {
						z = z->parent;
						rotateRight(z);
					}
					if (z->parent != NULL) {
						z->parent->color = 0;
						if (z->parent->parent != NULL) {
							z->parent->parent->color = 1;
							rotateLeft(z->parent->parent);
						}
					}
				}
			}
		}
		root->color = 0;
	}

	void rotateLeft(Node* x) {
		Node* y = x->right;
		x->right = y->left;

		if (y->left != NULL) {
			y->left->parent = x;
		}

		y->parent = x->parent;

		if (x->parent == NULL) {
			root = y;
		} else if (x == x->parent->left) {
			x->parent->left = (y);
		} else {
			x->parent->right = y;
		}

		y->left = (x);
		x->parent = y;
	}

	void rotateRight(Node* x) {
		Node* y = x->left;
		x->left = (y->right);

		if (y->right != NULL) {
			y->right->parent = x;
		}

		y->parent = x->parent;

		if (x->parent == NULL) {
			root = y;
		} else if (x == x->parent->left) {
			x->parent->left = (y);
		} else {
			x->parent->right = y;
		}

		y->right = x;
		x->parent = y;
	}

	// DSW balancing

	int tree_to_vine (Node* r) {
	   Node* vineTail = r;
	   Node* remainder = vineTail->right;

	   int size = 0;
	   Node* tempPtr;
	   while (remainder != NULL) {
		  // if no leftward subtree, move rightward
		  if (remainder->left == NULL) {
			 vineTail = remainder;
			 remainder = remainder->right;
			 size++;
		  }
		  // else eliminate the leftward subtree by rotations
		  else {
			  // rightward rotation
			 tempPtr = remainder->left;
			 remainder->left = tempPtr->right;
			 tempPtr->right = remainder;
			 remainder = tempPtr;
			 vineTail->right = tempPtr;
		  }
	   }

	   return size;
	}

	int fullSize(int size) {
		int n = 1;
		while (n <= size) {
			n = n + n + 1;
		}
		return n / 2;
	}

	void compression(Node* root, int count) {
		Node* scanner = root;
		// leftward rotation
		for (int i = 0; i < count; i++) {
			Node* child = scanner->right;
			scanner->right = child->right;
			scanner = scanner->right;
			child->right = scanner->left;
			scanner->left = child;
		}
	}

	void vine_to_tree(Node* root, int size) {
		int fullCount = fullSize(size);
		compression(root, size - fullCount);
		for (size = fullCount; size > 1; size /= 2) {
			compression(root, size / 2);
		}
	}

	void balanceDSW () {
		Node* pseudo_root = new Node(-1);
		pseudo_root->right = root;

		int size = tree_to_vine(pseudo_root);
		vine_to_tree(pseudo_root, size);

		root = pseudo_root->right;
	}

	// balance

	void convertToOST () {
		updateSizes(root);
		mode = "OST";
	}

	void convertRBT () {
		updateColors(root);
		mode = "RBT";
	}

	inline int size (Node* x) {
		if (x == NULL) return 0;
		return x->size;
	}

	Node* rotR (Node* h) {
          Node* x = h->left;
		h->left = x->right;
		x->right = h;

		h->size = size(h->left) + size(h->right) + 1;
		x->size = size(x->left) + size(x->right) + 1;

		return x;
	}

	Node* rotL (Node* h) {
          Node* x = h->right;
          h->right = x->left;
          x->left = h;

          h->size = size(h->left) + size(h->right) + 1;
		x->size = size(x->left) + size(x->right) + 1;

          return x;
	}

	Node* partR (Node* h, int k) {

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


	Node* balanceR (Node* h) {
	   if (h == NULL || h->size < 2) return h;

	   h = partR (h, h->size / 2);

	   h->left  = balanceR (h->left);
	   h->right = balanceR (h->right);

	   return h;
	}

	void balance () {
		clock_t start, finish;
		start = clock();
		convertToOST();
		finish = clock();
		//cout << "Converting to ost time: " << ((double) (finish - start)) / 1000 << " ms" << endl;

		start = clock();
		root = balanceR(root);
		finish = clock();
		//cout << "BalanceR time: " << ((double) (finish - start)) / 1000 << " ms" << endl;
	}


	// destructor

	virtual ~Tree() {};

	// public methods to show performance

	int insert (int k) {
		return insertNode(new Node(k));
	}

	void draw () {
		drawTree(root);
	}

	void printInOrder () {
		printInOrder(root);
	}

	void printHeight () {
		cout << "Tree height: " << getHeight(root) << endl;
	}

	void printSize () {
		cout << "Tree size: " << count(root) << endl;
	}

	void destroy () {
		destroyTree(root);
	}

	// private methods

	void drawTree (Node* x) {
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

	void destroyTree(Node* node) {
		if (node != NULL) {
			destroyTree(node->left);
			destroyTree(node->right);
			delete node;
		}
	}

	inline int Max( int l, int r) {
		return l > r ? l : r;
	}

	void printInOrder (Node* node) {
		if (node != NULL) {
			printInOrder(node->left);
			cout << node->key << endl;
			printInOrder(node->right);
		}
	}

	void printRoot () {
		if (root != NULL) {
			cout << "Root is: " << root->key << endl;
		}
	}

	int updateSizes (Node* h) {
	   if (h == NULL) return 0;

	   int leftSize = updateSizes(h->left);
	   int rightSize = updateSizes(h->right);

	   h->size = 1 + leftSize + rightSize;

	   return h->size;
	}

	int getHeight (Node* node) {
		if (node == NULL) {
			return 0;
		} else {
			return 1 + Max (getHeight(node->left), getHeight(node->right));
		}
	}

	int count (Node* node) {
		if (node == NULL) {
			return 0;
		} else {
			return 1 + count(node->left) + count(node->right);
		}
	}

	void updateColors (Node* h) {
		if (h == NULL) return;

		h->color = 0;

		updateColors(h->right);
		updateColors(h->left);
	}

	// correct tree after balanceR
	void updateParents (Node* node, Node* parent) {
		 if (node != NULL) {
			 updateParents (node->left, node);
			 updateParents (node->right, node);
             node->parent = parent;
	     }
	}

	void tmp (Node* node) {
		if (node == root) {
			node->color = 0;
			node->blackQuota = round(getHeight(node) / 2);
		} else if (node->parent->color == 1) {
			node->color = 0;
			node->blackQuota = node->parent->blackQuota;
		} else {
			// node->parent is black
		}

//		if n is root,
//		    n.color = black
//		    n.black-quota = height n / 2, rounded up.
//
//		else if n.parent is red,
//		    n.color = black
//		    n.black-quota = n.parent.black-quota.
//
//		else (n.parent is black)
//		    if n.min-height < n.parent.black-quota, then
//		        error "shortest path was too short"
//		    else if n.min-height = n.parent.black-quota then
//		        n.color = black
//		    else (n.min-height > n.parent.black-quota)
//		        n.color = red
//		    either way,
//		        n.black-quota = n.parent.black-quota - 1
	}
};
