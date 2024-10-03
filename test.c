#include <stdio.h>

int main() {
    int age;
    char name[100];

    printf("Enter your age: ");
    scanf("%d", &age); // User enters an integer, e.g., 25
    // Newline character remains in the input buffer

    // Here we attempt to read a string
    printf("Enter your name: ");
    scanf("%[^\n]", name); // Expected to read until newline

    // Output the collected information
    printf("Age: %d\n", age);
    printf("Name: %s\n", name);

    return 0;
}