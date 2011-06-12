#ifndef __CUSBDEVICE_H__
#define __CUSBDEVICE_H__
#define USBCOMMANDLINE //used in i2c_header
#define USBRETRY 3
#include <usb.h>
#include "opendevice.h" //contains usbOpenDevice
#include "robot-control/requests.h"
#include "robot-control/usbconfig.h"
#include "robot-control/i2c_header.h"
#include "robot-control/CServo.h"

#define SERVOS 12
//usb device--------------------------------------------------------------------
class CUsbDevice{
    public:
        CUsbDevice();
        int8_t connected;
        int8_t connect();
        void readServoData();
        void readServoData(CServo2 *servos);
        void sendServoData();
        void sendServoData(CServo2 *servos);
        void printA();
        void printB();
        int getData();
        char PSControllerDataBuffer[BUFLEN_SERVO_DATA];
    private:
        usb_dev_handle *handle;
        char servoDataBuffer[BUFLEN_SERVO_DATA];
        char vendor[USB_CFG_VENDOR_NAME_LEN+1];
        char product[USB_CFG_DEVICE_NAME_LEN+1];
        unsigned char rawVid[2];
        unsigned char rawPid[2];
        int vid,pid;
        int sendCtrlMsg(int request, int reqType, int wval, int wind, char *buffer);
};
#endif
