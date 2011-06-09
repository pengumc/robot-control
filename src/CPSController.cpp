#ifndef __PSCONTROLLER_CP__
#define __PSCONTROLLER_CP__
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
//   private:
        PSC_data_t dat;
        
};

CPSController::CPSController(){
    dat.Rx_mid = 128;
    dat.Ry_mid = 128;
    dat.Lx_mid = 128;
    dat.Ly_mid = 128;
}

int8_t CPSController::getRx(){
    return (dat.Rx - dat.Rx_mid);
}
int8_t CPSController::getRy(){
    return (dat.Ry - dat.Ry_mid);
}
int8_t CPSController::getLx(){
    return (dat.Lx - dat.Lx_mid);
}
int8_t CPSController::getLy(){
    return (dat.Ly - dat.Ly_mid);
}

void CPSController::saveMid(){
    dat.Rx_mid = dat.Rx;
    dat.Ry_mid = dat.Ry;
    dat.Lx_mid = dat.Lx;
    dat.Ly_mid = dat.Ly;
}

void CPSController::setData(uint8_t SS_Dpad, uint8_t Shoulder_Shapes,
        uint8_t Rx, uint8_t Ry, uint8_t Lx, uint8_t Ly)
{
    dat.previous_SS_Dpad = dat.SS_Dpad;
    dat.previous_Shoulder_Shapes = dat.Shoulder_Shapes;
    dat.SS_Dpad = SS_Dpad;
    dat.Shoulder_Shapes = Shoulder_Shapes;
    dat.Rx = Rx;
    dat.Ry = Ry;
    dat.Lx = Lx;
    dat.Ly = Ly;
}

uint8_t CPSController::getSSDpad(uint8_t button){
    if(CHK(dat.SS_Dpad, button)==0) return 1;
    else return 0;
}

uint8_t CPSController::getShoulderShapes(uint8_t button){
    if(CHK(dat.Shoulder_Shapes, button)==0) return 1;
    else return 0;
}


#endif

