
#include <boost/program_options.hpp>
#include "simulation.h"
#include "simulation.cpp"


namespace po = boost::program_options;
using namespace std;

/* Template of a simulation file */

// function which launch the simulation after having defined the agentx1

int main(int argc, char **argv){
  int Nagents(200000);
  vector<agentbase*> traders;
  double d_sigma = 1.,moy_buy = 11 ,moy_sell = 10,T = .2 ,forget = .1;
  double p1(0.2),p2(.8);
  double th1(0.3),th2 =0.7;
  int  iter(50000) ;//,np1(0),np2(0)  nm1p1(0),nm1p2(0),
  double alpha(1.);
  double fastfrac  = 0;
  string o_findist = "fidist.dat"; 
  string market_fich = "no";
  string mode = "fbfs";
  string prefix = "";
  int NSnapshot = -1;
    //double mean[2] = {0,0};
    //parametersing command line arguments
   po::options_description desc("Programm for the simulation of the multiagent with fbfs which have not fictious score, i.e. there alpha is 1");
  desc.add_options()
    ("help,h","Print help")
    ("nagents",po::value<int>(&Nagents)->default_value(20000),"Number of agents in the simulation")
    ("NSnapshot",po::value<int>(&NSnapshot)->default_value(-1),"Number of steps between each snapshot of the agents' scores distribution, negative numbers mean no snapshot")
    ("th1",po::value<double>(&th1)->default_value(.3), "Value of theta1")
    ("fastfrac",po::value<double>(&fastfrac)->default_value(0.),"Fraction of agents with fparam = 1.")
    ("th2",po::value<double>(&th2)->default_value(.7),"Value of theta2")
    ("nsteps",po::value<int>(&iter)->default_value(5000),"Number of steps in the simulation")
    ("fparam",po::value<double>(&forget)->default_value(.01),"Forgetting parameter of the agent")
    ("pb1",po::value<double>(&p1)->default_value(.2),"Probability of buying of population 1")
    ("pb2",po::value<double>(&p2)->default_value(.8),"Probability of buying of population 2")
    ("T",po::value<double>(&T)->default_value(.2),"Temperature")
    ("outdist",po::value<string>(&o_findist)->default_value("findist.dat"),"Output file for the distribution of the scores of the agents in the final state. If set to not then no output file is created")
    ("mbuy",po::value<double>(&moy_buy)->default_value(11),"Average bids")
    ("msell",po::value<double>(&moy_sell)->default_value(10),"Average asks")
    ("sigma",po::value<double>(&d_sigma)->default_value(1.),"Variance of the bid and asks distribution")
    ("alpha",po::value<double>(&alpha)->default_value(1.),"Alpha parameter for the fictious scores")
    ("mar_ts",po::value<string>(&market_fich)->default_value("marketts.dat"),"Prefix of the file where the market time serie will be stored. If the name is no then no ts file is created")
    ("mode",po::value<string>(&mode)->default_value("fbfs"),"Type of agents in the simulation. It can be either \n - fbfs for fixewd buy sell agents or \n - adaptive for agents with adaptive buy/sell preferences")
    ("prefix",po::value<string>(&prefix)->default_value(""),"Prefixe before the name of the files of the simulations"); 
  po::variables_map vm;
  try
    {
  po::store(po::parse_command_line(argc, argv, desc),
	    vm); // can throw
  if (vm.count("help"))
    {
      cout << desc << endl ;
      return 0 ;
    }
  po::notify(vm);
    }
  catch(po::error& e)
    {
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
      std::cerr << desc << std::endl;
      return 1;
    }
  // END parsing command line argument
  cout << "============================" << endl 
       << "Multi-agents simulation of the double auction model" << endl 
       << "with 2 markets" << endl 
       << "============================" << endl << endl << endl;
  cout << "p1 = " << p1 << endl
       << "p2 = " << p2 << endl
       << "th1 = " << th1 <<  endl 
       << "th2 = " << th2 << endl
       << "r = " << forget << endl
       << "Temperature = " << T << endl
       << "Fraction of agents with r = 1 : " << fastfrac << endl
       << "fictitious play coefficient (alpha) = " << alpha << endl
       << "<b> = " << moy_buy << endl 
       << "<a> = " << moy_sell << endl  
       << "Offer variance = " << d_sigma << endl 
       << "Type of traders = " << mode << endl
       << "=======================================" << endl 
       << "Itterations between two snapshots = " << NSnapshot << endl 
       << "niterations = " << iter << endl
       << "number of agents : " << Nagents << endl ;  
  cout << "=======================================" << endl ;
  cout << "Percentage of the simulation remaining" << endl ;
  ofstream simoutputall;
  ofstream mfinstat(prefix + "mfinstat.dat",ios::app); //  output for the saturation of the market
  mfinstat << "p1\t p2 \t th1 \t th2 \t fparam \t T \t fastfrac \t m1bos \t m2bos" << endl ;  // writing the header of the file
  if (o_findist != "no")
    {
      simoutputall.open(prefix + o_findist);
    }
  // name of the string output
  string ts_m1(prefix + "id_1_" + market_fich), ts_m2(prefix + "id_2_" + market_fich); 
  if (market_fich =="no") {
    ts_m2= "no";
    ts_m2= "no";
  }
  market m1(1,th1,ts_m1), m2(2,th2,ts_m2);
  traders.clear();
  
  // Initialisatio depending on the mode chosen
  if (mode == "fbfs")
    {
      int x1((Nagents*fastfrac)/2.),x2(Nagents*fastfrac),x3((Nagents*(1+fastfrac))/2.);
      cout << " number of the fast agents :" << x1 << " ; "<< x2 << " ; "<< x3 <<  endl;
      for(int i=0;i<Nagents;i++){
	traders.push_back(new agentfbfsalpha);
	if (i < x1) traders[i]->init(d_sigma,1.,moy_buy,moy_sell,i,T,&p1);
	if (i >= x1 && i < x2) traders[i]->init(d_sigma,1.,moy_buy,moy_sell,i,T,&p2);
	if (i >= x2&& i < x3) traders[i]->init(d_sigma,forget,moy_buy,moy_sell,i,T,&p1);
	if (i >= x3 ) traders[i]->init(d_sigma,forget,moy_buy,moy_sell,i,T,&p2);
	traders[i]->change_parameter("alpha",alpha);
	traders[i]->init_score((double)forget); // qunch disorder of the initial average score
      }
    }
  else if (mode == "adaptive")
    {
      for(int i=0;i<Nagents;i++){
	traders.push_back(new agentadaptive);
	if (i < Nagents *fastfrac) traders[i]->init(d_sigma,1.,moy_buy,moy_sell,i,T,&p1);
	else  traders[i]->init(d_sigma,forget,moy_buy,moy_sell,i,T,&p1);
	traders[i]->change_parameter("alpha",alpha);
	traders[i]->init_score((double)forget); // qunch disorder of the initial average score
      }
    }
  Simulation simu(m1,m2,traders,mode,prefix);
  cout << NSnapshot << endl ;
  simu.n_itterations(iter, NSnapshot);
  // computing the market saturation 
  mfinstat << p1 << "\t" << p2 << "\t" << th1 << "\t" << th2 << "\t" 
	   << forget << "\t" << T << "\t" << fastfrac << "\t" << m1.display()
	   << "\t" << m2.display() << endl ;
  // final distribution of the agents scores
  if (mode == "adaptive")simoutputall << "AB1\tAS1\tAB2\tAS2"<< "\t" << "fparam" << "\t" << "T" << "\t" << "th1" << "\t" << "th2" << "\t" << "fastfrac" << "\t last_score" <<"\t"<< "cgscore"<< endl ;
  if (mode == "fbfs") simoutputall << "score" << "\t" << "pb" << "\t" << "fparam" << "\t" << "T" << "\t" << "th1" << "\t" << "th2" << "\t" << "fastfrac" << "\t last_score" <<"\t"<< "cgscore"<< "\t"<< "last_choice"<< endl ;
  
  for (auto ag : traders)
    {
      if (mode == "adaptive") simoutputall << ag->get_score()[0]<<"\t" <<ag->get_score()[1]<<"\t" <<ag->get_score()[2]<<"\t"<<  ag->get_score()[3]<<"\t"   << ag->get_forget()  << "\t" << T << "\t" << th1 << "\t" << th2  << "\t"<< fastfrac << "\t" << ag->get_last_score() << "\t" << ag->get_doub("cgscore") <<  endl ;
      if (mode == "fbfs") simoutputall << ag->get_score()[0]<<"\t" << *((double*)ag->get_param()) << "\t"  << ag->get_forget()  << "\t" << T << "\t" << th1 << "\t" << th2  << "\t"<< fastfrac << "\t" << ag->get_last_score() << "\t" << ag->get_doub("cgscore") <<"\t"<< ag->get_choice()<<  endl ;
    }
  
  return 0;
}

