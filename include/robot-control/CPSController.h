#ifndef __CPSCONTROLLER_H__
#define __CPSCONTROLLER_H__

#include <stdint.h>
#include <stdio.h>

//buttons
#define START 3
#define SELECT 0
#define UP 4
#define DOWN 6
#define LEFT 7
#define RIGHT 5
#define TRIANGLE 4
#define CIRCLE 5
#define SQUARE 7
#define CROSS 6
#define L1 2
#define L2 0
#define R1 3
#define R2 1
#define CHK(x,y) (x&(1<<y)) 

typedef struct CONTROLLER_DATA {
  uint8_t SS_Dpad;
  uint8_t Shoulder_Shapes;
  uint8_t previous_SS_Dpad;
  uint8_t previous_Shoulder_Shapes;
  uint8_t Rx;
  uint8_t Ry;
  uint8_t Lx;
  uint8_t Ly;
  uint8_t Rx_mid;
  uint8_t Ry_mid;
  uint8_t Lx_mid;
  uint8_t Ly_mid;
}PSC_data_t ;


class CPSController{
    public:
        CPSController();
        void setData(
        uint8_t SS_Dpad, uint8_t Shoulder_Shapes,
        uint8_t Rx, uint8_t Ry, uint8_t Lx, uint8_t Ly
        );
        ///returns 0 if the button is up, 1 if down
        uint8_t getShoulderShapes(uint8_t button);
        uint8_t getSSDpad(uint8_t button);
        void saveMid();
        int8_t getRx();
        int8_t getRy();
        int8_t getLx();
        int8_t getLy();
        void printData();
   private:
        PSC_data_t dat;
        uint8_t connected;
        
};
#endif

