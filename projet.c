#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RESERVATIONS 100
#define MAX_LENGTH 50

int tracker=0;

typedef struct{             //define a date sturct to use in the main struct 
    int day;
    int month;
    int year;
}Date;
typedef enum{               //define an enum for status because it contains 4 constants
    VALID = 1,
    REPORTED = 2,
    CANCELED = 3,
    TREATED = 4
}Status;

typedef struct{                 //this is the main reservation structure
    char *firstName;
    char *lastName;         
    char *phone;                 //I used pointers just so i can use the memory allocation which could help if theres gonna be a lot of Reservations
    char *age;
    int id;
    Status status;
    Date date;
}Reservation;

Reservation *reservations;

//main functions
void add_Reservation();
void edit_Reservation(int id);
void delete_Reservation(int id);
void display_Reservation();
void displayAllReservation();
void sort_byName();
void sort_byDate();
void sort_byStatus();
void search_byID();
void search_byName();
void search_byDate();
float calc_avgAge();
void patientcount_ByAgeGroup();
void ReservationStatisticsByStatus();

//utility functions
void clearBuffer();
void inputReservation(Reservation *reservation);
int validateAge(char *age);
int validatePhone(char *phone);
int validateDate(Date date);
int generateID();
int findbyID(int id,Reservation *reservation);
const char* statusConversionToString(Status status);
void init_ReservationsArray();
void free_ReservationsArray();
void add10inputs();

int main(){





}
//lets make the utility functions first because
// we will need to use them inside the main functions

void clearBuffer(){     //clear buffer would save us a lot of time with miss input errors
    char c;
    while((c=getchar()!='\n') && c != EOF);
}
void inputReservation(Reservation *reservation){
    printf("\n\t\tReservation first name : ");
    scanf(" %[^\n]",reservation->firstName);
    printf("\n\t\tReservation last name : ");
    scanf(" %[^\n]",reservation->lastName);

        printf("\n\t\tReservation phone number : ");
        scanf(" %[^\n]",reservation->phone);
        while(!validatePhone(reservation->phone)){
            printf("\n\t\tEnter a valid phone number : ");
            scanf(" %[^\n]",reservation->phone);
        }

        printf("\n\t\tReservation age : ");
        scanf(" %[^\n]",reservation->age);
        while(!validateAge(reservation->age)){
            printf("\n\t\tEnter a valid age : ");
            scanf(" %[^\n]",reservation->age);
        }

        int statusindx;
        printf("\n\t\tReservation status :");
        printf("\n\t\t\tFor valid type 1");
        printf("\n\t\t\tFor reported type 2");
        printf("\n\t\t\tFor canceled type 3");
        printf("\n\t\t\tFor treated type 4");
        printf("\n\t\t\tType(1 to 4): ");
        while(scanf("%d",&statusindx)!=1 || statusindx<1 || statusindx>4){
            printf("\n\t\t\tInvalid input, Type (1 to 4): ");
            clearBuffer();
        }
        reservation->status = (status)statusindx;
        
        printf("\n\t\tReservation Date(format DD/MM/YYYY) : ");
        while(scanf("%d/%d/%d",&reservation->date.day,&reservation->date.month,&reservation->date.year)!=3 || !validateDate(reservation->date)){
            printf("\n\t\tEnter a valid Date with the given format(DD/MM/YYYY): ");
            scanf("%d/%d/%d",&reservation->date.day,&reservation->date.month,&reservation->date.year);
            clearBuffer();
        }
        clearBuffer();

}
int validateAge(char *age){
    return (age>=0 && age<=120);  //its either gonna return 1 or 0 (true or false)


}
int validatePhone(char *phone){
    int length = strlen(phone);
    if (length==0 || phone != NULL){
        return 0;
    }
    for(int i=0;i<length;i++){
        if (phone[i] < '0' || phone[i]>'9') {
            return 0;
        }
    }
    return 1;
}
int validateDate(Date date){
    if(date.year<1900 || date.year>2024) return 0;
    if(date.month<1 || date.month>12) return 0;
    if(date.day<1 || date.day>31) return 0;
    return 1;
}
int generateID(){
    int idcounter = 1500;
    return ++idcounter; //incrementation behind the variable so it assigns the value then returns it
}
int findbyID(char *id,Reservation *reservation){
    for(int i=0;i<tracker;i++){
        if((id == reservation[i].id)) return i;
    }
}
const char* statusConversionToString(Status status){
    switch(status){
        case VALID: return "Valid";
        case REPORTED: return "Reported";
        case CANCELED: return "Canceled";
        case TREATED: return "Treated";
        default : return "Invalid";
    }
}
void init_ReservationsArray(){
    reservations = (Reservation*)malloc(sizeof(Reservation)*MAX_RESERVATIONS);
    if(reservations == NULL){
        printf("Memory allocation failed");
        exit(1);
    }

}
void free_ReservationsArray(){
    free(reservations);
}

void add10inputs(Reservation *reservations){
    reservations[0] = add_Reservation(1, "Alice", "Smith", "123-456-7890", "25", VALIDATED, 15, 4, 2023);
    reservations[1] = add_Reservation(2, "Bob", "Johnson", "234-567-8901", "30", POSTPONED, 20, 5, 2023);
    reservations[2] = add_Reservation(3, "Charlie", "Brown", "345-678-9012", "22", CANCELED, 10, 6, 2023);
    reservations[3] = add_Reservation(4, "David", "Wilson", "456-789-0123", "28", PROCESSED, 12, 7, 2023);
    reservations[4] = add_Reservation(5, "Eva", "Martinez", "567-890-1234", "35", VALIDATED, 25, 8, 2023);
    reservations[5] = add_Reservation(6, "Frank", "Garcia", "678-901-2345", "40", POSTPONED, 30, 9, 2023);
    reservations[6] = add_Reservation(7, "Grace", "Lee", "789-012-3456", "20", CANCELED, 1, 10, 2023);
    reservations[7] = add_Reservation(8, "Henry", "Walker", "890-123-4567", "32", PROCESSED, 14, 11, 2023);
    reservations[8] = add_Reservation(9, "Isabella", "Hall", "901-234-5678", "27", VALIDATED, 22, 12, 2023);
    reservations[9] = add_Reservation(10, "Jack", "Young", "012-345-6789", "31", POSTPONED, 5, 1, 2024);
}

//main functions now
void add_Reservation(){
    int nb;
    if(tracker>=MAX_LENGTH){
        printf("\nThe reservation list is full");
        return;
    }
    printf("\nHow many reservations to add exactly : ");
    while(scanf("%d",&nb)!=1 || nb+tracker>MAX_RESERVATIONS){
        printf("\n\tType a valid input from 1 to %d",MAX_RESERVATIONS-tracker);
        clearBuffer();
    }
    clearBuffer();

    for(int i=tracker;i<tracker+nb;i++){
        reservations[i].firstName = (char*)malloc(sizeof(char)*MAX_LENGTH);
        reservations[i].lastName = (char*)malloc(sizeof(char)*MAX_LENGTH);
        reservations[i].phone = (char*)malloc(sizeof(char)*MAX_LENGTH);
        reservations[i].age = (char*)malloc(sizeof(char)*MAX_LENGTH);
        printf("\n\tReservation number %d",i+1);
        inputReservation(&reservation[i]);
        reservations[i].id = generateID();
    }
    printf("\n\t\t\t%d reservation(s) added successfully!",nb);
    tracker+=nb;
}
void edit_Reservation(int id){
    if(tracker==0) {
        printf("\nreservation list is empty ");
        return;
    }
    for(int i=0;i<tracker;i++){
        if(reservations[i].id==id){
            printf("\nReservation found:");
            display_Reservation();
            printf("\n\tReservation number %d",i+1);
            inputReservation(&Reservation[i]);
            printf("\nReservation updated successfully!");
            return;
        }
    }
    printf("\nReservation with the given ID wasnt found.");
}
void delete_Reservation(int id){
    if(tracker==0){
        printf("\nreservation list is empty ");
        return;
    }
    char answer;
    for(int i=0;i<tracker;i++){
        if(reservations[i].id==id){
            printf("\nReservation found:");
            display_Reservation();
            printf("\nAre you sure you want to delete it?? (y or n): ");
            while(scanf("%c",&answer)!=1 || (tolower(c)!='y' && tolower(c)!='n')){
                printf("\nType Y for yes, N for no : ");
                clearBuffer();
            }
            clearBuffer();
            if(tolower(c)=='n'){
                printf("\nDeletion was canceled.");
                return;
            }
            for(int j=i;j<tracker-1;j++){
                reservation[j]=reservation[j+1];
            }
            tracker--;
            printf("\nReservation with ID %d deleted successfully!\n", id);
            break;
        }
    }
    printf("\nReservation with the %d ID wasnt found!",id);
}

void display_Reservation(Reservation *reservation){
    printf("\n\tReservation ID : %d",reservation->id);
    printf("\n\tReservation last name : %s ",reservation->lastName);
    printf("\n\tReservation first name : %s",reservation->firstName);
    printf("\n\tReservation phone : %s",reservation->phone);
    printf("\n\tReservation age : %s",reservation->age);
    printf("\n\tReservation status : %s",statusConversionToString(reservation->status));
    printf("\n\tReservation date(DD/MM/YYYY) : %02d/%02d/%04d",reservation->date.day,reservation->date.month,reservation->date.year);
}
void displayAllReservation(){
    if(tracker==0){
        printf("\nReservations list is empty.");
        return;
    }
    for(int i=0;i<tracker;i++){
        printf("\nReservation number %d",i+1);
        display_Reservation(&reservations[i]);
    }
}
void sort_byName();
void sort_byDate();
void sort_byStatus();
void search_byID();
void search_byName();
void search_byDate();
float calc_avgAge();
void patientcount_ByAgeGroup();
void ReservationStatisticsByStatus();