

/*!
 @file agentfbfsalpha.cpp
 * @author Robin NICOLE
 * @version 1.0
 * @brief agents with fixed preferences to buy and sell, they only update their preferences toward market 1 and 2
 */

#ifndef AGENTFBFSALPHA_CPP
#include "agentfbfsalpha.h"
#define AGENTFBFSALPHA_CPP
  /*! 
    !@brief reset the parameters of the agents for a new turn
    !*/ 
void agentfbfsalpha::newturn(){
  bid = 0;
}
/*! 
 * @brief initialisation of the agent
 * @param sig : variance of the agent
 * @param forget : forgetting parameter of the agent
 * @param mb : average bid of the agent
 * @param ms : average seling price of the agent
 * @param idee : a number to identify the agent
 * @param Temp : temperatue of the agent 
 * @param probab : pointer to a double which is the probability of buying of the agent
 !*/ 
void agentfbfsalpha::init(const double& sig,const double& forget,const double& mb,const double& ms,const int& idee,const double& T,void* probab){
  sigma = sig;
  fparam = forget ;
  mean_buy = mb;
  mean_sell  = ms;
  id = idee ;
  choice = 1;
  action = 0;
  success=false;
  proba=*((double*)probab);
  //   cout << id<< endl ;
  last_m=1;
  Temp = T;
  last_score = 0 ;
  A1 = 0 ;
  A2 = 0 ; 
  delta = 0  ;
  cgscore = 0 ; 
}
int agentfbfsalpha::init_score(const double& var)
{
  if (var <= 0)
    {
      delta = 0;
      cerr << "The entered variance is lowetr than 0" ; 
      return 1;
    }
  else
    {
      A1 = rand_gaussian(var,0);
      A2 = rand_gaussian(var,0);
      delta = rand_gaussian(var,0);
      return 0 ; 
    }
}
int agentfbfsalpha::choice_market(){
  
  double random = myran.doub();
  if (random < sigmoid((double)delta,Temp)) choice = 1;
  else choice = 2 ;
  action = choose_action();
  return choice;
}
/*! 
  ! @brief get a random number between 0 and 1 and if it is lower than the proba of buying then buy else sell
  !*/ 
int agentfbfsalpha::choose_action(){
  double rand = myran.doub();
  if( rand < proba)
    {
      action = 0;
      return 0;
    }
  else 
    {
      action = 1;
     return 1;
    }
}

/*! 
 * @brief sigmoid(delta) is the probability to buy at market 1
  !*/ 
double agentfbfsalpha::sigmoid(const double& x,const double& Temp) const {
  if (Temp > 0 )
    return (double)1/((double)1+(double) exp(-x/Temp));
  else
    {
      if (x > 0) return 1.;
      else return 0. ;
    }
}
 /*!
  * @brief return the agent offer 
  */ 
double agentfbfsalpha::offer(){
  double output(0) ;
  if (action==0)  output = rand_gaussian(1.,mean_buy);
  if (action==1)  output = rand_gaussian(1.,mean_sell);
  bid = output;
  return output;
}

double agentfbfsalpha::rand_gaussian(const double& stdev,const double& mean) const {
  double v1,v2,s;
  do {
    v1=2.0*myran.doub()-1.;
    v2=2.0*myran.doub()-1.;
    s = v1*v1 + v2*v2;
  } while ( s >= 1.0 );

  if (s == 0.0)
    return 0.0;
  else
    return (mean+stdev*v1*sqrt(-2.0 * log(s) / s));
}
double agentfbfsalpha::get_doub(const string& name) // const override
{
  if (name == "T") return Temp ; 
  else if (name == "cgscore") return cgscore ;
  else 
    {
      cerr << "Wrong name of variable to get" << endl ;
      return 0.; 
    }
}

int agentfbfsalpha::change_parameter(const string& name,const double& val){
  

  if (name == "alpha" )   // Changing variable alpha
    {
      if (val > 1. || val < 0.) 
	{
	  cerr << "the value of alpha must be between 0 and 1" << endl ;
	  return 1;
	}
      alpha = val;
      return 0;
    }
  else if (name == "score" ) 
    {
      delta = val;
      A1 = val/(double) 2.;
      A2 = -val/(double) 2.;
      return 0 ;
    }
  else if (name  == "pm1")
    {
      const double valtmp = - Temp  * log((double) 1 / (double) val - 1. ) ;
      delta = valtmp;
      A1 = valtmp/(double) 2.;
      A2 = -valtmp/(double) 2.;
      return 0;
    }
  
  else
    {// error case 
      cerr << "the name you entered doesn't match any variable" << endl; 
      return 1;
    }
}
/*!
 * @brief update the scores  
 */ 
double* agentfbfsalpha::update_score(const double& score){  
  if (choice ==1 )
    {
      A1 = (1-fparam)*A1+score*fparam;
      A2 = A2*(1-alpha*fparam);
    }
  if (choice == 2) 
    {
      A2 = (1-fparam)*A2+score*fparam;
      A1 = (1-alpha* fparam)*A1;
    }
  delta = (A1 - A2);
  last_score = score ;
  cgscore = cgscore * (1-fparam) + score * fparam ;
  return &delta ;
}

int agentfbfsalpha::HeavisideTheta(const double& x) const 
{
  if (x > 0 ) return 1.;
  else if (x == 0)  return 0;
  else return 0 ; 
} 

void agentfbfsalpha::print()
{
  cout <<"alpha :"<< alpha<<endl;
  cout <<"choice :"<< choice<<endl;/*!< choice of the agent */
  cout <<"action :"<< action<<endl; /*!< action of the agent 0 for buy 1 for sell */
  cout <<"sigma :"<< sigma<<endl; /*!< variance of agent's bid  */
  cout <<"delta :"<< delta<<endl; /*! difference between the scores Ai at market i : A2-A1 */
  cout <<"fparam :"<< fparam<<endl; /*!< forgetting parameter of the agent */
  cout <<"bid :"<< bid<<endl; /*!< value of the bid of the agent */
  cout <<"mean_buy :"<< mean_buy<<endl;/*!<average bid          */
  cout <<"mean_sell :"<< mean_sell<<endl;/*!< average ask */
  cout <<"id :"<< id<<endl; /*!< id of the agent  */
  cout <<"success :"<< success<<endl; ; /*!< true if the last trade was successfull */
  cout <<"last_score :"<< last_score<<endl;
  cout <<"last_m :"<< last_m<<endl;/*!< last market chosen by the agent */
  cout <<"probas :"<< proba<<endl; /*!< probability of buying*/
  cout <<"Temperature :"<< Temp<<endl;
  cout <<"A1 :"<< A1<<endl;
  cout << "A2 :"<<A2<<endl;
}
#endif








