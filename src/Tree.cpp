#include <iostream>
#include <cstdlib>
#include "Node.cpp"

using namespace std;

class Tree {
public:
    Node* root;


	Tree () {
		root = NULL;
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
		z->left = NULL;
		z->right = NULL;
		z->color = 1;
		//insertFixup(z);

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

	virtual ~Tree() {};


//    void compression(Node* root, int count) {
//        Node* scanner = root;
//        //Leftward rotation
//        for (int i = 0; i < count; i++) {
//            Node* child = scanner->right;
//            scanner->right = child->right;
//            scanner = scanner->right;
//            child->right = scanner->left;
//            scanner->left = child;
//        }
//    }
//
//    int fullSize(int size) {
//        int n = 1;
//        while (n <= size) {
//            n = n + n + 1;
//        }
//        return n / 2;
//    }
//
//    void vine_to_tree(Node* root, int size) {
//        int fullCount = fullSize(size);
//        compression(root, size - fullCount);
//        for (size = fullCount; size > 1; size /= 2) {
//            compression(root, size / 2);
//        }
//    }

};

