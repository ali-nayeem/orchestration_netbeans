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

/*
 *
 */
void hc()
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
    HillClimbing<Indi> hc(exploit, eval, 0, true);
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
    globalFinalResult << SERIAL_LOAD / initialSolution.fitness()<< endl;

}

void hcga()
{
    MappingEvalFunc<Indi> eval;
    //initial solution
    Indi initialSolution;
    fairRandom(initialSolution);
    eval(initialSolution);
    //tweaks
    UniformMonCrossOver<Indi> exploit;
    //THE ALGORITHM
    HybridHillClimbing<Indi> hcga(exploit, eval, 0);
    //In the name of Allah
    hcga(initialSolution);
    //final print
    double speedUp = SERIAL_LOAD / initialSolution.fitness();
    cout << "Serial load " << SERIAL_LOAD << endl;
    cout << "speedup " << speedUp << endl;
    //cout << "Final Solution:" << endl ;
    //initialSolution.print();
    //indiSave << initialSolution;
    globalFinalResult << SERIAL_LOAD / initialSolution.fitness()<< endl;

}

void ga()
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
    eoGenContinue<Indi> continuator((time_t) param["maxGen"]);
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
    ourGA <Indi> ga(select, xover, tweak, eval, checkpoint, fairRandom);
    //HybridGA<Indi> hga(select, xover, tweak, eval, checkpoint, fairRandom, param["steadyGen"], (ACTORS + EDGES)*10);
    //GAforHC <Indi> ga(select, xover, explore, eval, param["hcIter"]);

    //Bismillah
    ga(pop);
    globalFinalResult << SERIAL_LOAD / ga.Best.fitness()<< endl;
}

void hga()
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
    eoGenContinue<Indi> continuator((time_t) param["maxGen"]);
    //    //CHECKPOINT
    eoCheckPoint<Indi> checkpoint(continuator);

    //THE ALGORITHM
    HybridGA<Indi> hga(select, xover, tweak, eval, checkpoint, fairRandom, param["steadyGen"], (ACTORS + EDGES)*10);

    //Bismillah
    hga(pop);
    globalFinalResult << SERIAL_LOAD / hga.Best.fitness()<< endl;
}

int main(int argc, char** argv)
{
    for (int i = 0; i < TOTAL_DATASETS; i++)
    {
        //if (io[datasets[i]].compare("Y") == 0)
        // {
        io["input"] = datasets[i];
        gatherAllInfo();
        rng.reseed((uint32_t) param["seed"]);
        cout << "seed=" << (uint32_t) param["seed"] << endl;
        for (int j = 0; j < param["totalExp"]; j++)
        {
            if (io["algo"].compare("hc") == 0)
            {
                hc();
            }
            else if (io["algo"].compare("hcga") == 0)
            {
                hcga();
            }
            else if (io["algo"].compare("ga") == 0)
            {
                ga();
            }
            else if (io["algo"].compare("hga") == 0)
            {
                hga();
            }
            else
            {
                cout << "Error in algo name";
                exit(1);
            }
        }
        releaseAllInfo();
        //}
    }

    return 0;
}

