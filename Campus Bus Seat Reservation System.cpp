#include <stdio.h>
#include <conio.h>
#include <string.h>

#define MAX_SEATS 20

struct Seat {
    int number;
    char status[20];
    char destination[100];
};

struct Seat seats[MAX_SEATS];

void makeSeatReservation(char email[]);
void cancelSeatReservation(char email[]);
void displayRegular(char email[]);
void displayAdmin();
void saveSeatStatus();
void loadSeatStatus();
int isCLSUEmail(char email[], char password[]);
void clearSeatReservationHistory();

int main() {
    loadSeatStatus(); // Load seat status from file

    // clrscr();
    char email[1000], pw[1000];

    printf("\n=============== WELCOME =============");
    again:
    printf("\nEnter your email: ");
    fgets(email, sizeof(email), stdin);
    // Remove newline character
    if (email[strlen(email) - 1] == '\n')
        email[strlen(email) - 1] = '\0';

    printf("Enter your password: ");
    fgets(pw, sizeof(pw), stdin);
    // Remove newline character
    if (pw[strlen(pw) - 1] == '\n')
        pw[strlen(pw) - 1] = '\0';

    if (isCLSUEmail(email, pw)) {
        if (strcmp(email, "admin@clsu2.edu.ph") == 0) {
            // clrscr();
            printf("\nWelcome, Admin!");
            displayAdmin();
        } else {
            // clrscr();
            printf("\nWelcome, %s!", email);
            displayRegular(email);
        }
    } else {
        printf("\n\nInvalid Email or Password! Email must be a CLSU account.");
        goto again;
    }

    saveSeatStatus(); // Save the seat status to the file before logging out

    getch();
    return 0;
}

void displayRegular(char email[]) {
    int choice;

    printf("\n\n============= REGULAR USER MENU =============\n");
    printf("\n1. Make Seat Reservation");
    printf("\n2. Cancel Seat Reservation");
    printf("\n3. Log Out\n");

    again1:
    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        makeSeatReservation(email);
        break;
    case 2:
        cancelSeatReservation(email);
        break;
    case 3:
        // clrscr();
        printf("\nSuccessfully logged out! Changes made are saved.");
        break;
    default:
        printf("\nInvalid choice. Please try again.");
        goto again1;
        break;
    }
}

void makeSeatReservation(char email[]) {
    int seatNumber;
    char destination[100];

    printf("\nEnter your destination: ");
    getchar(); // Clear input buffer
    fgets(destination, sizeof(destination), stdin);
    // Remove newline character
    if (destination[strlen(destination) - 1] == '\n')
        destination[strlen(destination) - 1] = '\0';

    seat:
    printf("\nEnter the seat number you want to reserve (1-%d): ", MAX_SEATS);
    scanf("%d", &seatNumber);

    if (seatNumber < 1 || seatNumber > MAX_SEATS) {
        printf("\nInvalid seat number. Please try again.");
        goto seat;
    }

    if (strcmp(seats[seatNumber - 1].status, "RESERVED") == 0) {
        printf("\nSorry, seat %d is already reserved. Please choose another seat.", seatNumber);
        goto seat;
    } else {
        strcpy(seats[seatNumber - 1].status, "RESERVED");
        strcpy(seats[seatNumber - 1].destination, destination);
        printf("\nSeat %d reserved successfully!", seatNumber);
    }

    saveSeatStatus();
}

void cancelSeatReservation(char email[]) {
    int seatNumber;

    againSeat:
    printf("\nEnter the seat number you want to cancel reservation: ");
    scanf("%d", &seatNumber);

    if (seatNumber < 1 || seatNumber > MAX_SEATS) {
        printf("\nInvalid seat number!");
        goto againSeat;
        return;
    }

    if (strcmp(seats[seatNumber - 1].status, "AVAILABLE") == 0) {
        printf("\nSeat is not reserved! No need for cancelling.\n");
    } else {
        strcpy(seats[seatNumber - 1].status, "AVAILABLE");
        strcpy(seats[seatNumber - 1].destination, "");
        printf("\nSeat reservation cancelled successfully.");
    }

    saveSeatStatus();
}

void displayAdmin() {
    int choice;

    printf("\n\n============= ADMIN MENU =============\n");
    printf("\n1. View Seat Reservations");
    printf("\n2. Clear Seat Reservation History");
    printf("\n3. Log Out\n");

    again2:
    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        // clrscr();
        printf("\n============= SEAT RESERVATIONS =============\n\n");
        printf("SEAT#\tSTATUS\t\tDESTINATION\n");
        printf("=============================================\n");
        for (int i = 0; i < MAX_SEATS; i++) {
            printf("%d\t%s\tgoing to %s\n", seats[i].number, seats[i].status, seats[i].destination);
        }
        break;
    case 2:
        clearSeatReservationHistory();
        break;
    case 3:
        // clrscr();
        printf("\nSuccessfully logged out! Changes made are saved.");
        break;
    default:
        printf("\nInvalid choice. Please try again.");
        goto again2;
    }
}

void saveSeatStatus() {
    FILE *fp;
    fp = fopen("Seat_Status.txt", "w");
    if (fp == NULL) {
        printf("\nError in saving seat status to file.");
        return;
    }

    for (int i = 0; i < MAX_SEATS; i++) {
        fprintf(fp, "%d %s %s\n", seats[i].number, seats[i].status, seats[i].destination);
    }

    fclose(fp);
}

void loadSeatStatus() {
    FILE *fp;
    fp = fopen("Seat_Status.txt", "r");
    if (fp == NULL) {
        printf("\nError in loading seat status from file.");
        return;
    }

    char line[200];
    for (int i = 0; i < MAX_SEATS; i++) {
        if (fgets(line, sizeof(line), fp) != NULL) {
            sscanf(line, "%d %s %99[^\n]", &seats[i].number, seats[i].status, seats[i].destination);
        }
    }

    fclose(fp);
}

void clearSeatReservationHistory() {
    again:
    printf("\nAre you sure you want to clear all seat reservation history? (y/n): ");
    char confirmation;
    scanf(" %c", &confirmation);

    if (confirmation == 'Y' || confirmation == 'y') {
        for (int i = 0; i < MAX_SEATS; i++) {
            seats[i].number = i + 1;
            strcpy(seats[i].status, "AVAILABLE");
            strcpy(seats[i].destination, "");
        }
        printf("\nSeat reservation history cleared successfully!");
        saveSeatStatus();
    } else if (confirmation == 'N' || confirmation == 'n') {
        printf("\nSeat reservation history not cleared.");
    } else {
        printf("\nInvalid choice!\n");
        goto again;
    }
}

int isCLSUEmail(char email[], char password[]) {
    if (strstr(email, "@clsu2.edu.ph") != NULL && strcmp(password, "12345678") == 0)
        return 1;
    else
        return 0;
}