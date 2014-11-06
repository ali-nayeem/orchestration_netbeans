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

    void operator()(EOT & S)
    {
        int gen = 0;
        EOT R;
        ofstream stat(io["stat"].c_str());
        eoTimeCounter elapsedTime;
        double & passedTime = elapsedTime.value();
        stat << passedTime << "," << SERIAL_LOAD / S.fitness() << "," << S.avgLoad() << "," << S.stdDevLoad() << "," << S.fitness() << endl;
        do
        {
            gen++;

            R = S;
            tweak(R);
            eval(R);
            //update time
            elapsedTime();
            if (R > S)
            {
                S = R;
                cout << "Fitness updated at gen: " << gen << " sec:" << passedTime <<" . New speedup,Fitness,avg,stdDev: " << SERIAL_LOAD / S.fitness() << " , "<<S.fitness() << " , " << S.avgLoad() << " , " << S.stdDevLoad() << endl;
                stat << passedTime << "," << SERIAL_LOAD / S.fitness() << "," << S.avgLoad() << "," << S.stdDevLoad() << "," << S.fitness() << endl;

            }
        }
        while (passedTime < param["maxTime"]);
        stat.close();
        cout << endl << "Total Time:" << passedTime << endl;

    }

private:
    /// eoInvalidateMonOp invalidates the embedded operator
    eoInvalidateMonOp<EOT> tweak;
    // eoInvalidateQuadOp invalidates the embedded operator
    eoEvalFunc<EOT>& eval;
    int maxGen;

};

template <class EOT>
class SteepestAscent
{
public:

    // added this second ctor as I didn't like the ordering of the parameters
    // in the one above. Any objection :-) MS

    SteepestAscent(eoMonOp<EOT>& _tweak, eoEvalFunc<EOT>& _eval) : tweak(_tweak), eval(_eval)
    {
    }

    void operator()(EOT & S)
    {
        int gen = 0;
        EOT R;
        EOT W;
        ofstream stat(io["stat"].c_str());
        eoTimeCounter elapsedTime;
        do
        {
            gen++;

            R = S;
            tweak(R);
            eval(R);
            for (int i = 1; i < param["nTweaks"]; i++)
            {
                W = S;
                tweak(W);
                eval(W);
                if (W > R)
                {
                    R = W;
                }
            }
            if (R > S)
            {
                S = R;
                cout << "Fitness updated at gen: " << gen << ". New Fitness,avg,stdDev: " << S.fitness() << "," << S.avgLoad() << "," << S.stdDevLoad() << endl;

            }
            stat << S.fitness() << "," << S.avgLoad() << "," << S.stdDevLoad() << "," << SERIAL_LOAD / S.fitness() << endl;
            elapsedTime();
        }
        while (elapsedTime.value() < param["maxTime"]);
        stat.close();
        cout << endl << "Total Time:" << elapsedTime.value() << endl;

    }

private:
    /// eoInvalidateMonOp invalidates the embedded operator
    eoInvalidateMonOp<EOT> tweak;
    // eoInvalidateQuadOp invalidates the embedded operator
    eoEvalFunc<EOT>& eval;

};

template<class EOT>
class SteepestAscentWithReplacement
{
public:

    SteepestAscentWithReplacement(eoMonOp<EOT>& _tweak, eoEvalFunc<EOT>& _eval) : tweak(_tweak), eval(_eval)
    {
    }

    void operator()(EOT & S)
    {
        int gen = 0;
        EOT R;
        EOT W;
        EOT Best;
        Best = S;
        ofstream stat(io["stat"].c_str());
        eoTimeCounter elapsedTime;
        do
        {
            gen++;

            R = S;
            tweak(R);
            eval(R);
            for (int i = 1; i < param["nTweaks"]; i++)
            {
                W = S;
                tweak(W);
                eval(W);
                if (W > R)
                {
                    R = W;
                }
            }
            S = R;
            if (S > Best)
            {
                Best = S;
                cout << "Fitness updated at gen: " << gen << ". New Fitness, avg, stdDev: " << Best.fitness() << " , " << Best.avgLoad() << " , " << Best.stdDevLoad() << endl;

            }
            stat << Best.fitness() << "," << Best.avgLoad() << "," << Best.stdDevLoad() << "," << SERIAL_LOAD / Best.fitness() << endl;
            elapsedTime();
        }
        while (elapsedTime.value() < param["maxTime"]);
        stat.close();
        cout << endl << "Total Time:" << elapsedTime.value() << endl;
    }

private:
    /// eoInvalidateMonOp invalidates the embedded operator
    eoInvalidateMonOp<EOT> tweak;
    // eoInvalidateQuadOp invalidates the embedded operator
    eoEvalFunc<EOT>& eval;

};

template <class EOT>
class SimulatedAnnealing
{
public:

    // added this second ctor as I didn't like the ordering of the parameters
    // in the one above. Any objection :-) MS

    SimulatedAnnealing(eoMonOp<EOT>& _tweak, eoEvalFunc<EOT>& _eval) : tweak(_tweak), eval(_eval)
    {
    }

    void operator()(EOT & S)
    {
        int gen = 0;
        double totalTime = param["maxTime"], pChoose, exp;
        EOT R;
        EOT Best = S;
        ofstream stat(io["stat"].c_str());
        eoTimeCounter elapsedTime;
        double & passedTime = elapsedTime.value();
        do
        {
            gen++;

            R = S;
            tweak(R);
            eval(R);

            exp = (S.fitness() - R.fitness()) / (totalTime - passedTime);
            pChoose = pow(E, exp);

            if (R > S || rng.uniform() < pChoose)
            {
                S = R;
            }

            //cout << pChoose << endl;

            if (S > Best)
            {
                Best = S;
                cout << "Fitness updated at gen: " << gen << ". New Fitness, avg, stdDev: " << Best.fitness() << " , " << Best.avgLoad() << " , " << Best.stdDevLoad() << endl;
            }
            stat << Best.fitness() << "," << Best.avgLoad() << "," << Best.stdDevLoad() << "," << SERIAL_LOAD / Best.fitness() << endl;

            elapsedTime();
        }
        while (passedTime < totalTime);
        stat.close();
        cout << endl << "Total Time:" << passedTime << endl;

    }

private:
    /// eoInvalidateMonOp invalidates the embedded operator
    eoInvalidateMonOp<EOT> tweak;
    // eoInvalidateQuadOp invalidates the embedded operator
    eoEvalFunc<EOT>& eval;

};

#endif	/* HILL_CLIMBING_H */

