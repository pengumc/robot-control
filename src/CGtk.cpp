#ifndef __CGTK_CPP__
#define __CGTK_CPP__
#include <stdint.h>
#include <gtk/gtk.h>

#ifndef SERVOS
    #define SERVOS 12
#endif
#ifndef Q_LEGS
    #define Q_LEGS 4
#endif
#define TIMEOUT 200 //timeout in ms
static void close_window(){gtk_main_quit();}
static gboolean key_press_callback(GtkWidget* widget, GdkEvent *event, gpointer data);
static gboolean timeout1(gpointer data);
static void timeout_disconnected(gpointer data);
class CGtk{
    public:
        uint8_t running;
        CGtk(CQPed *Q);
        void run();
        void connect_timeout();
        void updateServoData();
        void updatePositions();
        void updatePSControllerData();
    private:
        GtkWidget *window;
        GtkWidget *hbox_main;
        GtkWidget *vbox_left;
        GtkWidget *vbox_mid;
        GtkWidget *vbox_right;
	    GtkWidget *servo_label[SERVOS];
        GtkWidget *position_label[Q_LEGS];
        GtkWidget *da; ///drawing area
        CQPed *qp;
        guint timeoutID;
        friend gboolean key_press_callback(GtkWidget* widget, GdkEvent *event, gpointer data);
        friend gboolean timeout1(gpointer data);
        friend void timeout_disconnected(gpointer data);
};

CGtk::CGtk(CQPed *Q){
    running=0;
    gtk_init(NULL,NULL);
    //build gui elements
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    hbox_main = gtk_hbox_new(TRUE,0);
    vbox_left = gtk_vbox_new(TRUE,0);
    vbox_right = gtk_vbox_new(TRUE,0);
    vbox_mid = gtk_vbox_new(TRUE,0);
    gtk_box_pack_start(GTK_BOX(hbox_main), vbox_left,TRUE, TRUE,0);
    gtk_box_pack_start(GTK_BOX(hbox_main), vbox_mid,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(hbox_main), vbox_right,TRUE, TRUE,0);

    uint8_t i;
    char text[100];
    //left
    for(i=0;i<SERVOS;i++){
        sprintf(text, "%d:  angle %f | pw %d", i,0.0,0);
        servo_label[i] = gtk_check_button_new_with_label(text);
        gtk_box_pack_start(GTK_BOX(vbox_left), servo_label[i],TRUE,TRUE,0);
    }
    //mid
    da = gtk_drawing_area_new();

    //right
    for(i=0;i<Q_LEGS;i++){
        sprintf(text, "X%d Y%d Z%d",i,i,i);
        position_label[i] = gtk_label_new(text);
        gtk_box_pack_start(GTK_BOX(vbox_right), position_label[i], TRUE, TRUE, 0);
    }
    gtk_container_add(GTK_CONTAINER(window), hbox_main);
    //connect signals
    g_signal_connect(window, "destroy", G_CALLBACK(close_window),NULL);
    g_signal_connect(window, "key_press_event", G_CALLBACK(key_press_callback), (gpointer)this);
    connect_timeout();
    gtk_widget_show_all(window);
    qp = Q;
}

void CGtk::updateServoData(){
    qp->readFromDev();
    uint8_t i;
    char text[100];
    for(i=0;i<SERVOS;i++){
        sprintf(text, "%d: angle %f | pw %d", i,
            qp->getAngle(i),
            qp->getPW(i)
        );
        gtk_button_set_label(GTK_BUTTON(servo_label[i]),text);
    }
}

void CGtk::connect_timeout(){
    timeoutID = g_timeout_add_full(
        G_PRIORITY_DEFAULT,
        TIMEOUT,
        timeout1, 
        (gpointer)this, 
        timeout_disconnected);
}

void CGtk::run(){
    running=1;
    gtk_main();
}

void CGtk::updatePositions(){
//TODO
}

void CGtk::updatePSControllerData(){
    qp->readPSController()
}

#ifndef __MAIN__
int main(int argc, char* argv[]){
    CGtk gui;
    gui.run();
    return 0;
}
#endif


static gboolean key_press_callback(GtkWidget* widget, GdkEvent *event, gpointer data){
    guint key = ((GdkEventKey*)event)->keyval;
    GtkButton *checkbox = (GtkButton*)(((CGtk*)data)->servo_label[0]);
    char text[50];
    sprintf(text, "last button press: %i", key);
    gtk_button_set_label(checkbox, text);
    if(key == 'q') ((CGtk*)data)->running = 0;
    return TRUE;
}

static gboolean timeout1(gpointer data){
    CGtk* gui = ((CGtk*)data);
    //g_print("timeout\n");
    gui->updateServoData();
    if(gui->running) TRUE;
    else return FALSE;
}

static void timeout_disconnected(gpointer data){
    g_print("disconnected\n");
}

#endif

