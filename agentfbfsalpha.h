
/*!
 * @file agentfbfs.h
 * @author Robin NICOLE
 * @version 1.0
 * @brief header of the agent class. 
 * The agent class simulate the behavious of a trader.
 */
#ifndef NR_NR3
#define NR_NR3
#include "nr/nr3.h"
#endif
#ifndef NR_RAN
#define NR_RAN
#include "nr/ran.h"
#endif
#include <fstream>
#ifndef I_AGENTFBFSALPHA_H
#define I_AGENTFBFSALPHA_H
#include "agent.h"
#include <string>
/*!
 * \class agentfbfs 
 * \brief implement agents with fixed buy and sell preferences, all the comments are in the agentfbfs.cpp file
 */
class agentfbfsalpha : public agentbase{
 public:
  double alpha = 1.;
  int choice;/*!< choice of the agent */
  int action ;/*!< action of the agent 0 for buy 1 for sell */
  double sigma ;/*!< variance of agent's bid  */
  double delta; /*! difference between the scores Ai at market i : A2-A1 */
  double fparam ;/*!< forgetting parameter of the agent */
  double bid ;/*!< value of the bid of the agent */
  double mean_buy;/*!<average bid          */
  double mean_sell;/*!< average ask */
  int id ;/*!< id of the agent  */
  bool success ; /*!< true if the last trade was successfull */
  double last_score;
  int last_m;/*!< last market chosen by the agent */
  double proba; /*!< probability of buying*/
  double Temp;
  double A1,A2;
  double cgscore ;
  // obtaining the agent parameter 
  double get_last_score()const override
  {
    return last_score;
  };
  string type_agent() const override {
    return "fbfs";
  } ;
  double get_proba()const {
  /*!
   * @return proba of buying
   */
    return proba;};
  int get_id() const override {return id;};
  double get_temp() const override {return Temp;};
  double get_forget()const  override 
  {
    /*!
     * @return forgetting parameter
     */
    return fparam;};
 
  void* get_param() override {
    return &proba;
  };
  void* get_misc() override {
    return &last_score;  
  };
  vector<double> get_score() const override {
    
    /*!
     * @return score
     */
    return {A1-A2};};
  double get_doub(const string& name);
  int get_choice() const override {
    /*!
     * @return last_choice \in 1,2
     */
    return choice;};
  
  int get_action() const override {
    /*!
     * @return last_action 0 for buying 1 for selling
     */
    return action;};
  
  double get_bid() const override{
    /*!
     * @return last_bid
     */
    return bid;};
  //// End reading the agent parameter
     // for the case when T = 0 
  int set_temp(const double& T) override 
  {
    Temp = T;
    return 0;
    
  };
  int set_fparam(const double& fp) override 
  {
    fparam  = fp;
    return 0;
  };
  int change_parameter(const string& name,const double& val) override ;
  // Changing parameters of the agent 
  void newturn() override; // reinitialise agent for a newturn 
  void init(const double& sig,const double& forget,const double& mb,const double& ms,const int& idee,const double& T,void* probab) override ; // initialise the agent 
  // END Changing the parameter of an agent
  //void init() override ;
  
  int choose_action() ; // choosing wether to buy or sell ;
  int choice_market() override;
  double sigmoid(const double& x,const double& Temp) const ;
  double offer() override ;
  double rand_gaussian(const double& stdev,const double& mean) const ;
  double* update_score(const double& score) override ;
  void is_success(bool successing) override{success= successing;};
  int HeavisideTheta (const double& x) const ;
  int init_score(const double& var) override ;
  void print() override;
};
#endif  

