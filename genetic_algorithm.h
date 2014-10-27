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
          eoContinue<EOT>& _cont)
    : cont(_cont),
    mutate(_mutate),
    cross(_cross),
    select(_select),
    eval(_eval)
    {
        //best = NULL;
    }

    void operator()(eoPop<EOT>& P)
    {
        ofstream stat(io["stat"].c_str());
        eoPop<EOT> Q;
        EOT Best;
        int genCount = 0;
        //initialization
        for (int i = 0; i < param["popSize"]; i++)
        {
            EOT initial;
            fullyRandom(initial);
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
                stat << genCount << "," << Best.fitness() << "," << Best.avgLoad() << "," << Best.stdDevLoad() << "," << SERIAL_LOAD / Best.fitness() << endl;
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
            }
            //P <- Q
            P.swap(Q);
            //fitness evaluation
            apply<EOT > (eval, P);
            genCount++;
        }
        while (cont(P));
        stat.close();
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

public:
    EOT best;

};


#endif	/* GENETIC_ALGORITHM_H */

