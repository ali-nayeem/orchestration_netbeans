/* 
 * File:   initialization.h
 * Author: MAN
 *
 * Created on October 10, 2014, 8:58 AM
 */

#ifndef INITIALIZATION_H
#define	INITIALIZATION_H


ptrdiff_t myRandom (ptrdiff_t i) { return rng.random(i);}

template <class Indi> void fairRandom(Indi & _indi)
{
    vector<unsigned> actorVec(ACTOR_LIST, ACTOR_LIST + ACTORS);
    //printVec(actorVec);
    random_shuffle ( actorVec.begin(), actorVec.end(), myRandom);
    for(unsigned i = 0; i < ACTORS; i++)
    {
        _indi.assignActor(actorVec[i] , i % PROCESSORS);
    }
}

template <class Indi> void fullyRandom(Indi & _indi)
{
    for(unsigned i = 0; i < ACTORS; i++)
    {
        _indi.assignActor(i , rng.random(PROCESSORS));
    }
}
#endif	/* INITIALIZATION_H */

