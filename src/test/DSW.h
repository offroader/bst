// From Stout and Warren - translated from Pascal to C by Tim Rolfe,
// expanded and with an additional function to track tree height

// Reference:  Quentin F. Stout and Bette L. Warren,
// "Tree Rebalancing in Optimal Time and Space",
// Communications of the ACM, Vol. 29, No. 9
// (September 1986), p. 904.

// NOTE:  for ACM members, the full text is available through 
// http://www.acm.org/pubs/contents/journals/cacm/
// --- Navigate to Vol. 29, No. 9.

#ifndef DSW_H
#define DSW_H

#include "BST.h"

//#define VERBOSE    // Show vine and compacted tree

class DSW : public BST
{
   public:
      void rebalanceD();
      void rebalanceSW();
//    Sedgewick
      void SetN ( void )     // Set-up required before balanceR
      {  SetN ( Root );  }
      void balanceR ( void )
      {  Root = balanceR ( Root );  }
  
   private:
//    Sedgewick
      BasePtr rotR ( BasePtr h );
      BasePtr rotL ( BasePtr h );
      BasePtr partR( BasePtr h, int k );
      BasePtr balanceR ( BasePtr h );
//    Needed for Sedgewick's balancing
      int  SetN( BasePtr h );

      void tree_to_vine (BasePtr root, int &size);
      void compression  (BasePtr root, int count);
      void vine_to_tree (BasePtr root, int n);
      void vine_to_treeD(BasePtr root, int n);
      void correctTree  (BasePtr root, BasePtr Parent);
};
#endif
