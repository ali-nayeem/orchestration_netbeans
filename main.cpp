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
    param["seed"] = 1413089664;
    cout << "seed=" << (uint32_t) param["seed"] << endl;
    rng.reseed(param["seed"]);

    //fitness evaluators
    MappingEvalFunc<Indi> eval;

    //initial solution
    Indi inialSolution;
    fairRandom(inialSolution);
    eval(inialSolution);

    //tweaks
    UniformMonCrossOver<Indi> exploit;
    Mutation<Indi> explore;
    eoPropCombinedMonOp<Indi> tweak(exploit, param["rExploit"]);
    tweak.add(explore, 1 - param["rExploit"], true);

    //THE ALGORITHM
    HillClimbing<Indi> hc(tweak, eval, param["maxGen"]);
    cout << "Initial" << endl << inialSolution << endl;
    hc(inialSolution);
    cout << "Final" << endl << inialSolution << endl;
   // eval(inialSolution);
   // cout << "Final" << endl << inialSolution << endl;

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
    

     main_function( argc, argv);
      //printAdjInfo();

    return 0;
}

