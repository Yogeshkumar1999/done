#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#define TRUE 1
#define FALSE 0
#define COM 3
#define MAX_XAM 3

pthread_t students[3];   								 
pthread_t Agent, Agent_A, Agent_B, Agent_C;        		
pthread_t Exam_A, Exam_B, Exam_C;			  		
pthread_mutex_t taker, exam;							
int completed[3];         								
int completed_so_far = 0,  exams[3] = {0, 0, 0};	
sem_t pen, paper, Question_paper, SemAgent, SemPaper, Sempen, SemQuestion_paper;
void mangal (int id) {
    pthread_mutex_lock (&exam);                       
    if(completed_so_far>=MAX_XAM){				
        printf("MAX EXAMS HAVE BEEN COMPLETED ");
        pthread_mutex_unlock (&exam);                  
        return;
    }
    if (id == 1) {										
         exams[0]++;
    } else if (id == 2) {
         exams[1]++;
    } else {
         exams[2]++;
    }
    completed_so_far++;
    pthread_mutex_unlock (&exam);                     
    switch (id){

        case 1:
            printf("Student paper  STARTED writing...\n");
          
            break;
        case 2:
            printf("Student pen  STARTED writing......\n");
           
            break;
        case 3:
            printf("Student Question_paper  STARTED writing... \n");
         
            break;
    }
   sleep(1);
    pthread_mutex_lock (&exam);                   		
    switch (id){

        case 1:
            printf("Student paper   FINESHED exam..\n");
          
            break;
        case 2:
             printf("Student pen   FINESHED exam..\n");
           
            break;
        case 3:
            printf("Student Question_paper   FINESHED exam... \n");
           
            break;
    }
    printf("Exams completed So far...\n");				
    printf("| Paper | pen | Question_paper |\n");
    printf("| %d    | %d  |      %d        |\n", exams[0], exams[1], exams[2]);
    pthread_mutex_unlock (&exam);                        
}
void *exam_a (void *in) {								
    while (1) {											
        sem_wait (&paper);								
        pthread_mutex_lock(&taker);					
        if (completed[0] == COM) {
            printf("\n\n");
            printf(" The table is full of paper   \n");
                if (completed[1]) {
                completed[0]--;
                completed[1]--;
                sem_post (&SemQuestion_paper);
            } else if (completed[2]) {
                completed[2]--;
                completed[0]--;
                sem_post (&Sempen);
            }
        } else {	
            printf("\n\n");
            printf("Teacher: Paper  is on the table \n");
            if (completed[1]) {
                completed[1]--;
                sem_post (&SemQuestion_paper);
            } else if (completed[2]) {
                completed[2]--;
                sem_post (&Sempen);
            } else if (completed[0] < COM) {
                completed[0]++;
            }
        } 
        pthread_mutex_unlock(&taker);
    }
}

void *exam_b (void *in) {								
    while (1) {
        sem_wait (&pen);
        pthread_mutex_lock(&taker);
        if (completed[1] == COM) {
            printf("\n\n");
            printf(" The table is full of pens  \n");
            if (completed[0]) {
                completed[1]--;
                completed[0]--;
                sem_post (&SemQuestion_paper);
            } else if (completed[2]) {
                completed[1]--;
                completed[2]--;
                sem_post (&SemPaper);
            }

        } else {
            printf("\n\n");
            printf("Teacher: pen is on the Table\n");
            if (completed[0]) {
                completed[0]--;
                sem_post (&SemQuestion_paper);
            } else if (completed[2]) {
                completed[2]--;
                sem_post (&SemPaper);
            } else if (completed[1] < COM) {
                completed[1]++;
            }
        }
        pthread_mutex_unlock(&taker);
    }
}

void *exam_c (void *in) {								
    while (1) {
        sem_wait (&Question_paper);
        pthread_mutex_lock(&taker);
        if (completed[2] == COM) {
            
            printf(" The table is full of Question_papers  \n");

            if (completed[0]) {
                completed[2]--;
                completed[0]--;
                sem_post (&Sempen);
            } else if (completed[1]) {
                completed[2]--;
                completed[1]--;
                sem_post (&SemPaper);
            }
        } else {
            printf("\n\n");
            printf("Teacher: Question_paper  is on the Table\n");
            if (completed[0]) {
                completed[0]--;
                sem_post (&Sempen);
            } else if (completed[1]) {
                completed[1]--;
                sem_post (&SemPaper);
            } else if (completed[2] < 3) {
                completed[2]++;
            }
        }
        pthread_mutex_unlock(&taker);
    }
}


void *agent (void *in) {								
    while (1) {
        sleep(1);
        sem_post (&SemAgent);
    }
}
void *agent_C (void *in) {								
    while (1) {
        sem_wait (&SemAgent);
        printf("\n\n");
        printf("Teacher : Suppling (PAPER AND PEN)  \n");

        sem_post (&paper);
        sem_post (&pen);
    }
}
void *agent_A (void *in) {								
    while (1) {
        sem_wait (&SemAgent);		
        printf("\n\n");
        printf("Teacher : Suppling (QUESTION_PAPER AND PEN)\n");
        sem_post (&pen);
        sem_post (&Question_paper);
    }
}

void *agent_B (void *in) {								 
    while (1) {
        sem_wait (&SemAgent);
        printf("\n\n");
        printf("Teacher : Suppling (PAPER AND QUESTION_PAPER) \n");
        sem_post (&paper);
        sem_post (&Question_paper);
    }
}


void *student_pen (void *in) {						
    while (1) {
        sem_wait (&Sempen);
        pthread_mutex_lock (&exam);  
        if (completed_so_far >= MAX_XAM) {		
            pthread_mutex_unlock (&exam); 
            pthread_exit(NULL);
        } else {
            printf("\n\n");								
            printf("Student pen  READY to write...\n");
           
            pthread_mutex_unlock (&exam);  
             mangal(2);
        }
    }
}

void *student_paper (void *in) {							
    while (1) {
        sem_wait (&SemPaper);
        pthread_mutex_lock (&exam);  
        if (completed_so_far >= MAX_XAM) {		
            pthread_mutex_unlock (&exam); 
            pthread_exit(NULL);
        } else {
            printf("\n\n");								
            printf("Student paper READY to write...\n");
        
            pthread_mutex_unlock (&exam);  
             mangal(1);
        }
    }
}


void *student_Question_paper (void *in) {						
    while (1) {
        sem_wait (&SemQuestion_paper);
        pthread_mutex_lock (&exam);
        if (completed_so_far >= MAX_XAM) {		
            pthread_mutex_unlock (&exam); 
            pthread_exit(NULL);
        } else {  
            printf("Student Questio_paper   READY to complete... \n");
         
            pthread_mutex_unlock (&exam);  
             mangal(3);
        }
    }
}
int main () {
    printf("Student0  with paper  \n");
    printf("Student1  with pen \n");
    printf("Student2 with Question_paper  \n");
  int i;
    for( i = 0; i < 2; i++) {
        completed[i] = FALSE;
    }
    sem_init (&pen, 1, 0);
    sem_init (&paper, 1, 0);
    sem_init (&Question_paper, 1, 0);
    sem_init (&SemAgent, 1, 0);
    sem_init (&SemPaper, 1, 0);
    sem_init (&Sempen, 1, 0);
    sem_init (&SemQuestion_paper, 1, 0);
    pthread_create(&Agent, NULL, &agent, NULL);
    pthread_create(&students[0], NULL, &student_paper, NULL);
    pthread_create(&students[1], NULL, &student_pen, NULL);
    pthread_create(&students[2], NULL, &student_Question_paper, NULL);
    pthread_create(&Agent_A, NULL, &agent_A, NULL);
    pthread_create(&Agent_B, NULL, &agent_B, NULL);
    pthread_create(&Agent_C, NULL, &agent_C, NULL);
    pthread_create(&Exam_A, NULL, &exam_a, NULL);
    pthread_create(&Exam_B, NULL, &exam_b, NULL);
    pthread_create(&Exam_C, NULL, &exam_c, NULL);
    pthread_join(students[0], NULL);
    pthread_join(students[1], NULL);
    pthread_join(students[2], NULL);
    printf("\n\n");
    printf("student containing paper with him have completed %d exams\n", exams[0]);
    printf("student containing paper with him have completed  %d exams\n",  exams[1]);
    printf("student containing paper with him have completed %d exams\n",  exams[2]);
     return 0;
} 
