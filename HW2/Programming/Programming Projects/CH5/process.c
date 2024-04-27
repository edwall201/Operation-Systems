#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int original_burst_time;
} Process;

void fcfs(Process processes[], int n);
void sjf(Process processes[], int n);
void priority_scheduling(Process processes[], int n);
void round_robin(Process processes[], int n, int quantum);
void priority_round_robin(Process processes[], int n, int quantum);

int main() {
    int n, i, quantum;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    Process processes[n];
    for (i = 0; i < n; i++) {
        printf("Enter arrival time, burst time, and priority for process %d: ", i+1);
        scanf("%d %d %d", &processes[i].arrival_time, &processes[i].burst_time, &processes[i].priority);
        processes[i].pid = i+1;
        processes[i].remaining_time = processes[i].burst_time;
    }
    
    Process processes_for_FCFS[n];
    Process processes_for_SJF[n];
    Process processes_for_priority[n];
    Process processes_for_round_robin[n];
    Process processes_for_priority_round_robin[n];
    for (int i = 0; i < n; i++) {
    	processes_for_FCFS[i] = processes[i];
    	processes_for_SJF[i] = processes[i];
    	processes_for_priority[i] = processes[i];
    	processes_for_round_robin[i] = processes[i];
    	processes_for_priority_round_robin[i] = processes[i];
    }
    printf("\nFCFS Scheduling:\n");
    fcfs(processes_for_FCFS, n);
    
    printf("\nSJF Scheduling:\n");
    sjf(processes_for_SJF, n);
    
    printf("\nPriority Scheduling:\n");
    priority_scheduling(processes_for_priority, n);
    
    printf("\nEnter time quantum: ");
    scanf("%d", &quantum);
    
    printf("\nRound Robin Scheduling:\n");
    round_robin(processes_for_round_robin, n, quantum);
    
    printf("\nPriority with Round Robin Scheduling:\n");
    priority_round_robin(processes_for_priority_round_robin, n, quantum);
    
    return 0;
}

void fcfs(Process processes[], int n) {
    int i, total_waiting_time = 0, total_turnaround_time = 0;
    
    printf("Process\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    
    for (i = 0; i < n; i++) {
        processes[i].waiting_time = (i == 0) ? 0 : (processes[i-1].waiting_time + processes[i-1].burst_time);
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
        
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].pid, processes[i].arrival_time, processes[i].burst_time, 
               processes[i].waiting_time, processes[i].turnaround_time);
    }
    
    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / n);
}

void sjf(Process processes[], int n) {
    int i, j, total_waiting_time = 0, total_turnaround_time = 0, current_time = 0, min_burst_time, min_index;
    
    printf("Process\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    
    for (i = 0; i < n; i++) {
        min_burst_time = INT_MAX;
        min_index = -1;
        
        for (j = 0; j < n; j++) {
            if (processes[j].arrival_time <= current_time && processes[j].remaining_time < min_burst_time && processes[j].remaining_time > 0) {
                min_burst_time = processes[j].remaining_time;
                min_index = j;
            }
        }
        
        if (min_index == -1) {
            current_time++;
            i--;
            continue;
        }
        
        processes[min_index].remaining_time = 0;
        processes[min_index].waiting_time = current_time - processes[min_index].arrival_time;
        processes[min_index].turnaround_time = processes[min_index].waiting_time + processes[min_index].burst_time;
        total_waiting_time += processes[min_index].waiting_time;
        total_turnaround_time += processes[min_index].turnaround_time;
        current_time += processes[min_index].burst_time;
        
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\n", processes[min_index].pid, processes[min_index].arrival_time, processes[min_index].burst_time,
               processes[min_index].waiting_time, processes[min_index].turnaround_time);
    }
    
    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / n);
}

void priority_scheduling(Process *processes, int n) {
    int i, j, total_waiting_time = 0, total_turnaround_time = 0;
    int current_time = 0, completed = 0;
    int highest_priority, highest_priority_index;

    printf("Process\tArrival Time\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\n");

    for (i = 0; i < n; i++) {
        processes[i].original_burst_time = processes[i].burst_time;
    }

    while (completed < n) {
        highest_priority = 9999;
        highest_priority_index = -1;

        for (i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].priority < highest_priority && processes[i].burst_time > 0) {
                highest_priority = processes[i].priority;
                highest_priority_index = i;
            }
        }

        if (highest_priority_index == -1) {
            current_time++;
            continue;
        }

        processes[highest_priority_index].burst_time--;

        if (processes[highest_priority_index].burst_time == 0) {
            completed++;

            int waiting_time = current_time + 1 - processes[highest_priority_index].arrival_time - processes[highest_priority_index].original_burst_time;
            int turnaround_time = current_time + 1 - processes[highest_priority_index].arrival_time;

            total_waiting_time += waiting_time;
            total_turnaround_time += turnaround_time;

            printf("P%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[highest_priority_index].pid, processes[highest_priority_index].arrival_time,
                   processes[highest_priority_index].original_burst_time, processes[highest_priority_index].priority, waiting_time, turnaround_time);
        }

        current_time++;
    }

    printf("\nAverage Waiting Time: %.2f\n", (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / n);
}

void round_robin(Process *processes, int n, int time_quantum) {
    int i, total_waiting_time = 0, total_turnaround_time = 0;
    int current_time = 0, completed = 0;
    int burst_remaining[MAX_PROCESSES];

    printf("Process\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");

    for (i = 0; i < n; i++) {
        burst_remaining[i] = processes[i].burst_time;
    }

    while (completed < n) {
        for (i = 0; i < n; i++) {
            if (burst_remaining[i] > 0) {
                if (burst_remaining[i] > time_quantum) {
                    current_time += time_quantum;
                    burst_remaining[i] -= time_quantum;
                } else {
                    current_time += burst_remaining[i];
                    int waiting_time = current_time - processes[i].arrival_time - processes[i].burst_time;
                    int turnaround_time = current_time - processes[i].arrival_time;

                    total_waiting_time += waiting_time;
                    total_turnaround_time += turnaround_time;

                    printf("P%d\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].pid, processes[i].arrival_time,
                           processes[i].burst_time, waiting_time, turnaround_time);

                    burst_remaining[i] = 0;
                    completed++;
                }
            }
        }
    }

    printf("\nAverage Waiting Time: %.2f\n", (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / n);
}

void priority_round_robin(Process processes[], int n, int quantum) {
    int i, time = 0, total_waiting_time = 0, total_turnaround_time = 0, completed = 0, highest_priority, highest_priority_index;
    
    printf("Process\tArrival Time\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\n");
    
    while (completed < n) {
        highest_priority = INT_MAX;
        highest_priority_index = -1;
        
        for (i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && processes[i].priority < highest_priority && processes[i].remaining_time > 0) {
                highest_priority = processes[i].priority;
                highest_priority_index = i;
            }
        }
        
        if (highest_priority_index == -1) {
            time++;
            continue;
        }
        
        if (processes[highest_priority_index].remaining_time <= quantum) {
            time += processes[highest_priority_index].remaining_time;
            processes[highest_priority_index].remaining_time = 0;
            completed++;
            
            processes[highest_priority_index].waiting_time = time - processes[highest_priority_index].arrival_time - processes[highest_priority_index].burst_time;
            processes[highest_priority_index].turnaround_time = time - processes[highest_priority_index].arrival_time;
            total_waiting_time += processes[highest_priority_index].waiting_time;
            total_turnaround_time += processes[highest_priority_index].turnaround_time;
            
            printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[highest_priority_index].pid, processes[highest_priority_index].arrival_time,
                   processes[highest_priority_index].burst_time, processes[highest_priority_index].priority,
                   processes[highest_priority_index].waiting_time, processes[highest_priority_index].turnaround_time);
        }
        else {
            time += quantum;
            processes[highest_priority_index].remaining_time -= quantum;
        }
    }
    
    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / n);
}
