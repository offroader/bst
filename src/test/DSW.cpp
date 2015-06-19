#include "DSW.h"
//#define DAY        // Use the Strip function

// From Stout and Warren - translated from Pascal to C by Tim Rolfe,
// expanded and with an additional function re. node height and parent

// Rebalance procedure:  go from tree to vine, then
// return from vine to tree.

// To avoid the special case of processing at the root, this uses
// a "pseudo-root" is passed --- comparable with a dummy header node
// for a linked list.

// Reference:  Quentin F. Stout and Bette L. Warren,
// "Tree Rebalancing in Optimal Time and Space",
// Communications of the ACM, Vol. 29, No. 9
// (September 1986), p. 904.

// NOTE:  for ACM members, the full text is available through 
// http://www.acm.org/pubs/contents/journals/cacm/
// --- Navigate to Vol. 29, No. 9.

// Reverse in-order traversal, pushing nodes onto a
// linked list (through the ->Right link) during the
// traversal.  Tree passed through "node" is destroyed.
void Strip ( BasePtr node, BasePtr &List, int &size )
{  if ( node != NULL )
   {//Retain pointer to left subtree.
      BasePtr Left = node->Left;
    //In the degenerate tree, all left pointers are NULL
      node->Left  = NULL;
    //Reversed; hence right, middle, then left.
      Strip ( node->Right, List, size );

    //Stack discipline in building the list
      node->Right = List;
      List = node;
      size++;           // Count the number of nodes

      Strip ( Left, List, size );
   }
}

void DSW::rebalanceD()
// Public member function:  Do the DSW algorithm to balance the tree
{//Declare as automatic variable; remember to pass as pointer
   BaseCell pseudo_root( -1, NULL, NULL, Root );

   int size;

// Null out List for entry of first item onto the stack
   BasePtr List = NULL;

// Day's transformation of tree to backbone:  Strip
   size = 0;
   Strip ( pseudo_root.Right, List, size );
   pseudo_root.Right = List;
#ifdef VERBOSE
   Pretty(pseudo_root.Right, cout);  cin.ignore(255, '\n');
#endif

   vine_to_treeD (&pseudo_root, size);
#ifdef VERBOSE
   Pretty(pseudo_root.Right, cout);  cin.ignore(255, '\n');
#endif

   Root = pseudo_root.Right;
   correctTree (Root, NULL);
}

void DSW::rebalanceSW()
// Public member function:  Do the DSW algorithm to balance the tree
{//Declare as automatic variable; remember to pass as pointer
   BaseCell pseudo_root( -1, NULL, NULL, Root );

   int size;

// Stout/Warren transformation of tree to vine
   tree_to_vine (&pseudo_root, size);
#ifdef VERBOSE
   Pretty(pseudo_root.Right, cout);  cin.ignore(255, '\n');
#endif

   vine_to_tree (&pseudo_root, size);
#ifdef VERBOSE
   Pretty(pseudo_root.Right, cout);  cin.ignore(255, '\n');
#endif

   Root = pseudo_root.Right;
   correctTree (Root, NULL);
}

// Tree to Vine algorithm:  a "pseudo-root" is passed ---
// comparable with a dummy header for a linked list.
void DSW::tree_to_vine ( BasePtr root, int &size )
{  BasePtr vineTail, remainder, tempPtr;

   vineTail = root;
   remainder = vineTail->Right;
   size = 0;

   while ( remainder != NULL )
   {//If no leftward subtree, move rightward
      if ( remainder->Left == NULL )
      {  vineTail = remainder;
         remainder = remainder->Right;
         size++;
      }
//    else eliminate the leftward subtree by rotations
      else  /* Rightward rotation */
      {  tempPtr = remainder->Left;
         remainder->Left = tempPtr->Right;
         tempPtr->Right = remainder;
         remainder = tempPtr;
         vineTail->Right = tempPtr;
      }
   }
}

// Stout and Warren have a nested procedure --- not
// supported by C.  So the nested procedure is given
// here prior to the code that uses it.

void DSW::compression ( BasePtr root, int count )
{  BasePtr scanner, child;
   int     j;

   scanner = root;
   for ( j = 0; j < count; j++ )
   {//Leftward rotation
      child = scanner->Right;
      scanner->Right = child->Right;
      scanner = scanner->Right;
      child->Right = scanner->Left;
      scanner->Left = child;
   }  // end for
}  // end compression

// Code added by Tim Rolfe:  Expands on Warren & Stout's
// notation involving powers, floors, and base-2 logs
int FullSize ( int size )    // Full portion complete tree
{  int Rtn = 1;
   while ( Rtn <= size )      // Drive one step PAST FULL
      Rtn = Rtn + Rtn + 1;   // next pow(2,k)-1
   return Rtn/2;
}

void DSW::vine_to_tree ( BasePtr root, int size )
{
   int full_count = FullSize (size);
   compression(root, size - full_count);
   for ( size = full_count ; size > 1 ; size /= 2 )
      compression ( root, size / 2 );
}

// Loop structure taken directly from Day's code
void DSW::vine_to_treeD( BasePtr root, int size )
{  int NBack = size - 1;
   for ( int M = NBack / 2; M > 0; M = NBack / 2)
   {  compression ( root, M );
      NBack = NBack - M - 1;
   }
}
// Code added by Tim Rolfe
inline int Max( int l, int r)     // used by correctHeight
{  return l > r ? l : r;  }

// Traverse entire tree, correcting heights and parents
void DSW::correctTree( BasePtr node, BasePtr Parent )
{  if ( node != NULL )
   {  int LtHt, RtHt;

      correctTree (node->Left, node);
      correctTree (node->Right, node);
      node->Parent = Parent;
      LtHt = node->Left  ? node->Left->Height  : 0;
      RtHt = node->Right ? node->Right->Height : 0;
      node->Height = 1 + Max( LtHt, RtHt );
   }
}

// Sedgewick material
BasePtr DSW::balanceR ( BasePtr h )
{
   if ( !h || h->Util < 2 ) return h;
   h = partR ( h, h->Util/2);
   h->Left  = balanceR ( h->Left );
   h->Right = balanceR ( h->Right);
   return h;
}

BasePtr DSW::rotR ( BasePtr h )
{  int rNr = h->Right ? h->Right->Util : 0,
       rNl = h->Left->Right ? h->Left->Right->Util : 0,
       lN  = h->Left->Left  ? h->Left->Left->Util  : 0;
   BasePtr x = h->Left;  h->Left = x->Right; x->Right = h;
   h->Util = rNr + rNl;  x->Util = lN + h->Util;
   return x;
}

BasePtr DSW::rotL ( BasePtr h )
{  int lNl = h->Left ? h->Left->Util : 0,
       lNr = h->Right->Left ? h->Right->Left->Util : 0,
       rN  = h->Right->Right ? h->Right->Right->Util : 0;
   BasePtr x = h->Right; h->Right = x->Left; x->Left = h;
   h->Util = lNl + lNr; x->Util = rN + h->Util;
   return x;
}

BasePtr DSW::partR( BasePtr h, int k )
{
   int t = h->Left ? h->Left->Util : 0;
   if ( t > k )
   {  h->Left = partR(h->Left, k); h = rotR(h); }
   if ( t < k )
   {  h->Right = partR(h->Right, k-t-1); h = rotL(h); }
   return h;
}

int DSW::SetN( BasePtr h )
{
   int lN, rN;

   if ( h == NULL ) return 0;

   lN = SetN ( h->Left );
   rN = SetN ( h->Right);
   h->Util = 1 + lN + rN;
   return h->Util;
}
