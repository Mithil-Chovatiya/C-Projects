#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to execute a system command and capture its output
void execute_command(const char *cmd, char *output, size_t size) {
    FILE *fp = _popen(cmd, "r");
    if (fp == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    size_t len = 0;
    while (fgets(output + len, size - len, fp) != NULL) {
        len += strlen(output + len);
    }

    _pclose(fp);
}

int main() {
    char profile_name[256];
    char cmd[512];
    char output[8192];

    // Prompt the user to enter the Wi-Fi profile name
    printf("Enter the Wi-Fi profile name: ");
    fgets(profile_name, sizeof(profile_name), stdin);

    // Remove the newline character from the input
    size_t len = strlen(profile_name);
    if (len > 0 && profile_name[len-1] == '\n') {
        profile_name[len-1] = '\0';
    }

    // Construct the command to show the Wi-Fi profile details
    snprintf(cmd, sizeof(cmd), "netsh wlan show profile name=\"%s\" key=clear", profile_name);
    execute_command(cmd, output, sizeof(output));
    //For Password
    printf("\nKey Content is your password\n");
    // Print the output of the command
    printf("\nProfile: %s\n", profile_name);
    printf("%s\n", output);

    // Find the password in the output
    char *key_content = strstr(output, "Key Content");
    if (key_content != NULL) {
        key_content += 12; // Skip "Key Content" text
        char password[256];
        sscanf(key_content, ": %255[^\r\n]", password);
        printf("Password: %s\n", password);
    } else {
        printf("Password not found or not set.\n");
    }

    return 0;
}
