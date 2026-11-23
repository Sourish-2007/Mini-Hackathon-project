#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUBJECTS 5

struct Student {
    char name[50];
    int id;
    float marks[SUBJECTS];
};


/* ---------- Function Prototypes ---------- */

void add();
void search();
void update();
void deleteStudent();
void maths();
void display();
void exitProgram();

/* Sub-functions of maths */
void average();
void pass();
void grade();


/* =========================================
   HELPER: Calculate Percentage
   ========================================= */

float getPercentage(struct Student s) {

    float total = 0;

    for (int i = 0; i < SUBJECTS; i++) {
        total = total + s.marks[i];
    }

    return total / SUBJECTS;
}


/* =========================================
   1. ADD STUDENT
   ========================================= */

void add() {

    struct Student s;
    FILE *fp;

    fp = fopen("students.dat", "ab");

    if (fp == NULL) {
        printf("\nError opening file!\n");
        return;
    }

    printf("\n===== ADD STUDENT =====\n");

    printf("Enter Student Name: ");
    scanf(" %[^\n]", s.name);

    printf("Enter Student ID: ");
    scanf("%d", &s.id);

    printf("\nEnter marks of 5 subjects:\n");

    for (int i = 0; i < SUBJECTS; i++) {

        do {
            printf("Subject %d: ", i + 1);
            scanf("%f", &s.marks[i]);

            if (s.marks[i] < 0 || s.marks[i] > 100) {
                printf("Marks must be between 0 and 100.\n");
            }

        } while (s.marks[i] < 0 || s.marks[i] > 100);
    }

    fwrite(&s, sizeof(s), 1, fp);

    fclose(fp);

    printf("\nStudent added successfully!\n");
}


/* =========================================
   2. SEARCH
   ========================================= */

void search() {

    FILE *fp;
    struct Student s;
    struct Student topper;

    int choice;
    int id;
    int found = 0;
    int first = 1;

    char name[50];

    fp = fopen("students.dat", "rb");

    if (fp == NULL) {
        printf("\nNo student records found.\n");
        return;
    }

    printf("\n===== SEARCH =====\n");
    printf("1. Search by Name\n");
    printf("2. Search by ID\n");
    printf("3. Search Topper\n");

    printf("Enter choice: ");
    scanf("%d", &choice);


    /* Search by Name */

    if (choice == 1) {

        printf("Enter Student Name: ");
        scanf(" %[^\n]", name);

        while (fread(&s, sizeof(s), 1, fp)) {

            if (strcmp(s.name, name) == 0) {

                printf("\nStudent Found!\n");
                printf("Name: %s\n", s.name);
                printf("ID: %d\n", s.id);

                printf("Marks: ");

                for (int i = 0; i < SUBJECTS; i++) {
                    printf("%.2f ", s.marks[i]);
                }

                printf("\nPercentage: %.2f%%\n",
                       getPercentage(s));

                found = 1;
            }
        }

        if (found == 0)
            printf("\nStudent not found.\n");
    }


    /* Search by ID */

    else if (choice == 2) {

        printf("Enter Student ID: ");
        scanf("%d", &id);

        while (fread(&s, sizeof(s), 1, fp)) {

            if (s.id == id) {

                printf("\nStudent Found!\n");
                printf("Name: %s\n", s.name);
                printf("ID: %d\n", s.id);

                printf("Marks: ");

                for (int i = 0; i < SUBJECTS; i++) {
                    printf("%.2f ", s.marks[i]);
                }

                printf("\nPercentage: %.2f%%\n",
                       getPercentage(s));

                found = 1;

                break;
            }
        }

        if (found == 0)
            printf("\nStudent not found.\n");
    }


    /* Search Topper */

    else if (choice == 3) {

        while (fread(&s, sizeof(s), 1, fp)) {

            if (first == 1 ||
                getPercentage(s) > getPercentage(topper)) {

                topper = s;
                first = 0;
            }
        }

        if (first == 0) {

            printf("\n===== CLASS TOPPER =====\n");

            printf("Name: %s\n", topper.name);
            printf("ID: %d\n", topper.id);

            printf("Percentage: %.2f%%\n",
                   getPercentage(topper));
        }

        else {
            printf("\nNo students found.\n");
        }
    }

    else {
        printf("\nInvalid choice.\n");
    }

    fclose(fp);
}


/* =========================================
   3. UPDATE
   ========================================= */

void update() {

    FILE *fp;
    struct Student s;

    int id;
    int choice;
    int found = 0;

    fp = fopen("students.dat", "rb+");

    if (fp == NULL) {
        printf("\nNo student records found.\n");
        return;
    }

    printf("\nEnter Student ID to update: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(s), 1, fp)) {

        if (s.id == id) {

            found = 1;

            printf("\nStudent: %s\n", s.name);

            printf("\nWhat do you want to update?\n");

            printf("1. Name\n");
            printf("2. ID\n");
            printf("3. Marks\n");
            printf("4. Everything\n");

            printf("Enter choice: ");
            scanf("%d", &choice);


            /* Update Name */

            if (choice == 1) {

                printf("Enter new name: ");
                scanf(" %[^\n]", s.name);
            }


            /* Update ID */

            else if (choice == 2) {

                printf("Enter new ID: ");
                scanf("%d", &s.id);
            }


            /* Update Marks */

            else if (choice == 3) {

                printf("Enter new marks:\n");

                for (int i = 0; i < SUBJECTS; i++) {

                    do {

                        printf("Subject %d: ", i + 1);
                        scanf("%f", &s.marks[i]);

                    } while (s.marks[i] < 0 ||
                             s.marks[i] > 100);
                }
            }


            /* Update Everything */

            else if (choice == 4) {

                printf("Enter new name: ");
                scanf(" %[^\n]", s.name);

                printf("Enter new ID: ");
                scanf("%d", &s.id);

                printf("Enter new marks:\n");

                for (int i = 0; i < SUBJECTS; i++) {

                    do {

                        printf("Subject %d: ", i + 1);
                        scanf("%f", &s.marks[i]);

                    } while (s.marks[i] < 0 ||
                             s.marks[i] > 100);
                }
            }

            else {

                printf("\nInvalid choice.\n");
                fclose(fp);
                return;
            }


            /* Move back to current record */

            fseek(fp, -(long)sizeof(s), SEEK_CUR);

            /* Rewrite updated record */

            fwrite(&s, sizeof(s), 1, fp);

            printf("\nStudent updated successfully!\n");

            break;
        }
    }

    if (found == 0) {
        printf("\nStudent not found.\n");
    }

    fclose(fp);
}


/* =========================================
   4. DELETE STUDENT
   ========================================= */

void deleteStudent() {

    FILE *fp;
    FILE *temp;

    struct Student s;

    int choice;
    int id;
    int found = 0;

    char name[50];

    fp = fopen("students.dat", "rb");

    if (fp == NULL) {
        printf("\nNo student records found.\n");
        return;
    }

    temp = fopen("temp.dat", "wb");

    if (temp == NULL) {
        printf("\nUnable to create temporary file.\n");
        fclose(fp);
        return;
    }

    printf("\n===== DELETE STUDENT =====\n");

    printf("1. Delete by Name\n");
    printf("2. Delete by ID\n");

    printf("Enter choice: ");
    scanf("%d", &choice);


    if (choice == 1) {

        printf("Enter Student Name: ");
        scanf(" %[^\n]", name);

        while (fread(&s, sizeof(s), 1, fp)) {

            if (strcmp(s.name, name) == 0) {

                found = 1;

            } else {

                fwrite(&s, sizeof(s), 1, temp);
            }
        }
    }


    else if (choice == 2) {

        printf("Enter Student ID: ");
        scanf("%d", &id);

        while (fread(&s, sizeof(s), 1, fp)) {

            if (s.id == id) {

                found = 1;

            } else {

                fwrite(&s, sizeof(s), 1, temp);
            }
        }
    }


    else {

        printf("\nInvalid choice.\n");

        fclose(fp);
        fclose(temp);

        remove("temp.dat");

        return;
    }


    fclose(fp);
    fclose(temp);


    if (found == 1) {

        remove("students.dat");

        rename("temp.dat", "students.dat");

        printf("\nStudent deleted successfully!\n");

    } else {

        remove("temp.dat");

        printf("\nStudent not found.\n");
    }
}


/* =========================================
   MATHS -> AVERAGE
   ========================================= */

void average() {

    FILE *fp;
    struct Student s;

    float sum = 0;
    int count = 0;

    fp = fopen("students.dat", "rb");

    if (fp == NULL) {
        printf("\nNo student records found.\n");
        return;
    }

    while (fread(&s, sizeof(s), 1, fp)) {

        sum = sum + getPercentage(s);

        count++;
    }

    fclose(fp);

    if (count == 0) {

        printf("\nNo students found.\n");

    } else {

        printf("\nClass Average = %.2f%%\n",
               sum / count);
    }
}


/* =========================================
   MATHS -> PASS PERCENTAGE
   ========================================= */

void pass() {

    FILE *fp;
    struct Student s;

    int totalStudents = 0;
    int passedStudents = 0;

    fp = fopen("students.dat", "rb");

    if (fp == NULL) {
        printf("\nNo student records found.\n");
        return;
    }

    while (fread(&s, sizeof(s), 1, fp)) {

        totalStudents++;

        int passed = 1;

        /*
           Student must get at least 40
           in every subject to pass.
        */

        for (int i = 0; i < SUBJECTS; i++) {

            if (s.marks[i] < 40) {
                passed = 0;
                break;
            }
        }

        if (passed == 1) {
            passedStudents++;
        }
    }

    fclose(fp);


    if (totalStudents == 0) {

        printf("\nNo students found.\n");

    } else {

        float passPercentage;

        passPercentage =
            ((float)passedStudents / totalStudents) * 100;

        printf("\nTotal Students  : %d\n",
               totalStudents);

        printf("Passed Students : %d\n",
               passedStudents);

        printf("Pass Percentage : %.2f%%\n",
               passPercentage);
    }
}


/* =========================================
   MATHS -> GRADE
   ========================================= */

void grade() {

    FILE *fp;
    struct Student s;

    float percentage;

    fp = fopen("students.dat", "rb");

    if (fp == NULL) {
        printf("\nNo student records found.\n");
        return;
    }


    printf("\n===== STUDENT GRADES =====\n");


    while (fread(&s, sizeof(s), 1, fp)) {

        percentage = getPercentage(s);

        printf("\nName       : %s\n", s.name);
        printf("ID         : %d\n", s.id);
        printf("Percentage : %.2f%%\n", percentage);


        /* Check whether student failed any subject */

        int failed = 0;

        for (int i = 0; i < SUBJECTS; i++) {

            if (s.marks[i] < 40) {
                failed = 1;
                break;
            }
        }


        if (failed == 1) {

            printf("Grade      : F\n");

        }

        else if (percentage >= 90) {

            printf("Grade      : A+\n");

        }

        else if (percentage >= 80) {

            printf("Grade      : A\n");

        }

        else if (percentage >= 70) {

            printf("Grade      : B\n");

        }

        else if (percentage >= 60) {

            printf("Grade      : C\n");

        }

        else if (percentage >= 50) {

            printf("Grade      : D\n");

        }

        else if (percentage >= 40) {

            printf("Grade      : E\n");

        }

        else {

            printf("Grade      : F\n");
        }
    }

    fclose(fp);
}


/* =========================================
   5. MATHS
   ========================================= */

void maths() {

    int choice;

    printf("\n===== MATHS / ANALYSIS =====\n");

    printf("1. Class Average\n");
    printf("2. Pass Percentage\n");
    printf("3. Student Grades\n");

    printf("Enter choice: ");
    scanf("%d", &choice);


    switch (choice) {

        case 1:
            average();
            break;

        case 2:
            pass();
            break;

        case 3:
            grade();
            break;

        default:
            printf("\nInvalid choice.\n");
    }
}


/* =========================================
   6. DISPLAY ALL STUDENTS
   ========================================= */

void display() {

    FILE *fp;
    struct Student s;

    fp = fopen("students.dat", "rb");

    if (fp == NULL) {

        printf("\nNo student records found.\n");
        return;
    }


    printf("\n");
    printf("================================================================================================================\n");

    printf("%-20s %-8s ", "NAME", "ID");

    for (int i = 1; i <= SUBJECTS; i++) {
        printf("SUB%d     ", i);
    }

    printf("AVERAGE\n");

    printf("================================================================================================================\n");


    while (fread(&s, sizeof(s), 1, fp)) {

        printf("%-20s %-8d ",
               s.name,
               s.id);

        for (int i = 0; i < SUBJECTS; i++) {

            printf("%-8.2f ",
                   s.marks[i]);
        }

        printf("%.2f%%\n",
               getPercentage(s));
    }


    printf("================================================================================================================\n");

    fclose(fp);
}


/* =========================================
   7. EXIT
   ========================================= */

void exitProgram() {

    printf("\n=================================\n");
    printf(" Student Result Analyzer Closed\n");
    printf("=================================\n");

    exit(0);
}


/* =========================================
   MAIN
   ========================================= */

int main() {

    int choice;

    while (1) {

        printf("\n\n");
        printf("========================================\n");
        printf("   STUDENT RESULT & GRADE ANALYZER\n");
        printf("========================================\n");

        printf("1. Add Student\n");
        printf("2. Search\n");
        printf("3. Update Student\n");
        printf("4. Delete Student\n");
        printf("5. Maths / Analysis\n");
        printf("6. Display All Students\n");
        printf("7. Exit\n");

        printf("========================================\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);


        switch (choice) {

            case 1:
                add();
                break;

            case 2:
                search();
                break;

            case 3:
                update();
                break;

            case 4:
                deleteStudent();
                break;

            case 5:
                maths();
                break;

            case 6:
                display();
                break;

            case 7:
                exitProgram();
                break;

            default:
                printf("\nInvalid choice! Enter 1-7.\n");
        }
    }

    return 0;
}