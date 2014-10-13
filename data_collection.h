/* 
 * File:   data_collection.h
 * Author: MAN
 *
 * Created on October 8, 2014, 7:04 PM
 */

#ifndef DATA_COLLECTION_H
#define	DATA_COLLECTION_H

#include <climits>
#include <list>


using namespace std;

vector <unsigned> *COST_ADJ_LIST;
double ** COST_ADJ_MAT;
double * EXECUTION_TIME;
unsigned ACTORS, EDGES, PROCESSORS, *ACTOR_LIST;

map<string, float> param; //list of parameters
map<string, string> io; //list of io filenames

#endif	/* DATA_COLLECTION_H */

void readParam()
{
    ifstream fin("param.txt");
    string name;
    float value;
    while (!fin.eof())
    {
        fin >> name >> value;
        //cout << name << ":" << value << ",";
        param[name] = value;
        cout << name << "=" << param[name] << endl;
    }
    fin.close();

}

void readIO()
{
    ifstream fin("io.txt");
    string name;
    string value;
    while (!fin.eof())
    {
        fin >> name >> value;
        //cout << name << ":" << value << ",";
        io[name] = value;
        cout << name << "=" << io[name] << endl;
    }
    fin.close();

}

template <class X> void print2dArray(X ** array, unsigned dim, unsigned row = -1)
{
    if (row == -1)
    {
        row = dim;
    }
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            cout << array[i][j] << ",";
        }
        cout << endl;
    }
}

template <class X> void printVec(vector<X> &v)
{
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << "-";
    }
    cout << endl;
}
void printAdjInfo()
{
     for (int i = 0; i < ACTORS; i++)
    {
        cout << i << "(" << EXECUTION_TIME[i] << ")" << ":";
        for (int j = 0; j < COST_ADJ_LIST[i].size(); j++)
        {
            cout << COST_ADJ_LIST[i][j] << "(" << COST_ADJ_MAT[i][COST_ADJ_LIST[i][j]] << ")" << "-";
        }
        cout << endl;
    }
}
void readData()
{
    ifstream fin(io["input"].c_str());
    unsigned u, v;
    double w;
    fin >> PROCESSORS;
    fin >> ACTORS >> EDGES;

    COST_ADJ_LIST = new vector<unsigned>[ACTORS];
    COST_ADJ_MAT = new double* [ACTORS];
    EXECUTION_TIME = new double[ACTORS];
    ACTOR_LIST = new unsigned[ACTORS];
    for (unsigned i = 0; i < ACTORS; i++)
    {
        ACTOR_LIST[i] = i;
        COST_ADJ_MAT[i] = new double[ACTORS];
        fin >> EXECUTION_TIME[i];
        cout << EXECUTION_TIME[i] << endl;
    }
    for (int i = 0; i < ACTORS; i++)
    {
        COST_ADJ_MAT[i][i] = 0;
        for (int j = i + 1; j < ACTORS; j++)
        {
            COST_ADJ_MAT[i][j] = COST_ADJ_MAT[j][i] = 0;
        }
    }
    for (int i = 0; i < EDGES; i++)
    {
        fin >> u >> v >> w;
        COST_ADJ_MAT[u - 1][v - 1] = w;
        COST_ADJ_LIST[u - 1].push_back(v - 1);
    }

    //test
   // print2dArray(COST_ADJ_MAT, ACTORS);
//    for (int i = 0; i < ACTORS; i++)
//    {
//        cout << i << "(" << EXECUTION_TIME[i] << ")" << ":";
//        for (int j = 0; j < COST_ADJ_LIST[i].size(); j++)
//        {
//            cout << COST_ADJ_LIST[i][j] << "(" << COST_ADJ_MAT[i][COST_ADJ_LIST[i][j]] << ")" << "-";
//        }
//        cout << endl;
//    }
    
    //printAdjInfo();
    fin.close();
}

void gatherAllInfo()
{
    readParam();
    readIO();
    readData();
    //    floydWarshallWithPath();
    //    findDS();
}