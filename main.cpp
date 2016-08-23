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
#include "timer.h"
#include "hill_climbing.h"
#include "genetic_algorithm.h"

typedef eoMinimizingFitness MyFitT;
typedef Mapping<MyFitT> Indi;

template <class EOT>
long int MappingEvalFunc<EOT>::fitEvalCount = 0;
/*
 *
 */
void hc(string currentData)
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

    eoPropCombinedMonOp<Indi> tweak(opTweak, 0.2);
    tweak.add(exploit, 0.1, true);
    tweak.add(gExplore, 0.3, true);
    tweak.add(explore, 0.4, true);

    eoPropCombinedMonOp<Indi> revTweak(opTweak, 0.3);
    revTweak.add(exploit, 0.4, true);
    revTweak.add(gExplore, 0.2, true);
    revTweak.add(explore, 0.1, true);

    map<string, eoMonOp<Indi>*> tweakList;
    tweakList["exploit"] = &exploit;
    tweakList["explore"] = &explore;
    tweakList["opTweak"] = &opTweak;
    tweakList["gExplore"] = &gExplore;
    tweakList["tweak"] = &tweak;
    tweakList["revTweak"] = &revTweak;
    
    if(tweakList.count(io["tweak"]) == 0)
    {
        cout << "Specify tweak correctly in io.txt" ;
        exit(1);
    }


    //THE ALGORITHM
    HillClimbing<Indi> hc(*tweakList[io["tweak"]], eval, param["hc_"+currentData], true, param["maxTime"]);
    //SteepestAscent<Indi> sa(tweak,eval);
    //SteepestAscentWithReplacement<Indi> sar(tweak,eval);
    //SimulatedAnnealing<Indi> simAnn(tweak, eval, param["maxGen"]);
    //NewIdea<Indi> ni(exploit,explore, eval, param["maxGen"]);

    //initial print
    //cout << "Initial Solution:" << endl;
    //initialSolution.print();

    //In the name of Allah
    hc(initialSolution);

    //final print
    //cout << "Final" << endl << initialSolution << endl;
    double speedUp = SERIAL_LOAD / initialSolution.fitness();
    cout << "Serial load " << SERIAL_LOAD << endl;
    cout << "speedup " << speedUp << endl;
    //cout << "Final Solution:" << endl ;
    //initialSolution.print();
    //indiSave << initialSolution;
    globalFinalResult << SERIAL_LOAD / initialSolution.fitness() << " " << hc.totalTime << endl;

}

void hcga(string currentData)
{
    MappingEvalFunc<Indi> eval;
    //initial solution
    Indi initialSolution;
    fairRandom(initialSolution);
    eval(initialSolution);
    //tweaks
    UniformMonCrossOver<Indi> exploit;
    //THE ALGORITHM
    HybridHillClimbing<Indi> hcga(exploit, eval, param["hcga_"+currentData]);
    //In the name of Allah
    hcga(initialSolution);
    //final print
    double speedUp = SERIAL_LOAD / initialSolution.fitness();
    cout << "Serial load " << SERIAL_LOAD << endl;
    cout << "speedup " << speedUp << endl;
    //cout << "Final Solution:" << endl ;
    //initialSolution.print();
    //indiSave << initialSolution;
    globalFinalResult << SERIAL_LOAD / initialSolution.fitness() << " " << hcga.totalTime << endl;

}

void ga(string currentData)
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
    OptimisticTweak<Indi> opTweak;
    UniformMonCrossOver<Indi> exploit;
    Mutation<Indi> explore;
    GuidedMutation<Indi> gExplore;

    eoPropCombinedMonOp<Indi> tweak(opTweak, 0.2);
    tweak.add(exploit, 0.1, true);
    tweak.add(gExplore, 0.3, true);
    tweak.add(explore, 0.4, true);

    eoPropCombinedMonOp<Indi> revTweak(opTweak, 0.3);
    revTweak.add(exploit, 0.4, true);
    revTweak.add(gExplore, 0.2, true);
    revTweak.add(explore, 0.1, true);

    map<string, eoMonOp<Indi>*> tweakList;
    tweakList["exploit"] = &exploit;
    tweakList["explore"] = &explore;
    tweakList["opTweak"] = &opTweak;
    tweakList["gExplore"] = &gExplore;
    tweakList["tweak"] = &tweak;
    tweakList["revTweak"] = &revTweak;
    
    if(tweakList.count(io["tweak"]) == 0)
    {
        cout << "Specify tweak correctly in io.txt" ;
        exit(1);
    }

    //termination
    eoGenContinue<Indi> continuator((time_t) param["ga_"+currentData]);
    //    //CHECKPOINT
    eoCheckPoint<Indi> checkpoint(continuator);
    //    // Create a counter parameter
    //    eoValueParam<unsigned> generationCounter(0, "Generation");
    //    // Create an incrementor (sub-class of eoUpdater). Note that the
    //    // parameter's value is passed by reference,
    //    // so every time the incrementer is updated (every generation),
    //    // the data in generationCounter will change.
    //    eoIncrementor<unsigned> increment(generationCounter.value());
    //    // Add it to the checkpoint,
    //    // so the counter is updated (here, incremented) every generation
    //    checkpoint.add(increment);
    //    // now some statistics on the population:
    //    // Best fitness in population
    //    eoBestFitnessStat<Indi> bestStat;
    //    // Second moment stats: average and stdev
    //    eoSecondMomentStats<Indi> SecondStat;
    //    // Add them to the checkpoint to get them called at the appropriate time
    //    checkpoint.add(bestStat);
    //    checkpoint.add(SecondStat);
    //    // The Stdout monitor will print parameters to the screen ...
    //    eoStdoutMonitor monitor(false);
    //    // when called by the checkpoint (i.e. at every generation)
    //    checkpoint.add(monitor);
    //    // the monitor will output a series of parameters: add them
    //    monitor.add(generationCounter);
    //    monitor.add(bestStat);
    //    monitor.add(SecondStat);

    //THE ALGORITHM
    ourGA <Indi> ga(select, xover, *tweakList[io["tweak"]], eval, checkpoint, fairRandom, param["ga_"+currentData]);
    //HybridGA<Indi> hga(select, xover, tweak, eval, checkpoint, fairRandom, param["steadyGen"], (ACTORS + EDGES)*10);
    //GAforHC <Indi> ga(select, xover, explore, eval, param["hcIter"]);

    //Bismillah
    ga(pop);
    globalFinalResult << SERIAL_LOAD / ga.Best.fitness() << " " << ga.totalTime << endl;
}

void hga(string currentData)
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
    //GuidedMutation<Indi> explore;
    OptimisticTweak<Indi> opTweak;
    UniformMonCrossOver<Indi> exploit;
    Mutation<Indi> explore;
    GuidedMutation<Indi> gExplore;
    eoPropCombinedMonOp<Indi> tweak(opTweak, 0.2);
    tweak.add(exploit, 0.1, true);
    tweak.add(gExplore, 0.3, true);
    tweak.add(explore, 0.4, true);

    //termination
    eoGenContinue<Indi> continuator((time_t) param["hga_"+currentData]);
    //    //CHECKPOINT
    eoCheckPoint<Indi> checkpoint(continuator);

    //THE ALGORITHM
    HybridGA<Indi> hga(select, xover, tweak, eval, checkpoint, fairRandom, param["steadyGen"], (ACTORS + EDGES)*10, param["hga_"+currentData]);

    //Bismillah
    hga(pop);
    globalFinalResult << SERIAL_LOAD / hga.Best.fitness() << " " << hga.totalTime << endl;
}

int main(int argc, char** argv)
{
    for (int i = 0; i < TOTAL_DATASETS; i++)
    {
        //if (io[datasets[i]].compare("Y") == 0)
        // {
        MappingEvalFunc<Indi>::clearFitEvalCount();
        io["input"] = datasets[i];
        string currentData = datasets[i];
        gatherAllInfo();
        rng.reseed((uint32_t) param["seed"]);
        cout << "seed=" << (uint32_t) param["seed"] << endl;
        for (int j = 0; j < param["totalExp"]; j++)
        {
            if (io["algo"].compare("hc") == 0)
            {
                hc(currentData);
            }
            else if (io["algo"].compare("hcga") == 0)
            {
                hcga(currentData);
            }
            else if (io["algo"].compare("ga") == 0)
            {
                ga(currentData);
            }
            else if (io["algo"].compare("hga") == 0)
            {
                hga(currentData);
            }
            else
            {
                cout << "Error in algo name";
                exit(1);
            }
        }
        fitEvalStat << io["algo"] << " : "<< io["tweak"] << " : "<< datasets[i] << " : " << round(MappingEvalFunc<Indi>::getFitEvalCount()/param["totalExp"]);
        releaseAllInfo();
        //}
    }

    return 0;
}

