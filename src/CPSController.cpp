#include "robot-control/CPSController.h"

CPSController::CPSController(){
    dat.SS_Dpad = 0xFF;
    dat.Shoulder_Shapes = 0xFF;
    dat.Rx = 128;
    dat.Ry = 128;
    dat.Lx = 128;
    dat.Ly = 128;
    dat.Rx_mid = 128;
    dat.Ry_mid = 128;
    dat.Lx_mid = 128;
    dat.Ly_mid = 128;
}

void CPSController::printData(){
    printf("playstation controller state:\n");
    printf("SS_Dpad 0x%X\nSdhoulder_Shapes 0x%x\n", dat.SS_Dpad, dat.Shoulder_Shapes);
    printf("R: %d | %d\n", getRx(), getRy());
    printf("L: %d | %d\n", getLx(), getLy());
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




