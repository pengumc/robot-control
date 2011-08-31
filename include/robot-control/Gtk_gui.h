#ifndef __GTK_GUI_CPP__
#define __GTK_GUI_CPP__
#include <stdint.h>
#include <gtk/gtk.h>
#include "robot-control/Defaults.h"
#include "robot-control/CQPed.h"
#include "robot-control/Graph.h"

#define GUI_GTKBUILDERFILE "gui.builder"

typedef struct _data{
    GtkWidget *window;
} Data;


///callback function to quit the gui
G_MODULE_EXPORT gboolean
cb_delete_event(GtkWidget *window, GdkEvent *event, Data *data);

class Gtk_gui{
    public:
       
        
        ///constructor
        Gtk_gui();
        
        ///run the main gtk gui (blocks)
        void run();
    
    
    private:
        void _initGtk();
        void _fillDataFromBuilder(GtkBuilder *builder, Data *d);
        
        GtkBuilder *builder;
        Data data;    


};


#endif

