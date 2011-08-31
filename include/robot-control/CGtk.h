#ifndef __CGTK_CPP__
#define __CGTK_CPP__
#include <stdint.h>
#include <gtk/gtk.h>
#include "robot-control/Defaults.h"
#include "robot-control/CQPed.h"
#include "robot-control/Graph.h"

#define GUI_TIMEOUT 50 //timeout in ms
#define GUI_KEYBOARD_SPEED 0.2
//label formats, keep arguments constant, or change the code
#define GUI_SERVO_LABEL_FORMAT "<b>%d</b>: angle <span foreground=\"blue\">% .2g</span> | pw <span color=\"red\">%d</span> "
#define GUI_POSITION_LABEL_FORMAT "X: % .3g\nY: % .3g\nZ:% .3g"
#define GUI_ADC_LABEL_FORMAT "X: %d | % .4g\nY: %d | % .4g"
#define GUI_DRAW_SCALE 9.0
#define GUI_LINEWIDTH 4
#define GUI_DA_SIZE 300

static void close_window();
static gboolean key_press_callback(GtkWidget* widget, GdkEvent *event, gpointer data);
static gboolean timeout1(gpointer data);
static void timeout_disconnected(gpointer data);
static void paint(GtkWidget *widget, GdkEventExpose *eev, gpointer data);
static void paintTop(GtkWidget *widget, GdkEventExpose *eev, gpointer data);
static void paintGP(GtkWidget *widget, GdkEventExpose *eev, gpointer data);
static void paintGraph(GtkWidget *widget, GdkEventExpose *eev, gpointer data);
static void connect_clicked_cb(GtkButton *button, gpointer data);
static void controller_clicked_cb(GtkButton *button, gpointer data);
static void spinSz1Changed(GtkSpinButton *spinButton, gpointer data);
static void spinSw1Changed(GtkSpinButton *spinButton, gpointer data);
static void spinSz2Changed(GtkSpinButton *spinButton, gpointer data);
static void spinSw2Changed(GtkSpinButton *spinButton, gpointer data);

void drawLeg(cairo_t *cr, gpointer data, uint8_t leg, double  startX,double startY);
void drawLeg_around_0(cairo_t *cr, gpointer data,  uint8_t leg, double  startX,double startY);
void drawLegTop(cairo_t *cr, gpointer data,  uint8_t leg, double  startX,double startY);
void drawLineThrough(cairo_t *cr, double x1, double y1, double x2, double y2);
void drawCross(cairo_t *cr, double x, double y);
void createLineFromGraph(cairo_t *cr, Graph* graph, double zeroY, double scale);



class CGtk{
    public:
        uint8_t running;
        CGtk(CQPed *Q);
        uint8_t selected_leg;
        Graph graphX;
        Graph graphY;
        void run();
        void connect_timeout();
        void updateServoData();
        void updatePositions();
        void updateGamePadDrawing();
        void updateADC();
    private:
        GtkWidget *window;
        GtkWidget *vbox_main;        
        GtkWidget *gamepadDrawing;
        GtkWidget *hbox_bottom;
        GtkWidget *hbox_main;
        GtkWidget *vbox_left;
        GtkWidget *hbox_button;
        GtkWidget *vbox_mid;
        GtkWidget *vbox_right;

        GtkWidget *table_spinButtons;
        GtkWidget *spinButtonSz1;
        GtkObject *adjustmentSz1;
        GtkWidget *spinButtonSz1Label;
        GtkWidget *spinButtonSw1;
        GtkObject *adjustmentSw1;
        GtkWidget *spinButtonSw1Label;
        GtkWidget *spinButtonSz2;
        GtkObject *adjustmentSz2;
        GtkWidget *spinButtonSz2Label;
        GtkWidget *spinButtonSw2;
        GtkObject *adjustmentSw2;
        GtkWidget *spinButtonSw2Label;

        GtkWidget *button_connect;
        GtkWidget *button_leg;
	    GtkWidget *servo_label[SERVOS];
        GtkWidget *position_label[QP_LEGS];
        GtkWidget *adc_label;
        GtkWidget *da; ///drawing area
        GtkWidget *topDa; //top view drawing area
        GtkWidget *graphDa; //graph drawing area
        CQPed *qp;
        void show_disconnected();
        void show_connected();
        void show_right();        
        void show_left();
        guint timeoutID;
        
        friend gboolean key_press_callback(GtkWidget* widget, GdkEvent *event, gpointer data);
        friend gboolean timeout1(gpointer data);
        friend void timeout_disconnected(gpointer data);
        friend void paint(GtkWidget *widget, GdkEventExpose *eev, gpointer data);
        friend void paintTop(GtkWidget *widget, GdkEventExpose *eev, gpointer data);
        friend void paintGraph(GtkWidget *widget, GdkEventExpose *eev, gpointer data);
        friend void connect_clicked_cb(GtkButton *button, gpointer data);
        friend void controller_clicked_cb(GtkButton *button, gpointer data);
        friend void paintGP(GtkWidget *widget, GdkEventExpose *eev, gpointer data);
        friend void drawLeg(cairo_t *cr, gpointer data, uint8_t leg, double  startX,double startY);
        friend void drawLeg_around_0(cairo_t *cr, gpointer data,  uint8_t leg, double  startX,double startY);
        friend void drawLegTop(cairo_t *cr, gpointer data,  uint8_t leg, double  startX,double startY);
        friend void spinSz1Changed(GtkSpinButton *spinButton, gpointer data);
        friend void spinSw1Changed(GtkSpinButton *spinButton, gpointer data);
        friend void spinSz2Changed(GtkSpinButton *spinButton, gpointer data);
        friend void spinSw2Changed(GtkSpinButton *spinButton, gpointer data);
};
#endif
