#include <gtk/gtk.h>
#include <math.h>

int brightness = 50;
int sharpness;
int contrast;
int saturation;
char ss_label[20] = "0.020000";
long int shutter_speed = 20000;
int iso = 450;
char resolution[2][5] = {"1920", "1080"};


static void brightness_control(GtkAdjustment *adj){
    brightness = (int)adj->value;
}

static void sharpness_control(GtkAdjustment *adj){
    sharpness = (int)adj->value;
}

static void contrast_control(GtkAdjustment *adj){
    contrast = (int)adj->value;
}

static void saturation_control(GtkAdjustment *adj){
    saturation = (int)adj->value;
}

static void shutter_speed_control(GtkAdjustment *adj, GtkLabel *label){
    //(shutter_speed != -1) ? (shutter_speed = (long int)adj->value) : (shutter_speed = -1);
    if(shutter_speed != -1){
        shutter_speed = 0.00000002 * pow(10, (double)adj->value * 8);
        if(label){
            sprintf(ss_label, "%f", (double)shutter_speed/1000000);
            gtk_label_set_text(label, ss_label);
        }
        
    }else{
        shutter_speed = -1;
    }
}

static void iso_control(GtkAdjustment *adj){
    iso = (int)adj->value;
}

static void resolution_control(GtkWidget *item, char* res){
    for(int i = 0; i<5; i++){
        resolution[0][i] = '\0';
        resolution[1][i] = '\0';
    }
    strncpy(resolution[0], res, 4);
    strncpy(resolution[1], res+4, 4);
}