/*
 * File:   main.cpp
 * Author: MAN
 *
 * Created on October 3, 2014, 3:56 PM
 */

// standard includes
#include <iostream>


// the general include for eo
#include <eo>

using namespace std;

#include "data_collection.h"
#include "individual.h"
#include "initialization.h"
#include "fitness_evaluation.h"
#include "tweak.h"
#include "quad_crossover.h"
#include "hill_climbing.h"
#include "genetic_algorithm.h"

typedef eoMinimizingFitness MyFitT;
typedef Mapping<MyFitT> Indi;

/*
 *
 */
void main_function(int argc, char **argv)
{


    //individual file read/write
    //ofstream indiSave(io["indi"].c_str());
    //ifstream indiLoad(io["indiLoad"].c_str());

    //fitness evaluators
    MappingEvalFunc<Indi> eval;

    //initial solution
    Indi initialSolution;
    fairRandom(initialSolution);
    //indiLoad >> initialSolution ;
    eval(initialSolution);

    //tweaks
    OptimisticTweak<Indi> opTweak;
    UniformMonCrossOver<Indi> exploit;
    Mutation<Indi> explore;
    GuidedMutation<Indi> gExplore;
    eoPropCombinedMonOp<Indi> tweak(opTweak, 0.5);
    tweak.add(exploit, 0.5, true);
   // tweak.add(gExplore, 0.2, true);
   // tweak.add(explore, 0.1, true);
    //HybridTweak<Indi> hybridTweak(exploit, explore);

    //THE ALGORITHM
    HillClimbing<Indi> hc(exploit, eval, param["maxGen"]);
    HybridHillClimbing<Indi> hcga(exploit, eval, param["maxGen"]);
    //SteepestAscent<Indi> sa(tweak,eval);
    //SteepestAscentWithReplacement<Indi> sar(tweak,eval);
    SimulatedAnnealing<Indi> simAnn(tweak,eval,param["maxGen"]);
    //NewIdea<Indi> ni(exploit,explore, eval, param["maxGen"]);

    //initial print
    //cout << "Initial Solution:" << endl;
    //initialSolution.print();

    //In the name of Allah
    hcga(initialSolution);

    //final print
    cout << "Final" << endl << initialSolution << endl;
    double speedUp = SERIAL_LOAD / initialSolution.fitness();
    cout << "Serial load " << SERIAL_LOAD << endl;
    cout << "speedup " << speedUp << endl;
    //cout << "Final Solution:" << endl ;
    //initialSolution.print();
    //indiSave << initialSolution;

}

void ga_run()
{
    //fitness evaluators
    MappingEvalFunc<Indi> eval;

    //population
    eoPop<Indi> pop;

    //selection
    eoDetTournamentSelect<Indi> selectOne(param["tFit"]);
    double perc = (param["popSize"] - param["elite"]) / param["popSize"];
    eoSelectPerc<Indi> select(selectOne, perc);

    //crossover
    MappingQuadCrossover<Indi> xover;

    //mutations
    //UniformMonCrossOver<Indi> exploit;
    GuidedMutation<Indi> explore;
    //eoPropCombinedMonOp<Indi> tweak(exploit, param["rExploit"]);
    //tweak.add(explore, 1 - param["rExploit"], true);


    //termination
    eoGenContinue<Indi> continuator((time_t) param["maxGen"]);
    //CHECKPOINT
    eoCheckPoint<Indi> checkpoint(continuator);
    // Create a counter parameter
    eoValueParam<unsigned> generationCounter(0, "Generation");
    // Create an incrementor (sub-class of eoUpdater). Note that the
    // parameter's value is passed by reference,
    // so every time the incrementer is updated (every generation),
    // the data in generationCounter will change.
    eoIncrementor<unsigned> increment(generationCounter.value());
    // Add it to the checkpoint,
    // so the counter is updated (here, incremented) every generation
    checkpoint.add(increment);
    // now some statistics on the population:
    // Best fitness in population
    eoBestFitnessStat<Indi> bestStat;
    // Second moment stats: average and stdev
    eoSecondMomentStats<Indi> SecondStat;
    // Add them to the checkpoint to get them called at the appropriate time
    checkpoint.add(bestStat);
    checkpoint.add(SecondStat);
    // The Stdout monitor will print parameters to the screen ...
    eoStdoutMonitor monitor(false);
    // when called by the checkpoint (i.e. at every generation)
    checkpoint.add(monitor);
    // the monitor will output a series of parameters: add them
    monitor.add(generationCounter);
    monitor.add(bestStat);
    monitor.add(SecondStat);

    //THE ALGORITHM
    //HybridGAchild <Indi> ga(select, xover, explore, eval, checkpoint, fairRandom,param["steadyGen"],param["hcIter"]);
    GAforHC <Indi> ga(select, xover, explore, eval,param["hcIter"]);
    
    //Bismillah
    
    Indi initialSolution;
    fairRandom(initialSolution);
    eval(initialSolution);
    for(int i =0; i<param["maxGen"];i++)
    {
        ga.setInitialSol(initialSolution);
        ga(pop);
        initialSolution = ga.Best;
    }

    //cout<<pop.best_element().fitness()<<endl<<pop.worse_element().fitness()<<endl;


}

int main(int argc, char** argv)
{
    gatherAllInfo();
    //param["seed"] = 1413089664; //1413089664 or time(0)
    //cout << "seed=" << (uint32_t) param["seed"] << endl;
    rng.reseed((uint32_t) param["seed"]);
    if (io["algo"].compare("pop") == 0)
    {
        ga_run();
    }
    else
    {
        main_function(argc, argv);
    }
    ////       int myints[] = {16, 2, 50, 29,0};
    ////       vector<int> v(myints, myints + sizeof (myints) / sizeof (int));
    ////       for(int i =0 ; i<20 ;i++)
    ////       {
    ////           cout<< rng.roulette_wheel(v) << endl;
    ////       }
    //    //    printVec(fifth);
    //    //    vector<unsigned> actorVec(ACTOR_LIST, ACTOR_LIST + ACTORS);
    //    //    printVec(actorVec);
    //    //    random_shuffle ( actorVec.begin(), actorVec.end(), myRandom);
    //    //    printVec(actorVec);
    //    //fullyRandom(a);
    //    //cout<<endl<<a;
    //    param["seed"] = time(0);
    //    cout << "seed=" << (uint32_t) param["seed"] << endl;
    //    rng.reseed(param["seed"]);
    //    Indi a;
    //    fairRandom(a);
    //    MappingEvalFunc<Indi> eval;
    //    eval(a);
    //    cout << endl << a << endl;
    //    //a.assignActor(0, 1);
    //    UniformMonCrossOver<Indi> uniCross;
    //    uniCross(a);
    //    a.invalidate();
    //    eval(a);
    //    cout << endl << a << endl;
    //    Mutation<Indi> m;
    //    eoInvalidateMonOp<Indi> mut(m);
    //    cout << mut(a) << endl;
    //    eval(a);
    //    cout << endl << a << endl;


    //main_function(argc, argv);

    //printAdjInfo();

    return 0;
}

