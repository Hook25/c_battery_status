#include<stdio.h>
#include<string.h>

#ifndef NOTIFY 
#define NOTIFY 1
#endif

#define LINE_SIZE 80
#define BATTERY "Battery"
#define STATUS "[%s%3s]"

#define S_CHARGING "Charging,"
#define S_UNKNOWN  "Unknown,"
#define S_FULL "Full,"
#define S_DISCHARGING "Discharging,"
#define S_NOT_CHARGING "Not"
#define S_NOT_CHARGING_1 "charging," //well the thing has a space in it

#define L_C "C"
#define L_U "U"
#define L_D "D"
#define L_F "F"
#define L_N "N"

#ifdef DEBUG
#define expect(x) do{if(!(x)){ printf("Failed expectance at %s:%d", __FILE__, __LINE__); return 1;}}while(0)
#else
#define expect(x) do{if(!(x)){ return 1; }}while(0)
#endif

#if NOTIFY
#include <libnotify/notify.h>
#include <ctype.h>
#define NAME_TITLE "BatteryStatus"

char charging = 0;

#define NOTIFICATION_PERC 5

void notify(char *message){
  notify_init(NAME_TITLE);
	NotifyNotification *msg = notify_notification_new(NAME_TITLE, 
    message, "battery");
	notify_notification_show (msg, 0);
	g_object_unref(G_OBJECT(msg));
	notify_uninit();
}
char *skip_heading_spaces(char *input){
  char c = *input;
  while(c && isspace(c)){
    input++;
    c = *input;
  }
  return input;
}
int should_show(double perc){
  return perc <= NOTIFICATION_PERC && !charging;
}
int guard(char *level, char print){
  static long part_level, n;
  if(print && should_show(((double)part_level)/n)){
    notify("Battery is low");
  }else if(!print){
    level = skip_heading_spaces(level);
    part_level += strtol(level, 0, 10);
    n++; 
  }
  return 0;
}
#else
int guard(char *level, char print){ return 0;}
#endif

int elab_line(char line[LINE_SIZE]){
  static const char sep[2] = " ";
  char *i = strtok(line, sep);
  expect(i);
  char *label = L_U;
  expect(strcmp(i, BATTERY) == 0);
  i = strtok(0, sep); //batt number
  expect(i);
  i = strtok(0, sep);
  expect(i);
  if(!(strcmp(i, S_CHARGING))){
    label = L_C;
    charging = 1;
  }else if(!(strcmp(i, S_DISCHARGING))){
    label = L_D;
  }else if(!(strcmp(i, S_FULL))){
    label = L_F;
    guard("100", 0); 
    printf(STATUS, label, "ULL");
    return 0; //sucks, but works
  }else if(!strcmp(i, S_NOT_CHARGING)){
    label = L_N;
    i = strtok(0, sep);
    expect(!(strcmp(i, S_NOT_CHARGING_1)));
  }else{
    expect(!(strcmp(i, S_UNKNOWN)));
  }
  i = strtok(0, sep);
  expect(i);
  i = strtok(i, "%");
  expect(i);
  printf(STATUS, label, i);
  guard(i, 0);
  return 0;
}

int main(void){
  const char *cmd = "acpi";
  static char line[LINE_SIZE]; //auto 0 static alloc
  FILE *f = popen(cmd, "r");
  if(!f){
    fprintf(stderr, "Pipe failed to open\n");
    return 1;
  }
  int l = 0;
  while(fgets(line, LINE_SIZE, f)){
    l+=1;
    int err = elab_line(line);
    if(err){
      fprintf(stderr, "Failed to elab line %d\n", l);
    }
    memset(line, 0, sizeof(line));
  }
  printf("\n");
  guard(0, 1);
  return 0;

}
