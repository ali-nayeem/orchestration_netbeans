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