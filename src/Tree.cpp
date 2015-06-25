#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Node.cpp"

using namespace std;

class Tree {
public:
    Node* root;
    Node* maxNode;


	Tree () {
		root = NULL;
		maxNode = NULL;
	}

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

	void insertMax(Node* z) {
		Node* y = NULL;
		Node* x = maxNode;
		if (x != NULL) {
			if (z->key >= x->key) {
				maxNode = z;
			} else {
				x = root;
			}
		} else {
			maxNode = z;
		}

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
		} else {
			if (z->key < y->key) {
				y->left = z;
			} else if (z->key > y->key) {
				y->right = z;
			} else {
				cout << "The key: " << z->key << " has already been added!" << endl;
				return;
			}
		}
		z->color = 1;
		insertFixup(z);
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

	void balance () {
		Node* pseudo_root = new Node(-1);
		pseudo_root->right = root;

		int size = tree_to_vine(pseudo_root);
		vine_to_tree(pseudo_root, size);

		root = pseudo_root->right;
	}

	virtual ~Tree() {};

	void drawTree (Node* x) {
		static int node_level = -1;
		if (x != NULL) {
			node_level += 1;
			drawTree(x->right);
			for (int i = 0; i < 7 * node_level; i++) {
				printf(" ");
			}
			printf("%d \n" , x->key);
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

	void printHeight () {
		cout << "Tree height: " << calculateHeight(root) << endl;
	}

	void printInOrder (Node* node) {
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
			cout << "RBT root is: " << root->key << endl;
		}
	}

	int count (Node* node) {
		if (node == NULL) {
			return 0;
		} else {
			return 1 + count(node->left) + count(node->right);
		}
	}

	void printSize () {
		cout << "Tree size: " << count(root) << endl;
	}

	int updateSizes (Node* h) {
	   if ( h == NULL ) return 0;

	   int lN = updateSizes(h->left);
	   int rN = updateSizes(h->right);
	   h->color = 1 + lN + rN;

	   return h->color;
	}

	void convertToOST () {
		updateSizes(root);
	}

	int calculateHeight (Node* node) {
		if (node == NULL) {
			return 0;
		} else {
			return 1 + Max (count(node->left), count(node->right));
		}
	}

};

