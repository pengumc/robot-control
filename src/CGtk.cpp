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
#define SPEED 0.2
#define LABEL_FORMAT "%d:  angle %f | pw %d"
#define POSLABEL_FORMAT "X: %f\nY: %f\nZ:%f"

static void close_window(){gtk_main_quit();}
static gboolean key_press_callback(GtkWidget* widget, GdkEvent *event, gpointer data);
static gboolean timeout1(gpointer data);
static void timeout_disconnected(gpointer data);
static void paint(GtkWidget *widget, GdkEventExpose *eev, gpointer data);
static void connect_clicked_cb(GtkButton *button, gpointer data);

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
        GtkWidget *hbox_button;
        GtkWidget *vbox_mid;
        GtkWidget *vbox_right;
        GtkWidget *button_connect;
        GtkWidget *button_controller;
	    GtkWidget *servo_label[SERVOS];
        GtkWidget *position_label[Q_LEGS];
        GtkWidget *da; ///drawing area
        CQPed *qp;
        void show_disconnected();
        void show_connected();
        guint timeoutID;
        friend gboolean key_press_callback(GtkWidget* widget, GdkEvent *event, gpointer data);
        friend gboolean timeout1(gpointer data);
        friend void timeout_disconnected(gpointer data);
        friend void paint(GtkWidget *widget, GdkEventExpose *eev, gpointer data);
        friend void connect_clicked_cb(GtkButton *button, gpointer data);

};


CGtk::CGtk(CQPed *Q){
    running=0;
    gtk_init(NULL,NULL);
    //build gui elements
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"Robot_control");
    hbox_main = gtk_hbox_new(FALSE,0);
    vbox_left = gtk_vbox_new(FALSE,0);
    hbox_button = gtk_hbox_new(FALSE,0);
    vbox_right = gtk_vbox_new(TRUE,0);
    vbox_mid = gtk_vbox_new(TRUE,0);
    gtk_box_pack_start(GTK_BOX(hbox_main), vbox_left,FALSE, TRUE,0);
    gtk_box_pack_start(GTK_BOX(hbox_main), vbox_mid,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(hbox_main), vbox_right,TRUE, TRUE,0);

    uint8_t i;
    char text[100];
    //left
    button_connect = gtk_button_new();
    show_disconnected();
    gtk_widget_set_size_request(button_connect, 32,32);
    button_controller = gtk_toggle_button_new();
    gtk_widget_set_size_request(button_controller, 32,32);
    gtk_box_pack_start(GTK_BOX(vbox_left), hbox_button,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(hbox_button),button_connect,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(hbox_button),button_controller,FALSE,FALSE,0);
    for(i=0;i<SERVOS;i++){
        sprintf(text, LABEL_FORMAT, i,0.0,0);
        servo_label[i] = gtk_check_button_new_with_label(text);
        gtk_box_pack_start(GTK_BOX(vbox_left), servo_label[i],TRUE,TRUE,0);
    }
    //mid
    da = gtk_drawing_area_new();
    gtk_widget_set_size_request(da, 300,-1);
    gtk_box_pack_start(GTK_BOX(vbox_mid),da,TRUE,TRUE,0);
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
    g_signal_connect(button_connect, "clicked", G_CALLBACK(connect_clicked_cb), (gpointer)this);
    connect_timeout();
    g_signal_connect(G_OBJECT(da), "expose_event", G_CALLBACK(paint), (gpointer)this);
    gtk_widget_show_all(window);
    qp = Q;
}

void CGtk::show_connected(){
    GtkWidget *img;
    img = gtk_image_new_from_stock(GTK_STOCK_APPLY, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(button_connect),img);
}
void CGtk::show_disconnected(){
    GtkWidget *img;
    img = gtk_image_new_from_stock(GTK_STOCK_DIALOG_ERROR, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(button_connect),img);
}

void CGtk::updateServoData(){
    qp->readFromDev();
    uint8_t i;
    char text[100];
    for(i=0;i<SERVOS;i++){
        sprintf(text, LABEL_FORMAT, i,
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
    uint8_t i;
    char text[50];
    for(i=0;i<Q_LEGS;i++){
        sprintf(text, POSLABEL_FORMAT,qp->getX(i),qp->getY(i),qp->getZ(i));
        gtk_label_set_text(GTK_LABEL(position_label[i]),text);
    }
}

void CGtk::updatePSControllerData(){
    qp->readPSController();
}


static gboolean key_press_callback(GtkWidget* widget, GdkEvent *event, gpointer data){
    guint key = ((GdkEventKey*)event)->keyval;
    CGtk* gui = ((CGtk*)data);
    switch(key){
    case 'q':
        gui->running = 0;
        break;
    case '`':
        gui->qp->reset();
        gui->qp->sendToDev();
        break;
    case 'w':
        if(gui->qp->moveRelative(0,-SPEED,0)==0)
        gui->qp->sendToDev();
        break;
    case 's':
        if(gui->qp->moveRelative(0,SPEED,0)==0)
        gui->qp->sendToDev();
        break;
    case 'a':
        if(gui->qp->moveRelative(SPEED,0,0)==0)
        gui->qp->sendToDev();
        break;
    case 'd':
        if(gui->qp->moveRelative(-SPEED,0,0)==0)
        gui->qp->sendToDev();
        break;
    }
    usleep(10000);//allow device to transmit before next command;
    gui->updateServoData();
    gui->updatePositions();
    paint(gui->da, NULL, gui);
    return TRUE;
}


static void connect_clicked_cb(GtkButton *button, gpointer data){
    CGtk* gui = ((CGtk*)data);
    gui->qp->reset();
    gui->qp->sendToDev();
    timeout1(data);
}

static gboolean timeout1(gpointer data){
    CGtk* gui = ((CGtk*)data);
    if(gui->qp->getConnected()>1) gui->show_connected();
    else gui->show_disconnected();
    if(gui->running) TRUE;
    else return FALSE;
}

static void timeout_disconnected(gpointer data){
    g_print("disconnected timeout\n");//placeholder
}

#define BG_COLOR 0.95
#define SCALE 9.0
#define LINEWIDTH 4
static void paint(GtkWidget *widget, GdkEventExpose *eev, gpointer data){
    CGtk* gui = ((CGtk*)data);
    GtkAllocation alloc;
    gtk_widget_get_allocation(widget, &alloc);
    //g_print("paint, size = %i x %i\n", alloc.width, alloc.height);
    cairo_t *cr;
    cr = gdk_cairo_create(widget->window);
    cairo_set_line_width(cr,5);
    double A, B, C,a,b, c;
    //clear area
    cairo_set_source_rgb(cr,BG_COLOR,BG_COLOR,BG_COLOR);
    cairo_paint(cr);
    double x= alloc.width/2; 
    double y= alloc.height/2;
    A = gui->qp->lengths.A[0];
    B = gui->qp->lengths.B[0];
    C = gui->qp->lengths.C[0];
    a = gui->qp->getAngle(0);
    b = gui->qp->getAngle(1);
    c = gui->qp->getAngle(2);
    cairo_set_source_rgb(cr, 0,0,0);
    //move to leg endpoint, and draw base line
    x += 100;
    y += 50;
    cairo_move_to(cr, alloc.width,y);
    cairo_line_to(cr, 0, y);
    cairo_stroke(cr);
     //line to servo[1]
    cairo_set_source_rgb(cr, 1,0,0);
    cairo_move_to(cr, x,y);
    x -= (cos(c + b) * C) * SCALE;
    y += (sin(c + b) * C) * SCALE;
    cairo_line_to(cr, x, y);
    cairo_stroke(cr);
    //line to servo[0]
    cairo_set_source_rgb(cr, 0,0,1);
    cairo_move_to(cr, x,y);
    x -= (cos(b)*B)*SCALE;
    y += (sin(b) * B)*SCALE;
    cairo_line_to(cr, x, y);
    cairo_stroke(cr);
    //line to servo[4]
    cairo_set_source_rgb(cr, 1,0.9,0);
    cairo_move_to(cr, x,y);
    x -= (12)*SCALE; //distance between servo[0] and 3
    y += 0;
    cairo_line_to(cr, x, y);
    cairo_stroke(cr);
    //load new values
    A = gui->qp->lengths.A[1];
    B = gui->qp->lengths.B[1];
    C = gui->qp->lengths.C[1];
    a = gui->qp->getAngle(3);
    b = gui->qp->getAngle(4);
    c = gui->qp->getAngle(5);
    //line to servo[5]
    cairo_move_to(cr, x,y);
    x += (cos(b) * B )*SCALE;
    y -= (sin(b) * B )* SCALE;
    cairo_set_source_rgb(cr, 0,0,1);
    cairo_line_to(cr, x, y);
    cairo_stroke(cr);
    //line to endpoint
    cairo_move_to(cr, x,y);
    x += SCALE * C * cos(b-c);
    y -= SCALE * C * sin(b-c);
    cairo_set_source_rgb(cr, 1,0,0);
    cairo_line_to(cr, x, y);
    cairo_stroke(cr);
    
    cairo_destroy(cr);
}
#endif

