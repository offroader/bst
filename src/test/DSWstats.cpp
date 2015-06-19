/* Build BST/AVL trees from a sampling of permutations of N items, and
   accumulate statistics.

   Author:  Timothy Rolfe

   Set up for some minimal parallel processing:  UNIX environment, with
   straight "fork" available.  The fork call comes after the files have
   been opened, so that the two processes share the same output file.
   Then the two processes alternate in the tree sizes being tested ---
   the child process does the even-offset trees from the start, the
   parent process does the odd-offset trees.  If the two processors
   have similar loading, the file should be generated in proper order.
   If it isn't, a simple sort based on the first field will get it
   into proper order.
*/
#include <iostream>
#include <fstream>    // ofstream
#include <stdlib.h>     // exit
#include <time.h>       // clock (timing statistics)
#include "AVL.h"        // AVL class --- use one with equal keys allowed
#include "DSW.h"        // DSW derived class

using namespace std;

// If this is a dual-processor UNIX machine, we can actually use a
// simple fork and run two sets of experiments in parallel.
  
//#define DUAL_PROCESSOR  // UNIX environment, dual-processor computer

#ifdef DUAL_PROCESSOR
#include <unistd.h>     // fork (at least for Linux)
#endif

int main3 (int argc, char* argv[])
{
   AVL    Atree;
   DSW    Tree;
   int    k;            // Loop variable
   int    N,            // Number of nodes
          Nstep,        // Increment for N
          Nlim;         // Largest number to run
   long   Pass,         // Which pass in the Monte Carlo runs
          Npasses;      // Number of passes being averaged
   time_t Seed;         // Start of random number sequence
   ofstream Fout("DSWstats.csv",ios::app);    // Accumulate statistics
   double T0, T1, T2, T3, T4;

   cout.precision(3);
   Fout.precision(3);
   cout.setf(ios::fixed | ios::showpoint);
   Fout.setf(ios::fixed | ios::showpoint);

   cout << "Initial number of nodes:  ";

   if ( argc > 1 )
   {
      N = atoi(argv[1]);
      cout << N << endl;
   }
   else
   {
      cin >> N;
   }

   cout << "Largest number of nodes:  ";

   if ( argc > 2 )
   {
      Nlim = atoi(argv[2]);
      cout << Nlim << endl;
   }
   else
   {
      cin >> Nlim;
   }

   cout << "Step size in generating BST statistics:  ";

   if ( argc > 3 )
   {
      Nstep = atoi(argv[3]);
      cout << Nstep << endl;
   }
   else
   {
      cin >> Nstep;
   }

   cout << "Number of Monte Carlo passes to average:  ";

   if ( argc > 4 )
   {
      Npasses = atoi(argv[4]);
      cout << Npasses << endl;
   }
   else
   {
      cin >> Npasses;
      cin.ignore(255, '\n');
   }

   Fout << "N, Base Line, D time, SW time, Sedge time, AVL time" << endl;
   cout << "N, Base Line, D time, SW time, Sedge time, AVL time" << endl;

   Seed = time(NULL);
#ifdef DUAL_PROCESSOR   
// If it IS dual-processor, fork the second process, then start
// two different random sequences and arrange to alternate tree
// sizes during the simulation.
   pid_t Child = fork();
   if ( Child != 0 )    // I.e., if this is the parent process
      N += Nstep;       // Do the odd-offset ones
   Nstep += Nstep;      // Each process steps by twice as much

#endif

   for ( ; N <= Nlim ; N += Nstep )
   {
      clock_t Start;

#ifndef DUAL_PROCESSOR   
      srand(Seed);   // Get a chance start to rand() sequence
#else                   
// Chance start to rand() sequence --- different for parent and child
   srand(Seed + Child);
#endif
      Start = clock();
      for (Pass = 0; Pass < Npasses; Pass++)
      {
         for ( k = 0; k < N; k++ )
            Tree.Insert ( rand() );
//       NO tree balancing to get base line
         Tree.Empty();
      }

      T0 = double(clock()-Start) / CLOCKS_PER_SEC * 1000 / Npasses;

#ifndef DUAL_PROCESSOR   
      srand(Seed);   // Get a chance start to rand() sequence
#else                   
// Chance start to rand() sequence --- different for parent and child
   srand(Seed + Child);
#endif
      Start = clock();
      for (Pass = 0; Pass < Npasses; Pass++)
      {
         for ( k = 0; k < N; k++ )
            Tree.Insert ( rand() );
         Tree.rebalanceD();
         Tree.Empty();
      }

      T1 = double(clock()-Start) / CLOCKS_PER_SEC * 1000 / Npasses;

#ifndef DUAL_PROCESSOR   
      srand(Seed);   // Get a chance start to rand() sequence
#else                   
// Chance start to rand() sequence --- different for parent and child
   srand(Seed + Child);
#endif
      Start = clock();
      for (Pass = 0; Pass < Npasses; Pass++)
      {
         for ( k = 0; k < N; k++ )
            Tree.Insert ( rand() );
         Tree.rebalanceSW();
         Tree.Empty();
      }

      T2 = double(clock()-Start) / CLOCKS_PER_SEC * 1000 / Npasses;
#ifndef DUAL_PROCESSOR   
      srand(Seed);   // Get a chance start to rand() sequence
#else                   
// Chance start to rand() sequence --- different for parent and child
   srand(Seed + Child);
#endif
      Start = clock();
      for (Pass = 0; Pass < Npasses; Pass++)
      {
         for ( k = 0; k < N; k++ )
            Tree.Insert ( rand() );
         Tree.SetN();
         Tree.balanceR();
         Tree.Empty();
      }

      T3 = double(clock()-Start) / CLOCKS_PER_SEC * 1000 / Npasses;

      Start = clock();

#ifndef DUAL_PROCESSOR   
      srand(Seed);   // Get a chance start to rand() sequence
#else                   
// Chance start to rand() sequence --- different for parent and child
      srand(Seed + Child);
#endif
      for (Pass = 0; Pass < Npasses; Pass++)
      {
         for ( k = 0; k < N; k++ )
            Atree.Insert ( rand() );
         Atree.Empty();
      }

      T4 = double(clock()-Start) / CLOCKS_PER_SEC * 1000 / Npasses;

      cout << N << ',' << T0 << ',' << T1 << ',' << T2 << ','
           << T3 << ',' << T4 << endl;

      Fout << N << ',' << T0 << ',' << T1 << ',' << T2 << ','
           << T3 << ',' << T4 << endl;
      Seed = rand();
   }
   
   return 0;
}
