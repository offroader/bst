/* BST_Lib.h */

/* Binary Search Tree class

   Author:  Timothy Rolfe

   Changes from earlier:
      Delete implemented --- always goes right (inorder succ)
      equal keys allowed (go left on equal)

   Data Organization (struct)
      Data area   --- implemented here as a minimal single integer
      Height      --- height within the tree of THIS node
      Util        --- miscellaneous int value, should we every need it
      Parent link --- allow upward adjustment of heights
      Left link   --- left subtree
      Right link  --- right subtree

      Note:  constructor and destructor also declared for the struct

   Data Structure (class) BST --- binary search tree

      public member functions:

      constructor --- for the list/tree itself
      destructor  --- ditto
      Walk        --- traverse the data structure, showing the values
                      in increasing order.
      Pretty      --- Display the tree structure (level-order traversal)
      TstNR(Opt)  --- Exercise Carrano (Opt=2) and Weiss (Opt=1 or 3)
                      non-recursive traversal algorithms.
      Insert      --- insert one cell into the data structure
      Find        --- find, based on data value; returns pointer to cell
      Delete      --- remove one cell from the data structure
      AvgDepth    --- average depth of all tree nodes
      Height      --- height of the entire tree
*/

#ifndef BST_H  // Protect against multiple #include calls
#define BST_H

#include <stdlib.h>     // NULL definition
#include <iostream>   // ostream

using namespace std;

typedef struct BaseCell*  BasePtr;

struct BaseCell    // Binary Search Tree
{                  //   Left  for "<" subtree
                   //   Right for ">" subtree

   int     Data;   // Realistic case, this could be quite large
   int     Height; // Height of THIS NODE
   int     Util;   // Generic utility area for an int value
   BasePtr Parent,
           Left,
           Right;

   BaseCell ( int D = 0,
              BasePtr P = NULL, BasePtr L = NULL, BasePtr R = NULL )
   : Data(D), Parent(P), Left(L), Right(R), Height(1)
   { /* nothing else! */ }
   ~BaseCell (void);    // Allow for debugging display
};

/* Binary search tree --- identical keys are forbidden.          */
class BST
{  public:

      BST(void) : Root(NULL), Free(NULL), Nitems(0)
      { /* Nothing else */ }

      void Empty(void);      // Empty out a tree
      ~BST(void);

      void    Walk(void);
      void    Pretty(ostream&);
      void    Insert(int);
      void    Delete(int);
      BasePtr Find(int);

//    Test non-recursive algorithms:
//       Opt = 1:  Weiss-style  pre-order
//       Opt = 2:  Carrano's     in-order
//       Opt = 3:  Weiss-style post-order
      void    TstNR(int Opt)
      {  if   (Opt==2)  Carrano(Root);
         else           Weiss(Root, Opt);
         cout << endl;
      }

      int     Size(void)
      {  return Size(Root);  }
      double  AvgDepth(void)
      {  return AvgDepth(Root); }
//    Height function is simple enough to implement here
      int     Height(void)   // Root = NULL means empty = 0
      {  return Root == NULL ? 0 : Root->Height;  }
      int     HeightLt(void)
      {  return (Root && Root->Left) ? Root->Left->Height : 0; }
      int     HeightRt(void)
      {  return (Root && Root->Right) ? Root->Right->Height : 0; }

   protected:

//    Generate a node, either re-using from the Free list or constructed
      BasePtr Build( int D = 0, 
                     BasePtr P = NULL, BasePtr L = NULL, BasePtr R = NULL );

//    Push Node onto Free list, then set to NULL
      void Recycle( BasePtr &Node );

      void Carrano(BasePtr);
      void Weiss  (BasePtr, int);
//       Opt = 1:  Weiss-style pre-order
//       Opt = 3:  Weiss-style post-order
      void InOrder(BasePtr); // Called from Walk()

      void SetPos (BasePtr, int&);     // Compute line positions
      void Pretty (BasePtr, ostream&); // Level-order display

      void Autumn(BasePtr&); // In-order walk deleting nodes

      void Insert(BasePtr, int);     // Used by public Insert(int)

      void Delete(BasePtr, int);     // Used by public Delete(int)

      void AdjustHt(BasePtr);        // Used after Insert and Delete

      double AvgDepth(BasePtr);

      int Size(BasePtr);             // Traverse, counting nodes

//    Total number of nodes and depth of each node
      void DepthStats(BasePtr, int, long&, double&);

      BasePtr Root,
              Current;
      int Nitems;                 // Used in the protected tree walk
      BasePtr Free;               // Nodes for re-use
};

// Both Carrano() and Weiss() make use of a stack of
// node entries, so it is more efficient to implement
// the class rather than to embed the logic in both
// of those functions.
class BSTstack
{  public:
      BSTstack ( void ) : Root(NULL)  { /* nothing else */ }
     ~BSTstack ( void );

      int  Empty ( void )
      {  return Root == NULL; }

      void Push( BasePtr  Item, int  Vst ) 
      {  Root = new StackCell(Item, Vst, Root);  }

      void Pop ( BasePtr &Item, int &Vst );

   private:
      struct StackCell 
      {  BasePtr    Node;
         int        Visit;
         StackCell *Next;
         StackCell ( BasePtr Item, int Vst, StackCell *Nxt = NULL )
         : Node(Item), Visit(Vst), Next(Nxt) { /* Nothing else */ }
      } *Root;

};

#endif
