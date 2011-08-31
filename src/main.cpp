#define __MAIN__

#include "robot-control/CQPed.h"
#include "robot-control/CGtk.h"
#include "robot-control/Gtk_gui.h"

int main(int argc, char* argv[]){
  CQPed qwop;
//    CGtk gui(&qwop);
//    gui.run();
    Gtk_gui gui;
    gui.run();

    return 0;
}


