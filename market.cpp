/* === MARKET_CPP ===
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
void market::newturn(){
  if (stepnumber != 4 ) cerr << "you reinitialise the market without having ending a turn" << endl ;
  bids.clear();
  ask.clear();
  do_not_trade.clear();
  tp = -1;
  stepnumber = 0;
  step += 1 ; 
}

market::market(int iden, double thetai,string fname){
  id = iden;
  theta = thetai;
  notrade = 0 ;
  tp = -1;
  stepnumber = 4;
  if (fname != "no")
    {
      ts = true;
      ts_file.open(fname);
      ts_file << "id \t nstep \t tp \t theta \t notrade \t vbids \t vasks " << endl;
    }
  else 
    {
      ts = false ;
    }
      step = 0 ; 
}


void market::collect_offers(vector<agentbase*>& agents, int Nagents)
{
  if (stepnumber != 0 ) // checking that you are using the market in the good order
    {
      cerr << "reinitialise the market before going to step 1. Run newturn()" << endl ;
      return ;
    }
  stepnumber = 1;
  double offer;
  prop temp;
  int action ;
  // other cases
  for(auto &it : agents){ // check all the traders proposal
    if(it->get_choice() == id){ // if that market is concerned start treatment of the datas
      temp.trader = (it)->get_id(); // copying the trader 
      offer = (it)->get_bid();
      action = (it)->get_action();
      if (action == 0 ) // case of an offer to buy
	{
	  temp.value = offer ;
	  bids.push_back(temp);
	}
      if (action ==1 ){ // case of an offer to sell
	temp.value = offer ;
	ask.push_back(temp);
      }
    }// end condition on the condition the trader wants to deal with this market
  }// end of the loop over all the traders
}

double market::trading_price(){
  if (stepnumber != 1 ) // checking that you are using the market in the good order
    {
      cerr << "you need to collect offers (collect_offers) before setting a trading price" << endl ;
      return 0 ;
    }
  ///
  stepnumber = 2;
  double sumbids(0),sumasks(0),mbids(0),masks(0);
  if (bids.empty())
    {
      //cout << "Message from market " <<  id << " : the bid vector is empty" << endl;
      notrade = 1;
      return 0 ;
    } // end if bids is empty
  if (ask.empty())
    {
      //cout << "Message from market " <<  id << " : the ask vector is empty" << endl;
      notrade = 1;
      return 0 ;
    } // end if⅜ ask is empty
  // calculating average bids and asks
  for (auto &it1 : bids){
    sumbids+=(it1).value;
  }// summing all the values of the bids
  butot = bids.size();
  mbids = sumbids/bids.size();
  for (auto &it1 : ask)
    {
      sumasks+=(it1).value;
    }// summing all the asks
  masks = sumasks/ask.size(); // average of the asks 
  nsellers =ask.size(); // number of buyers
  nbuyers= bids.size();// number of sellers 
  tp = masks+theta*(mbids-masks);
  astot = ask.size();
  //cout << "trading price" << tp << endl;
  return tp;
}

int market::clean_orders(){
  if (stepnumber != 2 ) // checking that you are using the market in the good order
    {
      cerr << "you need to collect offers (collect_offers) before setting a trading price" << endl ;
      return -1 ;
    }
   //////////////////////////////////////////////////////////////////////////////////////////////////////
  stepnumber = 3;
  // count number of valid bids
  int vbids(0),vasks(0),tempo;
  vector<prop> temp_bids,temp_ask; // list of valid bids and asks
  for (auto &it : bids) { // check the validity of each buyer offer
    if ((it).value > tp ) 
      {
      vbids++;
      temp_bids.push_back((it));
      }//end checking if bid is valid 
    else // tells the trader not to trade e
      {
	do_not_trade.push_back((it).trader);
      }
  }// end loop on bids higher than tp
  for (auto &it : ask) {// check the validity of each seller offer
    if ((it).value < tp ){
      vasks++;
      temp_ask.push_back((it));
    }//end checking if ask is valid
    else// don't let the trader trade
      {
	do_not_trade.push_back((it).trader);
      }
  }//end loop asks lower than tp
  validbids=vbids; // check ratio of accepted bids
  validasks=vasks; //  check ratio of accepted asks
  // we replace the list of bids and asks by the one of valid bids and asks
  bids = temp_bids;
  ask = temp_ask;
  if (ask.empty() || bids.empty()){ // when one of the list is empty, no trade can occure
    notrade = 1 ;
    return 0 ;
  }
  //> Computing the ratio abuyers/asellers
  abuysell = (double) vbids / (double) vasks;
  if (vasks > vbids){
    while (ask.size() != bids.size()){// randomly removing some of the traders asks
      tempo = myran.int32()%(ask.size()) ;
      do_not_trade.push_back(ask[tempo].trader);
      ask[tempo]=*(ask.end()-1);
      ask.erase(ask.end()-1 );
    } // end loop to remove randomly sellers
  }
  if (vbids > vasks){  
    while (ask.size() != bids.size()){// randomly removing some of the traders bids
      tempo = myran.int32()%(bids.size()) ;
      do_not_trade.push_back(bids[tempo].trader);
      bids[tempo]=*(bids.end()-1);
      bids.erase(bids.end()-1 );
    }// end loop to remove randomly buyers
  }
  return 0;
}


double market::reward_trader(vector<agentbase*>& agents){
   if (stepnumber != 3 ) // checking that you are using the market in the good order
    {
      cerr << "You are using the functions in the wrong order " << endl ;
      return -1 ;
    }
   stepnumber = 4;
   
    if (notrade==1) // loop if no trade occurs
     {
       // starting with bids
       for (std::vector<prop>::iterator it = bids.begin();it!=bids.end(); ++it) {
    	cout << (*it).trader << endl ;
    	agents[(*it).trader]->update_score(0);
    	agents[(*it).trader]->is_success(false);
       }
       // then do the same with asks
       for (std::vector<prop>::iterator it = ask.begin();it!=ask.end(); ++it) {
    	cout << (*it).trader << endl ;
   	agents[(*it).trader]->update_score(0);
    	agents[(*it).trader]->is_success(false);
       }
     }
   //for (std::vector<int>::iterator iter=do_not_trade.begin();iter!=do_not_trade.end();iter++) 
      //      cout << (*iter) << endl ;
   for (auto &iter : do_not_trade)
      {// tell top the agent who didn't trade that they failed 
	//     cout << (*iter) << endl ;
        agents[(iter)]->update_score(0);
        agents[(iter)]->is_success(false);
      }
   for (auto &it : bids) {
     //cout << (*it).trader << endl ;
     if ((it).value-tp < 0 ) cerr << (it).value-tp << endl ; // output in case of an error od the market  
     agents[(it).trader]->update_score((it).value-tp);
     agents[(it).trader]->is_success(true);
   }
   // then do the same with asks
   for (auto &it :  ask) {
     // cout << (*it).trader << endl ;
     if (tp- (it).value < 0) cerr << tp- (it).value << endl ; // output in case of an error od the market  
     agents[(it).trader]->update_score(tp- (it).value);
     agents[(it).trader]->is_success(true);
   }
   // writing the resutls in the timeserie file
   if ( ts)   ts_file << id << "\t" << step << "\t" << tp << "\t" << theta << "\t" << notrade << "\t" << validbids << "\t" << validasks << endl ;
   return 0;
}
// double market::meanscoreb(double offer) 
// {

// }
std::string market::display(){
  return to_string(abuysell) ;
}


