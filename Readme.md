# Code used in "Dynamical selection of Nash equilibria using Experience Weighted Attraction Learning"
This code is a supplementary material to the article Dynamical selection of Nash equilibria using Experience Weighted Attraction Learning" (see the preprint [here](https://arxiv.org/abs/1706.09763). 
## Installation
- To compile this code, you need to install the boost library with `boost_program_options`.
- Once it is done, you can compile this code with the command `make`.

## Options

  ```
  -h [ --help ]                    Print help
  --nagents arg (=20000)           Number of agents in the simulation
  --NSnapshot arg (=-1)            Number of steps between each snapshot of the
                                   agents distributions, negative numbers mean
                                   no snapshot
  --th1 arg (=0.29999999999999999) Value of theta1
  --fastfrac arg (=0)              Fraction of agents with fparam = 1.
  --th2 arg (=0.69999999999999996) Value of theta2
  --nsteps arg (=5000)             Number of steps in the simulation
  --fparam arg (=0.01)             Forgetting parameter of the agent (r in the paper)
  --pb1 arg (=0.20000000000000001) Probability of buying of population 1
  --pb2 arg (=0.80000000000000004) Probability of buying of population 2
  --T arg (=0.20000000000000001)   Temperature (1/beta in the paper)
  --outdist arg (=findist.dat)     File for the distirbution of the agents in
                                   the final state if set 'not' then no output
                                   file is created
  --mbuy arg (=11)                 Average bids
  --msell arg (=10)                Average asks
  --sigma arg (=1)                 Variance of the bid and asks distribution
  --alpha arg (=1)                 Alpha parameter for the fictious scores
  --mar_ts arg (=marketts.dat)     Prefix of the file where the
                                   market time serie will be stored. If the parameter 
								   is no, no ts file is created
  --mode arg (=fbfs)               Type of agents in the simulation. It can be
                                   either
                                    - fbfs for fixewd buy sell agents or
                                    - adaptive for agents with adaptive
                                   buy/sell preferences
  --prefix arg                     Prefixe before the name of the files of
                                   the simulations
```
The meaning of each of those parameters is explained in the article "Dynamical selection of Nash equilibria using Experience Weighted Attraction Learning" to be published in PLOS One or in the preprint [here](https://arxiv.org/abs/1706.09763))
