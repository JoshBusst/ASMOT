#include <time.h>
#include <inttypes.h>
#include <stdio.h>

// #include "sparse.h"


#include <stdio.h>

// Parent struct
typedef struct {
    char name[50];
    int age;
} Person;

// Child struct
typedef struct {
    Person person;  // Inherit/embed Person
    char job[50];
} Employee;

int main(void) {
    // Instance of parent
    Person p = {
        .name = "Alice",
        .age = 30
    };

    // Instance of child
    Employee e = {
        .person = {
            .name = "Bob",
            .age = 40
        },
        .job = "Engineer"
    };

    // Print parent
    printf("Person:\n");
    printf("  Name: %s\n", p.name);
    printf("  Age: %d\n", p.age);

    // Print child
    printf("\nEmployee:\n");
    printf("  Name: %s\n", e.person.name);
    printf("  Age: %d\n", e.person.age);
    printf("  Job: %s\n", e.job);

    return 0;
}