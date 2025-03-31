#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h> // For sleep()

#define MAX_PATIENTS 100

// Structure to store patient information
struct Patient {
    int pid;
    int arrival_time;
    int treatment_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int is_completed;
};

// Function to perform real-time SRTF scheduling
void srtf(struct Patient patients[], int *n) {
    int completed = 0, time = 0, min_remaining_time = INT_MAX;
    int shortest = -1;

    while (1) {
        shortest = -1;
        min_remaining_time = INT_MAX;

        // Find the patient with the shortest remaining treatment time
        for (int i = 0; i < *n; i++) {
            if (patients[i].arrival_time <= time &&
                !patients[i].is_completed &&
                patients[i].remaining_time < min_remaining_time) {
                min_remaining_time = patients[i].remaining_time;
                shortest = i;
            }
        }

        if (shortest == -1) {
            printf("No patients at time %d. Waiting...\n", time);
            time++;
            sleep(1);  // Simulating real-time delay
            continue;
        }

        // Treat the selected patient for 1 unit of time
        printf("At time %d: Treating Patient %d (Remaining Time: %d)\n", time, patients[shortest].pid, patients[shortest].remaining_time);
        patients[shortest].remaining_time--;
        time++;
        sleep(1); // Simulating real-time processing

        // If treatment is completed
        if (patients[shortest].remaining_time == 0) {
            completed++;
            patients[shortest].completion_time = time;
            patients[shortest].turnaround_time = patients[shortest].completion_time - patients[shortest].arrival_time;
            patients[shortest].waiting_time = patients[shortest].turnaround_time - patients[shortest].treatment_time;
            patients[shortest].is_completed = 1;
            printf("Patient %d completed treatment at time %d.\n", patients[shortest].pid, time);
        }

        // Check if all patients are completed
        if (completed == *n) {
            printf("\nAll patients have been treated!\n");
            break;
        }
    }
}

// Function to add multiple patients at once
void add_patients(struct Patient patients[], int *n) {
    int count;
    printf("How many patients do you want to add? ");
    scanf("%d", &count);

    for (int i = 0; i < count; i++) {
        if (*n >= MAX_PATIENTS) {
            printf("Max patient limit reached!\n");
            return;
        }

        printf("Enter arrival time and treatment time for Patient %d: ", *n + 1);
        scanf("%d %d", &patients[*n].arrival_time, &patients[*n].treatment_time);

        patients[*n].pid = *n + 1;
        patients[*n].remaining_time = patients[*n].treatment_time;
        patients[*n].is_completed = 0;

        (*n)++;
    }
    printf("%d patients added successfully!\n", count);
}

// Function to display final results
void display_results(struct Patient patients[], int n) {
    printf("\nFinal Results:\n");
    printf("PID\tArrival Time\tTreatment Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               patients[i].pid,
               patients[i].arrival_time,
               patients[i].treatment_time,
               patients[i].completion_time,
               patients[i].turnaround_time,
               patients[i].waiting_time);
    }
}

// Function to display average Turnaround Time and Waiting Time
void display_avg_times(struct Patient patients[], int n) {
    float total_tat = 0, total_wt = 0;
    for (int i = 0; i < n; i++) {
        total_tat += patients[i].turnaround_time;
        total_wt += patients[i].waiting_time;
    }
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

int main() {
    struct Patient patients[MAX_PATIENTS];
    int n = 0, choice;

    printf("Welcome to the Real-Time SRTF Hospital System!\n");

    while (1) {
        printf("\n1. Add patients\n2. Start treatment\n3. Show results\n4. Show average TAT and WT\n5. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_patients(patients, &n);
                break;
            case 2:
                if (n == 0) {
                    printf("No patients to treat!\n");
                } else {
                    srtf(patients, &n);
                }
                break;
            case 3:
                display_results(patients, n);
                break;
            case 4:
                if (n == 0) {
                    printf("No patients to calculate averages!\n");
                } else {
                    display_avg_times(patients, n);
                }
                break;
            case 5:
                printf("Exiting... Thank you!\n");
                return 0;
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
}
