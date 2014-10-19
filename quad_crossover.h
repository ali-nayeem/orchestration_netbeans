/* 
 * File:   quad_crossover.h
 * Author: MAN
 *
 * Created on October 19, 2014, 9:10 AM
 */

#ifndef QUAD_CROSSOVER_H
#define	QUAD_CROSSOVER_H
#include <eoOp.h>
template<class GenotypeT>
class MappingQuadCrossover : public eoQuadOp<GenotypeT>
{
public:
    /**
     * Ctor - no requirement
     */
    // START eventually add or modify the anyVariable argument

    MappingQuadCrossover()
    //  MappingQuadCrossover( varType  _anyVariable) : anyVariable(_anyVariable)
    // END eventually add or modify the anyVariable argument
    {
        // START Code of Ctor of an eoOneMaxEvalFunc object
        // END   Code of Ctor of an eoOneMaxEvalFunc object
    }

    /// The class name. Used to display statistics

    std::string className() const
    {
        return "MappingQuadCross0ver";
    }

    /**
     * eoQuad crossover - modifies both parents
     * @param _genotype1 The first parent
     * @param _genotype2 The second parent
     */
    bool operator()(GenotypeT& _genotype1, GenotypeT& _genotype2)
    {
        bool modified(false);
        unsigned p1,p2;
        // START code for crossover of _genotype1 and _genotype2 objects
        for (int actor = 0; actor < ACTORS; actor++)
        {
            if (param["pCross"] >= rng.uniform())
            {
                p1 = _genotype1[actor];
                p2 = _genotype2[actor];
                _genotype1.assignActor(actor,p2);
                _genotype2.assignActor(actor,p1);
                modified = true;

            }
        }
        /** Requirement
         * if (at least one genotype has been modified) // no way to distinguish
         *     oneAtLeastIsModified = true;
         * else
         *     oneAtLeastIsModified = false;
         */
        return modified;
        // END code for crossover of _genotype1 and _genotype2 objects
    }

private:
    // START Private data of an MappingQuadCrossover object
    //  varType anyVariable;		   // for example ...
    // END   Private data of an MappingQuadCrossover object
};

#endif	/* QUAD_CROSSOVER_H */

