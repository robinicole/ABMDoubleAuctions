/*!
 @file simulation.cpp
 * @author Robin NICOLE
 * @version 1.0
 * @brief file where there is the function simulation which launch a simulation with the agents given as parameters
 */
#ifndef SIMULATION_CPP
#define SIMULATION_CPP
#include "simulation.h"
#include <string>
#include <fstream>
/*!
 * @brief Consturctor of the class simulation. Need to get a vector of agents 
 * and and the preferences of the two markets
 * @param @param th1 : preference of the first market toward buyer used to set the trading price
 * @param m1 : pointer to the first market
 * @param m2 : pointer to the second market 
 */

inline double sigmoid(const double& x, const double& T)
{
  return 1./(1+exp(-x/T));
}
Simulation::Simulation(market& m1t,market& m2t,vector<agentbase*> ag,string mo, string m_prefix){
  //  cout << Temp << " " << theta1 << " " << theta2 << " "<< itterations ;
  prefix  = m_prefix ; 
  Nagents = ag.size();
  // copying the pointers to the traders vector
  traders = ag;
  debug = false ;
  m1 = &m1t;
  m2 = &m2t;
  if (mo == "adaptive") mode = mo ; 
  else if (mo == "fbfs") mo = "fbfs";
  else cout << "There is a problem with the mode parameter" << endl ;
  fractions.open(prefix  + "fractions.dat");
  fractions << "f1\tf2\t" << endl;

}
double Simulation::av_score(const double pb)
{
  double cumul = 0 ;
  int size = 0 ; 
  
  for (auto i : traders )
    {
       if (*((double*) i->get_param() ) == pb ) 
       	{
       	  cumul += (double) 1 / (1 + exp(-(double)i->get_score()[0]/(double) i-> get_temp()));
       	  size++;
       	}
    }
  return (double) cumul / (double)size;
}
int Simulation::oneitt()
{
  for(std::vector<agentbase*>::iterator it=traders.begin();it !=traders.end();it++) 
    {
      (*it)->choice_market();
      (*it)->offer();
    }
  m1->newturn(); // initializing the values of m2 for a newturn
  m1->collect_offers(traders,Nagents); //< the market collect the offers made by the traders
  
  m1->trading_price() ; //< using it, it set the trading price
  m1->clean_orders(); //< then it delete the orders which do not match trading conditions0
  
  m1->reward_trader(traders); //< hence a reward is given to trader who traded succesfully
  // same for m2
  m2->newturn();
  m2->collect_offers(traders,Nagents);
  m2->trading_price() ;
  m2->clean_orders();
  m2->reward_trader(traders);
  return 0;
}

int Simulation::n_itterations(int n,int NSnapshot)
{
  // if one choose to compute the dyamic then creating the corresponding file
  for(int i=0;i<n;i++) {
    if (mode == "adaptive")
      {
	cout << "\r" <<(double) i * (double) 100 /(double) n  << "%" << flush ;
      }
    if (mode == "fbfs") 
      {
	cout << "\r" <<(double) i * (double) 100 /(double) n  << "%" << " f1  = " << av_score(.2) << "\t f2 = " << av_score(.8) << flush ;
	fractions << av_score(.2) << "\t" << av_score(.8) << endl;
      }
    oneitt();
    if ( Nsnap == NSnapshot)
      {
	ofstream simoutputall(prefix + "dits_snapshot" + to_string(SnapshotNumber) + ".dat");
	SnapshotNumber ++;
	if (mode == "fbfs") simoutputall << "score\tpb\tfparam\tlast_score\tcgscore\tlast_choice"<< endl ;
	for (auto ag : traders)
	  {
	    if (mode == "fbfs") simoutputall << ag->get_score()[0]<<"\t" << *((double*)ag->get_param()) << "\t"  << ag->get_forget()  << "\t" << ag->get_last_score() << "\t" << ag->get_doub("cgscore") <<"\t"<< ag->get_choice()<<  endl ;
	  }
	Nsnap = 0 ;
      }
    Nsnap ++;
    if ( mode == "fbfs")
    //computing the dynamic of the population
      {
	double scorep1(0), scorep2(0.);
	int incr1(0), incr2(0);
	for(auto ag : traders)
	  {
	    const double T = ag->get_doub("T");
	    if (*((double*)ag->get_param())==.2)
	      {
		scorep1 += sigmoid(ag->get_score()[0],T);
		incr1++;
	      }
	    if (*((double*)ag->get_param())==.8)
	      {
		scorep2 += sigmoid(ag->get_score()[0],T);
	    incr2++;
	      }
	  }
      }
  }
  return 0;
  
}
  
int Simulation::set_temp(double& T)
{
  for(auto ag : traders)
    {
      ag->set_temp(T);
    }
  return 0;
}


int Simulation::set_fparam(double& fp)
{
  for(auto ag : traders)
    {
      ag->set_fparam(fp);
    }
  return 0;
}

#endif
