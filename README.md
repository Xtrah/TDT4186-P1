# Alarm in C

This is a simple Unix-based alarm clock written in C, for the course [TDT4186 Operating Systems](https://www.ntnu.edu/studies/courses/TDT4186/2021) at NTNU spring 2022.

## Four test cases

Run the program using `./compile_and_run.sh`.

### Scheduling alarms

1. Run the program.
2. Schedule an alarm by entering `s`.
3. Enter time in format `YYYY-MM-DD_HH:MM:SS`.
4. Observe that your alarm is created by entering `l` to list alarms.
5. Schedule additional alarms and observe that they are all listed, until the maximum amount of alarms (10) is reached.
6. Observe that you are unable to schedule more than 10 alarms.
7. Exit the program by typing `x` in the menu.

### Cancelling alarms

1. Run the program.
2. Schedule an alarm by entering `s`.
3. Enter time in format `YYYY-MM-DD_HH:MM:SS`.
4. Observe that your alarm is created by entering `l` to list alarms.
5. Enter `c` with the alarm number to cancel alarm
6. Observe that your alarm is cancelled by entering `l` to list alarms.
7. Exit the program by typing `x` in the menu.

### Test alarm ringtone (requires Linux)

1. Run the program.
2. Schedule an alarm by entering `s`.
3. Enter time in format `YYYY-MM-DD_HH:MM:SS`.
4. Listen for an alarm tone when the alarm time is reached.
5. Exit the program by typing `x` in the menu.

### Check that zombies are killed

1. Open a second terminal window and run `top` to see processes. Processes marked with "Z" are zombies. Observe that there are no zombie processes.
2. In the primary terminal, run the program and schedule an alarm.
3. Cancel the alarm with `c` using the alarm number.
4. Observe the zombie process in the `top` output.
5. Executing any new command, such as scheduling, listing or cancelling alarms, will catch and kill all existing zombies.
6. Exit the program by typing `x` in the menu.
