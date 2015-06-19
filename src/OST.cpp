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
		vine_to_tree(pseudo_root, size);

		root = pseudo_root->right;
	}


	// Display the BST horizontally --- based on a level-order traversal
	void pretty(ostream &out)
	{
	   int skip = 0;

	   if ( root == NULL )  // Nothing to display!
	   {  cout << "Empty tree!\n"; return;  }

	   setPos (root, skip); // Find line position for each node
	   pretty (root, out);  // Level-order traversal displaying the nodes
	}                       // one line for each level, in proper position


	// Find line position for each node --- based on in-order traversal
	void setPos (OSTNode* node, int &skip)
	{
	// If the nodes were all printed on one line, their order is based
	// in an in-order traversal.  Skip shows number of positions to skip
	// to properly position the node.  Note that is MUST be a reference
	// parameter --- the root depends on the reference parameter to come
	// back with the size of the entire left subtree, for instance.
	   if ( node )
	   {
	      setPos (node->left, skip);
	      node->util = skip;     // Store skip value in Util data member
	      skip = skip + 2;       // Update for printing THIS node
	      setPos(node->right, skip);
	   }
	}


	// Pretty-print:  each tree level prints on one line, with the node
	// horizontally positioned to be visually the parent of its children.
	void pretty (OSTNode* Node, ostream &Out)
	{//Level-order traversal requires a queue --- build a quick one here.
	   struct Queue
	   {
	      OSTNode* Node;
	      int Level;    // Change in level is where the endl prints
	      Queue* Next;

	      //Queue(OSTNode* N, int Lvl, Queue *Nxt = NULL) : Node(N), Level(Lvl), Next(Nxt) { /* nothing else */ }

	      Queue (OSTNode* n, int lvl, Queue *Nxt = NULL) {
	    	  Node = n;
	    	  Level = lvl;
	      }

	   };

	   Queue *Front = NULL, *Back;

	   int Level = 0, Position = 0;

	// Level-order traversal:  initialize the work queue with the root

	   Front = Back = new Queue(Node, 0);  // Node initially IS the root

	   while ( Front ) // Keep going till there's nothing left to do!
	   {
	      Queue *Add,                 // Queue pointer when we add a job
	            *Current = Front;     // Begin the dequeue operation

	      Front = Front->Next;        // Complete the dequeue operation

	      Node = Current->Node;       // Node we're currently working on
	      if (Node->left)             // Enqueue child nodes
	      {
	         Add = new Queue (Node->left, Current->Level+1);
	         if ( Front )             // Begin the enqueue
	            Back->Next = Add;
	         else
	            Front = Add;
	         Back = Add;              // Finish the enqueue
	      }
	      if (Node->right)
	      {
	         Add = new Queue (Node->right, Current->Level+1);
	         if ( Front )
	            Back->Next = Add;
	         else
	            Front = Add;
	         Back = Add;
	      }
	//    Check for line-break:  change in tree level
	      if ( Current->Level != Level )
	      {
	         Out << endl;
	         Position = 0;
	         Level = Current->Level;
	      }
	//    Skip over to proper position
	      for ( ; Position < Node->util ; Position++ )
	         Out << ' ';
	      Out << ( Node->key < 10 ? "0" : "" ) << Node->key;
	      Position += 2;    // Update position for the output just done
	      delete Current;   // Return heap space for re-use.
	   }
	   Out << endl;         // Final line termination.
	}

};



