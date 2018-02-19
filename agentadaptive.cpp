

/*!
 @file agentadaptive.cpp
 * @author Robin NICOLE
 * @version 1.0
 * @brief agents with fixed preferences to buy and sell, they only update their preferences toward market 1 and 2
 */

#ifndef AGENTADAPTIVE_CPP
#include "agentadaptive.h"
#define AGENTADAPTIVE_CPP
  /*! 
    !@brief reset the parameters of the agents for a new turn
    !*/ 
void agentadaptive::newturn(){
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
void agentadaptive::init(const double& sig,const double& forget,const double& mb,const double& ms,const int& idee,const double& T,void* probab){
  sigma = sig;
  fparam = forget ;
  mean_buy = mb;
  mean_sell  = ms;
  id = idee ;
  choice = 1;
  action = 0;
  success=false;
  //   cout << id<< endl ;
  last_m=1;
  Temp = T;
  last_score = 0 ;
  AB1 = 0 ;
  AB2 = 0 ; 
  AS1 = 0 ; 
  AS2 = 0 ;
  cgscore = 0 ; 
}
int agentadaptive::init_score(const double& var)
{
  if (var <= 0)
    {
      delta = 0;
      cerr << "The entered variance is lowetr than 0" ; 
      return 1;
    }
  else
    {
      AB1 = rand_gaussian(var,0);
      AB2 = rand_gaussian(var,0);
      AS1 = rand_gaussian(var,0);
      AS2 = rand_gaussian(var,0);
      //delta = rand_gaussian(var,0);
      return 0 ; 
    }
}
int agentadaptive::choice_market(){
  
  double random = myran.doub();
  double ls1 = exp(AS1/Temp),lb1 = exp(AB1/Temp),ls2 = exp(AS2/Temp),lb2 = exp(AB2/Temp) ; 
  double norm  = ls1 + lb1 + ls2 + lb2 ; 
  lb1 = ls1 + lb1 ;
  ls2 = lb1 + ls2 ; 
  if (random < ls1 / norm )
    {
      action = 1 ; 
      choice = 1 ;
    }
  else if (random < lb1 / norm )
    {
      action = 0 ; 
      choice = 1 ;
    }
   else if (random < ls2 / norm )
    {
      action = 1 ; 
      choice = 2 ;
    }
   else 
     {
       action = 0 ;
       choice = 2 ; 
     }
  return action;
}
/*! 
  ! @brief get a random number between 0 and 1 and if it is lower than the proba of buying then buy else sell
  !*/ 

double agentadaptive::sigmoid(const double& x,const double& Temp) const {
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
double agentadaptive::offer(){
  double output(0) ;
  if (action==0)  output = rand_gaussian(1.,mean_buy);
  if (action==1)  output = rand_gaussian(1.,mean_sell);
  bid = output;
  return output;
}

double agentadaptive::rand_gaussian(const double& stdev,const double& mean) const {
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
double agentadaptive::get_doub(const string& name)
{
  if (name == "T") return Temp ; 
  else if (name == "cgscore") return cgscore ;
  else 
    {
      cerr << "Wrong name of variable to get" << endl ;
      return 0.; 
    }
}

int agentadaptive::change_parameter(const string& name,const double& val){
  

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
  
  else
    {// error case 
      cerr << "the name you entered doesn't match any variable" << endl; 
      return 1;
    }
}
/*!
 * @brief update the scores  
 */ 
double* agentadaptive::update_score(const double& score){  
  if (choice ==1 && action == 0)
    {
      AB1 = (1-fparam)*AB1 + score*fparam;
      AB2 = AB2*(1-alpha*fparam);
      AS1 = AS1*(1-alpha*fparam);
      AS2 = AS2*(1-alpha*fparam);
    }
  if (choice ==1 && action == 1)
    {
      AS1 = (1-fparam)*AS1 + score*fparam;
      AB2 = AB2*(1-alpha*fparam);
      AB1 = AB1*(1-alpha*fparam);
      AS2 = AS2*(1-alpha*fparam);
    }
  if (choice ==2 && action == 1)
    {
      AS2 = (1-fparam)*AS2 + score*fparam;
      AB2 = AB2*(1-alpha*fparam);
      AB1 = AB1*(1-alpha*fparam);
      AS1 = AS1*(1-alpha*fparam);
    }
  if (choice ==2 && action == 0)
    {
      AB2 = (1-fparam)*AB2+score*fparam;
      AS2 = AS2*(1-alpha*fparam);
      AS1 = AS1*(1-alpha*fparam);
      AB1 = AB1*(1-alpha*fparam);
    }
  last_score = score ;
  cgscore = cgscore * (1-fparam) + score * fparam ;
  return &delta ;
}

int agentadaptive::HeavisideTheta(const double& x) const 
{
  if (x > 0 ) return 1.;
  else if (x == 0)  return 0;
  else return 0 ; 
} 

void agentadaptive::print()
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
 
  cout <<"Temperature :"<< Temp<<endl;

}
#endif








