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
//servo class------------------------------------------------------------------
class CServo{
    public:
        CServo();
        void reset();
        void reset_NEW();
        uint8_t maxPulse;
        uint8_t minPulse;
        uint8_t midPulse;
        double direction;
        double offset;
        double K;
        double angle;
        uint8_t pulsewidth;

        uint8_t isValidAngle(double a);
        void flipDirection();
        char setAngle(double a);
        double pulsewidthToAngle();
        double pulsewidthToAngle(uint8_t pw);
        uint8_t angleToPulsewidth();
        uint8_t angleToPulsewidth(double a);
        friend class CUsbDevice;
    private:
        CAngle phi;
};
//for servo 2 and 5: direction = -1, offset = -PI/2
CServo::CServo(){reset();}

void CServo::reset_NEW(){
    phi = 0;
    maxPulse=0;
}
/** Default values are hardcoded here.*/
void CServo::reset(){
    angle=0;
    maxPulse = 96;
    minPulse = 48;
    midPulse = 72;
    direction = 1.0;
    offset = 0;
    K = 0.034;
    pulsewidth = angleToPulsewidth();
}

void CServo::flipDirection(){
    char pw;
    if (direction == -1.0){
        direction=1.0;
        pw= maxPulse;
        maxPulse = minPulse;
        minPulse = pw;
    }else{
        direction = -1.0;
        pw= maxPulse;
        maxPulse = minPulse;
        minPulse = pw;
    }
}

uint8_t CServo::isValidAngle(double a){
    if (a > pulsewidthToAngle(minPulse) &&
        a < pulsewidthToAngle(maxPulse)) return 1;
    else return 0;
}

char CServo::setAngle(double a){
//return 0 on success, 1 if maxed
    char tempPW;
//    printf("setAngle in: %f\n",a);
//    printf("max angle = %f\n", pulsewidthToAngle(maxPulse));
//    printf("min angle = %f\n", pulsewidthToAngle(minPulse));
//    printf("pulsewidth a = %d\n", angleToPulsewidth(a));
    if (a > pulsewidthToAngle(maxPulse)) {
        pulsewidth = maxPulse;
        angle = pulsewidthToAngle(maxPulse);
    }else if (a < pulsewidthToAngle(minPulse)){
        pulsewidth = minPulse;
        angle = pulsewidthToAngle(minPulse);
    }else{
        pulsewidth = angleToPulsewidth(a);
        angle = a;
    }
       
}

uint8_t CServo::angleToPulsewidth(){
    return (((angle-offset) / K) / direction) + midPulse;
}

uint8_t CServo::angleToPulsewidth(double a){
    //printf("angleToPulsewidth got %f\n",a);
    return (((a-offset) / K) / direction) +midPulse;
}

double CServo::pulsewidthToAngle(){
    return (pulsewidth - ((minPulse+maxPulse)/2)) * direction * K + offset;
}

double CServo::pulsewidthToAngle(uint8_t pw){
    return (pw- midPulse) * direction * K + offset;
}


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
        void getData();
    private:
        usb_dev_handle *handle;
        char servoDataBuffer[BUFLEN_SERVO_DATA];
        char PSControllerDataBuffer[BUFLEN_SERVO_DATA];
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
    servoDataBuffer = SERVO_DATA_EMPTY;
    PSControllerDataBuffer = SERVO_DATA_EMPTY;
    usb_init();
}

void CUsbDevice::getData(){
    if(connected<1) return;
    char i;
    sendCtrlMsg(CUSTOM_RQ_GET_DATA, USB_ENDPOINT_IN,0,0,PSControllerDataBuffer);
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
    if (sendCtrlMsg(CUSTOM_RQ_LOAD_POS_FROM_I2C, USB_ENDPOINT_IN,0,0,servoDataBuffer)==0){
        usleep(10000);
        i=sendCtrlMsg(CUSTOM_RQ_GET_POS, USB_ENDPOINT_IN,0,0,servoDataBuffer);
        if(i>0){
            //printf("received %d {",i);
            for (i=0;i<BUFLEN_SERVO_DATA;i++){
                //printf("%d,",servoDataBuffer[i]);
                if ((servoDataBuffer[i] != servoDataBuffer[0]) || (servoDataBuffer[i] > USB_HIGHEST_RQ)) k++;
            }
            //printf("}\n");
            if(k==BUFLEN_SERVO_DATA){
                for (i=0;i<BUFLEN_SERVO_DATA;i++){
                    servos[i].setPW( servoDataBuffer[i]);
                    //servos[i].setAngle = servos[i].pulsewidthToAngle();
                }
            }else {
                printf("not ready, trying again.\n");
            }
        }
    }else printf("readServoData: device was not ready\n");


}


void CUsbDevice::sendServoData(){
    int i;
    i = sendCtrlMsg(CUSTOM_RQ_SET_DATA, USB_ENDPOINT_OUT, 0,0,servoDataBuffer);
    printf("sendServoData: %d send\n",i);
}

void CUsbDevice::sendServoData(CServo2 *servos){
    int i;
    for (i=0;i<BUFLEN_SERVO_DATA;i++){
        servoDataBuffer[i] = servos[i].getPW();
    }

    i = sendCtrlMsg(CUSTOM_RQ_SET_DATA, USB_ENDPOINT_OUT, 0,0,servoDataBuffer);
    printf("sendServoData: %d send\n",i);
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
