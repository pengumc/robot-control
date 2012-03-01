#include "robot-control/Graph.h"

//------------
// CONSTRUCTOR
//------------
Graph::Graph(){
    //set all data points to 0
    memset((void*)data, 0, sizeof(GraphValue)*GRAPH_DATA_LENGTH);
    
    index =0;
 
}



//---------------
// ADD DATA POINT
//---------------
GraphIndex
Graph::addPoint(GraphValue graphValue){
    index++;
    if(index >= GRAPH_DATA_LENGTH) index = 0;
    data[index] = graphValue;
    return index;
}



//----------
// GET INDEX
//----------
GraphIndex
Graph::getIndex(){
    return index;
}



//-------------------
// GET VALUE AT INDEX
//-------------------
GraphValue
Graph::getValue(GraphIndex requestedIndex){
    if (requestedIndex < GRAPH_DATA_LENGTH)
        return data[requestedIndex];
    else
        return 0;
}
