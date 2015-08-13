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

    HillClimbing(eoMonOp<EOT>& _tweak, eoEvalFunc<EOT>& _eval, int _maxGen, bool _saveStat, int _maxTime) : tweak(_tweak), eval(_eval), maxGen(_maxGen), saveStat(_saveStat), maxTime(_maxTime)
    {
    }

    void operator()(EOT & S)
    {
        int gen = 0;
        EOT R;
        // ofstream stat(io["stat"].c_str());
        Timer elapsedTime;
        const double & passedTime = elapsedTime.value();
        if (saveStat)
        {
            stat << gen << " " << passedTime << " " << SERIAL_LOAD / S.fitness() << endl;
        }
        do
        {
            gen++;

            R = S;
            tweak(R);
            eval(R);

            if (R > S)
            {
                S = R;
                cout << "HC Fitness updated at gen: " << gen << " sec:" << passedTime << " . New speedup,Fitness,avg,stdDev: " << SERIAL_LOAD / S.fitness() << " , " << S.fitness() << " , " << S.avgLoad() << " , " << S.stdDevLoad() << endl;
                if (saveStat)
                {
                    stat << gen << " " << passedTime << " " << SERIAL_LOAD / S.fitness() << endl;
                }
                //    stat << "," << gen <<passedTime << "," << SERIAL_LOAD / S.fitness() << "," << S.avgLoad() << "," << S.stdDevLoad() << "," << S.fitness() << endl;
            }

            //update time
            elapsedTime();
        }
        while (passedTime < maxTime || gen < maxGen);
        if (saveStat)
        {
            stat << gen << " " << passedTime << " " << SERIAL_LOAD / S.fitness() << endl;
        }
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
    bool saveStat;
    int maxTime;
public:
    double totalTime;

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
        //ofstream stat(io["stat"].c_str());
        Timer elapsedTime;
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
        //stat.close();
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
        //ofstream stat(io["stat"].c_str());
        Timer elapsedTime;
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
        S = Best;
        //stat.close();
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

    SimulatedAnnealing(eoMonOp<EOT>& _tweak, eoEvalFunc<EOT>& _eval, int _maxGen) : tweak(_tweak), eval(_eval), maxGen(_maxGen)
    {
    }

    void operator()(EOT & S)
    {
        int gen = 0;
        double temp = param["initTemp"], pChoose, exp, coolingRate = param["coolingRate"];
        EOT R;
        EOT Best = S;
        // ofstream stat(io["stat"].c_str());
        Timer elapsedTime;
        const double & passedTime = elapsedTime.value();
        do
        {
            gen++;

            R = S;
            tweak(R);
            eval(R);

            exp = (SERIAL_LOAD / R.fitness() - SERIAL_LOAD / S.fitness()) / temp;
            pChoose = pow(E, exp);

            if (R > S || rng.uniform() < pChoose)
            {
                S = R;
            }
            temp = temp * coolingRate;
            //cout << pChoose << endl;

            if (S > Best)
            {
                Best = S;
                cout << "SimAnn Fitness updated at gen: " << gen << ". New Fitness, avg, stdDev: " << SERIAL_LOAD / Best.fitness() << " , " << Best.avgLoad() << " , " << Best.stdDevLoad() << endl;
            }
            //stat << Best.fitness() << "," << Best.avgLoad() << "," << Best.stdDevLoad() << "," << SERIAL_LOAD / Best.fitness() << endl;

            elapsedTime();
        }
        while (gen < maxGen);
        S = Best;
        // stat.close();
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
class NewIdea
{
public:

    // added this second ctor as I didn't like the ordering of the parameters
    // in the one above. Any objection :-) MS

    NewIdea(eoMonOp<EOT>& _exploit, eoMonOp<EOT>& _explore, eoEvalFunc<EOT>& _eval, int _maxGen) : exploit(_exploit), explore(_explore), eval(_eval), maxGen(_maxGen)
    {
    }

    void operator()(EOT & S)
    {
        int gen = 0;
        EOT R;
        //ofstream stat(io["stat"].c_str());
        Timer elapsedTime;
        const double & passedTime = elapsedTime.value();
        stat << passedTime << "," << SERIAL_LOAD / S.fitness() << "," << S.avgLoad() << "," << S.stdDevLoad() << "," << S.fitness() << endl;
        do
        {
            //update time
            elapsedTime();
            gen++;

            R = S;

            exploit(R);
            eval(R);

            if (R > S)
            {
                S = R;
                cout << "Fitness updated at gen: " << gen << " sec:" << passedTime << " . New speedup,Fitness,avg,stdDev: " << SERIAL_LOAD / S.fitness() << " , " << S.fitness() << " , " << S.avgLoad() << " , " << S.stdDevLoad() << endl;
                stat << passedTime << "," << SERIAL_LOAD / S.fitness() << "," << S.avgLoad() << "," << S.stdDevLoad() << "," << S.fitness() << endl;
            }
            //update time
            //elapsedTime();
            explore(R);
            eval(R);
            if (R > S)
            {
                S = R;
                cout << "Fitness updated at gen: " << gen << " sec:" << passedTime << " . New speedup,Fitness,avg,stdDev: " << SERIAL_LOAD / S.fitness() << " , " << S.fitness() << " , " << S.avgLoad() << " , " << S.stdDevLoad() << endl;
                stat << passedTime << "," << SERIAL_LOAD / S.fitness() << "," << S.avgLoad() << "," << S.stdDevLoad() << "," << S.fitness() << endl;
            }
        }
        while (passedTime < param["maxTime"]);
        stat << param["maxTime"] << "," << SERIAL_LOAD / S.fitness() << "," << S.avgLoad() << "," << S.stdDevLoad() << "," << S.fitness() << endl;
        //stat.close();
        cout << endl << "Total Time:" << passedTime << endl;

    }

private:
    /// eoInvalidateMonOp invalidates the embedded operator
    eoInvalidateMonOp<EOT> exploit;
    eoInvalidateMonOp<EOT> explore;

    // eoInvalidateQuadOp invalidates the embedded operator
    eoEvalFunc<EOT>& eval;
    int maxGen;

};

#endif	/* HILL_CLIMBING_H */

