#include "robot-control/CGtk.h"

//------------------------------------------------------------------------------

static void close_window(){
    gtk_main_quit();
}



CGtk::CGtk(CQPed *Q){ 
    running = 0; 
    selected_leg = 0;
    qp = Q;
    
    gtk_init(NULL,NULL);
    //build gui elements
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"Robot_control");
    vbox_main = gtk_vbox_new(FALSE,2);
    hbox_main = gtk_hbox_new(FALSE,2);
    vbox_left = gtk_vbox_new(FALSE,2);
    hbox_button = gtk_hbox_new(FALSE,2);
    table_spinButtons = gtk_table_new(4, 2, FALSE);
    vbox_right = gtk_vbox_new(TRUE,2);
    gtk_widget_set_size_request(vbox_right, 100, -1);
    vbox_mid = gtk_vbox_new(TRUE,2);
    gtk_box_pack_start(GTK_BOX(vbox_main), hbox_main, FALSE, TRUE,2);
    gtk_box_pack_start(GTK_BOX(hbox_main), vbox_left,FALSE, TRUE,2);
    gtk_box_pack_start(GTK_BOX(hbox_main), vbox_mid,TRUE,TRUE,2);
    gtk_box_pack_start(GTK_BOX(hbox_main), vbox_right,TRUE, TRUE,2);

    //left
    button_connect = gtk_button_new();
    show_disconnected();
    gtk_widget_set_size_request(button_connect, 32,32);
    button_leg = gtk_button_new();
    show_right();
    gtk_widget_set_size_request(button_leg, 32,32);
    gtk_box_pack_start(GTK_BOX(vbox_left), hbox_button,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(hbox_button),button_connect,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(hbox_button),button_leg,FALSE,FALSE,0);
    uint8_t i;
    char text[100];
    for(i=0;i<SERVOS;i++){
        sprintf(text, GUI_SERVO_LABEL_FORMAT, i,0.0,0);
        servo_label[i] = gtk_label_new(NULL); 
        gtk_label_set_markup(GTK_LABEL(servo_label[i]), text);
        gtk_box_pack_start(GTK_BOX(vbox_left), servo_label[i],FALSE,TRUE,0);
    }
    //spinbuttons
    gtk_box_pack_start(GTK_BOX(vbox_left), table_spinButtons, FALSE, FALSE,0);
    adjustmentSz2 = gtk_adjustment_new(qp->kalman1.getSz(),
                                       -10.0, 10.0, 0.001, 0.0, 0.0);
    adjustmentSw2 = gtk_adjustment_new(qp->kalman1.getSw(),
                                       -10.0, 10.0, 0.001, 0.0, 0.0);
    spinButtonSz2 = gtk_spin_button_new(GTK_ADJUSTMENT(adjustmentSz2), 0.0, 3);
    spinButtonSw2 = gtk_spin_button_new(GTK_ADJUSTMENT(adjustmentSw2), 0.0, 3);
    spinButtonSz2Label = gtk_label_new("Sz2: ");
    spinButtonSw2Label = gtk_label_new("Sw2: ");
    adjustmentSz1 = gtk_adjustment_new(qp->kalman2.getSz(),
                                       -10.0, 10.0, 0.001, 0.0, 0.0);
    adjustmentSw1 = gtk_adjustment_new(qp->kalman2.getSw(),
                                       -10.0, 10.0, 0.001, 0.0, 0.0);
    spinButtonSz1 = gtk_spin_button_new(GTK_ADJUSTMENT(adjustmentSz1), 0.0, 3);
    spinButtonSw1 = gtk_spin_button_new(GTK_ADJUSTMENT(adjustmentSw1), 0.0, 3);
    spinButtonSz1Label = gtk_label_new("Sz1: ");
    spinButtonSw1Label = gtk_label_new("Sw1: ");
    gtk_table_attach_defaults( GTK_TABLE(table_spinButtons),
                               spinButtonSz1Label,
                               0, 1, 0, 1 );
    gtk_table_attach_defaults( GTK_TABLE(table_spinButtons),
                               spinButtonSz1,
                               1, 2, 0, 1 );
    gtk_table_attach_defaults( GTK_TABLE(table_spinButtons),
                               spinButtonSw1Label,
                               0, 1, 1, 2 );
    gtk_table_attach_defaults( GTK_TABLE(table_spinButtons),
                               spinButtonSw1,
                               1, 2, 1, 2 );
    gtk_table_attach_defaults( GTK_TABLE(table_spinButtons),
                               spinButtonSz2Label,
                               0, 1, 2, 3 );
    gtk_table_attach_defaults( GTK_TABLE(table_spinButtons),
                               spinButtonSz2,
                               1, 2, 2, 3 );
    gtk_table_attach_defaults( GTK_TABLE(table_spinButtons),
                               spinButtonSw2Label,
                               0, 1, 3, 4 );
    gtk_table_attach_defaults( GTK_TABLE(table_spinButtons),
                               spinButtonSw2,
                               1, 2, 3, 4 );
    graphDa = gtk_drawing_area_new();
    gtk_widget_set_size_request(graphDa, GUI_DA_SIZE,GUI_DA_SIZE);
    gtk_box_pack_end(GTK_BOX(vbox_left), graphDa, FALSE, TRUE, 0);

    //mid
    da = gtk_drawing_area_new();
    gtk_widget_set_size_request(da, GUI_DA_SIZE,GUI_DA_SIZE-100);
    gtk_box_pack_start(GTK_BOX(vbox_mid),da,FALSE,TRUE,0);
    topDa = gtk_drawing_area_new();
    gtk_widget_set_size_request(topDa,GUI_DA_SIZE,GUI_DA_SIZE);
    gtk_box_pack_start(GTK_BOX(vbox_mid),topDa, FALSE, TRUE, 0);
    
    //right
    for(i=0;i<QP_LEGS;i++){
        sprintf(text, "X%d Y%d Z%d",i,i,i);
        position_label[i] = gtk_label_new(text);
        gtk_box_pack_start(GTK_BOX(vbox_right),
                          position_label[i],
                          TRUE,
                          TRUE,
                          0);
    }
    adc_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox_right), adc_label, TRUE, TRUE, 0);
    //gamepad drawing
    gamepadDrawing = gtk_drawing_area_new();
    gtk_box_pack_start(GTK_BOX(vbox_main), gamepadDrawing, FALSE, FALSE,0);
    gtk_widget_set_size_request(gamepadDrawing, -1, 150);
    //add main container to window
    gtk_container_add(GTK_CONTAINER(window), vbox_main);
    
    //connect signals
    g_signal_connect(window,
                    "destroy",
                    G_CALLBACK(close_window),
                    NULL);
    g_signal_connect(window,
                    "key_press_event",
                    G_CALLBACK(key_press_callback),
                    (gpointer)this);
    g_signal_connect(button_connect, 
                    "clicked", 
                    G_CALLBACK(connect_clicked_cb),
                    (gpointer)this);
    g_signal_connect(button_leg,
                    "clicked",
                    G_CALLBACK(controller_clicked_cb),
                    (gpointer)this);
    g_signal_connect(G_OBJECT(da),
                    "expose_event", 
                    G_CALLBACK(paint), 
                    (gpointer)this);
    g_signal_connect(G_OBJECT(gamepadDrawing),
                    "expose_event", 
                    G_CALLBACK(paintGP), 
                    (gpointer)this);    
    g_signal_connect(G_OBJECT(topDa), 
                    "expose_event", 
                    G_CALLBACK(paintTop), 
                    (gpointer)this);
    g_signal_connect(G_OBJECT(graphDa), 
                    "expose_event", 
                    G_CALLBACK(paintGraph), 
                    (gpointer)this);

    g_signal_connect(G_OBJECT(spinButtonSz1), 
                    "value-changed", 
                    G_CALLBACK(spinSz1Changed), 
                    (gpointer)qp);
    g_signal_connect(G_OBJECT(spinButtonSw1), 
                    "value-changed", 
                    G_CALLBACK(spinSw1Changed), 
                    (gpointer)qp);
    g_signal_connect(G_OBJECT(spinButtonSz2), 
                    "value-changed", 
                    G_CALLBACK(spinSz2Changed), 
                    (gpointer)qp);
    g_signal_connect(G_OBJECT(spinButtonSw2), 
                    "value-changed", 
                    G_CALLBACK(spinSw2Changed), 
                    (gpointer)qp);
    connect_timeout();
    gtk_widget_show_all(window);
}

void CGtk::show_connected(){
    GtkWidget *img;
    img = gtk_image_new_from_stock(GTK_STOCK_APPLY, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(button_connect),img);
}
void CGtk::show_disconnected(){
    GtkWidget *img;
    img = gtk_image_new_from_stock(GTK_STOCK_DIALOG_ERROR, 
                                   GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(button_connect),img);
}

void CGtk::show_left(){
    GtkWidget *img;
    img = gtk_image_new_from_stock(GTK_STOCK_GO_BACK, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(button_leg),img);
}

void CGtk::show_right(){
    GtkWidget *img;
    img = gtk_image_new_from_stock(GTK_STOCK_GO_FORWARD, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(button_leg),img);
}

void CGtk::updateADC(){
    char text[100];
    sprintf(text, GUI_ADC_LABEL_FORMAT,
            qp->adc[0],
            qp->kalman1.getX(), 
            qp->adc[1],
            qp->kalman2.getX()
           );
    gtk_label_set_markup(GTK_LABEL(adc_label), text);
}

void CGtk::updateGamePadDrawing(){
    paintGP(gamepadDrawing, NULL, this);
}

void CGtk::updateServoData(){
    qp->readFromDev();
    uint8_t i;
    char text[100];
    for(i=0;i<SERVOS;i++){
        sprintf(text, GUI_SERVO_LABEL_FORMAT, i,
            qp->getAngle(i),
            qp->getPW(i)
        );
        //gtk_button_set_label(GTK_BUTTON(servo_label[i]),text);
        gtk_label_set_markup(GTK_LABEL(servo_label[i]),text);
    }
}

void CGtk::connect_timeout(){
    timeoutID = g_timeout_add_full(
        G_PRIORITY_DEFAULT,
        GUI_TIMEOUT,
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
    for(i=0;i<QP_LEGS;i++){
        sprintf(text, GUI_POSITION_LABEL_FORMAT,
            qp->getRelativeServoX(i, LEG_ENDPOINT),
            qp->getRelativeServoY(i, LEG_ENDPOINT),
            qp->getRelativeServoZ(i, LEG_ENDPOINT)
        );
        gtk_label_set_text(GTK_LABEL(position_label[i]),text);
    }
}

 

static gboolean 
key_press_callback(GtkWidget* widget, GdkEvent *event, gpointer data){
    guint key = ((GdkEventKey*)event)->keyval;
    CGtk* gui = ((CGtk*)data);
    gboolean keypressHandled = TRUE;
    switch(key){
    case 'q':
        gui->running = 0;
        break;
    case '`':
        gui->qp->reset();
        gui->qp->sendToDev();
        break;
    case 'w':
        if(gui->qp->changeAllLegs(0,-GUI_KEYBOARD_SPEED,0)==0)
        gui->qp->sendToDev();
        break;
    case 's':
        if(gui->qp->changeAllLegs(0,GUI_KEYBOARD_SPEED,0)==0)
        gui->qp->sendToDev();
        break;
    case 'a':
        if(gui->qp->changeAllLegs(GUI_KEYBOARD_SPEED,0,0)==0)
        gui->qp->sendToDev();
        break;
    case 'd':
        if(gui->qp->changeAllLegs(-GUI_KEYBOARD_SPEED,0,0)==0)
        gui->qp->sendToDev();
        break;
    case 'W':
        if(gui->qp->changeSingleLeg(gui->selected_leg, 0, -GUI_KEYBOARD_SPEED,0)==0)
        gui->qp->sendToDev();
        break;
    case 'S':
        if(gui->qp->changeSingleLeg(gui->selected_leg, 0, GUI_KEYBOARD_SPEED,0)==0)
        gui->qp->sendToDev();
        break;
    case 'A':
        if(gui->qp->changeSingleLeg(gui->selected_leg, GUI_KEYBOARD_SPEED,0,0)==0)
        gui->qp->sendToDev();
        break;
    case 'D':
        if(gui->qp->changeSingleLeg(gui->selected_leg, -GUI_KEYBOARD_SPEED,0,0)==0)
        gui->qp->sendToDev();
        break;
    case 'f':
        if(gui->qp->changeMainBodyAngle(0,GUI_KEYBOARD_SPEED/10,0)==0)
        gui->qp->sendToDev();
        break;
    case 'h':
        if(gui->qp->changeMainBodyAngle(0,-GUI_KEYBOARD_SPEED/10,0)==0)
        gui->qp->sendToDev();
        break;
    case 't':
        if(gui->qp->changeMainBodyAngle(0,0,GUI_KEYBOARD_SPEED/10)==0)
        gui->qp->sendToDev();
        break;
    case 'g':
        if(gui->qp->changeMainBodyAngle(0,0,-GUI_KEYBOARD_SPEED/10)==0)
        gui->qp->sendToDev();
        break;
    case 'z':
        if(gui->qp->changeAllLegs(0,0,-GUI_KEYBOARD_SPEED)==0)
        gui->qp->sendToDev();
        break;
    case 'x':
        if(gui->qp->changeAllLegs(0,0,GUI_KEYBOARD_SPEED)==0)
        gui->qp->sendToDev();
        break;
    default :
        keypressHandled = FALSE;
    }
    usleep(10000);//allow device to transmit before next command;
    gui->updateServoData();
    gui->updatePositions();
    paint(gui->da, NULL, gui);
    return keypressHandled;
}


static void connect_clicked_cb(GtkButton *button, gpointer data){
    CGtk* gui = ((CGtk*)data);
    gui->qp->reset();
    gui->qp->sendToDev();
    timeout1(data);
}
static void controller_clicked_cb(GtkButton *button, gpointer data){
    CGtk* gui = ((CGtk*)data);
    if(gui->selected_leg == 0){
        gui->selected_leg = 1;
        gui->show_left();
    }else{
        gui->selected_leg = 0;
        gui->show_right();
    }
}


static gboolean timeout1(gpointer data){
    CGtk* gui = ((CGtk*)data);
    if(gui->running == 0) return FALSE;
    gui->qp->getUsbData();
    gui->qp->fillPSController();
    gui->qp->fillADC();
    gui->updateADC();
    gui->graphX.addPoint(gui->qp->kalman1.getX());
    gui->graphY.addPoint(gui->qp->kalman2.getX());
    gui->updateGamePadDrawing();
    if(gui->qp->moveByStick()){
        gui->qp->sendToDev();
    //usleep(10000);//allow device to transmit before next command;
    //gui->updateServoData();

    }
    gui->updatePositions();    
    paint(gui->da, NULL, gui);
    paintTop(gui->topDa, NULL, gui);
    paintGraph(gui->graphDa, NULL, gui);
    if(gui->qp->getConnected()>1) gui->show_connected();
    else gui->show_disconnected();
    return TRUE;
}

static void timeout_disconnected(gpointer data){
    g_print("disconnected timeout\n");//placeholder
}


static void spinSz1Changed(GtkSpinButton *spinButton, gpointer data){
    CQPed *qp = ((CQPed *)data);
    qp->kalman1.setSz((double) (gtk_spin_button_get_value(spinButton)));

}

static void spinSw1Changed(GtkSpinButton *spinButton, gpointer data){
    CQPed *qp = ((CQPed *)data);
    qp->kalman1.setSw((double) (gtk_spin_button_get_value(spinButton)));

}
static void spinSz2Changed(GtkSpinButton *spinButton, gpointer data){
    CQPed *qp = ((CQPed *)data);
    qp->kalman2.setSz((double) (gtk_spin_button_get_value(spinButton)));

}

static void spinSw2Changed(GtkSpinButton *spinButton, gpointer data){
    CQPed *qp = ((CQPed *)data);
    qp->kalman2.setSw((double) (gtk_spin_button_get_value(spinButton)));

}






//------------ CAIRO STUFF ---------------------------

void drawTriangle(cairo_t *cr, double x, double y, double size, uint8_t turn){
    double mirror;
    if(turn % 2){ // 1 and 3
        mirror = pow(-1.0,turn/2);
        cairo_move_to(cr, x + mirror * size/2, y);
        cairo_rel_line_to(cr, -mirror*size, size/2);
        cairo_rel_line_to(cr, 0, -size);        
    }else{ //0 and 2
        mirror =  pow(-1.0, turn/2);
        cairo_move_to(cr, x, y - mirror *size/2);
        cairo_rel_line_to(cr, size/2, mirror * size);
        cairo_rel_line_to(cr, -size,0);
    }
    cairo_close_path(cr);
    cairo_stroke(cr);
}


#define BG_COLOR 1
#define DPADSPACING 35 //also used for shapes
#define STICKSIZE  60
//draw shapes for evey button, vary the color depending on the button state
static void paintGP(GtkWidget *widget, GdkEventExpose *eev, gpointer data){
    CGtk* gui = ((CGtk*)data);
    GtkAllocation alloc;
    gtk_widget_get_allocation(widget, &alloc);
    cairo_t *cr;
    cr = gdk_cairo_create(widget->window);
    cairo_set_source_rgb(cr,BG_COLOR,BG_COLOR,BG_COLOR);
    cairo_paint(cr);
    const double baseX = alloc.width/6*3;
    const double baseY = alloc.height/3;
//DPAD
    const double dpadCenterX = alloc.width/6*1;
    const double dpadCenterY = alloc.height * 1/3;
    //up button
    if(gui->qp->pscon.getSSDpad(UP)) cairo_set_source_rgb(cr, 1,0,0);
    else cairo_set_source_rgb(cr, 0,0,0);
    drawTriangle(cr, dpadCenterX, dpadCenterY - DPADSPACING, 20,0);
    //down button
    if(gui->qp->pscon.getSSDpad(DOWN)) cairo_set_source_rgb(cr, 1,0,0);
    else cairo_set_source_rgb(cr, 0,0,0);
    drawTriangle(cr, dpadCenterX, dpadCenterY + DPADSPACING, 20,2);
    //right button
    if(gui->qp->pscon.getSSDpad(RIGHT)) cairo_set_source_rgb(cr, 1,0,0);
    else cairo_set_source_rgb(cr, 0,0,0);
    drawTriangle(cr, dpadCenterX+DPADSPACING, dpadCenterY, 20,1);
    //left button
    if(gui->qp->pscon.getSSDpad(LEFT)) cairo_set_source_rgb(cr, 1,0,0);
    else cairo_set_source_rgb(cr, 0,0,0);
    drawTriangle(cr, dpadCenterX-DPADSPACING, dpadCenterY, 20,3);
//START SELECT
    //start
    if(gui->qp->pscon.getSSDpad(START)) cairo_set_source_rgb(cr, 1,0,0);
    else cairo_set_source_rgb(cr, 0,0,0);
    cairo_rectangle(cr, baseX+DPADSPACING/2, baseY,30, 15);
    cairo_stroke(cr);
    //select
    if(gui->qp->pscon.getSSDpad(SELECT)) cairo_set_source_rgb(cr, 1,0,0);
    else cairo_set_source_rgb(cr, 0,0,0);
    cairo_rectangle(cr, baseX-30-DPADSPACING/2, baseY,30, 15);
    cairo_stroke(cr);
//shapes
    const double shapesCenterX = alloc.width/6*5;
    const double shapesCenterY = alloc.height * 1/3;
    //triangle
    if(gui->qp->pscon.getShoulderShapes(TRIANGLE)) 
        cairo_set_source_rgb(cr, 1,0,0);
    else 
        cairo_set_source_rgb(cr, 0,0,0);
    drawTriangle(cr, shapesCenterX, shapesCenterY - DPADSPACING, 20,0);
    //cross
    if(gui->qp->pscon.getShoulderShapes(CROSS)) 
        cairo_set_source_rgb(cr, 1,0,0);
    else 
        cairo_set_source_rgb(cr, 0,0,0);
    cairo_move_to(cr, shapesCenterX+10, shapesCenterY + DPADSPACING-10);
    cairo_rel_line_to(cr, -20, +20);
    cairo_rel_move_to(cr, 20,0);
    cairo_rel_line_to(cr, -20, -20);
    cairo_stroke(cr);
    //square
    if(gui->qp->pscon.getShoulderShapes(SQUARE)) 
        cairo_set_source_rgb(cr, 1,0,0);
    else 
        cairo_set_source_rgb(cr, 0,0,0);
    cairo_rectangle(cr, shapesCenterX-DPADSPACING-10,shapesCenterY-10,20,20);
    cairo_stroke(cr);
    //circle
    if(gui->qp->pscon.getShoulderShapes(CIRCLE)) 
        cairo_set_source_rgb(cr, 1,0,0);
    else 
        cairo_set_source_rgb(cr, 0,0,0);
    cairo_arc(cr, 
              shapesCenterX + DPADSPACING, 
              shapesCenterY, 
              10, 
              0, 
              -2*PI-0.0001);
    cairo_stroke(cr);
//left stick
    const double leftCenterX = (dpadCenterX+baseX)/2;
    const double leftCenterY = alloc.height/3*2;
    //boundary circle
    cairo_set_source_rgb(cr, 0,0,0);
    cairo_move_to(cr, leftCenterX,leftCenterY-STICKSIZE/2);
    cairo_rel_line_to(cr, 0, STICKSIZE);
    cairo_move_to(cr, leftCenterX-STICKSIZE/2, leftCenterY);
    cairo_rel_line_to(cr, STICKSIZE, 0);
    cairo_arc(cr, leftCenterX, leftCenterY, STICKSIZE/2, 0, -2*PI-0.0001);
    cairo_stroke(cr);
    //marker
    cairo_set_source_rgb(cr, 1,0,0);
    double dx = ((double)gui->qp->pscon.getLx())/140*STICKSIZE/2;
    double dy = ((double)gui->qp->pscon.getLy())/140*STICKSIZE/2;
    cairo_arc(cr,
              leftCenterX +dx, 
              leftCenterY + dy,
              STICKSIZE/10, 
              0, 
              -2*PI-0.0001);
    cairo_stroke(cr);
//right stick
    const double rightCenterX = (shapesCenterX + baseX)/2;
    const double rightCenterY = alloc.height/3*2;
    //boundary circle
    cairo_set_source_rgb(cr, 0,0,0);
    cairo_move_to(cr, rightCenterX,rightCenterY-STICKSIZE/2);
    cairo_rel_line_to(cr, 0, STICKSIZE);
    cairo_move_to(cr, rightCenterX-STICKSIZE/2, rightCenterY);
    cairo_rel_line_to(cr, STICKSIZE, 0);
    cairo_arc(cr, rightCenterX, rightCenterY, STICKSIZE/2, 0, -2*PI-0.0001);
    cairo_stroke(cr);
    //marker
    cairo_set_source_rgb(cr, 1,0,0);
    dx = ((double)gui->qp->pscon.getRx())/140*STICKSIZE/2;
    dy = ((double)gui->qp->pscon.getRy())/140*STICKSIZE/2;
    cairo_arc(cr, 
              rightCenterX +dx, 
              leftCenterY + dy,
              STICKSIZE/10, 
              0, 
              -2*PI-0.0001);
    cairo_stroke(cr);
//shoulder buttons
    //L1
    if(gui->qp->pscon.getShoulderShapes(L1)) cairo_set_source_rgb(cr, 1,0,0);
    else cairo_set_source_rgb(cr, 0,0,0);
    cairo_rectangle(cr, 0, 0, 30, 15);
    cairo_stroke(cr);
    //L2
    if(gui->qp->pscon.getShoulderShapes(L2)) cairo_set_source_rgb(cr, 1,0,0);
    else cairo_set_source_rgb(cr, 0,0,0);
    cairo_rectangle(cr, 0, 30, 30, 15);
    cairo_stroke(cr);
    //R1
    if(gui->qp->pscon.getShoulderShapes(R1)) cairo_set_source_rgb(cr, 1,0,0);
    else cairo_set_source_rgb(cr, 0,0,0);
    cairo_rectangle(cr, alloc.width-30, 0, 30, 15);
    cairo_stroke(cr);
    //R2
    if(gui->qp->pscon.getShoulderShapes(R2)) cairo_set_source_rgb(cr, 1,0,0);
    else cairo_set_source_rgb(cr, 0,0,0);
    cairo_rectangle(cr, alloc.width-30, 30, 30, 15);
    cairo_stroke(cr);
                    
//grid
//    cairo_move_to(cr, alloc.width/6*1, 0);
//    cairo_rel_line_to(cr, 0, alloc.height);
//    cairo_move_to(cr, alloc.width/6*2, 0);
//    cairo_rel_line_to(cr, 0, alloc.height);
//    cairo_move_to(cr, alloc.width/6*3, 0);
//    cairo_rel_line_to(cr, 0, alloc.height);
//    cairo_move_to(cr, alloc.width/6*4, 0);
//    cairo_rel_line_to(cr, 0, alloc.height);
//    cairo_move_to(cr, alloc.width/6*5, 0);
//    cairo_rel_line_to(cr, 0, alloc.height);
//    cairo_stroke(cr);    
        cairo_destroy(cr);    

}

//no rotation support, deprecated
void drawLeg(cairo_t *cr, gpointer data,  uint8_t leg, double  startX,double startY){
    CGtk* gui = ((CGtk*)data);    
    cairo_set_source_rgb(cr, 0,0.8, 0);
    double x = startX;
    double y = startY;
    //endpoint -> servo 2
    cairo_rectangle(cr, x-5, y-5, 10, 10);
    cairo_move_to(cr, x,y);
    x +=( gui->qp->legs[leg]->getX(2) -gui->qp->legs[leg]->getX(3) )*GUI_DRAW_SCALE; 
    y -=( gui->qp->legs[leg]->getY(2) -gui->qp->legs[leg]->getY(3) )*GUI_DRAW_SCALE; 
    cairo_line_to(cr, x,y);
    
    cairo_rectangle(cr, x-5, y-5, 10, 10);
    cairo_move_to(cr, x,y);
    x +=( gui->qp->legs[leg]->getX(1) -gui->qp->legs[leg]->getX(2) )*GUI_DRAW_SCALE; 
    y -=( gui->qp->legs[leg]->getY(1) -gui->qp->legs[leg]->getY(2) )*GUI_DRAW_SCALE; 
    cairo_line_to(cr, x,y);

    cairo_rectangle(cr, x-5, y-5, 10, 10);
    cairo_move_to(cr, x,y);
    x +=( gui->qp->legs[leg]->getX(0) -gui->qp->legs[leg]->getX(1) )*GUI_DRAW_SCALE; 
    y -=( gui->qp->legs[leg]->getY(0) -gui->qp->legs[leg]->getY(1) )*GUI_DRAW_SCALE; 
    cairo_line_to(cr, x,y);
    cairo_rectangle(cr, x-5, y-5, 10, 10);
    
    cairo_stroke(cr);
}

void drawLeg_around_0(cairo_t *cr, gpointer data,  uint8_t leg, double  startX,double startY){
    CGtk* gui = ((CGtk*)data);    
    double x = startX;
    double y = startY;
    //0 -> servo 0
//    cairo_rectangle(cr, x-5, y-5, 10, 10);
    cairo_move_to(cr, x,y);
    x +=( gui->qp->legs[leg]->getX(0) )*GUI_DRAW_SCALE; 
    y -=( gui->qp->legs[leg]->getY(0) )*GUI_DRAW_SCALE; 
    cairo_line_to(cr, x,y);
    //servo 0 -> servo 1
    cairo_rectangle(cr, x-5, y-5, 10, 10);
    cairo_move_to(cr, x,y);
    x +=( gui->qp->legs[leg]->getX(1) -gui->qp->legs[leg]->getX(0) )*GUI_DRAW_SCALE; 
    y -=( gui->qp->legs[leg]->getY(1) -gui->qp->legs[leg]->getY(0) )*GUI_DRAW_SCALE; 
    cairo_line_to(cr, x,y);
    //servo 1 -> servo 2    
    cairo_rectangle(cr, x-5, y-5, 10, 10);
    cairo_move_to(cr, x,y);
    x +=( gui->qp->legs[leg]->getX(2) -gui->qp->legs[leg]->getX(1) )*GUI_DRAW_SCALE; 
    y -=( gui->qp->legs[leg]->getY(2) -gui->qp->legs[leg]->getY(1) )*GUI_DRAW_SCALE; 
    cairo_line_to(cr, x,y);
    //servo 2 -> endPoint
    cairo_rectangle(cr, x-5, y-5, 10, 10);
    cairo_move_to(cr, x,y);
    x +=( gui->qp->legs[leg]->getX(3) -gui->qp->legs[leg]->getX(2) )*GUI_DRAW_SCALE; 
    y -=( gui->qp->legs[leg]->getY(3) -gui->qp->legs[leg]->getY(2) )*GUI_DRAW_SCALE; 
    cairo_line_to(cr, x,y);
    cairo_rectangle(cr, x-5, y-5, 10, 10);
    
    cairo_stroke(cr);
}

void drawLineThrough(cairo_t *cr, double x1, double y1, double x2, double y2){
    const double dx = x2 - x1;
    const double dy = y2 - y1;
    const double b = y1 - dy/dx * x1;
    //assume color is set
    cairo_move_to(cr, x1, y1);
    cairo_rel_move_to(cr, -x1, dy/dx * -x1);
    cairo_rel_line_to(cr, x2 + x1, dy/dx * (x1+x2));
    cairo_stroke(cr);
}

//--------------------------
// PAINT QUADRUPED SIDEVIEW
//--------------------------
static void paint(GtkWidget *widget, GdkEventExpose *eev, gpointer data){
    CGtk* gui = ((CGtk*)data);
    GtkAllocation alloc;
    gtk_widget_get_allocation(widget, &alloc);
    cairo_t *cr;
    cr = gdk_cairo_create(widget->window);
    cairo_set_line_width(cr,GUI_LINEWIDTH);
    double startX = alloc.width/2;
    double startY = 100;
    //clear area
    cairo_set_source_rgb(cr,BG_COLOR,BG_COLOR,BG_COLOR);
    cairo_paint(cr);

    //draw side view
    cairo_set_line_width(cr, 0.7);
    cairo_set_source_rgb(cr, 1,0,0);  
    drawLeg_around_0(cr,data, 2, startX, startY);
    drawLeg_around_0(cr,data, 3, alloc.width/2, startY);
    cairo_set_source_rgb(cr, 0,0, 0.9);
    drawLeg_around_0(cr,data, 0, alloc.width/2, startY);
    drawLeg_around_0(cr,data, 1, alloc.width/2, startY);
    
    //draw line through both endPoints
    cairo_set_source_rgb(cr,1,0,0);    
    drawLineThrough(cr,
        alloc.width/2 +  gui->qp->legs[1]->getX(3) * GUI_DRAW_SCALE,
        startY - gui->qp->legs[1]->getY(3) * GUI_DRAW_SCALE,
        alloc.width/2 +  gui->qp->legs[0]->getX(3) * GUI_DRAW_SCALE,
        startY - gui->qp->legs[0]->getY(3) * GUI_DRAW_SCALE
    );
    //line thourgh main body
    cairo_set_source_rgb(cr,0,0,0);    
    cairo_set_line_width(cr,1);    
    
    drawLineThrough(cr,
        alloc.width/2 +  gui->qp->legs[1]->getX(0) * GUI_DRAW_SCALE,
        startY - gui->qp->legs[1]->getY(0) * GUI_DRAW_SCALE,
        alloc.width/2 +  gui->qp->legs[0]->getX(0) * GUI_DRAW_SCALE,
        startY - gui->qp->legs[0]->getY(0) * GUI_DRAW_SCALE
    );
    
    cairo_destroy(cr);
    return;
}

//-----------------
// PAINT SIDE VIEW
//-----------------
static void paintTop(GtkWidget *widget, GdkEventExpose *eev, gpointer data){
    CGtk* gui = ((CGtk*)data);
    GtkAllocation alloc;
    gtk_widget_get_allocation(widget, &alloc);
    cairo_t *cr;
    cr = gdk_cairo_create(widget->window);
    cairo_set_line_width(cr,GUI_LINEWIDTH);
    double startX = alloc.width/2;
    double startY = alloc.height/2;
    //clear area
    cairo_set_source_rgb(cr,BG_COLOR,BG_COLOR,BG_COLOR);
    cairo_paint(cr);
    //draw legs
    cairo_set_line_width(cr, 0.5);

    cairo_set_source_rgb(cr, 0,0,1); 
    drawLegTop(cr, data, 0, startX, startY);
    drawLegTop(cr, data, 1, startX, startY);
    cairo_set_source_rgb(cr, 1,0,0); 
    drawLegTop(cr, data, 2, startX, startY);
    drawLegTop(cr, data, 3, startX, startY);
    
    //draw triangles
    cairo_set_source_rgb(cr, 0,0.8,0);
    cairo_set_line_width(cr, 0.7);
    drawLineThrough(cr, 
        startX + gui->qp->legs[0]->getX(3) * GUI_DRAW_SCALE,
        startY - gui->qp->legs[0]->getZ(3) * GUI_DRAW_SCALE,
        startX + gui->qp->legs[3]->getX(3) * GUI_DRAW_SCALE,
        startY - gui->qp->legs[3]->getZ(3) * GUI_DRAW_SCALE
    );
    drawLineThrough(cr, 
        startX + gui->qp->legs[1]->getX(3) * GUI_DRAW_SCALE,
        startY - gui->qp->legs[1]->getZ(3) * GUI_DRAW_SCALE,
        startX + gui->qp->legs[2]->getX(3) * GUI_DRAW_SCALE,
        startY - gui->qp->legs[2]->getZ(3) * GUI_DRAW_SCALE
    );
        
    
    cairo_set_source_rgb(cr, 0,0,0); 
    drawCross(cr, startX, startY);

}

void drawCross(cairo_t *cr, double x, double y){
    //assume color is set
    cairo_move_to(cr, 0, y);
    cairo_rel_line_to(cr, GUI_DA_SIZE,0);
    cairo_move_to(cr,x, 0);
    cairo_rel_line_to(cr, 0, GUI_DA_SIZE);
    cairo_stroke(cr);

}

//------------------
// DRAW TOPDOWN LEG
//------------------
void 
drawLegTop(cairo_t *cr, 
           gpointer data,
           uint8_t leg, 
           double startX,
           double startY)
{
    CGtk* gui = ((CGtk*)data);    
    double x = startX;
    double y = startY;
    //0 -> servo 0
    cairo_move_to(cr, x,y);
    x +=( gui->qp->legs[leg]->getX(0) )*GUI_DRAW_SCALE; 
    y -=( gui->qp->legs[leg]->getZ(0) )*GUI_DRAW_SCALE; 
    cairo_line_to(cr, x,y);
    //servo 0 -> servo 1
    cairo_rectangle(cr, x-5, y-5, 10, 10);
    cairo_move_to(cr, x,y);
    x +=( gui->qp->legs[leg]->getX(1)
         -gui->qp->legs[leg]->getX(0) )*GUI_DRAW_SCALE; 
    y -=( gui->qp->legs[leg]->getZ(1) 
         -gui->qp->legs[leg]->getZ(0) )*GUI_DRAW_SCALE; 
    cairo_line_to(cr, x,y);
    //servo 1 -> servo 2    
    cairo_rectangle(cr, x-5, y-5, 10, 10);
    cairo_move_to(cr, x,y);
    x +=( gui->qp->legs[leg]->getX(2) 
         -gui->qp->legs[leg]->getX(1) )*GUI_DRAW_SCALE; 
    y -=( gui->qp->legs[leg]->getZ(2) 
         -gui->qp->legs[leg]->getZ(1) )*GUI_DRAW_SCALE; 
    cairo_line_to(cr, x,y);
    //servo 2 -> endPoint
    cairo_rectangle(cr, x-5, y-5, 10, 10);
    cairo_move_to(cr, x,y);
    x +=( gui->qp->legs[leg]->getX(3) 
         -gui->qp->legs[leg]->getX(2) )*GUI_DRAW_SCALE; 
    y -=( gui->qp->legs[leg]->getZ(3) 
         -gui->qp->legs[leg]->getZ(2) )*GUI_DRAW_SCALE; 
    cairo_line_to(cr, x,y);
    drawCross(cr, x,y);
    cairo_rectangle(cr, x-5, y-5, 10, 10);
    cairo_stroke(cr);

}


//--------------
//  PAINT GRAPH
//--------------
static void
paintGraph(GtkWidget *widget, GdkEventExpose *eev, gpointer data){
    CGtk* gui = ((CGtk*)data);    
    GtkAllocation alloc;
    gtk_widget_get_allocation(widget, &alloc);
    cairo_t *cr;
    cr = gdk_cairo_create(widget->window);
    cairo_set_line_width(cr,0.7);
    //clear
    cairo_set_source_rgb(cr, 1,1,1);
    cairo_paint(cr);
    //0 line
    cairo_set_source_rgb(cr, 0,0,0);
    cairo_move_to(cr, 0, alloc.height/2);
    cairo_rel_line_to(cr,alloc.width,0);
    cairo_stroke(cr);
    
    //graphs
    cairo_set_source_rgb(cr, 0,0,1);
    createLineFromGraph(cr, &(gui->graphX), alloc.height/2.0, 5.0);
    cairo_set_source_rgb(cr, 1,0,0);
    createLineFromGraph(cr, &(gui->graphY), alloc.height/2.0, 5.0);
    
}



//------------------------------
// CREATE LINE FROM GRAPH CLASS
//-------------------------------
void createLineFromGraph(cairo_t *cr, Graph* graph, double zeroY, double scale){
    //color and linewidth are assumed to be set
    cairo_move_to(cr, 0, zeroY);
    GraphIndex i;
    GraphIndex currentIndex = graph->getIndex();
    uint16_t xAxis = 0;
    for(i=currentIndex+1; i<GRAPH_DATA_LENGTH; i++){
        cairo_line_to(cr, xAxis, zeroY - (graph->getValue(i)*scale));
        xAxis++;
    }
    for(i=0; i < currentIndex+1; i++){
        cairo_line_to(cr, xAxis, zeroY - (graph->getValue(i)*scale));
        xAxis++;
    }
    cairo_stroke(cr);
    
}

