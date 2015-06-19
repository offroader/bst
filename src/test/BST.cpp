/* BST.cpp */

/* Binary Search Tree class

   Author:  Timothy Rolfe

   Data Organization (struct)
      Data area  --- implemented here as a minimal single integer
      Left link  --- left subtree
      Right link --- right subtree

      Note:  constructor and destructor also declared for the struct

   Data Structure (class) BST --- binary search tree

      public member functions:

      constructor --- for the list/tree itself
      destructor  --- ditto
      Walk        --- traverse the data structure, showing the values
                      in increasing order.
      Insert      --- insert one cell into the data structure
      Find        --- find, based on data value; returns pointer to cell
      Delete      --- remove one cell from the data structure
      AvgDepth    --- average depth of all tree nodes
      Height      --- height of the entire tree

   NOTE:  in light of future use of this class, it has been designed
          to RECYCLE tree nodes:  rather than deleting them, they are
   entered into a "Free" list (maintained as a stack throug the Right
   link).  Consequently, when a node is inserted, it is taken from
   that Free list unless it is empty (in which case it is explicitly
   constructed).
*/

#include <iostream>
#include <iomanip>    // uses setw
#include <stdlib.h>     // uses atoi
#include <ctype.h>      // uses toupper
#include <assert.h>
#include "BST.h"        // NOTE:  Defines the structs and classes

//////////////////////////////////////////////////////////////////

BaseCell::~BaseCell (void)   // Traversal to delete done by BST
{/*cout << "Deleting " << Data << endl;*/}

//////////////////////////////////////////////////////////////////

// Generate a node, either re-using from the Free list or constructed
BasePtr BST::Build( int D, BasePtr P, BasePtr L, BasePtr R )
{  BasePtr Rtn;

   if (Free == NULL)
   {  Rtn = new BaseCell (D, P, L, R);
      if ( Rtn == NULL )
      {  cout << "Out of heap space.  Aborting execution." << endl;
         exit(7);
      }
   }
   else
   {  Rtn = Free;
      Free = Free->Right;
      Rtn->Data = D; 
      Rtn->Parent = P; Rtn->Left = L; Rtn->Right = R;
   }
   return Rtn;
}

// Push Node onto Free list, then set to NULL
void BST::Recycle( BasePtr &Node )
{  Node->Right = Free;
   Free = Node;
   Node = NULL;
#ifdef DEBUG
   cout << "Recycling " << Free->Data << endl;
#endif
}

//////////////////////////////////////////////////////////////////

BST::~BST(void)      // Destructor
{  Autumn(Root);             // Traverse, deleting cells
   while (Free)              // Then empty out the Free list
   {  BasePtr Node = Free;
      Free = Free->Right;
      delete Node;           // Finally do an explicit delete
   }
}

void BST::Empty(void)      // Empty out a tree
{  Autumn(Root);  }     // Unlike ~BST, though, the tree remains

// In-order tree traversal to delete cells.  Called by ~BST.
void BST::Autumn(BasePtr &Node)
{  if (Node)
   {//Note:  Node->Right may be lost on the "delete Node"
      BasePtr Hold = Node->Right;

      Autumn(Node->Left);
      Recycle(Node);    // Note that this NULLs out Node
      Autumn(Hold);
   }
}

/******************* Recursive traversal *******************/

// Walk through the tree; display is to be ascending order
void BST::Walk(void)
{  if (Root)
   {  Nitems = 0;
      InOrder(Root);
      if (Nitems % 10)
         cout.put('\n');
   }
}

// To get ascending order, do an in-order traversal of the tree
void BST::InOrder(BasePtr Item)
{  if ( !Item ) return;                // I.e., NULL tree

   InOrder(Item->Left);                // Process Left sub-tree
   cout << setw(4) << Item->Data       // Process this node
        << '(' << Item->Height << ')';
   if ( ++Nitems % 10 == 0 )
      cout.put('\n');
   InOrder(Item->Right);               // Process Right sub-tree
}

/******************* Non-recursive traversals *******************/

// Empty out the stack before destroying it.
BSTstack::~BSTstack ( void )
{
   while ( Root != NULL )
   {
      StackCell *Tmp = Root;
      Root = Root->Next;
      delete Tmp;
   }
}

void BSTstack::Pop ( BasePtr &Item, int &Vst )
{
   StackCell *Tmp = Root;

   assert (Tmp != NULL);
   Root = Root->Next;

   Item = Tmp->Node;
   Vst  = Tmp->Visit;
   delete Tmp;
}

// Simplest possible Visit:  just print the node
inline void Visit ( BasePtr Node )
{  cout << Node->Data << "  " << flush;  }

// Non-recursive traversal algorithms

// Carrano's non-recursive in-order traversal
// alterations by Timothy Rolfe

void BST::Carrano(BasePtr Cur)  
{
   BSTstack Stack;
   int      Dmy = 0;

// Revision:  instead of a boolean loop variable, just use a break:

   while ( 1 )                    // Infinite loop exited via break
   {
//    Revision:  Carrano code is equivalent in behavior to a "while"
      while ( Cur != NULL )
      {
         Stack.Push ( Cur, Dmy );
         Cur = Cur->Left;
      }
//    What follows would be under Carrano's "else"
      if ( Stack.Empty() )
         break;                   // Exit the processing loop
      else
      {
         Stack.Pop ( Cur, Dmy );
         Visit (Cur);
         Cur = Cur->Right;
      }
   }
}

// Implementation of Weiss' logic for post-order traversal,
// generalized by Timothy Rolfe for pre-order and post-order.

void BST::Weiss  (BasePtr Node, int Opt)
//       Opt = 1:  pre-order
//       Opt = 3:  post-order
// NOTE: Opt = 2 does NOT properly handle in-order traversal
{
   BSTstack Stack;
   int      Nvisit;

   Stack.Push(Node, 1);

   while ( ! Stack.Empty() )
   {
      Stack.Pop ( Node, Nvisit );
      if ( Nvisit == Opt )
         Visit(Node);
      else
         Stack.Push ( Node, Nvisit+1 );
      if ( Nvisit == 1 )
      {//Push right-to-left to simulate recursive behavior
         if ( Node->Right != NULL )
            Stack.Push (Node->Right, 1);
         if ( Node->Left != NULL )
            Stack.Push (Node->Left, 1);
      }
   }
}

// Display the BST horizontally --- based on a level-order traversal
void BST::Pretty(ostream &Out)
{
   int Skip = 0;

   if ( Root == NULL )  // Nothing to display!
   {  cout << "Empty tree!\n"; return;  }

   SetPos (Root, Skip); // Find line position for each node
   Pretty (Root, Out);  // Level-order traversal displaying the nodes
}                       // one line for each level, in proper position

// Find line position for each node --- based on in-order traversal
void BST::SetPos (BasePtr Node, int &Skip)
{
// If the nodes were all printed on one line, their order is based
// in an in-order traversal.  Skip shows number of positions to skip
// to properly position the node.  Note that is MUST be a reference
// parameter --- the root depends on the reference parameter to come
// back with the size of the entire left subtree, for instance.
   if ( Node )
   {
      SetPos (Node->Left, Skip);
      Node->Util = Skip;     // Store skip value in Util data member
      Skip = Skip + 2;       // Update for printing THIS node
      SetPos (Node->Right, Skip);
   }
}

// Pretty-print:  each tree level prints on one line, with the node
// horizontally positioned to be visually the parent of its children.
void BST::Pretty (BasePtr Node, ostream &Out)
{//Level-order traversal requires a queue --- build a quick one here.
   struct Queue
   {
      BasePtr Node;
      int     Level;    // Change in level is where the endl prints
      Queue  *Next;

      Queue(BasePtr N, int Lvl, Queue *Nxt = NULL)
      : Node(N), Level(Lvl), Next(Nxt) { /* nothing else */ }
   }  *Front = NULL, *Back;

   int Level = 0, Position = 0;

// Level-order traversal:  initialize the work queue with the root

   Front = Back = new Queue(Node, 0);  // Node initially IS the root

   while ( Front ) // Keep going till there's nothing left to do!
   {
      Queue *Add,                 // Queue pointer when we add a job
            *Current = Front;     // Begin the dequeue operation

      Front = Front->Next;        // Complete the dequeue operation

      Node = Current->Node;       // Node we're currently working on
      if (Node->Left)             // Enqueue child nodes
      {
         Add = new Queue (Node->Left, Current->Level+1);
         if ( Front )             // Begin the enqueue
            Back->Next = Add;
         else
            Front = Add;
         Back = Add;              // Finish the enqueue
      }
      if (Node->Right)
      {
         Add = new Queue (Node->Right, Current->Level+1);
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
      for ( ; Position < Node->Util ; Position++ )
         Out << ' ';
      Out << ( Node->Data < 10 ? "0" : "" ) << Node->Data;
      Position += 2;    // Update position for the output just done
      delete Current;   // Return heap space for re-use.
   }
   Out << endl;         // Final line termination.
}

/******************* Insertion *******************/

// Insert the Value into its proper place in the binary search tree
void BST::Insert(int Value)
{  Insert(Root, Value);  }

// Little in-line function used in height adjustment
inline int max ( int L, int R )
{ return L > R ? L : R ; }

void BST::Insert(BasePtr Node, int Value)
{
   if ( Node == NULL )
      Root = Build (Value, Node);
   else
      while(1)     // breaks on node creation
      {
         if ( Value < Node->Data )
            if ( Node->Left != NULL )
               Node = Node->Left;
            else
            {
               Node = Node->Left = Build (Value, Node);
               break;
            }
         else                          
            if ( Node->Right != NULL )
               Node = Node->Right;
            else
            {
               Node = Node->Right = Build (Value, Node);
               break;
            }
      }
   AdjustHt(Node);
}

/******************* Deletion *******************/

// Delete the node with the Value passed
void BST::Delete(int Value)
{  Delete(Root, Value);  }

// Before the utility Delete function, some utility functions
// on BaseCell struct objects as free functions:

// Return the computed height based on the children
inline int NewHt( BasePtr L, BasePtr R )
{
   if ( !L && !R )           // Leaf node is height 1
      return 1;
   else if (!L)
      return 1 + R->Height;  // Half node cases
   else if (!R)
      return 1 + L->Height;
   else                      // Full node --- need max
      return 1 + max(L->Height, R->Height);
}

// Overloaded --- passed a single node and calls above routine
inline int NewHt ( BasePtr Node )
{  return Node ? NewHt (Node->Left, Node->Right) : 0;  }

void BST::AdjustHt(BasePtr Node)
{
   while (Node)
   {
      Node->Height = NewHt(Node);
      Node = Node->Parent;
   }
}

// Interchange the ->Data fields on two BaseCells passed by pointer
void SwapData (BasePtr d, BasePtr s)
{  int Temp = d->Data;  d->Data = s->Data;  s->Data = Temp; }

// The following always chooses the in-order predecessor as the
// replacement node on the delete.

void BST::Delete(BasePtr Node, int Value)
{
   BasePtr Parent = NULL, Child;

   while ( Node != NULL && Node->Data != Value )
      if ( Value < Node->Data )
         Node = Node->Left;
      else
         Node = Node->Right;

   if ( Node == NULL )
   {
      cerr << "Failed to find " << Value
           << " for deletion." << endl;
      return;
   }

// If Node has ANY children, transform it into its
// replacement node and delete THAT node.  The
// height adjustments will always be from wherever
// the Parent pointer ends up.
   if ( ! Node->Left && ! Node-> Right )    // I.e., leaf node
   {
      Parent = Node->Parent;
      if ( Parent == NULL )                 // Single-node tree
         Root = NULL;
      else if ( Value < Parent->Data )      // Traversed left
         Parent->Left = NULL;
      else                                  // else we went right
         Parent->Right = NULL;
      Recycle(Node);                        // only time Node itself goes
   }
   else if ( Node->Left == NULL )           // Half-full to the right
   {
      Parent = Node;
      Child  = Node->Right;
      SwapData (Parent, Child);
      Parent->Left  = Child->Left;
      if (Parent->Left)
         Parent->Left->Parent = Parent;     // CRITICAL to keep the
      Parent->Right = Child->Right;         // parent pointers valid
      if (Parent->Right)
         Parent->Right->Parent = Parent;
      Recycle (Child);
   }
   else if ( Node->Right == NULL )          // Half-full to the left
   {
      Parent = Node;
      Child  = Node->Left;
      SwapData (Parent, Child);
      Parent->Left  = Child->Left;
      if (Parent->Left)
         Parent->Left->Parent = Parent;
      Parent->Right = Child->Right;
      if (Parent->Right)
         Parent->Right->Parent = Parent;
      Recycle (Child);
   }
   else                                     // Full
   {
      Child = Node->Left;
      while (Child->Right)                  // In-order predecessor
      {
         Parent = Child;
         Child  = Parent->Right;
      }
      if ( Parent != NULL )                 // DID traverse in left subtree
      {
         SwapData (Node, Child);
         Parent->Right = Child->Left;
         if (Parent->Right)
            Parent->Right->Parent = Parent;
         Recycle (Child);
      }
      else                                  // I.e., Node->Left itself
      {
         Parent = Node;                     // for height adjustment
         SwapData (Node, Child);
         Parent->Left = Child->Left;
         if (Parent->Left)
            Parent->Left->Parent = Parent;
         Recycle (Child);
      }
   }
   AdjustHt (Parent);
}


/******************* Simple Find *******************/

// Find the cell with the data field corresponding to Value
BaseCell* BST::Find(int Value)
{  Current = Root;
   while ( Current != NULL && Current->Data != Value )
   {  if (Value < Current->Data)
         Current = Current->Left;
      else
         Current = Current->Right;
   }

   return Current;
}

/******************* Miscellaneous other *******************/

int BST::Size(BasePtr Node)  // Traverse, counting nodes
{
   if ( Node )
      return 1 + Size(Node->Left) + Size(Node->Right);
   else
      return 0;
}

// Average depth of nodes within the tree:
double BST::AvgDepth(BasePtr Node)
{  double SigmaD = 0;
   long   Nnodes = 0;

   if (Node == NULL) return 0;

// Root is at depth 1
   DepthStats (Node, 1, Nnodes, SigmaD);
   return SigmaD / Nnodes;
}

// Total number of nodes and depth of each node
void BST::DepthStats(BasePtr Node, int Deep, long &N, double &Sigma)
{  if ( Node != NULL )
   {  DepthStats(Node->Left, Deep+1, N, Sigma);
      DepthStats(Node->Right, Deep+1, N, Sigma);
      N++;
      Sigma += Deep;
   }
}
