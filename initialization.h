/* 
 * File:   initialization.h
 * Author: MAN
 *
 * Created on October 10, 2014, 8:58 AM
 */

#ifndef INITIALIZATION_H
#define	INITIALIZATION_H

ptrdiff_t myRandom(ptrdiff_t i)
{
    return rng.random(i);
}

template <class Indi> void fairRandom(Indi & _indi)
{
    vector<unsigned> actorVec(ACTOR_LIST, ACTOR_LIST + ACTORS);
    //printVec(actorVec);
    random_shuffle(actorVec.begin(), actorVec.end(), myRandom);
    for (unsigned i = 0; i < ACTORS; i++)
    {
        _indi.assignActor(actorVec[i], i % PROCESSORS);
    }
}
//to be completed
//template <class Indi> void fairRandomAdjacent(Indi & _indi)
//{
//    vector<unsigned> actorVec(ACTOR_LIST, ACTOR_LIST + ACTORS);
//    //printVec(actorVec);
//    int share = ACTORS/PROCESSORS;
//    random_shuffle(actorVec.begin(), actorVec.end(), myRandom);
//    
//};

template <class Indi> void fairAdjacent(Indi & _indi)
{
    vector<bool> isDistributed = vector<bool>(ACTORS, false);
    unsigned curProc = 0;
    for (unsigned i = 0; i < ACTORS; i++)
    {
        unsigned actor = actorLoadList[i].actor;
        if (isDistributed[actor] == false)
        {
            unsigned curProc = curProc % PROCESSORS;
            _indi.assignActor(actor, curProc);
            isDistributed[actor] = true;

            for (int j = 0; j < COST_ADJ_LIST[actor].size(); j++)
            {

                unsigned neighbor = COST_ADJ_LIST[actor][j];
                if (isDistributed[neighbor] == false)
                {
                    _indi.assignActor(neighbor, curProc);
                    isDistributed[neighbor] = true;
                }
            }
            curProc++;
        }
    }
}

template <class Indi> void fullyRandom(Indi & _indi)
{
    for (unsigned i = 0; i < ACTORS; i++)
    {
        _indi.assignActor(i, rng.random(PROCESSORS));
    }
}


#endif	/* INITIALIZATION_H */

