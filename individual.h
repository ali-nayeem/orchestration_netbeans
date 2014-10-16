/* 
 * File:   individual.h
 * Author: MAN
 *
 * Created on October 9, 2014, 7:24 PM
 */

#ifndef INDIVIDUAL_H
#define	INDIVIDUAL_H

#include<EO.h>
#include<cstring>
#include<math.h>





using namespace std;

template< class FitT>
class Mapping : public EO<FitT>
{
public:

    /** Ctor: you MUST provide a default ctor.
     * though such individuals will generally be processed
     * by some eoInit object
     */
    Mapping()
    {
        // START Code of default Ctor of an Mapping object
        //actor2processor = new unsigned[ACTORS];
        //memset(actor2processor, 0, ACTORS);
        //isModified = new bool[PROCESSORS];

        actor2processor = vector<unsigned>(ACTORS, 0);
        isModified = vector<bool>(PROCESSORS, true);
        load = vector<double>(PROCESSORS, 0);
        invLoad = vector<double>(PROCESSORS, 0);
        processor2actor = vector < vector<bool> > (PROCESSORS, vector<bool>(ACTORS, false));
        //        for(int i=0;i<PROCESSORS;i++)
        //        {
        //            printVec(processor2actor[i]);
        //        }
        //        processor2actor = new bool* [PROCESSORS];
        //        for (int i = 0; i < PROCESSORS; i++)
        //        {
        //            //isModified[i] = true;
        //            processor2actor[i] = new bool[ACTORS];
        //            memset(processor2actor[i], false, ACTORS);
        //        }
        // print2dArray(processor2actor, ACTORS, PROCESSORS);

        // END   Code of default Ctor of an Mapping object
    }

    virtual ~Mapping()
    {
        // START Code of Destructor 
        // END   Code of Destructor 
    }

    virtual string className() const
    {
        return "Mapping";
    }

    double findMinMax()
    {
        double max, min, sum;
        sum = max = min = load[0];
        maxPointer = minPointer = 0;
        for (int i = 1; i < PROCESSORS; i++) //we also calculate avg here
        {
            if (load[i] > max)
            {
                max = load[i];
                maxPointer = i;
            }
            else
            {
                if (load[i] <= min)
                {
                    min = load[i];
                    minPointer = i;
                }
            }

            sum = sum + load[i];
        }
        avg = sum / (1.0 * PROCESSORS);
        stdDev = standardDeviation(avg);
        return max;
    }

    unsigned maxProcessor()
    {
        if (modified(maxPointer))
        {
            cout << "Max processor may changed.";
            exit(1);
        }
        return maxPointer;
    }

    double avgLoad()
    {
        return avg;
    }

    double stdDevLoad()
    {
        return stdDev;
    }

    double standardDeviation(double _avg)
    {
        double stdDeviation = 0.0;
        double stdSum = 0.0;
        for (int i = 1; i < PROCESSORS; i++)
        {
            stdSum += pow(load[i] - _avg, 2.0);
        }
        stdDeviation = pow((stdSum / (1.0 * PROCESSORS)), 0.5);
        return stdDeviation;
    }

    unsigned assignActor(unsigned _actor, unsigned _newProcessor)
    {
        unsigned oldProcessor = actor2processor[_actor];
        actor2processor[_actor] = _newProcessor;

        processor2actor[oldProcessor][_actor] = false;
        processor2actor[_newProcessor][_actor] = true;

        modify(oldProcessor);
        modify(_newProcessor);

        return oldProcessor;
    }

    bool contains(unsigned _proc, unsigned _actor)
    {
        return processor2actor[_proc][_actor];
    }

    void modify(unsigned _processor)
    {
        isModified[_processor] = true;
        //load[_processor] = invLoad[_processor] = 0;
    }

    void finalize(unsigned _proc)
    {
        isModified[_proc] = false;
    }

    bool modified(unsigned _processor)
    {
        return isModified[_processor];
    }

    void setLoad(unsigned _processor, double _load)
    {
        load[_processor] = _load;
        invLoad[_processor] = 1 / _load;
        finalize(_processor);
    }
    unsigned operator [](int index)
    {
        if (index + 1 > ACTORS || index < 0)
        {
            cout << "Array bound exception inside Mapping " << index;
            exit(1);
        }
        return actor2processor[index];
    }

    unsigned rouletteWheelForMaxProcessor()
    {
        return rng.roulette_wheel(load);
    }

    unsigned rouletteWheelForMinProcessor()
    {
        return rng.roulette_wheel(invLoad);
    }

    unsigned rouletteWheelForMaxProcessor(int _except) //returns a processor except _except
    {
        double savedLoad = load[_except];
        load[_except] = 0; //zero probability
        unsigned chosen = rng.roulette_wheel(load);
        load[_except] = savedLoad;
        return chosen;
    }

    void swap(unsigned _p1, unsigned _p2, unsigned _actor)
    {
        unsigned emptyProc; //the processor currently not having the actor
        if (contains(_p1, _actor) == false)
        {
            emptyProc = _p1;
        }
        else
        {
            emptyProc = _p2;
        }
        assignActor(_actor, emptyProc);

    }

    /** printing... */
    void printOn(ostream& _os) const
    {
        // First write the fitness
        // EO<FitT>::printOn(_os);

        // START Code of default output
        for (int actor = 0; actor < ACTORS; actor++)
        {
            _os << actor2processor[actor] << " ";
        }
        /** HINTS
         * in EO we systematically write the sizes of things before the things
         * so readFrom is easier to code (see below)
         */

        // END   Code of default output
    }

    /** reading...
     * of course, your readFrom must be able to read what printOn writes!!!
     */
    void readFrom(istream& _is)
    {
        // of course you should read the fitness first!
        // EO<FitT>::readFrom(_is);
        // START Code of input
        unsigned processor;
        for (int actor = 0; actor < ACTORS; actor++)
        {
            _is >> processor;
            assignActor(actor,processor);
        }
        /** HINTS
         * remember the Mapping object will come from the default ctor
         * this is why having the sizes written out is useful
         */

        // END   Code of input
    }

    void print()
    {
        for (int j = 0; j < ACTORS; j++)
        {
            cout << j << "|";
        }
        cout << endl << "-----------------------------------------" << endl;
        for (int i = 0; i < PROCESSORS; i++)
        {

            for (int j = 0; j < ACTORS; j++)
            {
                cout << processor2actor[i][j] << "|";
            }
            cout << " : " << load[i] << endl;
        }
        cout << "-----------------------------------------" << endl;
        for (int i = 0; i < ACTORS; i++)
        {
            cout << actor2processor[i] << "|";
        }
        cout << endl;
    }

    // accessing and setting values


private: // put all data here
    // START Private data of an Mapping object
    unsigned maxPointer, minPointer; // * actor2processor;
    vector<unsigned> actor2processor;
    //bool *isModified, **processor2actor;
    vector<bool> isModified;
    vector< vector<bool> > processor2actor;
    double avg, stdDev;
    vector<double> load, invLoad;

    // END   Private data of an Mapping object
public:
};

#endif	/* INDIVIDUAL_H */

