//pthreads console input
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <termios.h>
#include <iostream>

using namespace std;

pthread_mutex_t _runMutex;
pthread_mutex_t _inputMutex;
int* _keyp;
uint32_t _interval;

void *_inputThread(void* args){
    do{
        if (pthread_mutex_trylock(&_inputMutex)==0){
            if (*_keyp==0){
                *_keyp = getc(stdin);   
                pthread_mutex_unlock(&_inputMutex);
            }else pthread_mutex_unlock(&_inputMutex);
        }else usleep(20);
    }while (pthread_mutex_trylock(&_runMutex) != 0);
    //put back old terminal settings
}

class CInputThread{
    public:
        static uint8_t n;
        CInputThread(int *p, uint32_t ms);
        ~CInputThread(){n--;};
        void start();
        void reset();
        void stop();
        uint32_t waitForInput(uint32_t ms);
    private:
        pthread_t threadID;
        struct termios new_settings, old_settings;
};

uint8_t CInputThread::n=0;

CInputThread::CInputThread(int *p, uint32_t ms){
    n++;
    _keyp = p;
    _interval = ms*1000;
}

void CInputThread::start(){
    //remove line buffering from terminal
    tcgetattr(0, &new_settings);
    tcgetattr(0, &old_settings);
    new_settings.c_lflag &= ~(new_settings.c_lflag|ICANON);
    tcsetattr(0, TCSANOW, &new_settings);
    pthread_mutex_lock(&_runMutex);
//TODO check and/or release inputmutex
    pthread_create(&threadID, NULL, _inputThread, NULL);
}

void CInputThread::stop(){
    pthread_mutex_unlock(&_runMutex);
    tcsetattr(0, TCSANOW, &old_settings);
    usleep(_interval);
}

void CInputThread::reset(){
    *_keyp = 0;
    pthread_mutex_unlock(&_inputMutex);
}
uint32_t CInputThread::waitForInput(uint32_t ms){
    //try to lock the mutex for handling the received input
    //sleep otherwise
    if(pthread_mutex_trylock(&_inputMutex)==0){
        return 0;
    }else usleep(ms);
    return 1;
}




