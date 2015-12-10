Install ParadisEO, Add include+library

There are 2 text files named io.txt and param.txt for specifying different parameters
Parameters with default value(pSwap, pMut, pCross, seed ) are be preceded by #: 
Dataset name:
io.txt => input <name>
Number of processors:
param.txt => processors <#>
Total running time:
param.txt => maxTime <#seconds>
Method:
io.txt => algo <pop/single>
To run GA:
io.txt => algo pop
To run HC Normal:
io.txt => algo single
param.txt => rExploit 0
To run HC Hybrid:
io.txt => algo single
param.txt => rExploit 0.7
Netbeans C++ proj
Inside orchestration folder there is a folder named data that contains the benchmark data.
##################
This is a Netbeans C++ Project created in Ubuntu 14.04 64-bit.  

At first Follow the steps:
1. Install g++
2. Install Oracle JDK Latest version
3. Install Netbeans IDE 8.0 
4. Install ParadisEO for Linux using Deb package found in http://paradiseo.gforge.inria.fr/index.php?n=Download.Download
5. Then open the project orchestration from Netbeans IDE 8.0
6. Resolve 

You 
There are 2 text files named io.txt and param.txt inside orchestration folder for specifying different parameters. 
Parameters with default value(pSwap, pMut, seed ) are preceded by #.
The most important parameter is rExploit in param.txt. 

Some important parameters:
Benchmark name:
io.txt => input <name of the benchmark>
Number of processors:
param.txt => processors <#>
Total running time:
param.txt => maxTime <#seconds>
Rate of exploitation:
param.txt => rExploit <rate[0,1]>