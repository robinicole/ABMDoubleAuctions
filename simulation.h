

 /*! @file main.cpp
  @brief main file of the multiagent simulation 
  of the agents trading with fixed buy and sell preferences 
  @author Robin NICOLE
  @bug no bugs 
*/

#ifndef SIMULATION_H
#define SIMULATION_H
#ifndef NR_NR3
#define NR_NR3
#include "nr/nr3.h"
#endif
#ifndef NR_RAN
#define NR_RAN
#include "nr/ran.h"
#endif

/*!
 @file simulation.h
 * @author Robin NICOLE
 * @version 1.0
 * @brief wile where is stored the header of the class which implement the multiagent simulation
 */
// standard library

#include <iostream>
#include <cmath>
#include <fstream>
//bool debug(false);

Ran myran(time(0));
#include "market.h" // class of market
#include "agentfbfsalpha.h"
#include "agentfbfsalpha.cpp"
#include "agentadaptive.h"
#include "agentadaptive.cpp"
#include "market.cpp"
using namespace std ;

/*! 
 * @class simulation
 * @brief class which is initialized with the vector of agents interacting in the economical system
 * and also the thetas of the two possible markets. then propose different ways of running the simulation
 */
class Simulation 

{
 private :
  vector<agentbase*> traders;
  market *m1, *m2;
  int Nagents;
  bool dynamic = true;
  string mode = "fbfs";
  int Nsnap = 0;
  int SnapshotNumber = 0;
 public :
  int set_dynamic(bool dyn) // choosing to compute the dynamic or not 
  {
    dynamic = dyn;
    return 0;
  }
  double av_score(const double pb);
  Simulation(market& m1t,market& m2t,vector<agentbase*> ag,string mo, string m_prefix);
  bool debug;
  string prefix ;
  int oneitt();
  int n_itterations(int n, int NSnapshot);
  int get_Nag(){return Nagents;};
  int set_temp(double& T);
  int set_fparam(double& fp);
  ofstream fractions;
};
#endif 
#ifndef HEAVISIDE_INC 
#define HEAVISIDE_INC
// defining a Heaviside function
int heaviside(double x) 
{
  if(x > 0 ) 
    {
      return 1;
    }
  else
    {
      return 0 ;
    }
}
#endif 
