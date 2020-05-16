#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "control_functions.h"
#include <gtk/gtk.h>

static void callback(GtkWidget * widget, gpointer data){
    g_print("You pressed button %s, brightness is %d\n", (gchar *) data, brightness);
}

static GtkWidget *make_menu_item(gchar *name, GCallback callback, char* data){
    GtkWidget *item;
    item = gtk_menu_item_new_with_label(name);
    g_signal_connect(item, "activate", callback, data);
    gtk_widget_show(item);
    return item;
}

static gboolean delete_event(GtkWidget *widget, gpointer data){
    gtk_main_quit();
    return FALSE;
}

static void auto_ss(GtkToggleButton *button, GtkAdjustment *adj){
    //button->active ? (shutter_speed = -1) : (shutter_speed = (long int)adj->value);
    if(button->active){
        shutter_speed = -1;
    }else{
        shutter_speed = 0;
        shutter_speed_control(adj, NULL);
    }
}

static void picture(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char command[200] = "\0";

    if(shutter_speed == -1){
        sprintf(command, "raspistill -sh %d -co %d -br %d -sa %d -ISO %d -w %s -h %s -o %d-%02d-%02d_%02d:%02d:%02d.jpg", 
                sharpness, contrast, brightness, saturation, iso, resolution[0], resolution[1], tm.tm_year+1900, tm.tm_mon +1,
                tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);    
        }else{
            sprintf(command, "raspistill -sh %d -co %d -br %d -sa %d -ISO %d -ss %d -w %s -h %s -o %d-%02d-%02d_%02d:%02d:%02d.jpg", 
                    sharpness, contrast, brightness, saturation, iso, shutter_speed, resolution[0], resolution[1], tm.tm_year+1900, tm.tm_mon +1,
                    tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        }

    printf("command: %s\n", command); //debug printf
    system(command); //creates the actual command that is sent into cmd
}

static void controls(){

    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *opt, *menu, *item;
    GtkWidget *box1, *box2, *box3, *box4;
    GtkObject *adj1, *adj2;
    GtkWidget *scale, *brightness_scale, *saturation_scale, *sharpness_scale, *contrast_scale, *iso_scale, *ss_scale;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "testiliukuri");
    g_signal_connect(window, "delete-event", G_CALLBACK (delete_event), NULL);

    gtk_container_set_border_width (GTK_CONTAINER(window), 10);

    box1 = gtk_hbox_new(FALSE, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box1),10);
    gtk_container_add(GTK_CONTAINER(window), box1);
    gtk_widget_show(box1);

    label = gtk_label_new("Brightness");
    gtk_box_pack_start(GTK_BOX(box1), label, FALSE, FALSE, 0);
    gtk_label_set_angle(GTK_LABEL(label), (gdouble) 90);
    gtk_widget_show(label);

    adj1 = gtk_adjustment_new(50, 0, 101, 0, 0, 1);
    g_signal_connect(adj1, "value_changed", G_CALLBACK(brightness_control), NULL);
    brightness_scale = gtk_vscale_new(GTK_ADJUSTMENT(adj1));
    gtk_widget_set_size_request(GTK_WIDGET(brightness_scale), 50, -1);
    gtk_box_pack_start(GTK_BOX(box1), brightness_scale, TRUE, TRUE, 0);
    gtk_widget_show(brightness_scale);

    box2 = gtk_vbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(box1), box2, TRUE, TRUE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(box2),10);
    gtk_widget_show(box2);

    button = gtk_button_new_with_label("debug");
    g_signal_connect(button, "clicked", G_CALLBACK(callback), (gpointer) "button 1");
    gtk_box_pack_start(GTK_BOX(box2), button, TRUE, TRUE, 0);
    gtk_widget_show(button);

    label = gtk_label_new("Resolution");
    gtk_box_pack_start(GTK_BOX(box2), label, FALSE, FALSE, 0);
    gtk_widget_show(label);
    opt = gtk_option_menu_new();
    menu = gtk_menu_new();

    item = make_menu_item("1920 x 1080", G_CALLBACK(resolution_control), "19201080");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

    item = make_menu_item("2560 x 1440", G_CALLBACK(resolution_control), "25601440");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

    item = make_menu_item("3840 x 2160", G_CALLBACK(resolution_control), "38402160");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

    gtk_option_menu_set_menu(GTK_OPTION_MENU(opt), menu);
    gtk_box_pack_start(GTK_BOX(box2), opt, TRUE, TRUE, 0);
    gtk_widget_show(opt);

    label = gtk_label_new("Sharpness");
    gtk_box_pack_start(GTK_BOX(box2), label, FALSE, FALSE, 0);
    gtk_widget_show(label);

    adj1 = gtk_adjustment_new(0, -100, 101, 0.1, 1, 1);
    g_signal_connect(adj1, "value_changed", G_CALLBACK(sharpness_control), NULL);
    sharpness_scale = gtk_hscale_new(GTK_ADJUSTMENT(adj1));
    gtk_widget_set_size_request(GTK_WIDGET(sharpness_scale), 200, -1);
    gtk_box_pack_start(GTK_BOX(box2), sharpness_scale, TRUE, TRUE, 0);
    gtk_widget_show(sharpness_scale);

    label = gtk_label_new("Contrast");
    gtk_box_pack_start(GTK_BOX(box2), label, FALSE, FALSE, 0);
    gtk_widget_show(label);

    adj1 = gtk_adjustment_new(0, -100, 101, 0.1, 1, 1);
    g_signal_connect(adj1, "value_changed", G_CALLBACK(contrast_control), NULL);
    contrast_scale = gtk_hscale_new(GTK_ADJUSTMENT(adj1));
    gtk_widget_set_size_request(GTK_WIDGET(contrast_scale), 200, -1);
    gtk_box_pack_start(GTK_BOX(box2), contrast_scale, TRUE, TRUE, 0);
    gtk_widget_show(contrast_scale);

    label = gtk_label_new("Saturation");
    gtk_box_pack_start(GTK_BOX(box2), label, FALSE, FALSE, 0);
    gtk_widget_show(label);

    adj1 = gtk_adjustment_new(0, -100, 101, 0.1, 1, 1);
    g_signal_connect(adj1, "value_changed", G_CALLBACK(saturation_control), NULL);
    saturation_scale = gtk_hscale_new(GTK_ADJUSTMENT(adj1));
    gtk_widget_set_size_request(GTK_WIDGET(saturation_scale), 200, -1);
    gtk_box_pack_start(GTK_BOX(box2), saturation_scale, TRUE, TRUE, 0);
    gtk_widget_show(saturation_scale);

    label = gtk_label_new("ISO");
    gtk_box_pack_start(GTK_BOX(box2), label, FALSE, FALSE, 0);
    gtk_widget_show(label);

    adj1 = gtk_adjustment_new(450, 100, 801, 0.1, 1, 1);
    g_signal_connect(adj1, "value_changed", G_CALLBACK(iso_control), NULL);
    iso_scale = gtk_hscale_new(GTK_ADJUSTMENT(adj1));
    gtk_widget_set_size_request(GTK_WIDGET(iso_scale), 200, -1);
    gtk_box_pack_start(GTK_BOX(box2), iso_scale, TRUE, TRUE, 0);
    gtk_widget_show(iso_scale);

    button = gtk_check_button_new_with_label("Automatic shutter speed");
    //gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
    gtk_box_pack_start (GTK_BOX(box2), button, TRUE, TRUE, 0);
    gtk_widget_show(button);

    label = gtk_label_new("Shutter speed");
    gtk_box_pack_start(GTK_BOX(box2), label, FALSE, FALSE, 0);
    gtk_widget_show(label);

    label = gtk_label_new(ss_label);
    gtk_box_pack_start(GTK_BOX(box2), label, FALSE, FALSE, 0);
    gtk_widget_show(label);

    adj2 = gtk_adjustment_new(1.5, 1, 3, 0.01, 0.01, 1);
    g_signal_connect(adj2, "value_changed", G_CALLBACK(shutter_speed_control), GTK_LABEL(label));
    ss_scale = gtk_hscale_new(GTK_ADJUSTMENT(adj2));
    gtk_widget_set_size_request(GTK_WIDGET(ss_scale), 200, -1);
    gtk_box_pack_start(GTK_BOX(box2), ss_scale, TRUE, TRUE, 0);
    gtk_scale_set_draw_value(GTK_SCALE(ss_scale), FALSE);
    gtk_widget_show(ss_scale);

    g_signal_connect(button, "toggled", G_CALLBACK(auto_ss), adj2);
    
    button = gtk_button_new_with_label("Snap");
    g_signal_connect(button, "clicked", G_CALLBACK(picture), NULL);
    gtk_box_pack_start(GTK_BOX(box2), button, TRUE, TRUE, 0);
    gtk_widget_show(button);
    gtk_widget_show(window);

}

int main(int argc, char *argv[]){

    gtk_init(&argc, &argv);
    controls();
    gtk_main();

    return 0;
}