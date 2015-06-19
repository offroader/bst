#ifndef AVL_H        // protect against multiple #includes
#define AVL_H

#include "BST.h"
//#define   TRACE    // Report node at which adjustment made

class AVL : public BST
{  public:
      AVL(void) : BST() { /* nothing else */ }
//    Test whether current state is valid
      int   Valid(void) // Working function is actually private
      {  return Valid(Root); }
//    Override public insert and delete to link into
//    the overridden private insert and delete.
      void  Insert(int V)
      {  Insert ( Root, V ); }
      void  Delete(int V)
      {  Delete ( Root, V ); }

   private:
//    Over-ride BST class insert and delete
      void Insert(BasePtr, int);
      void Delete(BasePtr, int);
      int  Valid (BasePtr);
//    Adjust height
      void HtAdjust(BasePtr);
//    If necessary, do rotations to enforce the AVL condition
      void AVLadjust(BasePtr);
//    Rotation functions (which also adjust heights)
      void RotateLeft (BasePtr);
      void RotateRight(BasePtr);
};

#endif
