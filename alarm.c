// Hello there
#include <stdlib.h>
#include <stdio.h>
#define __USE_XOPEN
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Format seconds since epoch to string YYYY-MM-DD HH:MM:SS
// https://zetcode.com/articles/cdatetime/
// Remember to free!
char* time_to_string(time_t raw_time) {
    if(raw_time == -1) return "Error";
    struct tm *local_time = localtime(&raw_time);

    if(local_time == NULL) return "Error";

    char* date_time_string = malloc(sizeof(char)*30);
    sprintf(date_time_string, "%02d-%02d-%02d_%02d:%02d:%02d",
            local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,  
            local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

    return date_time_string;
}

// Format string YYYY-MM-DD HH:MM:SS to seconds since epoch
time_t string_to_time(char* date_time_string) {
    struct tm time_struct;
    if(strptime(date_time_string, "%Y-%m-%d_%H:%M:%S", &time_struct) == NULL) return -1;
    return mktime(&time_struct);
}

void schedule() {
    printf("Schedule alarm at which date and time?\n");
    printf("YYYY-MM-DD_HH:MM:SS\n");
    
    //time_t alarm = 0;
    //time_t current_time = time(NULL);
    //do {
    char input[20];
    scanf("%s", input);
    time_t alarm_time = string_to_time(input);
    time_t current_time = time(NULL);

    if (current_time >= alarm_time) {
        printf("current time is bigger than alarm time\n");
        printf("current: %ld\nalarm: %ld\n", current_time, alarm_time);
        return;
    }

    pid_t alarm = fork();
    if (alarm != 0) {
        printf("Alarm will ring in %ld seconds.\n", alarm_time-current_time);
        sleep(alarm_time - current_time);
        printf("Alarm %d!\a\n", alarm);
        // kill(alarm, SIGKILL);
    }
    else {
        printf("Parent exited...\n");
    }
    //} while (current_time > alarm || alarm != 0);
}

void list() {

}

void cancel() {

}



int main() {
    
    time_t seconds = time(NULL);

    char* date_time = time_to_string(seconds);
    printf("Welcome to the alarm clock! It is currently today! %s\n", date_time);
    free(date_time);

    char input;
    
    while(input != 'x') {
    scanf("%c", &input);
        switch (input) {
            case 's': schedule(); break;
            case 'l': list(); break;
            case 'c': cancel(); break;
            case 'x': break;
            default: printf("Try again...");
        }
    }
    return 0;
}