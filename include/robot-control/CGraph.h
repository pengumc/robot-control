#ifndef __GRAPH_CPP__
#define __GRAPH_CPP__
#include <stdint.h>
#include <string.h> //for memset

#define GRAPH_DATA_TYPE uint8_t
#define GRAPH_DATA_LENGTH 300

class CGraph{
    public:
        CGraph();
        uint16_t addPoint(GRAPH_DATA_TYPE value);
        GRAPH_DATA_TYPE data[GRAPH_DATA_LENGTH];
        uint16_t index;
        
};

#endif

