/* 
 * File:   tweak.h
 * Author: MAN
 *
 * Created on October 11, 2014, 7:40 PM
 */

#ifndef TWEAK_H
#define	TWEAK_H

#include<cstring>

template<class GenotypeT>
class UniformMonCrossOver : public eoMonOp<GenotypeT>
{
public:
    /**
     * Ctor - no requirement
     */
    // START eventually add or modify the anyVariable argument

    UniformMonCrossOver() 
    //  UniformMonCrossOver( varType  _anyVariable) : anyVariable(_anyVariable)
    // END eventually add or modify the anyVariable argument
    {

        // START Code of Ctor of an UniformMonCrossOver object
        // END   Code of Ctor of an UniformMonCrossOver object
    }

    /// The class name. Used to display statistics

    string className() const
    {
        return "UniformMonCrossOver";
    }

    /**
     * modifies the parent
     * @param _genotype The parent genotype (will be modified)
     */
    bool operator()(GenotypeT & _genotype)
    {
        // START code for mutation of the _genotype object
        bool invalid = false;
        //unsigned maxProc = _genotype.rouletteWheelForMaxProcessor();
        unsigned maxProc = _genotype.maxProcessor();
        unsigned otherProc = rng.random(PROCESSORS);
        while(maxProc == otherProc)
        {
            otherProc = rng.random(PROCESSORS);
        }
        
        for (int actor = 0; actor < ACTORS; actor++)
        {
            if (param["pSwap"] >= rng.uniform())
            {
                if( _genotype.contains(maxProc,actor) != _genotype.contains(otherProc,actor))
                {
                    _genotype.swap(maxProc,otherProc,actor);
                    invalid = true;
                }
            }
        }
        
        return invalid;
        // END code for mutation of the _genotype object
    }

private:
    // START Private data of an UniformMonCrossOver object
  // END   Private data of an UniformMonCrossOver object
};

template<class GenotypeT>
class Mutation : public eoMonOp<GenotypeT>
{
public:
    /**
     * Ctor - no requirement
     */
    // START eventually add or modify the anyVariable argument

    Mutation() 
    //  Mutation( varType  _anyVariable) : anyVariable(_anyVariable)
    // END eventually add or modify the anyVariable argument
    {

        // START Code of Ctor of an Mutation object
        // END   Code of Ctor of an Mutation object
    }

    /// The class name. Used to display statistics

    string className() const
    {
        return "Mutation";
    }

    /**
     * modifies the parent
     * @param _genotype The parent genotype (will be modified)
     */
    bool operator()(GenotypeT & _genotype)
    {
        // START code for mutation of the _genotype object
        bool invalid = false;
        for(unsigned actor = 0; actor < ACTORS; actor++)
        {
            if (param["pMut"] >= rng.uniform())
            {
                unsigned oldProc = _genotype[actor];
                unsigned newProc = rng.random(PROCESSORS);
                while(oldProc == newProc)
                {
                    newProc = rng.random(PROCESSORS);
                }
                _genotype.assignActor(actor,newProc);
                invalid = true;
            }
        }
        return invalid;
        // END code for mutation of the _genotype object
    }

private:
    // START Private data of an Mutation object
     // END   Private data of an Mutation object
};

#endif	/* TWEAK_H */

