//windows console input handling thread
#ifndef _INPUT_INCLUDED_
#define _INPUT_INCLUDED_
#include <windows.h>
#include <process.h>

HANDLE _inputMutex, _runMutex;
int* _keyp;
DWORD _interval;

void _inputThread(void* args){
    do {
        if(WaitForSingleObject(_inputMutex,_interval)== WAIT_OBJECT_0){
            if (*_keyp == 0) *_keyp = getch();
            ReleaseMutex(_inputMutex);
        }
    } while (WaitForSingleObject(_runMutex, 0) == WAIT_TIMEOUT);
}

class CInputThread{
    public:
        CInputThread(int* p, DWORD ms);
        ~CInputThread();
        void start();
        void reset();
        void stop();
        DWORD waitForInput(DWORD ms);
};

//constructor
CInputThread::CInputThread(int* p, DWORD ms){
    _keyp = p;
    _interval = ms;
    _runMutex = CreateMutex(NULL, 0, NULL);
    _inputMutex = CreateMutex(NULL, 0, NULL);
}
//destructor
CInputThread::~CInputThread(){
    CloseHandle(_runMutex);
    CloseHandle(_inputMutex);   
}

void CInputThread::start(){
    WaitForSingleObject(_runMutex,0);
    if (WaitForSingleObject(_inputMutex,0)==0) {
        *_keyp = 0;
        ReleaseMutex(_inputMutex);
    }
    _beginthread(_inputThread, 0, NULL);
}

void CInputThread::reset(){
    *_keyp=0;
    ReleaseMutex(_inputMutex);
}

void CInputThread::stop(){
    ReleaseMutex(_runMutex);
    Sleep(_interval);
}


DWORD CInputThread::waitForInput(DWORD ms){
    return WaitForSingleObject(_inputMutex, ms);
}

#endif
