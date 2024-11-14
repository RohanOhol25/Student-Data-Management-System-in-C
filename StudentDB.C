#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LENGTH 50
#define FILE_NAME "students.dat"

// Structure to hold student data
typedef struct {
    int id;
    char name[NAME_LENGTH];
    int age;
    float gpa;
} Student;

// Function prototypes
void addStudent();
void viewStudents();
void modifyStudent();
void deleteStudent();
void clearScreen();

int main() {
    int choice;

    while (1) {
        clearScreen();
        printf("=== Student Database Management System ===\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Modify Student\n");
        printf("4. Delete Student\n");
        printf("5. Exit\n");
        printf("==========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                viewStudents();
                break;
            case 3:
                modifyStudent();
                break;
            case 4:
                deleteStudent();
                break;
            case 5:
                printf("Exiting the program. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
                break;
        }
        printf("Press Enter to continue...");
        getchar(); // To consume newline character
        getchar(); // To wait for user input
    }

    return 0;
}

// Function to add a new student
void addStudent() {
    Student student;
    FILE *file = fopen(FILE_NAME, "ab");
    if (!file) {
        perror("Unable to open file");
        return;
    }

    printf("Enter Student ID: ");
    scanf("%d", &student.id);
    printf("Enter Student Name: ");
    getchar(); // Consume newline
    fgets(student.name, NAME_LENGTH, stdin);
    student.name[strcspn(student.name, "\n")] = 0; // Remove newline
    printf("Enter Student Age: ");
    scanf("%d", &student.age);
    printf("Enter Student GPA: ");
    scanf("%f", &student.gpa);

    fwrite(&student, sizeof(Student), 1, file);
    fclose(file);
    printf("Student added successfully!\n");
}

// Function to view all students
void viewStudents() {
    Student student;
    FILE *file = fopen(FILE_NAME, "rb");
    if (!file) {
        perror("Unable to open file");
        return;
    }

    printf("=== List of Students ===\n");
    while (fread(&student, sizeof(Student), 1, file)) {
        printf("ID: %d, Name: %s, Age: %d, GPA: %.2f\n", student.id, student.name, student.age, student.gpa);
    }

    fclose(file);
}

// Function to modify a student's details
void modifyStudent() {
    Student student;
    int id, found = 0;
    FILE *file = fopen(FILE_NAME, "r+b");
    if (!file) {
        perror("Unable to open file");
        return;
    }

    printf("Enter Student ID to modify: ");
    scanf("%d", &id);

    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.id == id) {
            found = 1;
            printf("Current Name: %s, Current Age: %d, Current GPA: %.2f\n", student.name, student.age, student.gpa);
            printf("Enter new Name: ");
            getchar(); // Consume newline
            fgets(student.name, NAME_LENGTH, stdin);
            student.name[strcspn(student.name, "\n")] = 0; // Remove newline
            printf("Enter new Age: ");
            scanf("%d", &student.age);
            printf("Enter new GPA: ");
            scanf("%f", &student.gpa);

            fseek(file, -sizeof(Student), SEEK_CUR);
            fwrite(&student, sizeof(Student), 1, file);
            printf("Student updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }

    fclose(file);
}

// Function to delete a student
void deleteStudent() {
    Student student;
    int id, found = 0;
    FILE *file = fopen(FILE_NAME, "rb");
    FILE *tempFile = fopen("temp.dat", "wb");
    if (!file || !tempFile) {
        perror("Unable to open file");
        return;
    }

    printf("Enter Student ID to delete: ");
    scanf("%d", &id);

    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.id == id) {
            found = 1;
            printf("Student with ID %d has been deleted.\n", id);
            continue; // Skip writing this student to the temp file
        }
        fwrite(&student, sizeof(Student), 1, tempFile);
    }

    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }

    fclose(file);
    fclose(tempFile);

    
    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
