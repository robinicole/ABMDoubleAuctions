


/*!
 * \file agent.h
 * \author Robin NICOLE
 * \version 1.0
 * \brief header of agentb class which is the class from all the other classes derive 
 */
#ifndef INC_AGENT_H
#define INC_AGENT_H
/*!
 * \class agentbase
 * \brief virtual agent class from which all the other class derive. it allows to 
 * vary the agents behaviour by creating agent class which herit from agentbase
 */
class agentbase {
 public :   
  /*!
    * \brief return a string containing the name of the category of the agent
    */
  
  virtual string type_agent() const  = 0 ;
  virtual int get_id()const = 0;
  /*!
     * \brief return the forgetting parameter
     */
  virtual double get_forget() const = 0 ;
  virtual int change_parameter(const string& name,const double& val) = 0 ;
  /*!
   * \brief function to be used to get some parameters of the agents if needed
   */
   virtual void* get_param() =0;
  
   /*!
    * \brief return the score of the agent
    */
   virtual vector<double> get_score() const = 0 ;

   virtual void* get_misc() = 0 ; 
   /*!
    * \brief return the market chosen by the agent, it is used by the market to determin wther it is concerned or not by the offer
    * \return the id of the market chosen by the agent 
    */
   virtual int get_choice() const = 0 ;
   virtual double get_temp() const = 0 ;
   virtual double get_doub(const string& name) = 0 ;
   /*!
    * \brief return the action of the agent, it is used by the market to determin what is the kind of offer of the agent 
    * \return ain int corresponding to the action chosen by the agent 0 for buying and 1 for selling 
     */
  
   virtual int get_action() const  = 0 ; 
   /*! 
    * \return return the value of the bid of the agent, it is called by the market in collect_offers() 
    */
  
   virtual double get_bid() const = 0 ;
   /*!
    * \brief is used by the market to send the score he did to the agent, then the agent update its preferences
    * \param score : the benefit trader earned when he traded at the market 
    *
    */ 
   
   virtual double* update_score(const double& score) = 0 ;
    /*!
     * \brief used by the market to tell to the trader wther he failes or succeeded  in his last trade
     * \param successing : true if trade was succesful and false otherwise
     *
     */
   virtual void is_success(bool successing)=0; 
   
   virtual int choice_market() = 0 ;
   virtual void newturn() = 0 ;
   
   /*! 
    * \param sig variance of the bids and asks 
    * \param forget forgetting parameter 
    * \param mb average bid 
    * \param average ask  
    * \param Temp 
    * \param idee id of the agent 
    * \return void 
    */
   virtual void init(const double& sig,const double& forget,const double& mb,const double& ms,const int& idee,const double& T,void* probab) = 0 ;//{
   
   /* sigma = sig; */
   /* fparam = forget ; */
   /* mean_buy = mb; */
   /* mean_sell  = ms; */
   /* id = idee ; */
   /* choice = 1; */
   /* action = 0; */
   /* success=false; */
   /* last_m=1; */
   //}
   virtual int set_temp(const double& T) = 0 ; 
   virtual double offer() = 0 ;
   virtual int set_fparam(const double& fp) =0; 
   virtual int init_score(const double& var) = 0 ; 
   virtual void print() = 0 ;
   virtual double get_last_score() const = 0 ;;  
};
#endif
//Deffining class to displlay the agent 
#ifndef AGENTDISP_H
#define AGENTDISP_H


string mean_score(vector<agentbase*> &ags)
{
  stringstream ss;
  double scp1(0.), scp2(0.);
  int np1(0), np2(0);
  for (auto inc :  ags)
    {
      if (*((double*) inc ->get_param()) == .2) 
	{
	  scp1 += inc-> get_last_score() ; 
	  np1 ++ ;
	}
      if (*((double*) inc ->get_param()) == .8) 
	{
	  scp2 += inc-> get_last_score() ; 
	  np2 ++ ;
	}
    }
  ss << scp1/ (double) np1 << "\t" <<  scp2/ (double) np2 << endl ;
  return ss.str();
}


string mean4_score(vector<agentbase*> &ags)
{
  stringstream ss;
  double scp11(0.), scp21(0.),scp12(0.), scp22(0.);;
  int np11(0), np21(0),np12(0), np22(0);;
  for (auto inc :  ags)
    {
      if (*((double*) inc ->get_param()) == .2) 
	{
	  if (inc-> get_choice() == 1 )
	    {
	      scp11 += inc-> get_last_score() ; 
	      np11++;
	    }
	  else
	    {
	      scp12 += inc-> get_last_score() ; 
	      np12 ++ ;
	    }
	}
      if (*((double*) inc ->get_param()) == .8) 
	{
	  if (inc-> get_choice() == 2 )
	    {
	      scp21 += inc-> get_last_score() ; 
	      np21++;
	    }
	  else 
	    {
	      scp22 += inc-> get_last_score() ; 
	      np22 ++ ;
	    }
	}
    }
  ss << scp11/ (double) np11 << "\t" <<  scp12/ (double) np21 <<"\t" << scp21/ (double) np12 << "\t" <<  scp22/ (double) np22 << endl ;
  return ss.str();
}

string display_score(vector<agentbase*> &ags,int n)
{
  stringstream ss;
  ss << (ags)[n]->get_id() << "\t" << *((double*) (ags)[n]->get_param()) << "\t" << (ags)[n]->get_forget() << "\t" << (ags)[n]->get_last_score();
  return ss.str() ;
};	

#endif 
