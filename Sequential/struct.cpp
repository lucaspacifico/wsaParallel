#include <bits/stdc++.h>
#include "graphDefinitions.h"
using namespace std;

/****************************************************************************/
/*                    Structure of  WSA                                     */
/****************************************************************************/

typedef struct Split{
unsigned int ParentNode;
unsigned int ParentSplitIndex;
double SpliTime;
}Split;

typedef struct Water
{
    unsigned int BornNode;
    unsigned int DieNode;
    double ReachTime;
    unsigned int ParentSplitIndex;

    bool operator <(const Water &lop) const{
        bool b = ReachTime <= lop.ReachTime;
        return (b);
    }
}Water;

typedef vector<Split> SplitVector;
typedef map<int, SplitVector> MyMapVectorClass;
/****************************************************************************/
/*                    PRINT SPLIT LIST                                      */
/****************************************************************************/
void print_split(MyMapVectorClass SplitList){
  for (int i = 0; i < _NODE_COUNT; i++) {
   cout << "\nSize : "<<SplitList[i].size() <<endl << "[" << i << "] ";
    for (std::vector<Split>::iterator normalIterator = SplitList[i].begin() ; normalIterator != SplitList[i].end(); ++normalIterator)
        std::cout << "("<<normalIterator->ParentNode <<","<< normalIterator->ParentSplitIndex <<","<<normalIterator->SpliTime << ");" ;
        cout << endl;
    }
    return ;
}
/****************************************************************************/
/*                         BINARY SEARCH                                    */
/****************************************************************************/
unsigned int binary_search(unsigned int * packedVectorEdgesStartingNodes,unsigned int Node){
  unsigned int first = 0;
  unsigned int last = _EDGE_COUNT - 1;
  unsigned int middle = 0;

  if(Node == 0){
  middle = 0;
  }else{
    while(first <= last){
      middle = ( first + last )/2;
      if(packedVectorEdgesStartingNodes[middle] < Node){
        first = ++middle;
      }else if(packedVectorEdgesStartingNodes[middle] == Node){
        for(;packedVectorEdgesStartingNodes[middle] == packedVectorEdgesStartingNodes[middle- 1];middle--);
        break;
      }else{
        last = middle;
      }
    } // end while
  }//end else
  return middle;
}
/****************************************************************************/
/*              WSA                                                         */
/****************************************************************************/
void Find_WSA (unsigned int * packedVectorEdgesStartingNodes, unsigned int * packedVectorEdges,float * packedVectorEdgeWeights, unsigned int K) {

  set<Water> WaterList;
  std::set<Water>::iterator it;

  MyMapVectorClass SplitList;
  unsigned int pos;
  Water w1;
  vector <int> t;

  for(unsigned int i = 0 ; packedVectorEdgesStartingNodes[0] == packedVectorEdgesStartingNodes[i] ; ++i)
    WaterList.insert({packedVectorEdgesStartingNodes[i],packedVectorEdges[i],packedVectorEdgeWeights[i],(unsigned int)SplitList[packedVectorEdgesStartingNodes[i]].size() });
 while(!WaterList.empty())
  {

    it = WaterList.begin();
    w1 = *it;
    WaterList.erase(it++);

    pos = binary_search(packedVectorEdgesStartingNodes,w1.DieNode);

    if(SplitList[w1.DieNode].size() < K){
        SplitList[w1.DieNode].push_back({w1.BornNode,(w1.ParentSplitIndex == 0) ? 0 : (w1.ParentSplitIndex - 1) ,w1.ReachTime});
        for ( unsigned int i = ((pos <= 0) ? 0 : pos) ; packedVectorEdgesStartingNodes[i] == w1.DieNode; i++)
          WaterList.insert({packedVectorEdgesStartingNodes[i],packedVectorEdges[i],packedVectorEdgeWeights[i] + w1.ReachTime,(unsigned int) SplitList[packedVectorEdgesStartingNodes[i]].size()});
    }

  }
   print_split(SplitList);


}


/****************************************************************************/
/*             PRINT PATH FROM GIVEN SOURCE TO NODE                         */
/****************************************************************************/

// void Print_Shortest_Path(unsigned int Dest_Node,   MyMapVectorClass SplitList){
//   // Destination Node = Node
//   vector<Split>::iterator iterator = SplitList[0].begin();
//   Split s;
//   unsigned int u,v,pos;
//   unsigned int N = SplitList[0].size();
//
//   vector<int> path_list; // R : empty Path
//   vector<int> list; // p : empty list
//
//   v = Dest_Node;
//
//   for (unsigned int i = 0; i < N; i++) {
//     list.clear();
//     u = Dest_Node;
//     v = (iterator + i)->ParentNode;
//     s = *iterator;
//     pos = (iterator + i)->ParentSplitIndex;
//
//     while (pos != 0 AND v != 0) {
//       list.push_back(u);
//       u = v;
//       s
//     }
//     /* code */
//   }










// }
