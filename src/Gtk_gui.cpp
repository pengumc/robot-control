#include "robot-control/Gtk_gui.h"
//CALLBACKS--------------------------------------------------------------------
G_MODULE_EXPORT gboolean
cb_delete_event(GtkWidget *window, GdkEvent *event, Data *data){
    return(FALSE); //destroy window
}



//PUBLIC-----------------------------------------------------------------------
Gtk_gui::Gtk_gui(){
    _initGtk();    
}



void
Gtk_gui::run(){
    gtk_main();
}



//PRIVATE----------------------------------------------------------------------
void 
Gtk_gui::_initGtk(){
    gtk_init(NULL,NULL);
    

    builder = gtk_builder_new();
    guint err = gtk_builder_add_from_file(builder, GUI_GTKBUILDERFILE, FALSE);
    printf("add from file returned: %d\n", err);

    _fillDataFromBuilder(builder, &data);

    gtk_builder_connect_signals(builder, &data);

    //destroy builder
    g_object_unref( G_OBJECT( builder ) );

    gtk_widget_show(data.window);
    
}    



void
Gtk_gui::_fillDataFromBuilder(GtkBuilder *builder, Data *d){
    d->window = GTK_WIDGET( gtk_builder_get_object(builder, "window1"));
}
