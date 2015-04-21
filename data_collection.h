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
#define E 2.7183


using namespace std;

struct ActorLoad
{

    ActorLoad(unsigned _actor, double _load) : actor(_actor), load(_load)
    {
    }

    unsigned actor;
    double load;
};
vector <ActorLoad> actorLoadList;

bool comparator(ActorLoad i, ActorLoad j)
{
    return (i.load > j.load);
}
vector <unsigned> *COST_ADJ_LIST;
double ** COST_ADJ_MAT;
double * EXECUTION_TIME, SERIAL_LOAD = 0.0, *ACTOR_LOAD;
unsigned ACTORS, EDGES, PROCESSORS, *ACTOR_LIST;
ofstream stat;

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
template <class  Type> void printParam(Type param,ostream & out)
{
    typename Type::iterator it;
    out<<"Final parameters:";
    for(it = param.begin(); it != param.end(); it++)
    {
        out<< (*it).first <<"="<< (*it).second <<",";
    }
    out<<endl;
}
inline string stringify(double x)
{
  ostringstream o;
  if (!(o << x))
  {
      cout<<"Problem in double to string conversion.";
      exit(0);
  }
  return o.str();
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
    string path = "data/"+io["input"]+"/";
    io["path"] = path;
    io["input"] = path + io["input"] + "-inputga.dat"; //data/DES/DES-inputga.dat
    string tweak = (param["rExploit"] == 0)? "normal_tweak":"hybrid_tweak";
    io["config"] = "p" + stringify(param["processors"]) + "-" + io["algo"]+ "_method" + "-" + tweak + "-" ;
    io["stat"] = path + io["config"] +io["stat"];
    
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
    if(fin.is_open() == false)
    {
        cout<<"Error opening data file"<<endl;
        exit(0);
    }
    unsigned u, v;
    double w;
    fin >> PROCESSORS;
    fin >> ACTORS >> EDGES;

    COST_ADJ_LIST = new vector<unsigned>[ACTORS];
    COST_ADJ_MAT = new double* [ACTORS];
    EXECUTION_TIME = new double[ACTORS];
    ACTOR_LOAD = new double[ACTORS];
    ACTOR_LIST = new unsigned[ACTORS];
    SERIAL_LOAD = 0;
    for (unsigned i = 0; i < ACTORS; i++)
    {
        ACTOR_LIST[i] = i;
        COST_ADJ_MAT[i] = new double[ACTORS];
        fin >> EXECUTION_TIME[i];
        //cout << EXECUTION_TIME[i] << endl;
        SERIAL_LOAD += EXECUTION_TIME[i];
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
    for (int i = 0; i < ACTORS; i++)
    {
        //cout << i << "(" << EXECUTION_TIME[i] << ")" << ":";
        ACTOR_LOAD[i] = EXECUTION_TIME[i];
        for (int j = 0; j < COST_ADJ_LIST[i].size(); j++)
        {
            // cout << COST_ADJ_LIST[i][j] << "(" << COST_ADJ_MAT[i][COST_ADJ_LIST[i][j]] << ")" << "-";
            ACTOR_LOAD[i] += COST_ADJ_MAT[i][COST_ADJ_LIST[i][j]];
        }
        actorLoadList.push_back(ActorLoad(i, ACTOR_LOAD[i]));
        //test
       // cout << ACTOR_LOAD[i] << "-";
    }
    sort(actorLoadList.begin(), actorLoadList.end(), comparator);
//    for (int i = 0; i < ACTORS; i++)
//    {
//        cout<<actorLoadList[i].actor<<"="<<actorLoadList[i].load<<"-";
//    }

    //printAdjInfo();
    fin.close();
}

void autoAdjustParam()
{
    if(param["processors"] != 0)
    {
        PROCESSORS = param["processors"];
    }
    if(param["pSwap"] == 0)
    {
        param["pSwap"] = 2.4 * PROCESSORS / ACTORS;
    }
    if(param["pMut"] == 0)
    {
        param["pMut"] = 3.0 / ACTORS;
    }
    if(param["pCross"] == 0)
    {
        param["pCross"] = param["pMut"];
    }
    if(param["seed"] == 0)
    {
        param["seed"] = time(0);
    }
    
}

void gatherAllInfo()
{
    readParam();
    readIO();
    readData();
    autoAdjustParam();
    printParam(param,cout);
    printParam(io,cout);
    ofstream temp((io["path"]+io["config"]+"param.txt").c_str());
    printParam(param,temp);
    temp.close();
    //    floydWarshallWithPath();
    //    findDS();
}