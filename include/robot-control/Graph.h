#ifndef __GRAPH_CPP__
#define __GRAPH_CPP__
#include <stdint.h>
#include <string.h> //for memset


#define GRAPH_DATA_LENGTH 300

typedef int8_t GraphValue;
typedef uint16_t GraphIndex;

class Graph{
    public:
        Graph();
        GraphIndex addPoint(GraphValue value);
        GraphIndex getIndex();
        GraphValue getValue(GraphIndex requestedIndex);
        
    private:
        GraphValue data[GRAPH_DATA_LENGTH];
        GraphIndex index;
        
};

#endif

