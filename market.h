/* === MARKET_H === 
 * Instruction 
 * =============
 1) Run newturn to initialise the market 
  => function : int newturn 
 2) after all the agents have set up their bids and the market at which they want to trade collect the offers
  => function : void collect_offers(vector<agentbase*> , int)
 3) Then set the trading price 
  => function double trading_price()
 4) Reject all the non valid bids and asks and also rejecct randomly bids from the longest list of orders 
  => function int clean_orders()
 5) Reward all the traders by giving them their score 
  => funciton : reward_trader(vector<agentbase*>& agents)
 */
#include "agent.h"
#include <iostream>
#include <list>
//#include <map>
using namespace std;

// You just  need to set a global random number generator Ran(0)
/*! @struct prop
    @brief structure which contain the id of a trader and the value of its proposal. 
*/
struct prop {
  /*!
   * @brief id of the trader
   */
  int trader; 
  /*!
   * @brief value of its proposal (do not say wether its a bid or an ask)
   */
  double value;  
};

#ifndef MARKET_H
#define MARKET_H
/* 
 * Instruction 
 * =============
 1) Run newturn to initialise the market 
  => function : int newturn 
 2) after all the agents have set up their bids and the market at which they want to trade collect the offers
  => function : void collect_offers(vector<agentbase*> , int)
 3) Then set the trading price 
  => function double trading_price()
 4) Reject all the non valid bids and asks and also rejecct randomly bids from the longest list of orders 
  => function int clean_orders()
 5) Reward all the traders by giving them their score 
  => funciton : reward_trader(vector<agentbase*>& agents)
 */
class market{
/*!
  \class modelise a market
  \brief market class which interact with the agents and allow  them to trade together
 */
 public:

  market()
    {
    }
  /*! 
   * @brief constructor
   * @param iden : number which identify the market
   * @param thetai : preference of the market toward buyer and seller
   * */ 
  market(int iden,double thetai,string fname); 
  /*! 
   * @brief receiving a vector containing pointers to some traders, collect 
    offers look at the offers made by all traders and if they concern the id of the market
    they call the mener function of traders get_bid() to get the ammount of the offer and also get_action() to see 
    wether it is an ask or a bid then they are stored into the vectors<prop bids and asks>
    * @param traders : vector containing all the traders
    * @param Nagents : number of agents in the vector traders
    */ 
  void collect_offers(vector<agentbase*>& traders,int Nagents); // step 1
  /*! @brief set the trading price according to the average of bids and asks with the following formula 
    tp = <a> + theta (<b>-<a>)
    !*/
  double trading_price(); 
  /*! @brief - look at the bid and asks vectors and if the offer is not valid regarding the trading price then they 
   *  are stored in a vector temp_bids or temp_asks which will contain only the valid bids
   *  then if there is more buyer (n) than sellers (m) then among the n buyers m are selected to trade, the other 
   *  will not be allowed to trade. The vector bids and asks is updated according to the results
     !*/ 
  int clean_orders(); 
  /*!
   *  @brief according to the results of the trade telle to the agent wether they succeded or not using the member 
   *  function of agent is_success and give them the annount of their score : usign the function 
   *  update_score(abs(bid-tp)) 
   * @param agent pointer to the agents vector 
   *  */
  double reward_trader(vector<agentbase*>& agent); // step 4
  /*!
   *  @brief reinitialise the market for a new trading session
   * */
  void newturn(); 
  string display(); 
  //std::map<string,double> informations();
 public: // TODO Change tpublic to protected 
  int stepnumber ;   /*!> step number to check that you use the market in the good order */ 
  int step ; 
  int id; /*! id of the market*/ 
  double theta ; /*!> preference of the market  tp = <a> + theta (<b>-<a>) */
  double tp; /*!> trading price */
  vector<prop> bids,ask; /*!> listnof bids and asks sent by the trader*/ 
  int notrade; // 0 if there is some trades , 1 otherwise
  vector<int> do_not_trade ; // list of traders who did not trade 
  int butot, astot; // total size of the list of bids and ask at the beginning 
  int validbids;
  int validasks;
  double abuysell; // ratio of accepted buyers over acceted sellers
  int nbuyers;
  int nsellers;
  ofstream ts_file;
  bool ts ;
};
#endif
