#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
void display_Reservation(Reservation *reservation);
void displayAllReservation();
void sort_byName();
void sort_byDate();
void sort_byStatus();
void sort_byID();
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
int validDate(Date date);
int generateID();
int findbyID(int id,Reservation *reservation);
const char* statusConversionToString(Status status);
int cmpDate(Date date1,Date date2);
void init_ReservationsArray();
void free_ReservationsArray();
void add10inputs();

int main() {
    init_ReservationsArray();  // Initialize the reservation array
    add10inputs();  // Add 10 dummy reservations for testing
    int choice, id;
    do {
        printf("\nReservation System Menu:");
        printf("\n1. Add Reservation");
        printf("\n2. Edit Reservation");
        printf("\n3. Delete Reservation");
        printf("\n4. Display All Reservations");
        printf("\n5. Sort by last name");
        printf("\n6. Sort by date");
        printf("\n7. Sort by status");
        printf("\n8. Search by ID");
        printf("\n9. Search by last name");
        printf("\n10. Search by date");
        printf("\n11. Calculate average age of patients");
        printf("\n12. Display patient count by age group");
        printf("\n13. Reservation statistics by status");
        printf("\n14. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        clearBuffer();  // Clear any leftover input

        switch (choice) {
            case 1:
                add_Reservation();
                break;
                
            case 2:
                printf("\nEnter Reservation ID to edit: ");
                scanf("%d", &id);
                clearBuffer();
                edit_Reservation(id);
                break;
                
            case 3:
                printf("\nEnter Reservation ID to delete: ");
                scanf("%d", &id);
                clearBuffer();
                delete_Reservation(id);
                break;
                
            case 4:
                displayAllReservation();
                break;
                
            case 5:
                sort_byName();
                break;
                
            case 6:
                sort_byDate();
                break;
                
            case 7:
                sort_byStatus();
                break;
                
            case 8:
                printf("\nEnter Reservation ID to search: ");
                scanf("%d", &id);
                clearBuffer();
                search_byID(id);
                break;
                
            case 9:
                search_byName();
                break;
                
            case 10:
                search_byDate();
                break;
                
            case 11:
                printf("\nAverage age of patients: %.2f", calc_avgAge());
                break;
                
            case 12:
                patientcount_ByAgeGroup();
                break;
                
            case 13:
                ReservationStatisticsByStatus();
                break;
                
            case 14:
                free_ReservationsArray();
                printf("\nExiting...");
                exit(0);
                
            default:
                printf("\nInvalid choice. Please enter a number between 1 and 14.");
        }
    } while (choice != 14);  // Exit the loop if choice is 14
    
    return 0;
}
//lets make the utility functions first because
// we will need to use them inside the main functions

void clearBuffer(){     //clear buffer would save us a lot of time with miss input errors
    char c;
    while((c=getchar())!='\n' && c != EOF);
}
void inputReservation(Reservation *reservation){
    reservation->firstName = (char*)malloc(MAX_LENGTH * sizeof(char));
    reservation->lastName = (char*)malloc(MAX_LENGTH * sizeof(char));
    reservation->phone = (char*)malloc(MAX_LENGTH * sizeof(char));
    reservation->age = (char*)malloc(MAX_LENGTH * sizeof(char));
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
    reservation->status = (Status)statusindx;
        
    printf("\n\t\tReservation Date(format DD/MM/YYYY) : ");
    while(scanf("%d/%d/%d",&reservation->date.day,&reservation->date.month,&reservation->date.year)!=3 || !validDate(reservation->date)){
        printf("\n\t\tEnter a valid Date with the given format(DD/MM/YYYY): ");
        clearBuffer();
    }
    clearBuffer();
}
int validateAge(char *age){
    return (atoi(age)>=0 && atoi(age)<=120);  //its either gonna return 1 or 0 (true or false)
}
int validatePhone(char *phone){
    int length = strlen(phone);
    if (length==0 ){
        return 0;
    }
    for(int i=0;i<length;i++){
        if (phone[i] < '0' || phone[i]>'9') {
            return 0;
        }
    }
    return 1;
}
int validDate(Date date){
    if(date.year<1900 || date.year>2024) return 0;
    if(date.month<1 || date.month>12) return 0;
    if(date.day<1 || date.day>31) return 0;
    return 1;
}
int generateID(){
    static int idcounter = 1500;
    return idcounter++; //incrementation behind the variable so it assigns the value then returns it
}
int findbyID(int id,Reservation *reservation){
    for(int i=0;i<tracker;i++){
        if((id == reservation[i].id)) return i;
    }
    return -1;
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
int cmpDate(Date date1,Date date2){
    if(date1.year != date2.year){
        return date1.year - date2.year;
    }else if (date1.month != date2.month){
        return date1.month - date2.month;
    }else if (date1.day != date2.day){
        return date1.day - date2.day;
    }
    return 0;
}
void init_ReservationsArray(){
    reservations = (Reservation*)malloc(sizeof(Reservation)*MAX_RESERVATIONS);
    if(reservations == NULL){
        printf("Memory allocation failed");
        exit(1);
    }

}
void free_ReservationsArray(){
    for (int i = 0; i < tracker; i++) {
        free(reservations[i].firstName);
        free(reservations[i].lastName);
        free(reservations[i].phone);
        free(reservations[i].age);
    }
    free(reservations);
}

void add10inputs() {
    reservations[0] = (Reservation){strdup("Alice"), strdup("Smith"), strdup("1234567890"), strdup("25"), generateID(), VALID, {15, 4, 2023}};
    reservations[1] = (Reservation){strdup("Bob"), strdup("Johnson"), strdup("2345678901"), strdup("30"), generateID(), REPORTED, {20, 5, 2023}};
    reservations[2] = (Reservation){strdup("Charlie"), strdup("Brown"), strdup("3456789012"), strdup("22"), generateID(), CANCELED, {10, 6, 2023}};
    reservations[3] = (Reservation){strdup("David"), strdup("Wilson"), strdup("4567890123"), strdup("28"), generateID(), TREATED, {12, 7, 2023}};
    reservations[4] = (Reservation){strdup("Eva"), strdup("Martinez"), strdup("5678901234"), strdup("35"), generateID(), VALID, {25, 8, 2023}};
    reservations[5] = (Reservation){strdup("Frank"), strdup("Garcia"), strdup("6789012345"), strdup("40"), generateID(), REPORTED, {30, 9, 2023}};
    reservations[6] = (Reservation){strdup("Grace"), strdup("Lee"), strdup("7890123456"), strdup("20"), generateID(), CANCELED, {1, 10, 2023}};
    reservations[7] = (Reservation){strdup("Henry"), strdup("Walker"), strdup("8901234567"), strdup("32"), generateID(), TREATED, {14, 11, 2023}};
    reservations[8] = (Reservation){strdup("Isabella"), strdup("Hall"), strdup("9012345678"), strdup("27"), generateID(), VALID, {22, 12, 2023}};
    reservations[9] = (Reservation){strdup("Jack"), strdup("Young"), strdup("0123456789"), strdup("31"), generateID(), REPORTED, {5, 1, 2024}};
    tracker=10;
}



//main functions now
void add_Reservation(){
    int nb;
    if(tracker>=MAX_RESERVATIONS){
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
        printf("\n\tReservation number %d",i+1);
        inputReservation(&reservations[i]);
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
            display_Reservation(&reservations[i]);
            printf("\n\tReservation number %d",i+1);
            inputReservation(&reservations[i]);
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
            display_Reservation(&reservations[i]);
            printf("\nAre you sure you want to delete it?? (y or n): ");
            while(scanf("%c",&answer)!=1 || (tolower(answer) != 'y' && tolower(answer) != 'n')){
                printf("\nType Y for yes, N for no : ");
                clearBuffer();
            }
            clearBuffer();
            if(tolower(answer)=='n'){
                printf("\nDeletion was canceled.");
                return;
            }
            for(int j=i;j<tracker-1;j++){
                reservations[j]=reservations[j+1];
            }
            tracker--;
            printf("\nReservation with ID %d deleted successfully!\n", id);
            return;
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
void sort_byName(){             //bubble sorting
    if(tracker==0){
        printf("\nThe reservation list is empty .");
        return;
    }else if (tracker==1){
        printf("\nThe reservation list is sorted .");
        return;
    }
    Reservation temp;
    int flag;
    for(int i=0;i<tracker-1;i++){
        flag=0;
        for(int j=i+1;j<tracker;j++){
            if(strcasecmp(reservations[i].lastName,reservations[j].lastName)>0){
                temp = reservations[i];
                reservations[i]=reservations[j];
                reservations[j]=temp;
                flag=1;
            }
        }
        if(flag==0){
            printf("\nThe reservation list is sorted .");
            return;
        }
    }
}
void sort_byDate(){             //selective sorting
    if(tracker==0){
        printf("\nThe reservation list is empty .");
        return;
    }else if (tracker==1){
        printf("\nThe reservation list is sorted .");
        return;
    }
    Reservation temp;
    int minindx;
    for(int i=0;i<tracker-1;i++){
        minindx=i;
        for(int j=i+1;j<tracker;j++){
            if(cmpDate(reservations[minindx].date,reservations[j].date)>0){
                minindx=j;
            }
        }
        if(minindx != i){
            temp = reservations[i];
            reservations[i] = reservations[minindx];
            reservations[minindx]=temp;
        }
    }
    printf("\nReservations sorted by date successfully.");

}
void sort_byStatus(){            ////bubble sorting
    if(tracker==0){
        printf("\nThe reservation list is empty .");
        return;
    }else if (tracker==1){
        printf("\nThe reservation list is sorted .");
        return;
    }
    Reservation temp;
    int flag;
    do{
        flag=0;
        for (int i = 0; i < tracker - 1; i++) {
                if (reservations[i].status > reservations[i + 1].status) {
                    temp = reservations[i];
                    reservations[i] = reservations[i + 1];
                    reservations[i + 1] = temp;
                    flag = 1; 
                }
        }
    } while (flag);
    printf("\nReservations sorted by status successfully.");
}
void sort_byID(){
    if (tracker <= 1) {
        return;
    }
    Reservation temp;
    for (int i = 0; i < tracker - 1; i++) {
        for (int j = 0; j < tracker - i - 1; j++) {
            if (reservations[j].id > reservations[j + 1].id) {
                temp = reservations[j];
                reservations[j] = reservations[j + 1];
                reservations[j + 1] = temp;
            }
        }
    }
}
void search_byID(int id){            //interpolation search because IDS ARE UNIFORMLY DISTRIBUTED UNLESS A RES WAS DELETED
    if(tracker==0){
        printf("\nThe reservation list is empty .");
        return;
    }
    sort_byID();
    int high=tracker-1,low=0,pos;
    if(id<reservations[low].id || id>reservations[high].id ){
        printf("\nNo reservation was found.");
        return;
    }
    while(reservations[high].id>=id && reservations[low].id<=id && high>=low){
        if (reservations[high].id == reservations[low].id) {
            if (reservations[low].id == id) {
                printf("\nReservation found:");
                display_Reservation(&reservations[low]);
                return;
            } else {
                printf("\nNo reservation was found.");
                return;
            }
        }  
        pos = low + ((double)(high-low)/(reservations[high].id-reservations[low].id))*(id-reservations[low].id);
        if(reservations[pos].id==id){
            printf("\nReservation found:");
            display_Reservation(&reservations[pos]);
            return;
        }
        if (pos < low || pos > high) {
            printf("\nNo reservation was found.");
            return;
        }
        if(reservations[pos].id==id){
            printf("\nReservation found:");
            display_Reservation(&reservations[pos]);
            return;
        }
        if(reservations[pos].id<id){
            low=pos+1;
        }
        else{
            high=pos-1;
        }
    }
    printf("\nNo reservation was found.");
}
void search_byName(){                   //selective search
    char name[MAX_LENGTH];
    printf("Enter the last name to search for: ");
    scanf(" %[^\n]", name);  
    int flag=0;
    for(int i=0;i<tracker;i++){
        if(strcmp(reservations[i].lastName,name)==0){
            printf("\nReservation found:");
            display_Reservation(&reservations[i]);
            flag = 1;
            break;
        }
    }
    if(!flag){
        printf("\nNo reservation was found.");
    }
}
void search_byDate(){           //selective search 
    Date targetDate;
    int flag = 0;
    printf("Enter the date to search for (DD/MM/YYYY): ");
    while(scanf("%d/%d/%d", &targetDate.day, &targetDate.month, &targetDate.year) != 3 || !validDate(targetDate)) {
        printf("\nEnter a valid Date with the given format (DD/MM/YYYY): ");
        clearBuffer();
    }
    for(int i = 0; i < tracker; i++) {
        if(cmpDate(targetDate, reservations[i].date) == 0) {
            printf("\nReservation found:");
            display_Reservation(&reservations[i]);
            flag = 1;
        }
    }
    if (!flag) {
        printf("\nNo reservation was found on the entered date.");
    }
}
float calc_avgAge(){
    int sumAge=0;
    float avgAge;
    for(int i=0;i<tracker;i++){
        sumAge+=atoi(reservations[i].age);
    }
    avgAge=(float)sumAge/tracker;
    return avgAge;
}
void patientcount_ByAgeGroup(){
    int below18=0,betw19_35=0,above36=0;
    for(int i=0;i<tracker;i++){
        if(atoi(reservations[i].age)<=18){
            below18++;
        }else if(atoi(reservations[i].age)<=35 && atoi(reservations[i].age)>=19){
            betw19_35++;
        }
        else if( atoi(reservations[i].age)>=36){
            above36++;
        }
    }
    printf("\nPatient count by age group:");
    printf("\n\tPatients 18 and below : %d patients",below18);
    printf("\n\tPatients from 19 to 35 : %d patients",betw19_35);
    printf("\n\tPatients 36 and above : %d patients",above36);
}
void ReservationStatisticsByStatus(){
    int validnb=0,reportednb=0,cancelednb=0,treatednb=0;
    for(int i=0;i<tracker;i++){
        if(reservations[i].status == 1){
            validnb++;
        }else if(reservations[i].status == 2){
            reportednb++;
        }else if(reservations[i].status == 3){
            cancelednb++;
        }else {
            treatednb++;
        }
    }
    printf("\nReservations statistics by status: :");
    printf("\nValid reservation(s) : %d",validnb);
    printf("\nReported reservation(s) : %d",reportednb);
    printf("\nCanceled reservation(s) : %d",cancelednb);
    printf("\nTreated reservation(s) : %d",treatednb);
}