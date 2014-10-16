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
#include "hill_climbing.h"

typedef eoMinimizingFitness MyFitT;
typedef Mapping<MyFitT> Indi;

/*
 *
 */
void main_function(int argc, char **argv)
{
    param["seed"] = 1413089664;     //1413089664 or time(0)
    cout << "seed=" << (uint32_t) param["seed"] << endl;
    rng.reseed((uint32_t)param["seed"]);

    //individual file read/write
    //ofstream indiSave(io["indi"].c_str());
    ifstream indiLoad(io["indiLoad"].c_str());

    //fitness evaluators
    MappingEvalFunc<Indi> eval;

    //initial solution
    Indi initialSolution;
    fullyRandom(initialSolution);
     //indiLoad >> initialSolution ;
    eval(initialSolution);

    //tweaks
    UniformMonCrossOver<Indi> exploit;
    Mutation<Indi> explore;
    eoPropCombinedMonOp<Indi> tweak(exploit, param["rExploit"]);
    tweak.add(explore, 1 - param["rExploit"], true);
    //HybridTweak<Indi> hybridTweak(exploit, explore);

    //THE ALGORITHM
    HillClimbing<Indi> hc(tweak, eval, param["maxGen"]);
    //SteepestAscent<Indi> sa(tweak,eval);
    //SteepestAscentWithReplacement<Indi> sar(tweak,eval);

    //initial print
    cout << "Initial Solution:" << endl;
    initialSolution.print();

    //In the name of Allah
    hc(initialSolution);

    //final print
    cout << "Final" << endl << initialSolution << endl;
    double speedUp = SERIAL_LOAD / initialSolution.fitness();
    cout << "Serial load " << SERIAL_LOAD << endl;
    cout << "speedup " << speedUp;
    //cout << "Final Solution:" << endl ;
    //initialSolution.print();
    //indiSave << initialSolution;

}

int main(int argc, char** argv)
{
    gatherAllInfo();

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


    main_function(argc, argv);
    //printAdjInfo();

    return 0;
}

