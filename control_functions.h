#include <gtk/gtk.h>
#include <math.h>

#define ARGUMENTS 5

int brightness = 50;                        //default manual brightness
int sharpness;
int contrast;
int saturation = 0;
char ss_label[20] = "0.020000 s";             //starting label for ss
long int shutter_speed = 20000;             //default manual ss (microseconds)
int iso = 450;                              //default manual ISO
char resolution[2][5] = {"1920", "1080"};   //default resolution
char command_arguments[ARGUMENTS + 1][10];
char auto_settings = 0x00;

static void brightness_control(GtkAdjustment *adj){
    brightness = (int)adj->value;
    sprintf(command_arguments[0], "-br %d ", brightness);
}

static void sharpness_control(GtkAdjustment *adj){
    sharpness = (int)adj->value;
    sprintf(command_arguments[1], "-sh %d ", sharpness);
}

static void contrast_control(GtkAdjustment *adj){
    contrast = (int)adj->value;
    sprintf(command_arguments[2], "-co %d ", contrast);
}

static void saturation_control(GtkAdjustment *adj){
    saturation = (int)adj->value;
    sprintf(command_arguments[3], "-sa %d ", saturation);
}

static void iso_control(GtkAdjustment *adj){
    iso = (int)adj->value;
    sprintf(command_arguments[4], "-ISO %d ", iso);

}

static void shutter_speed_control(GtkAdjustment *adj, GtkLabel *label){
    shutter_speed = 0.00000002 * pow(10, (double)adj->value * 8); //function to better set shutter speed
    if(label){
        sprintf(ss_label, "%f s", (double)shutter_speed/1000000);
        gtk_label_set_text(label, ss_label);
    }
}

static void resolution_control(GtkWidget *item, char* res){
    for(int i = 0; i < 5; i++){
        resolution[0][i] = '\0';
        resolution[1][i] = '\0';
    }
    strncpy(resolution[0], res, 4);
    strncpy(resolution[1], res+4, 4);
}
