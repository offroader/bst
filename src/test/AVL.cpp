/* Implementation of AVL member functions

   Author:  Timothy Rolfe
*/
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctype.h>
#include "AVL.h"

int AVL::Valid(BasePtr Node)
{  int Lht, Rht;

   if (Node == NULL) return 1;
   Lht = Node->Left  == NULL ? 0 : Node->Left->Height;
   Rht = Node->Right == NULL ? 0 : Node->Right->Height;

   if ( abs(Lht-Rht) > 1 ) return 0;
   return Valid(Node->Left) && Valid(Node->Right);
}

// Over-ride BST class insert and delete
void AVL::Insert(BasePtr Node, int Value)
{
   BasePtr Parent = NULL,
           Child  = Node;

   if ( Node == NULL )
      Root = Build (Value, Node);
   else
      while(1)     // breaks out on node creation
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

   AVLadjust(Node);
}

// Interchange the ->Data fields on two BaseCells passed by pointer
void SwapData (BasePtr d, BasePtr s);  // Implemented in BST_Lib.cpp.

// The following always chooses the in-order predecessor as the
// replacement node on the delete.

void AVL::Delete(BasePtr Node, int Value)
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
   AVLadjust(Parent);
}

inline int max (int L, int R)
{  return L > R ? L : R;  }

// Adjust height
void AVL::HtAdjust(BasePtr Node)
{
   int Lht, Rht;

   if (Node == NULL)
      return;
   Lht = Node->Left  == NULL ? 0 : Node->Left->Height;
   Rht = Node->Right == NULL ? 0 : Node->Right->Height;
   Node->Height = 1 + max(Lht, Rht);
}

//    If necessary, do rotations to enforce the AVL condition
void AVL::AVLadjust(BasePtr Node)
{
   while ( Node != NULL )
   {
      int Lht = Node->Left  == NULL ? 0 : Node->Left->Height,
          Rht = Node->Right == NULL ? 0 : Node->Right->Height,
          Diff = Lht - Rht;

      if ( abs(Diff) > 1 ) // AVL condition is violated
      {
#ifdef TRACE
         cout << "Adjusting AVL at node " << Node->Data << endl;
#endif
         if ( Lht > Rht )// Left side two longer than right
         {  int Lck = Node->Left->Left  == NULL ? 0
                                                : Node->Left->Left->Height,
                Rck = Node->Left->Right == NULL ? 0
                                                : Node->Left->Right->Height;
            if ( Lck < Rck )
               RotateLeft (Node->Left); // Make left the longer
            RotateRight (Node);         // Adjust Node itself
         }
         else  // Mirror image logic to that above:  Right than left
         {  int Lck = Node->Right->Left  == NULL ? 0
                                                 : Node->Right->Left->Height,
                Rck = Node->Right->Right == NULL ? 0
                                                 : Node->Right->Right->Height;
            if ( Lck > Rck )
               RotateRight (Node->Right);
            RotateLeft (Node);
         }
      }
      HtAdjust(Node);   // Whatever has happened, adjust height.

      Node = Node->Parent;
   }
}

// Rotation functions (which also adjust heights)

// Note:  since we do not have a reference parameter, we use
//        the strategy of TRANSFORMING the two nodes involved
// in the rotation into each other, and then dealing out the
// children appropriately.

void AVL::RotateLeft (BasePtr Lt)
// Rotate leftward --- left node moves down, right node moves up.
{  BasePtr  Rt = Lt->Right,
            N1 = Lt, N2 = Rt;

   SwapData (N1, N2);        // Now N1 is Rt, N2 is Lt
   N1->Right = N2->Right;    // Rt->Right adjustment
   if (N1->Right != NULL)
      N1->Right->Parent = N1;
   N2->Right = N2->Left;     // Move across the middle subtree
   if (N2->Right != NULL)
      N2->Right->Parent = N2;
   N2->Left  = N1->Left;     // Lt->Left adjustment
   if (N2->Left != NULL)
      N2->Left->Parent = N2;
   N1->Left   = N2;          // Lt is left subchild of Rt
   N2->Parent = N1;

   HtAdjust (N2);            // NB:  adjust CHILD before PARENT
   HtAdjust (N1);
}

void AVL::RotateRight(BasePtr Rt)
{  BasePtr  Lt = Rt->Left,
            N1 = Rt, N2 = Lt;

   SwapData (N1, N2);        // Now N1 is Lt, N2 is Rt

   N1->Left = N2->Left;      // Lt->Left adjustment
   if (N1->Left != NULL)
      N1->Left->Parent = N1;
   N2->Left = N2->Right;     // Move across the middle subtree
   if (N2->Left != NULL)
      N2->Left->Parent = N2;
   N2->Right = N1->Right;    // Rt->Right adjustment
   if (N2->Right != NULL )
      N2->Right->Parent = N2;
   N1->Right  = N2;          // Rt is right subchild of Lt
   N2->Parent = N1;

   HtAdjust (N2);            // NB:  adjust CHILD before PARENT
   HtAdjust (N1);
}
