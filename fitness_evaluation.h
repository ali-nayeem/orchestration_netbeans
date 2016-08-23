/* 
 * File:   fitness_evaluation.h
 * Author: MAN
 *
 * Created on October 10, 2014, 7:35 PM
 */

#ifndef FITNESS_EVALUATION_H
#define	FITNESS_EVALUATION_H

template <class EOT>
class MappingEvalFunc : public eoEvalFunc<EOT>
{
public:
    /// Ctor - no requirement
    // START eventually add or modify the anyVariable argument
    static long int fitEvalCount;

    MappingEvalFunc()
    {
        // START Code of Ctor of an MappingEvalFunc object
        // END   Code of Ctor of an MappingEvalFunc object
    }

    /** Actually compute the fitness
     *
     * @param EOT & _eo the EO object to evaluate
     *                  it should stay templatized to be usable
     *                  with any fitness type
     */
    void operator()(EOT & _eo)
    {
        fitEvalCount++; //count all eval
        // test for invalid to avoid recomputing fitness of unmodified individuals
        if (_eo.invalid())
        {
            // START Code of computation of fitness of the RouteSet object
            fitEvalCount++;
            vector <double> load = vector<double>(PROCESSORS, 0);
            //vector <unsigned> modifiedProcList;
            for (unsigned actor = 0; actor < ACTORS; actor++)
            {
                unsigned holder = _eo[actor];
                //if (_eo.modified(holder))
                {
                   // modifiedProcList.push_back(holder);
                    load[holder] += EXECUTION_TIME[actor];

                    for (int i = 0; i < COST_ADJ_LIST[actor].size(); i++)
                    {
                        unsigned actorNeighbor = COST_ADJ_LIST[actor][i];
                        if (_eo.contains(holder, actorNeighbor) == false)
                        {
                            load[holder] += COST_ADJ_MAT[actor][actorNeighbor];
                        }
                    }
                }
            }

            for (int proc = 0; proc < PROCESSORS; proc++)
            {
               // if (_eo.modified(proc))
                {
                    _eo.setLoad(proc, load[proc]);
                }
            }
            _eo.fitness(_eo.findMinMax());

            //_eo.fitness(); 
        }
    }
    void static clearFitEvalCount()
    {
        fitEvalCount = 0;
    }
    
    long int static getFitEvalCount()
    {
        return fitEvalCount;
    }

private:
    // START Private data of an MappingEvalFunc object
    // END   Private data of an MappingEvalFunc object
};

#endif	/* FITNESS_EVALUATION_H */

