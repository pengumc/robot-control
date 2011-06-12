#ifndef __CUSBDEVICE__
#define __CUSBDEVICE__
#define USBCOMMANDLINE
#define USBRETRY 3
#include <usb.h>
#include "opendevice.h"
#include "requests.h"
#include "usbconfig.h"
#include "i2c_header.h"
#define SERVOS 12
#include "CAngle.cpp"

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

CUsbDevice::CUsbDevice(){
    vendor = {USB_CFG_VENDOR_NAME,0};
    product = {USB_CFG_DEVICE_NAME,0};
    rawVid = {USB_CFG_VENDOR_ID};
    rawPid = {USB_CFG_DEVICE_ID};
    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];
    connected=0;
    servoDataBuffer = SERVO_DATA_EMPTY;
    PSControllerDataBuffer = SERVO_DATA_EMPTY;
    usb_init();
}


int CUsbDevice::getData(){
    if(connected<1) return 0;
    char i;
    sendCtrlMsg(CUSTOM_RQ_GET_DATA, USB_ENDPOINT_IN,0,0,PSControllerDataBuffer);
    return 1;
//    printf("Right X = %d\n",PSControllerDataBuffer[5]);
//    printf("Right Y = %d\n",PSControllerDataBuffer[6]);
//    printf("Left X = %d\n",PSControllerDataBuffer[7]);
//    printf("Left Y = %d\n",PSControllerDataBuffer[8]);
//        printf("data received %d {",i);
//        for (i=0;i<BUFLEN_SERVO_DATA;i++){
//            printf("0x%hX,",PSControllerDataBuffer[i]);
//        }
//        printf("}\n");
}

void CUsbDevice::printA(){
    char i;
    printf("buffer A: {");
    for (i=0;i<BUFLEN_SERVO_DATA;i++){
        printf("%d,",servoDataBuffer[i]);
    }
    printf("}\n");
}
void CUsbDevice::printB(){
    char i;
    printf("buffer B: {");
    for (i=0;i<BUFLEN_SERVO_DATA;i++){
        printf("%d,",PSControllerDataBuffer[i]);
    }
    printf("}\n");
}

int8_t CUsbDevice::connect(){
    if (usbOpenDevice(&handle, vid, vendor, pid, product,
        NULL, NULL, NULL) != 0){
        fprintf(
            stderr,
            "USB device \"%s\" with vid=0x%x pid=0x%x\n  NOT FOUND\n",
            product, vid, pid);
        connected = 0;
    }else connected = USBRETRY;
    return connected;
}

void CUsbDevice::readServoData(){
    if(connected<1) return;
    int i; 
    char k;
    if (sendCtrlMsg(CUSTOM_RQ_LOAD_POS_FROM_I2C, USB_ENDPOINT_IN,0,0,servoDataBuffer)==0){;
        i=sendCtrlMsg(CUSTOM_RQ_GET_POS, USB_ENDPOINT_IN,0,0,servoDataBuffer);
        if(i>0){
                printf("received %d {",i);
                for (i=0;i<BUFLEN_SERVO_DATA;i++){
                    printf("%d,",servoDataBuffer[i]);
                }
                printf("}\n");
        }
        //store in servo's

    }else printf("readServoData: device was not ready\n");
}

void CUsbDevice::readServoData(CServo2* servos){
    if(connected<1)return;
    int i; 
    char k;
    if(sendCtrlMsg(CUSTOM_RQ_LOAD_POS_FROM_I2C, USB_ENDPOINT_IN,0,0,servoDataBuffer)==0){  
        usleep(10000);
        i=sendCtrlMsg(CUSTOM_RQ_GET_POS, USB_ENDPOINT_IN,0,0,servoDataBuffer);
        if(i>0){
            //printf("received %d {",i);
            for (i=0;i<BUFLEN_SERVO_DATA;i++){
                //printf("%d,",servoDataBuffer[i]);
                if ((servoDataBuffer[i] != servoDataBuffer[0]) 
                    || (servoDataBuffer[i] > USB_HIGHEST_RQ))
                    k++;
            }
            //printf("}\n");
            if(k==BUFLEN_SERVO_DATA){
                for (i=0;i<BUFLEN_SERVO_DATA;i++){
                    servos[i].setPW( servoDataBuffer[i]);
                    //printf("got %d:%d\n",i,servoDataBuffer[i]);
                    //servos[i].setAngle = servos[i].pulsewidthToAngle();
                }
            }else {
                printf("not ready, trying again.\n");
            }
        }
    }else printf("readServoData: LOAD_POS_FROM_I2C failed\n");


}


void CUsbDevice::sendServoData(){
    if(connected<1) return;
    int i;
    i = sendCtrlMsg(CUSTOM_RQ_SET_DATA, USB_ENDPOINT_OUT, 0,0,servoDataBuffer);
    //printf("sendServoData: %d send\n",i);
}

void CUsbDevice::sendServoData(CServo2 *servos){
    if(connected<1) return;
    int i;
    for (i=0;i<BUFLEN_SERVO_DATA;i++){
        servoDataBuffer[i] = servos[i].getPW();
    }

    i = sendCtrlMsg(CUSTOM_RQ_SET_DATA, USB_ENDPOINT_OUT, 0,0,servoDataBuffer);
    //printf("sendServoData: %d send\n",i);
}

int CUsbDevice::sendCtrlMsg(int request, int reqType, int wval, int wind, char *buffer){
    int cnt;
    unsigned char i;
    if(handle && connected){
        cnt = usb_control_msg(
            handle,
            USB_TYPE_VENDOR | USB_RECIP_DEVICE | reqType,
            request,
            wval,
            wind,
            buffer,
            BUFLEN_SERVO_DATA,
            1000
        );
        if (cnt < 0){
            fprintf(stderr, "usb_control_msg: %s\n",usb_strerror());
            connected--;
        } else connected = USBRETRY;
    }else {
        printf("sendCtrlMsg: not connected...\n");
        connected--;
    }
    return cnt;
}

#endif
