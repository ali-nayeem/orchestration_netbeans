/* 
 * File:   hill_climbing.h
 * Author: MAN
 *
 * Created on October 11, 2014, 8:44 PM
 */

#ifndef HILL_CLIMBING_H
#define	HILL_CLIMBING_H

template <class EOT>
class HillClimbing
{
public:

    // added this second ctor as I didn't like the ordering of the parameters
    // in the one above. Any objection :-) MS

    HillClimbing(eoMonOp<EOT>& _tweak, eoEvalFunc<EOT>& _eval, int _maxGen) : tweak(_tweak), eval(_eval), maxGen(_maxGen)
    {
    }

    //    HillClimbing(
    //           eoSelectPerc<EOT>& _select,
    //           eoQuadOp<EOT>& _cross,
    //           eoMonOp<EOT>& _mutate,
    //           eoEvalFunc<EOT>& _eval,
    //           eoContinue<EOT>& _cont,
    //           unsigned _elite)
    //    : cont(_cont),
    //    tweak(_mutate),
    //    cross(_cross),
    //    select(_select),
    //    elite(_elite),
    //    eval(_eval)
    //    {
    //        //best = NULL;
    //    }

    void operator()(EOT & S)
    {
        int gen = 0;
        EOT R;
        ofstream stat(io["stat"].c_str());
        eoTimeCounter elapsedTime;
        do
        {
            gen++;

            R = S;
            tweak(R);
            eval(R);
            if (R > S)
            {
                S = R;
                cout << "Fitness updated at gen: " << gen << ". New Fitness,avg,stdDev: " << S.fitness() << "," << S.avgLoad() << "," << S.stdDevLoad() << endl;

            }
            stat << S.fitness() << "," << S.avgLoad() << "," << S.stdDevLoad() <<"," << SERIAL_LOAD/S.fitness()  <<endl;
            elapsedTime();
            int sec = elapsedTime.value() ;
        }
        while ( elapsedTime.value() < param["maxTime"] );
        stat.close();
        cout <<endl<< "Total Time:" << elapsedTime.value() << endl;

    }

private:

    /// eoInvalidateMonOp invalidates the embedded operator
    eoInvalidateMonOp<EOT> tweak;
    // eoInvalidateQuadOp invalidates the embedded operator
    eoEvalFunc<EOT>& eval;
    int maxGen;
public:
    EOT best;

};


#endif	/* HILL_CLIMBING_H */

