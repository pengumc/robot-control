#ifndef __CINTERFACE_CPP__
#define __CINTERFACE_CPP__
#include <gtk/gtk.h>
static void *InterfaceThread(gpointer data){
    gtk_main();
    *((uint8_t*)data)=0;
}

static void close_window(){
    gtk_main_quit();
}

static void scale1_changed(GtkAdjustment *adj, gpointer data){
    g_print("%f\n", (gtk_range_get_value((GtkRange*)adj)));
}

#define LABEL_AMOUNT 12
class CInterface{
    public:
        CInterface();
        void main();
        void updateLabels(uint8_t *buffer);
        uint8_t running;
    private:
        GtkWidget *window;
        GtkWidget *label[LABEL_AMOUNT];
        GtkWidget *vbox1;
        GtkWidget *scale1;

};

CInterface::CInterface(){
    uint8_t i=0;
    char text[5];
    gtk_init(NULL, NULL);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(close_window),NULL);
    vbox1 = gtk_vbox_new(TRUE, 0);
    scale1 =gtk_hscale_new_with_range(-M_PI,M_PI,0.02);
    g_signal_connect(scale1, "value_changed", G_CALLBACK(scale1_changed),NULL);
    gtk_box_pack_start(GTK_BOX(vbox1), scale1, TRUE, TRUE, 0);
    for(i=0;i<LABEL_AMOUNT;i++){
        sprintf(text,"%d",i);
        label[i] = gtk_label_new(text);
        gtk_box_pack_start(GTK_BOX(vbox1), label[i], TRUE, TRUE, 0);
    }

    gtk_widget_set_size_request(vbox1, 300,-1);
    gtk_container_add(GTK_CONTAINER(window), vbox1);
    gtk_widget_show_all(window);
    g_thread_init(NULL);
}

void CInterface::main(){
    running=1;
    g_thread_create(InterfaceThread,(gpointer)&running, TRUE, NULL);
}

void CInterface::updateLabels(uint8_t *buffer){
    uint8_t i;
    char text[20];
    for(i=0;i<LABEL_AMOUNT;i++){
        sprintf(text,"%d : %d\n",i, buffer[i]);
        gtk_label_set_label(GTK_LABEL(label[i]),text);
    }
}
#ifndef __MAIN__

int main(int argc, char* argv[]){

}

#endif
#endif

