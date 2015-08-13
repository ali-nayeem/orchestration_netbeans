/* 
 * File:   genetic_algorithm.h
 * Author: MAN
 *
 * Created on October 19, 2014, 8:35 AM
 */

#ifndef GENETIC_ALGORITHM_H
#define	GENETIC_ALGORITHM_H

#include <eoInvalidateOps.h>
#include <eoContinue.h>
#include <eoPop.h>
#include <eoSelectOne.h>
#include <eoSelectPerc.h>
#include <eoEvalFunc.h>
#include <eoAlgo.h>
#include <apply.h>
#include <iostream>

template <class EOT>
class ourGA : public eoAlgo<EOT>
{
public:

    // added this second ctor as I didn't like the ordering of the parameters
    // in the one above. Any objection :-) MS

    ourGA(
          eoSelectPerc<EOT>& _select,
          eoQuadOp<EOT>& _cross,
          eoMonOp<EOT>& _mutate,
          eoEvalFunc<EOT>& _eval,
          eoContinue<EOT>& _cont,
          void (*_initializer)(EOT &), int _maxGen)
    : cont(_cont),
    mutate(_mutate),
    cross(_cross),
    select(_select),
    eval(_eval),
    initializer(_initializer), maxGen(_maxGen)
    {
        //best = NULL;
    }

    void operator()(eoPop<EOT>& P)
    {
        //ofstream stat(io["stat"].c_str());
        eoPop<EOT> Q;
        int genCount = 0;
        Timer elapsedTime;
        const double & passedTime = elapsedTime.value();
        //initialization
        for (int i = 0; i < param["popSize"]; i++)
        {
            EOT initial;
            initializer(initial);
            P.push_back(initial);
        }
        apply<EOT > (eval, P);
        Best = P[0];
        do
        {
            //update Best
            if (P.best_element() > Best)
            {
                Best = P.best_element();
                stat << genCount << " " << passedTime << " " << SERIAL_LOAD / Best.fitness() << endl;
                cout << "Fitness updated at gen: " << genCount << " sec:" << passedTime << " . New speedup,Fitness,avg,stdDev: " << SERIAL_LOAD / Best.fitness() << " , " << Best.fitness() << " , " << Best.avgLoad() << " , " << Best.stdDevLoad() << endl;
            }
            //selection
            select(P, Q); //select parents from P and put in Q
            //choose best individuals from P --- Elitism
            typename eoPop<EOT>::iterator it;
            for (unsigned i = 0; i < param["elite"]; i++)
            {
                it = P.it_best_element();
                Q.push_back(*it);
                P.erase(it);
            }
            //breeding
            for (unsigned i = 0; i < (param["popSize"] - param["elite"]) / 2; i++)
            {
                if (cross(Q[2 * i], Q[2 * i + 1])) // this crossover generates 2 Q from two parents
                {
                    Q[2 * i].invalidate();
                    Q[2 * i + 1].invalidate();
                    eval(Q[2 * i]);
                    eval(Q[2 * i + 1]);
                }
                if (mutate(Q[2 * i]))
                {
                    Q[2 * i].invalidate();
                }
                if (mutate(Q[2 * i + 1]))
                {
                    Q[2 * i + 1].invalidate();
                }
            }
            //P <- Q
            P.swap(Q);
            //fitness evaluation
            apply<EOT > (eval, P);
            genCount++;
            elapsedTime();
        }
        while (passedTime < param["maxTime"] || genCount < maxGen);
        stat << genCount << " " << passedTime << " " << SERIAL_LOAD / Best.fitness() << endl;
        //stat.close();
        cout << "Final sec:" << passedTime << " . Final speedup,Fitness,avg,stdDev: " << SERIAL_LOAD / Best.fitness() << " , " << Best.fitness() << " , " << Best.avgLoad() << " , " << Best.stdDevLoad() << endl;
        totalTime = passedTime;
    }

private:

    eoContinue<EOT>& cont;
    /// eoInvalidateMonOp invalidates the embedded operator
    eoInvalidateMonOp<EOT> mutate;
    // eoInvalidateQuadOp invalidates the embedded operator
    eoInvalidateQuadOp<EOT> cross;
    eoSelectPerc<EOT> select;
    eoEvalFunc<EOT>& eval;
    void (*initializer)(EOT &);
    int maxGen;

public:
    EOT Best;
    double totalTime;

};

template <class EOT>
class HybridGA : public eoAlgo<EOT>
{
public:

    // added this second ctor as I didn't like the ordering of the parameters
    // in the one above. Any objection :-) MS

    HybridGA(
             eoSelectPerc<EOT>& _select,
             eoQuadOp<EOT>& _cross,
             eoMonOp<EOT>& _mutate,
             eoEvalFunc<EOT>& _eval,
             eoContinue<EOT>& _cont,
             void (*_initializer)(EOT &), int _steadyGen, int _hcIter, int _maxGen)
    : cont(_cont),
    mutate(_mutate),
    cross(_cross),
    select(_select),
    eval(_eval),
    initializer(_initializer), steadyGen(_steadyGen), hcIter(_hcIter), maxGen(_maxGen)
    {
    }

    void operator()(eoPop<EOT>& P)
    {
        // ofstream stat(io["stat"].c_str());
        eoPop<EOT> Q;
        int genCount = 0, steadyCount = 0, convergeCount = 0;
        bool steadyState = false;
        Timer elapsedTime;
        const double & passedTime = elapsedTime.value();
        //hill climber
        //OptimisticTweak<EOT> opTweak;
        UniformMonCrossOver<EOT> exploit;
        //        Mutation<EOT> explore;
        //        GuidedMutation<EOT> gExplore;
        //        eoPropCombinedMonOp<EOT> tweak(opTweak, 0.4);
        //        tweak.add(exploit, 0.3, true);
        //        tweak.add(gExplore, 0.2, true);
        //        tweak.add(explore, 0.1, true);
        HillClimbing<EOT> hc(exploit, eval, hcIter, false, 0);
        //SimulatedAnnealing<EOT> hc(exploit, eval, hcIter);

        //initialization
        for (int i = 0; i < param["popSize"]; i++)
        {
            EOT initial;
            initializer(initial);
            P.push_back(initial);
            //  eval(P[i]);
            //  hc(P[i]);
        }
        apply<EOT > (eval, P);
        Best = P[0];
        do
        {
            //update Best
            if (P.best_element() > Best)
            {
                Best = P.best_element();
                stat << genCount << " " << passedTime << " " << SERIAL_LOAD / Best.fitness() << endl;
                cout << "GA Fitness updated at gen: " << genCount << " sec:" << passedTime << " . New speedup,Fitness,avg,stdDev: " << SERIAL_LOAD / Best.fitness() << " , " << Best.fitness() << " , " << Best.avgLoad() << " , " << Best.stdDevLoad() << endl;
                //for steady state
                steadyCount = 0;
                steadyState = false;
                convergeCount = 0;
            }
            else
            {
                steadyCount++;
                if (steadyCount == steadyGen)
                {
                    steadyState = true;
                }
                convergeCount++;
            }
            //            if (convergeCount > param["convergeGen"])
            //            {
            //                break;
            //            }
            //selection
            select(P, Q); //select parents from P and put in Q
            //choose best individuals from P --- Elitism
            typename eoPop<EOT>::iterator it;
            for (unsigned i = 0; i < param["elite"]; i++)
            {
                it = P.it_best_element();
                Q.push_back(*it);
                P.erase(it);
                //                if (steadyState)
                //                {
                //                     hc(Q[Q.size() - 1]);
                //                }
            }
            //breeding
            for (unsigned i = 0; i < (param["popSize"] - param["elite"]) / 2; i++)
            {
                if (cross(Q[2 * i], Q[2 * i + 1])) // this crossover generates 2 Q from two parents
                {
                    Q[2 * i].invalidate();
                    Q[2 * i + 1].invalidate();
                    eval(Q[2 * i]);
                    eval(Q[2 * i + 1]);
                }
                if (mutate(Q[2 * i]))
                {
                    Q[2 * i].invalidate();
                }
                if (mutate(Q[2 * i + 1]))
                {
                    Q[2 * i + 1].invalidate();
                }
                //for escaping steady state do hill climb
                if (steadyState)
                {
                    eval(Q[2 * i]);
                    hc(Q[2 * i]);
                    eval(Q[2 * i + 1]);
                    hc(Q[2 * i + 1]);
                }
            }
            if (steadyState)
            {
                steadyCount = 0;
                steadyState = false;
            }
            //P <- Q
            P.swap(Q);
            //fitness evaluation
            apply<EOT > (eval, P);
            genCount++;
            elapsedTime();
        }
        while (passedTime < param["maxTime"] || genCount < maxGen);
        if (P.best_element() > Best)
        {
            Best = P.best_element();
        }
        stat << genCount << " " << passedTime << " " << SERIAL_LOAD / Best.fitness() << endl;
        // stat.close();
        cout << "Final sec:" << passedTime << " . Final speedup,Fitness,avg,stdDev: " << SERIAL_LOAD / Best.fitness() << " , " << Best.fitness() << " , " << Best.avgLoad() << " , " << Best.stdDevLoad() << endl;
        totalTime = passedTime;
    }

private:

    eoContinue<EOT>& cont;
    /// eoInvalidateMonOp invalidates the embedded operator
    eoInvalidateMonOp<EOT> mutate;
    // eoInvalidateQuadOp invalidates the embedded operator
    eoInvalidateQuadOp<EOT> cross;
    eoSelectPerc<EOT> select;
    eoEvalFunc<EOT>& eval;
    void (*initializer)(EOT &);
    int steadyGen;
    int hcIter;
    int maxGen;

public:
    EOT Best;
    double totalTime;

};

template <class EOT>
class HybridGAchild : public eoAlgo<EOT>
{
public:

    // added this second ctor as I didn't like the ordering of the parameters
    // in the one above. Any objection :-) MS

    HybridGAchild(
                  eoSelectPerc<EOT>& _select,
                  eoQuadOp<EOT>& _cross,
                  eoMonOp<EOT>& _mutate,
                  eoEvalFunc<EOT>& _eval,
                  eoContinue<EOT>& _cont,
                  void (*_initializer)(EOT &), int _steadyGen, int _hcIter)
    : cont(_cont),
    mutate(_mutate),
    cross(_cross),
    select(_select),
    eval(_eval),
    initializer(_initializer), steadyGen(_steadyGen), hcIter(_hcIter)
    {
    }

    void operator()(eoPop<EOT>& P)
    {
        //ofstream stat(io["stat"].c_str());
        eoPop<EOT> Q;
        int genCount = 0, steadyCount = 0, convergeCount = 0;
        bool steadyState = false;
        Timer elapsedTime;
        const double & passedTime = elapsedTime.value();
        //hill climber
        OptimisticTweak<EOT> opTweak;
        UniformMonCrossOver<EOT> exploit;
        //        Mutation<EOT> explore;
        //        GuidedMutation<EOT> gExplore;
        //        eoPropCombinedMonOp<EOT> tweak(opTweak, 0.4);
        //        tweak.add(exploit, 0.3, true);
        //        tweak.add(gExplore, 0.2, true);
        //        tweak.add(explore, 0.1, true);
        HillClimbing<EOT> hc(exploit, eval, hcIter);

        //initialization
        EOT initial;
        initializer(initial);
        eval(initial);
        for (int i = 0; i < param["popSize"]; i++)
        {
            EOT indiv = initial;
            hc(indiv);
            P.push_back(indiv);
        }
        apply<EOT > (eval, P);
        Best = P[0];
        do
        {
            //update Best
            if (P.best_element() > Best)
            {
                Best = P.best_element();
                stat << genCount << "," << passedTime << "," << SERIAL_LOAD / Best.fitness() << "," << Best.avgLoad() << "," << Best.stdDevLoad() << "," << Best.fitness() << endl;
                cout << "GA Fitness updated at gen: " << genCount << " sec:" << passedTime << " . New speedup,Fitness,avg,stdDev: " << SERIAL_LOAD / Best.fitness() << " , " << Best.fitness() << " , " << Best.avgLoad() << " , " << Best.stdDevLoad() << endl;
                //for steady state
                steadyCount = 0;
                steadyState = false;
                convergeCount = 0;
            }
            else
            {
                steadyCount++;
                if (steadyCount == steadyGen)
                {
                    steadyState = true;
                }
                convergeCount++;
            }
            if (convergeCount > param["convergeGen"])
            {
                break;
            }
            //selection
            select(P, Q); //select parents from P and put in Q
            //choose best individuals from P --- Elitism
            typename eoPop<EOT>::iterator it;
            for (unsigned i = 0; i < param["elite"]; i++)
            {
                it = P.it_best_element();
                Q.push_back(*it);
                P.erase(it);
            }
            //breeding
            for (unsigned i = 0; i < (param["popSize"] - param["elite"]) / 2; i++)
            {
                if (cross(Q[2 * i], Q[2 * i + 1])) // this crossover generates 2 Q from two parents
                {
                    Q[2 * i].invalidate();
                    Q[2 * i + 1].invalidate();
                }
                if (mutate(Q[2 * i]))
                {
                    Q[2 * i].invalidate();
                }
                if (mutate(Q[2 * i + 1]))
                {
                    Q[2 * i + 1].invalidate();
                }
                //for escaping steady state do hill climb
                if (steadyState)
                {
                    eval(Q[2 * i]);
                    hc(Q[2 * i]);
                    eval(Q[2 * i + 1]);
                    hc(Q[2 * i + 1]);
                }
            }
            if (steadyState)
            {
                steadyCount = 0;
                steadyState = false;
            }
            //P <- Q
            P.swap(Q);
            //fitness evaluation
            apply<EOT > (eval, P);
            genCount++;
            elapsedTime();
        }
        while (cont(P));
        if (P.best_element() > Best)
        {
            Best = P.best_element();
        }
        stat << genCount << "," << param["maxTime"] << "," << SERIAL_LOAD / Best.fitness() << "," << Best.avgLoad() << "," << Best.stdDevLoad() << "," << Best.fitness() << endl;
        //stat.close();
        cout << "Final sec:" << passedTime << " . Final speedup,Fitness,avg,stdDev: " << SERIAL_LOAD / Best.fitness() << " , " << Best.fitness() << " , " << Best.avgLoad() << " , " << Best.stdDevLoad() << endl;
        //return Best;
    }

private:

    eoContinue<EOT>& cont;
    /// eoInvalidateMonOp invalidates the embedded operator
    eoInvalidateMonOp<EOT> mutate;
    // eoInvalidateQuadOp invalidates the embedded operator
    eoInvalidateQuadOp<EOT> cross;
    eoSelectPerc<EOT> select;
    eoEvalFunc<EOT>& eval;
    void (*initializer)(EOT &);
    int steadyGen;
    int hcIter;

public:
    EOT Best;

};

template <class EOT>
class GAforHC : public eoAlgo<EOT>
{
public:

    GAforHC(
            eoSelectPerc<EOT>& _select,
            eoQuadOp<EOT>& _cross,
            eoMonOp<EOT>& _mutate,
            eoEvalFunc<EOT>& _eval,
            int _hcIter)
    :
    mutate(_mutate),
    cross(_cross),
    select(_select),
    eval(_eval),
    hcIter(_hcIter)
    {
        //best = NULL;
    }

    void setInitialSol(EOT & _initialSol)
    {
        initialSol = _initialSol;
    }

    void operator()(eoPop<EOT>& P)
    {
        P.clear();
        //ofstream stat(io["stat"].c_str());
        eoPop<EOT> Q;
        int genCount = 0;
        Timer elapsedTime;
        const double & passedTime = elapsedTime.value();
        //hill climber
        OptimisticTweak<EOT> opTweak;
        UniformMonCrossOver<EOT> exploit;
        Mutation<EOT> explore;
        GuidedMutation<EOT> gExplore;
        eoPropCombinedMonOp<EOT> tweak(opTweak, 0.2);
        tweak.add(exploit, 0.1, true);
        tweak.add(gExplore, 0.3, true);
        tweak.add(explore, 0.4, true);
        //HillClimbing<EOT> hc(exploit, eval, hcIter);
        //initialization
        P.push_back(initialSol);
        for (int i = 0; i < param["popSize"] - 1; i++)
        {
            EOT indiv = initialSol;
            for (int i = 0; i < hcIter; i++)
            {
                if (tweak(indiv))
                {
                    indiv.invalidate();
                }
                eval(indiv);
                if (indiv.fitness() != initialSol.fitness())
                {
                    break;
                }
            }
            // cout << indiv.fitness();
            P.push_back(indiv);
        }
        apply<EOT > (eval, P);
        Best = initialSol;
        do
        {
            //update Best
            if (P.best_element() > Best)
            {
                Best = P.best_element();
                //stat << passedTime << "," << SERIAL_LOAD / Best.fitness() << "," << Best.avgLoad() << "," << Best.stdDevLoad() << "," << Best.fitness() << endl;
                cout << "GAforHC Fitness updated at gen: " << genCount << " sec:" << passedTime << " . New speedup,Fitness,avg,stdDev: " << SERIAL_LOAD / Best.fitness() << " , " << Best.fitness() << " , " << Best.avgLoad() << " , " << Best.stdDevLoad() << endl;
                break;
            }
            //selection
            select(P, Q); //select parents from P and put in Q
            //choose best individuals from P --- Elitism
            typename eoPop<EOT>::iterator it;
            for (unsigned i = 0; i < param["elite"]; i++)
            {
                it = P.it_best_element();
                Q.push_back(*it);
                P.erase(it);
            }
            //breeding
            for (unsigned i = 0; i < (param["popSize"] - param["elite"]) / 2; i++)
            {
                if (cross(Q[2 * i], Q[2 * i + 1])) // this crossover generates 2 Q from two parents
                {
                    Q[2 * i].invalidate();
                    Q[2 * i + 1].invalidate();
                    eval(Q[2 * i]);
                    eval(Q[2 * i + 1]);
                }
                if (tweak(Q[2 * i]))
                {
                    Q[2 * i].invalidate();
                }
                if (tweak(Q[2 * i + 1]))
                {
                    Q[2 * i + 1].invalidate();
                }
            }
            //P <- Q
            P.swap(Q);
            //fitness evaluation
            apply<EOT > (eval, P);
            genCount++;
            //cout << genCount;
            elapsedTime();
        }
        while (genCount < param["GAHCmaxGen"]);
        if (P.best_element() > Best)
        {
            Best = P.best_element();
        }
        //        stat << param["maxTime"] << "," << SERIAL_LOAD / Best.fitness() << "," << Best.avgLoad() << "," << Best.stdDevLoad() << "," << Best.fitness() << endl;
        //        stat.close();
        cout << "Final sec:" << passedTime << " . Final speedup,Fitness,avg,stdDev: " << SERIAL_LOAD / Best.fitness() << " , " << Best.fitness() << " , " << Best.avgLoad() << " , " << Best.stdDevLoad() << endl;
        //return Best;
    }

private:

    eoInvalidateMonOp<EOT> mutate;
    // eoInvalidateQuadOp invalidates the embedded operator
    eoInvalidateQuadOp<EOT> cross;
    eoSelectPerc<EOT> select;
    eoEvalFunc<EOT>& eval;
    EOT initialSol;
    int hcIter;

public:
    EOT Best;

};

template <class EOT>
class HybridHillClimbing
{
public:

    // added this second ctor as I didn't like the ordering of the parameters
    // in the one above. Any objection :-) MS

    HybridHillClimbing(eoMonOp<EOT>& _tweak, eoEvalFunc<EOT>& _eval, int _maxGen) : tweak(_tweak), eval(_eval), maxGen(_maxGen)
    {
    }

    void operator()(EOT & S)
    {
        int gen = 0, steadyCount = 0, convergeCount = 0;
        //population
        eoPop<EOT> pop;
        //selection
        eoDetTournamentSelect<EOT> selectOne(param["tFit"]);
        double perc = (param["popSize"] - param["elite"]) / param["popSize"];
        eoSelectPerc<EOT> select(selectOne, perc);
        //crossover
        MappingQuadCrossover<EOT> xover;
        //mutations
        GuidedMutation<EOT> explore;
        //THE ALGORITHM
        GAforHC<EOT> ga(select, xover, explore, eval, param["hcIter"]);

        EOT R;
        // ofstream stat(io["stat"].c_str());
        Timer elapsedTime;
        const double & passedTime = elapsedTime.value();
        stat << gen << " " << passedTime << " " << SERIAL_LOAD / S.fitness() << endl;
        do
        {
            gen++;

            R = S;
            tweak(R);
            eval(R);

            if (R > S)
            {
                S = R;
                steadyCount = 0;
                cout << "HC Fitness updated at gen: " << gen << " sec:" << passedTime << " . New speedup,Fitness,avg,stdDev: " << SERIAL_LOAD / S.fitness() << " , " << S.fitness() << " , " << S.avgLoad() << " , " << S.stdDevLoad() << endl;
                stat << gen << " " << passedTime << " " << SERIAL_LOAD / S.fitness() << endl;
            }
            else
            {
                steadyCount++;
                if (steadyCount > (ACTORS + EDGES)*10)
                {
                    convergeCount++;
                    //cout<<convergeCount<<endl;
                    //                    if(convergeCount > param["GAHConverge"])
                    //                    {
                    //                        break;
                    //                    }
                    ga.setInitialSol(S);
                    ga(pop);
                    if (ga.Best > S)
                    {
                        S = ga.Best;
                        convergeCount = 0;
                    }
                    steadyCount = 0;

                }

            }
            //update time
            elapsedTime();
        }
        while (gen < maxGen || passedTime < param["maxTime"]);
        stat << gen << " " << passedTime << " " << SERIAL_LOAD / S.fitness() << endl;
        // stat.close();
        cout << endl << "Total Time:" << passedTime << endl;
        totalTime = passedTime;
    }

private:
    /// eoInvalidateMonOp invalidates the embedded operator
    eoInvalidateMonOp<EOT> tweak;
    // eoInvalidateQuadOp invalidates the embedded operator
    eoEvalFunc<EOT>& eval;
    int maxGen;
public:
    double totalTime;

};
#endif	/* GENETIC_ALGORITHM_H */

