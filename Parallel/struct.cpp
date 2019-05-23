#include <bits/stdc++.h>
#include "graphDefinitions.h"
#include <omp.h>
using namespace std;

#define NUM_THREADS 7


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
}Water;

typedef vector<Split> SplitVector;
typedef map<int, SplitVector> MyMapSplitClass;

typedef vector<Water> WaterVector;
typedef map<double, WaterVector> MyWaterClass;



bool double_equals(double a, double b, double epsilon = 0.001)
{
    return std::abs(a - b) < epsilon;
}

/****************************************************************************/
/*                    PARALLEL STRUCTURE                                    */
/****************************************************************************/


int insert_split_list(MyMapSplitClass &Aux_Split, unsigned int aux_DieNode,unsigned int aux_BornNode, double aux_ReachTime,unsigned int  aux_ParentSplitIndex){
  for(vector<Split>::iterator insertIterator = Aux_Split[aux_DieNode].begin(); insertIterator != Aux_Split[aux_DieNode].end() ; insertIterator++){
    if(double_equals(aux_ReachTime,insertIterator->SpliTime) && aux_BornNode == insertIterator->ParentNode  && aux_ParentSplitIndex == insertIterator->ParentSplitIndex){
    return 0;
    }
  }

  Aux_Split[aux_DieNode].push_back({aux_BornNode,aux_ParentSplitIndex,aux_ReachTime});

  return 1;


}


/****************************************************************************/
/*                    MOVE CONTENT PARALLEL                                 */
/****************************************************************************/
void move_water(MyWaterClass &Sequential, MyWaterClass &Parallel){
std::map<double, WaterVector>::iterator Point_P = Parallel.begin();

  for(;Point_P != Parallel.end() ; ++Point_P){
    for(std::vector<Water>::iterator aux = Point_P->second.begin() ; aux != Point_P->second.end() ; ++aux){
      Sequential[aux->ReachTime].push_back({aux->BornNode,aux->DieNode,aux->ReachTime,aux->ParentSplitIndex});
    }
    Point_P->second.clear();
  }
Parallel.clear();
}

/****************************************************************************/
/*                    PRINT SPLIT LIST                                      */
/****************************************************************************/
void print_split(MyMapSplitClass SplitList){
  for (int i = 0; i < _NODE_COUNT; i++) {
   cout << "\nSize : "<<SplitList[i].size() <<endl << "[" << i << "] ";
    for (std::vector<Split>::iterator normalIterator = SplitList[i].begin() ; normalIterator != SplitList[i].end(); ++normalIterator)
        std::cout << "("<<normalIterator->ParentNode <<","<< normalIterator->ParentSplitIndex <<","<<normalIterator->SpliTime << ");" ;
        cout << endl;
    }
    return ;
}

void print_water(MyWaterClass WaterList){

  for(std::map<double, WaterVector>::iterator normalIterator = WaterList.begin() ; normalIterator != WaterList.end() ; ++normalIterator){
    cout << normalIterator->first << endl;
    for(std::vector<Water>::iterator aux = normalIterator->second.begin() ; aux != normalIterator->second.end() ; ++aux){
      cout << "("<<aux->BornNode << ","<<aux->DieNode << ","<<aux->ReachTime << ","<<aux->ParentSplitIndex <<  ");" << endl;
    }
  }
    return ;
}
/****************************************************************************/
/*              WSA                                                         */
/****************************************************************************/
void Find_WSA (unsigned int * packedVectorEdgesStartingNodes, unsigned int * packedVectorEdges,float * packedVectorEdgeWeights,vector<pair<uint,uint>> packedVectorPosNode, unsigned int K) {

  MyWaterClass WaterList, ParallelWaterList; // variables used to maintain the water list
  std::map<int, float> KeepValues;
  double max_value;


  MyMapSplitClass SplitList;
  unsigned int pos;

  #pragma omp parallel for schedule(static) shared(WaterList) num_threads(NUM_THREADS)
  for(uint i = packedVectorPosNode[0].first ; i < packedVectorPosNode[0].second ; ++i){
    #pragma omp critical
    {
    WaterList[packedVectorEdgeWeights[i]].push_back({packedVectorEdgesStartingNodes[i],packedVectorEdges[i],packedVectorEdgeWeights[i],(unsigned int)SplitList[packedVectorEdgesStartingNodes[i]].size()}) ;
    KeepValues[packedVectorEdges[i]] = packedVectorEdgeWeights[i];
    }
  }

  for(std::map<int, float>::iterator aux = KeepValues.begin() ;  aux != KeepValues.end() ; ++aux){
    #pragma omp parallel for schedule(static) shared(ParallelWaterList) num_threads(NUM_THREADS)
    for(uint i = packedVectorPosNode[aux->first].first ; i < packedVectorPosNode[(aux->first)].second ; ++i){
      #pragma omp critical
      {
        ParallelWaterList[(packedVectorEdgeWeights[i]+ aux->second)].push_back({packedVectorEdgesStartingNodes[i],packedVectorEdges[i],(packedVectorEdgeWeights[i]+ aux->second)
          ,(unsigned int)SplitList[packedVectorEdgesStartingNodes[i]].size()});
      }
    }
  }

while(!WaterList.empty()){
  if(WaterList.begin()->first >= ParallelWaterList.begin()->first){
    max_value = WaterList.begin()->first;
  }else{
    max_value = ParallelWaterList.begin()->first ;
  }

  move_water(WaterList, ParallelWaterList);

  for(std::map<double, WaterVector>::iterator current = WaterList.begin() ; current->first <= max_value ; ++current){
    if(current->first == 0 && WaterList.size() == 0 )
    break;

    for(std::vector<Water>::iterator aux = current->second.begin() ; aux != current->second.end() ; ++aux){
      if(SplitList[aux->DieNode].size() < K){
        if(insert_split_list(SplitList,aux->DieNode, aux->BornNode, aux->ReachTime,(aux->ParentSplitIndex == 0) ? 0 : (aux->ParentSplitIndex - 1)) )
        {
          #pragma omp parallel for schedule(static) shared(ParallelWaterList) num_threads(NUM_THREADS)
          for(uint i =  packedVectorPosNode[aux->DieNode].first ; i < packedVectorPosNode[(aux->DieNode)].second ; ++i){
            #pragma omp critical
            ParallelWaterList[(packedVectorEdgeWeights[i] + aux->ReachTime)].push_back({packedVectorEdgesStartingNodes[i],packedVectorEdges[i]
              ,packedVectorEdgeWeights[i] + aux->ReachTime,(unsigned int) SplitList[packedVectorEdgesStartingNodes[i]].size()});
          } // end for 144
        } // end if 142
        

      } // end if 140
    } // end for 139

    if(current->first == 0 && WaterList.size() == 0 ){
    break;
    } // end if 153
    else{
    WaterList.erase(current);
    } // end else 158
    // cout << "174..." << endl;
  }// end for 138
} // end while 128

// print_split(SplitList);
// print_water(WaterList);

}



/****************************************************************************/
/*             PRINT PATH FROM GIVEN SOURCE TO NODE                         */
/****************************************************************************/
