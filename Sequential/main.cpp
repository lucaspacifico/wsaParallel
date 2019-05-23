#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <locale>
#include <math.h>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/resource.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <vector>
#include <string.h>
#include "graphDefinitions.h"
#include "struct.cpp"

using namespace std;

unsigned char packedVectorSettled           [_NODE_COUNT];

unsigned int  packedVectorNodes             [_NODE_COUNT],
              packedVectorEdges             [_EDGE_COUNT],
              packedVectorEdgesStartingNodes[_EDGE_COUNT];

int           packedVectorPosNode[_EDGE_COUNT];



int           packedVectorPredecessors      [_NODE_COUNT];

float         packedVectorEdgeWeights       [_EDGE_COUNT],
              packedVectorDistances         [_NODE_COUNT];

//Misc variables
bool         displayMessages;
unsigned int iStartingNode;
string       sGraphName;

inline string intToString(const int t)
{
 stringstream ss;
 ss << t;
 return ss.str();
}

inline string doubleToString(const double t)
{
 stringstream ss;
 ss << t;
 return ss.str();
}

int getLowerBound(int u)
{ return packedVectorNodes[u]; }

int getUpperBound(int u)
{
 //Since we are handling an undirected graph, the last vertex has not
 //dissidents edges (ie, to its neighbors)
 if (u == (_NODE_COUNT - 1))
 { return _NODE_COUNT - 1; }
 else
 { return packedVectorNodes[u + 1] - 1; }
}

int loadGraph(string sFileName)
{
  unsigned int PosValue = 0;
 ifstream inputFile(sFileName.c_str());
 char inputLine[100];
 uint iCounter = 0,
      iStartingPoint = 0,
      iCurrentNode = (uint) -1;
 if (inputFile)
 {
  //Read edges and populate data structures
  while (!inputFile.eof())
  {
   memset(inputLine, '\0', sizeof(inputLine));
   inputFile.getline(inputLine, 160);
   int k=1;
   while (inputLine[k] != ' ' && inputLine[k]!='\0')
   { k++; }
   string inpstr = inputLine;
   if(inpstr.empty()) break;
   unsigned int iStartingNode = (unsigned int) atoi(const_cast<char*>(inpstr.substr(0, k).c_str()));

   int l=k+1;
   while (inputLine[l] != ' ' && inputLine[l]!='\0')
   { l++; }
   unsigned int iEndingNode = (unsigned int) atoi(const_cast<char*>(inpstr.substr(k + 1, l).c_str()));

   k=l+1;
   while (inputLine[k] != ' ' && inputLine[k]!='\0')
   { k++; }
   float fWeight = (float) atof(const_cast<char*>(inpstr.substr(l + 1, k).c_str()));
   //cout << "fWeight =" << fWeight << endl;
   packedVectorEdgesStartingNodes[iCounter] = iStartingNode;
   packedVectorEdges             [iCounter] = iEndingNode;
   packedVectorEdgeWeights       [iCounter] = fWeight;

   if(iStartingNode == PosValue){
    //  cout << PosValue << "->"  << iCounter << endl;
     packedVectorPosNode[PosValue] = iCounter;
     PosValue++;
   }

   if (iCounter == 0)
   {
    iCurrentNode = iStartingNode;
    packedVectorNodes[iStartingPoint] = iCounter;
   }
   else
   {
    if (iCurrentNode != iStartingNode)
    {
     iCurrentNode = iStartingNode;
     packedVectorNodes[++iStartingPoint] = iCounter;
    }
   }
   iCounter++;
  }
  if (displayMessages)
  { cout << "A graph with " << iCounter << " edges was loaded..." << endl; }
  return 0;
 }
 else
 {
  cout << "Could not open input data..." << endl;
  return 1;
 }
}

void initializeDataStructures()
{
 //
}

void showPackedData()
{
 cout << endl << "packedVectorEdges" << endl;
 for (int i = 0; i < _EDGE_COUNT; i++)
 { cout << i << ": " << packedVectorEdgesStartingNodes[i] << " -> " << packedVectorEdges[i] << "    weight: " << packedVectorEdgeWeights[i] << endl; }

 for(int i = 0 ; i < _EDGE_COUNT ; ++i)
   cout << "Node [" << i << "] >: " << packedVectorPosNode[i] << endl;
}


int main(int argc, char **argv)
{
uint K;
if(argc < 2)
cout << "ENTRY ERROR. ";
else{
K = strtol(argv[1],NULL,10);
 displayMessages = false;
 loadGraph(_GRAPH_NAME);
 // showPackedData();
 Find_WSA(packedVectorEdgesStartingNodes,packedVectorEdges,packedVectorEdgeWeights,K);
 }
 return 0;
}
