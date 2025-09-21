// gcc -O1 -o license_check license_check.c && strip license_check
#include <stdio.h>
#include <string.h>

void unlock_feature() {
    printf("Access granted! The premium feature is now unlocked.\n");
}

int check_license(char *input_license) {
    // Hardcoded license key
    static const char correct_license[] = "ABC123-XYZ789";
    
    // Check if the input matches the correct license
    if (strcmp(input_license, correct_license) == 0) {
        return 1; // License is valid
    } else {
        return 0; // License is invalid
    }
}

int main() {
    char input_license[50];
    
    printf("Enter your license key: ");
    scanf("%49s", input_license);
    
    // Check if the license is valid
    if (check_license(input_license)) {
        unlock_feature();
    } else {
        printf("Invalid license key. Access denied.\n");
    }
    
    return 0;
}
