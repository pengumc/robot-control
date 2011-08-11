#include "robot-control/CGraph.h"

CGraph::CGraph(){
    //set all data points to 0
    memset((void*)data, 0, sizeof(GRAPH_DATA_TYPE)*GRAPH_DATA_LENGTH);
    
    index =0;
 
}

uint16_t CGraph::addPoint(GRAPH_DATA_TYPE value){
    index++;
    if(index >= GRAPH_DATA_LENGTH) index = 0;
    data[index] = value;
    return index;
}
