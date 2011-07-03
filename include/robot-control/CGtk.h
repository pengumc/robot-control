#ifndef __CGTK_CPP__
#define __CGTK_CPP__
#include <stdint.h>
#include <gtk/gtk.h>
#include "robot-control/Defaults.h"
#include "robot-control/CQPed.h"

#define GUI_TIMEOUT 50 //timeout in ms
#define GUI_KEYBOARD_SPEED 0.2
//label formats, keep arguments constant, or change the code
#define GUI_SERVO_LABEL_FORMAT "<b>%d</b>: angle <span foreground=\"blue\">% .2g</span> | pw <span color=\"red\">%d</span> "
#define GUI_POSITION_LABEL_FORMAT "X: % .3g\nY: % .3g\nZ:% .3g"

#define GUI_DRAW_SCALE 9.0
#define GUI_LINEWIDTH 4

static void close_window();
static gboolean key_press_callback(GtkWidget* widget, GdkEvent *event, gpointer data);
static gboolean timeout1(gpointer data);
static void timeout_disconnected(gpointer data);
static void paint(GtkWidget *widget, GdkEventExpose *eev, gpointer data);
static void paintGP(GtkWidget *widget, GdkEventExpose *eev, gpointer data);
static void connect_clicked_cb(GtkButton *button, gpointer data);
void drawLeg(cairo_t *cr, gpointer data, uint8_t leg, double  startX,double startY);

class CGtk{
    public:
        uint8_t running;
        CGtk(CQPed *Q);
        void run();
        void connect_timeout();
        void updateServoData();
        void updatePositions();
        void updateGamePadDrawing();
    private:
        GtkWidget *window;
        GtkWidget *vbox_main;        
        GtkWidget *gamepadDrawing;
        GtkWidget *hbox_main;
        GtkWidget *vbox_left;
        GtkWidget *hbox_button;
        GtkWidget *vbox_mid;
        GtkWidget *vbox_right;
        GtkWidget *button_connect;
        GtkWidget *button_controller;
	    GtkWidget *servo_label[SERVOS];
        GtkWidget *position_label[QP_LEGS];
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
        friend void paintGP(GtkWidget *widget, GdkEventExpose *eev, gpointer data);
        friend void drawLeg(cairo_t *cr, gpointer data, uint8_t leg, double  startX,double startY);
};
#endif
