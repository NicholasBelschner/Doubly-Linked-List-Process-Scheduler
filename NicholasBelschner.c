#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node{
  struct node* next;
  struct node* prev;
  int* data1;
  int* data2;
  int* proc_num;
}node;

/* Creating the doubly linked list */
struct node* doubly_linked_list(struct node* node_data, struct node* head){
  if(head == NULL){
    head = node_data;
  }else{
    struct node* current = head;
    while(current -> next != NULL){
      current = current->next;
    }
    current->next = node_data;
    node_data->prev = current;
  }
  return head;
}

/* Function for creating the nodes for the doubly linked list */
struct node *create_node(int *data1, int *data2, int *proc_num, int num_elements){
  struct node *new_node = (struct node*)malloc(sizeof(struct node));
  new_node->data1 = (int*)malloc(num_elements * sizeof(int));
  new_node->data2 = (int*)malloc(num_elements * sizeof(int));
  new_node->proc_num = (int*)malloc(sizeof(int));
  memcpy(new_node->data1, data1, num_elements * sizeof(int));
  memcpy(new_node->data2, data2, num_elements * sizeof(int));
  memcpy(new_node->proc_num, proc_num, sizeof(int));
  new_node->prev = NULL;
  new_node->next = NULL;
  return new_node;
}

/* Used for when the bubbleSort function needs to switch the nodes */
void swap(struct node* node1, struct node* node2) {
  int* temp1 = node1->data1;
  int* temp2 = node1->data2;
  int* temp3 = node1->proc_num;

  node1->data1 = node2->data1;
  node1->data2 = node2->data2;
  node1->proc_num = node2->proc_num;

  node2->data1 = temp1;
  node2->data2 = temp2;
  node2->proc_num = temp3;
}

/* Creating bubbleSort for sorting through the nodes */ 
void bubbleSort(struct node* head, int num){
  int swapped;
  struct node* current;
  struct node* last = NULL;

  if (head == NULL)
    return;

  do {
    swapped = 0;
    current = head;

    while (current->next != last){
      if(num == 1){
        if (*(current->data2) > *(current->next->data2)) {
          swap(current, current->next);
          swapped = 1;
        }
      }else if (num == 2){
        if (*(current->data1) > *(current->next->data1)) {
          swap(current, current->next);
          swapped = 1;
        }
      }
      current = current->next;
    }
    last = current;
  }while (swapped);
}

/* Fuction for printing all the data in the doubly linked list. */
void print_list(struct node* head){
  struct node *current = head;

  while (current != NULL){
    printf("%d %d %d\n", *(current -> proc_num), *(current -> data1), *(current -> data2));
    current = current -> next;
  }
}

/* Created an add function for calculating and assigns waiting time to each node */
void add(struct node* head){
  struct node *current = head;
  int waiting_time = 0; 

  while(current != NULL){
    *(current -> data2) = waiting_time;
    waiting_time += *(current -> data1);

    current = current -> next;
  }
}

void print_order(struct node* head){ /* Function for calculations and prints.*/
  struct node* current = head;
  int totalProcesses = 0;
  int sum = 0;

  /* While loop for showing the sequense of execution in order */
  printf("The order of execution is ");
  while (current != NULL) {
    if (current->next != NULL) {
      printf("P%d -> ", *(current->proc_num));
    }else{
      printf("P%d ", *(current->proc_num));
    }   
    current = current->next;
    totalProcesses++;
  }   
  printf("\n");

  int sumWaitingTime = 0;
  int total_turnaround_time = 0;
  int current_waiting_time = 0;
  current = head;

  while (current != NULL) {
    sumWaitingTime += *(current->data2);
    total_turnaround_time += *(current->data2) + *(current->data1);
    current = current->next;
  }   

  float av_wTime = (float)sumWaitingTime / totalProcesses;
  printf("Average waiting time: %.2f\n", av_wTime);

  float averageTurnaroundTime = (float)total_turnaround_time / totalProcesses;
  printf("Average turnaround time: %.2f\n", averageTurnaroundTime);

}

int main(int argc, char *argv[]){
  struct node* head = NULL;
  FILE *fp = NULL;
  int num_pros[255];
  int burst_time[255];
  int arvl_time[255];
  int i = 1;

  /* Scanning the file from command line.*/
  fp = fopen(argv[1], "r");
  fscanf(fp, "%d", num_pros);
  printf("the number of processes: %d\n", *num_pros);
  
  int num_elements = sizeof(burst_time) / sizeof(burst_time[0]);

  /* While loop for counting the number of processes. */
  while(fscanf(fp, "%d %d", &burst_time, &arvl_time) == 2){
    struct node* word_node = create_node(burst_time, arvl_time, &i, num_elements);
    head = doubly_linked_list(word_node, head);  
    i++;
  }   
  
  fclose(fp);

  if(strcmp(argv[2], "FCFS") == 0){ /*Calling the functions for 'FCFS' algorithm*/
    
    printf("Unsorted list within file.\n");
    print_list(head);
    
    bubbleSort(head, 1);

    printf("Sorted list using 'FCFS'.\n");
    print_list(head);

    add(head);
    print_order(head);

  }else if(strcmp(argv[2], "SJF") == 0){  /*Calling the functions for 'SJF' algorithm*/
    
    printf("Unsorted list within file.\n");
    print_list(head);
    
    bubbleSort(head, 2);

    printf("Sorted list using 'FCFS'.\n");
    print_list(head);

    add(head);
    print_order(head);

  }else{
    printf("Error: Must put 'SJF' or 'FCFS' as the 3rd argument");
  }
  return 0;
}
