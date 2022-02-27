#include <stdlib.h>
#include <stdio.h>
#define __USE_XOPEN
#include <signal.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Struct that defines alarm-attributes
struct alarm {
    pid_t pid;
    time_t alarm_time;
    int isActive;
};

// Max allow 10 alarms at a time
struct alarm alarms[10] = {10};
int alarm_index = 0;

// Format seconds since epoch to string YYYY-MM-DD HH:MM:SS
// https://zetcode.com/articles/cdatetime/
// Remember to free the pointer!
char* time_to_string(time_t raw_time) {
    if(raw_time == -1) return "Error: Cannot convert negative time.\n";
    struct tm *local_time = localtime(&raw_time);

    if(local_time == NULL) return "Error: Could not parse time.\n";

    // Create date/time string based on time struct.
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
    time_struct.tm_isdst = -1; // ✨ magic to remove daylight saving time ✨
    return mktime(&time_struct);
}

// Prints RING and sounds alarm
void ring(int alarm_index) {
    printf("\nAlarm #%d says RINGRING!\n", alarm_index + 1); // Starting on alarm 1.
    if (execlp("mpg123", "mpg123", "-q", "./RINGTONE.mp3", NULL) < 0)
        printf("(Your system doesn't support sound. Use Linux or Mac and make sure mpg123 is installed!)\n");
        // WSL doesn't allow access to sound drivers and therefore cannot play sound
}

// Schedule an alarm with the format YYYY-MM-DD_HH:MM:SS
void schedule() {
    // Check if the alarm clock is full (max 10 alarms)
    if (alarm_index >= sizeof (alarms) / sizeof (struct alarm)) {   
        printf("Alarm clock is full of stuff!\n\n");
        return;
    }
    
    printf("Schedule alarm at which date and time?\n");
    printf("YYYY-MM-DD_HH:MM:SS\n");
    
    char input[20];
    scanf("%s", input);
    time_t alarm_time = string_to_time(input);
    time_t current_time = time(NULL);

    if (current_time >= alarm_time) {
        printf("Cannot set alarm in the past!\n");
        return;
    }

    pid_t alarm_pid = fork();
    
    // Insert alarm in array
    struct alarm alarm_struct;
    alarm_struct.pid = alarm_pid;
    alarm_struct.alarm_time = alarm_time;
    alarm_struct.isActive = 1;

    alarms[alarm_index] = alarm_struct;
    
    if (alarm_pid == 0) {
        // Child process schedules alarm and sleeps
        sleep(alarm_time - current_time);
        ring(alarm_index);
        exit(EXIT_SUCCESS);
    }
    // Continue in parent process
    else if (alarm_pid > 0) alarm_index++;
}

// Lists alarms
void list() {
    printf("-------------\n");
    for (int i = 0; i < alarm_index; i++) {
        struct alarm a = alarms[i];
        if (!a.isActive) printf("CANCELLED ");
        if (a.isActive && a.alarm_time < time(NULL)) printf("FINISHED ");
        char* time = time_to_string(a.alarm_time);
        printf("- ");
        printf("Alarm #%d - time: %s (PID: %d)\n", i+1, time, a.pid);
        free(time);
    }
    printf("-------------\n");
}
 
// Cancel alarm with the given ID
void cancel() {
    printf("Which alarm do you want to stop? ");
    int alarm_number;
    scanf("%d", &alarm_number);

    int alarm_index = alarm_number - 1;
    pid_t pid = alarms[alarm_index].pid;
    alarms[alarm_index].isActive = 0;
    kill(pid, SIGKILL);
    printf("Stopped alarm #%d (PID: %d)\n", alarm_number, pid);
}

// Runs the program and takes program action as input
int main() {
    
    time_t seconds = time(NULL);

    char* date_time = time_to_string(seconds);
    printf("Welcome to the alarm clock! It is currently today! %s\n", date_time);
    free(date_time);

    char input;
    
    while(input != 'x') {
    printf("Choose action > ");
    scanf(" %c", &input);
        switch (input) {
            case 's': schedule(); break;
            case 'l': list(); break;
            case 'c': cancel(); break;
            case 'x': printf("You are amazing! Good bye!\n"); break;
        }
        waitpid(-1, NULL, WNOHANG);
    }

    
    return 0;
}