#include <stdio.h>
#include <stdlib.h>


#define MAX_RESERVATIONS 100
#define MAX_LENGTH 50


typedef struct{             //define a substructure 
    int day;
    int month;
    int year;
}Date;
typedef enum{               //define an enum for status because it contains 4 constants
    valid,
    reported,
    canceled,
    treated
}Status;

typedef struct{                 //this is the main reservation structure
    char *firstName;
    char *lastName;         
    char *phone;
    char *age;
    Status status;
    int id;
    Date date;
}Reservation;

void addReservation(){
    
}