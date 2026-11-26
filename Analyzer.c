
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 100
#define SUBJECTS 5

// Student data
int roll[MAX];
char name[MAX][50];

// 2D array for subject-wise marks
int marks[MAX][SUBJECTS];

int total[MAX];
float percentage[MAX];
char grade[MAX];
char status[MAX];

int count = 0;

// Subject names
char subject[SUBJECTS][20] = {
    "English", "Maths", "Physics", "Chemistry", "Computer"
};

// ---------- INPUT FUNCTIONS ----------

int readInt(char message[]) {
    char line[100];
    int value;
    char extra;

    while (1) {
        printf("%s", message);

        if (fgets(line, sizeof(line), stdin) == NULL)
            continue;

        if (sscanf(line, "%d %c", &value, &extra) == 1)
            return value;

        printf("Invalid input! Enter a number.\n");
    }
}

void readName(char message[], char output[]) {
    printf("%s", message);
    fgets(output, 50, stdin);
    output[strcspn(output, "\n")] = '\0';

    while (strlen(output) == 0) {
        printf("Name cannot be empty. Enter again: ");
        fgets(output, 50, stdin);
        output[strcspn(output, "\n")] = '\0';
    }
}

// ---------- MODULE 1: DATA ENTRY ----------

void dataEntry() {
    int r;

    if (count >= MAX) {
        printf("\nStudent limit reached!\n");
        return;
    }

    r = readInt("\nEnter Roll Number: ");

    if (r <= 0) {
        printf("Invalid roll number!\n");
        return;
    }

    // Check duplicate roll number
    for (int i = 0; i < count; i++) {
        if (roll[i] == r) {
            printf("Roll number already exists!\n");
            return;
        }
    }

    roll[count] = r;

    readName("Enter Student Name: ", name[count]);

    printf("\nEnter Marks (0-100):\n");

    for (int j = 0; j < SUBJECTS; j++) {
        char message[100];

        sprintf(message, "%s: ", subject[j]);

        do {
            marks[count][j] = readInt(message);

            if (marks[count][j] < 0 ||
                marks[count][j] > 100) {
                printf("Invalid marks! Enter between 0 and 100.\n");
            }

        } while (marks[count][j] < 0 ||
                 marks[count][j] > 100);
    }

    count++;

    printf("\nStudent record added successfully!\n");
}

// ---------- MODULE 2: RESULT CALCULATION ----------

void calculateResult(int i) {
    int sum = 0;
    int fail = 0;

    for (int j = 0; j < SUBJECTS; j++) {
        sum += marks[i][j];

        if (marks[i][j] < 33)
            fail = 1;
    }

    total[i] = sum;
    percentage[i] = sum / (float)SUBJECTS;

    if (fail) {
        grade[i] = 'F';
        status[i] = 'F';
    }
    else {
        status[i] = 'P';

        if (percentage[i] >= 90)
            grade[i] = 'A';
        else if (percentage[i] >= 75)
            grade[i] = 'B';
        else if (percentage[i] >= 60)
            grade[i] = 'C';
        else if (percentage[i] >= 45)
            grade[i] = 'D';
        else
            grade[i] = 'E';
    }
}

// Display detailed marksheet
void displayMarksheet(int i) {

    // Always calculate before displaying
    calculateResult(i);

    printf("\n========== STUDENT MARKSHEET ==========\n");

    printf("Roll Number : %d\n", roll[i]);
    printf("Name        : %s\n", name[i]);

    printf("\nSubject-wise Marks:\n");

    printf("-----------------------------------\n");
    printf("%-20s %s\n", "Subject", "Marks");
    printf("-----------------------------------\n");

    for (int j = 0; j < SUBJECTS; j++) {
        printf("%-20s %d/100\n",
               subject[j], marks[i][j]);
    }

    printf("-----------------------------------\n");
    printf("Total       : %d / 500\n", total[i]);
    printf("Percentage  : %.2f%%\n", percentage[i]);
    printf("Grade       : %c\n", grade[i]);

    if (status[i] == 'P')
        printf("Result      : PASS\n");
    else
        printf("Result      : FAIL\n");

    printf("===================================\n");
}

// Display all results
void displayResults() {
    if (count == 0) {
        printf("\nNo student records available!\n");
        return;
    }

    printf("\n========== ALL STUDENT RESULTS ==========\n");

    printf("%-8s %-18s %-8s %-12s %-8s %-8s\n",
           "Roll", "Name", "Total",
           "Percentage", "Grade", "Status");

    for (int i = 0; i < count; i++) {
        calculateResult(i);

        printf("%-8d %-18s %-8d %-11.2f %-8c %-8s\n",
               roll[i],
               name[i],
               total[i],
               percentage[i],
               grade[i],
               status[i] == 'P' ? "PASS" : "FAIL");
    }
}

// ---------- MODULE 3: SEARCH MODULE ----------

// Case-insensitive name comparison
int compareName(char a[], char b[]) {
    int i = 0;

    while (a[i] && b[i]) {
        if (tolower((unsigned char)a[i]) !=
            tolower((unsigned char)b[i]))
            return 0;

        i++;
    }

    return a[i] == '\0' && b[i] == '\0';
}

// Search by roll number
void searchRoll() {
    int r = readInt("\nEnter Roll Number: ");

    for (int i = 0; i < count; i++) {
        if (roll[i] == r) {
            displayMarksheet(i);
            return;
        }
    }

    printf("\nStudent not found!\n");
}

// Search by name
void searchName() {
    char n[50];

    readName("\nEnter Student Name: ", n);

    int found = 0;

    for (int i = 0; i < count; i++) {
        if (compareName(name[i], n)) {
            displayMarksheet(i);
            found = 1;
        }
    }

    if (!found)
        printf("\nStudent not found!\n");
}

// Search menu
void searchModule() {
    int choice;

    printf("\n1. Search by Roll Number");
    printf("\n2. Search by Name");

    choice = readInt("\nEnter Choice: ");

    switch (choice) {
        case 1:
            searchRoll();
            break;

        case 2:
            searchName();
            break;

        default:
            printf("\nInvalid Choice!\n");
    }
}

// ---------- MODULE 4: ANALYSIS AND RANKING ----------

// Sort students by total marks
void sortResults() {
    int temp;
    float tempP;
    char tempC;
    char tempName[50];

    if (count == 0) {
        printf("\nNo records to sort!\n");
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {

            calculateResult(j);
            calculateResult(j + 1);

            if (total[j] < total[j + 1]) {

                // Swap roll numbers
                temp = roll[j];
                roll[j] = roll[j + 1];
                roll[j + 1] = temp;

                // Swap names
                strcpy(tempName, name[j]);
                strcpy(name[j], name[j + 1]);
                strcpy(name[j + 1], tempName);

                // Swap subject-wise marks
                for (int k = 0; k < SUBJECTS; k++) {
                    temp = marks[j][k];
                    marks[j][k] = marks[j + 1][k];
                    marks[j + 1][k] = temp;
                }
            }
        }
    }

    printf("\nResults sorted by total marks!\n");
    displayResults();
}

// Find topper
void findTopper() {
    if (count == 0) {
        printf("\nNo student records available!\n");
        return;
    }

    int top = 0;

    for (int i = 0; i < count; i++)
        calculateResult(i);

    for (int i = 1; i < count; i++) {
        if (total[i] > total[top])
            top = i;
    }

    printf("\n========== TOPPER DETAILS ==========\n");

    displayMarksheet(top);
}

// Subject-wise analysis
void subjectAnalysis() {
    if (count == 0) {
        printf("\nNo student records available!\n");
        return;
    }

    printf("\n======= SUBJECT-WISE ANALYSIS =======\n");

    for (int j = 0; j < SUBJECTS; j++) {
        int highest = marks[0][j];
        int lowest = marks[0][j];
        int sum = 0;

        for (int i = 0; i < count; i++) {
            sum += marks[i][j];

            if (marks[i][j] > highest)
                highest = marks[i][j];

            if (marks[i][j] < lowest)
                lowest = marks[i][j];
        }

        printf("\nSubject: %s\n", subject[j]);
        printf("Highest Marks : %d\n", highest);
        printf("Average Marks : %.2f\n",
               sum / (float)count);
        printf("Lowest Marks  : %d\n", lowest);
    }
}

// Calculate overall pass percentage
void passPercentage() {
    if (count == 0) {
        printf("\nNo student records available!\n");
        return;
    }

    int passed = 0;

    for (int i = 0; i < count; i++) {
        calculateResult(i);

        if (status[i] == 'P')
            passed++;
    }

    float passRate = (passed * 100.0f) / count;

    printf("\n======= PASS PERCENTAGE =======\n");
    printf("Total Students : %d\n", count);
    printf("Passed         : %d\n", passed);
    printf("Failed         : %d\n", count - passed);
    printf("Pass Percentage: %.2f%%\n", passRate);
}

// Analysis menu
void analysisModule() {
    int choice;

    do {
        printf("\n\n======= ANALYSIS AND RANKING =======");
        printf("\n1. Sort by Total Marks");
        printf("\n2. Find Topper");
        printf("\n3. Subject-wise Analysis");
        printf("\n4. Pass Percentage");
        printf("\n5. Back to Main Menu");

        choice = readInt("\nEnter Choice: ");

        switch (choice) {
            case 1:
                sortResults();
                break;

            case 2:
                findTopper();
                break;

            case 3:
                subjectAnalysis();
                break;

            case 4:
                passPercentage();
                break;

            case 5:
                break;

            default:
                printf("\nInvalid Choice!\n");
        }

    } while (choice != 5);
}

// ---------- MAIN FUNCTION ----------

int main() {
    int choice;

    do {
        printf("\n\n======================================");
        printf("\n ONLINE EXAMINATION RESULT ANALYZER");
        printf("\n======================================");

        printf("\n1. Result Data Entry Module");
        printf("\n2. Result Calculation Module");
        printf("\n3. Search Module");
        printf("\n4. Analysis and Ranking Module");
        printf("\n5. Display All Results");
        printf("\n6. Exit");

        choice = readInt("\n\nEnter Your Choice: ");

        switch (choice) {
            case 1:
                dataEntry();
                break;

            case 2:
                if (count == 0) {
                    printf("\nNo records available!\n");
                } else {
                    for (int i = 0; i < count; i++)
                        displayMarksheet(i);
                }
                break;

            case 3:
                searchModule();
                break;

            case 4:
                analysisModule();
                break;

            case 5:
                displayResults();
                break;

            case 6:
                printf("\nThank you for using the system!\n");
                break;

            default:
                printf("\nInvalid choice! Try again.\n");
        }

    } while (choice != 6);

    return 0;
}
