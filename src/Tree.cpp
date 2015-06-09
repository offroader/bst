#include <iostream>
#include <cstdlib>
#include <fstream>
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

	int tree_to_vine(Node* root) {
	    Node* vineTail = new Node(-1);
	    vineTail->right = root;
	    Node* remainder = vineTail->right;

	    root = vineTail;

	    Node* tempPtr;
	    int size = 0;
	    while (remainder != NULL) {
	        if (remainder->left == NULL) {
	            vineTail = remainder;
	            remainder = remainder->right;
	            size++;
	        } else {
	            // Rightward rotation
	            tempPtr = remainder->left;
	            remainder->left = tempPtr->right;
	            tempPtr->right = remainder;
	            remainder = tempPtr;
	            vineTail->right = tempPtr;
	        }
	    }

	    root = root->right;

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
		//Leftward rotation
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
		int size = tree_to_vine(root);
		vine_to_tree(root, size);
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

		void destroy () {
			destroyTree(root);
		}

		void destroyTree(Node* node) {
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

};

