//
// Created by cantonio on 02/10/2025.
//

#include <stdio.h>

#include <stdlib.h>

#include "interface/interface_cli.h"
#include "helper/input_validator.h"
// #include "interface_sdl.h"  // for sdl later

InterfaceVTable* current_interface = NULL;

InterfaceVTable* select_interface() {
    printf("Choose interface:\n");
    printf("1 - Terminal\n");
    printf("2 - SDL\n");
    printf("> ");
    fflush(stdout);

    const int size = 10;  // ← Make buffer bigger
    char choice_char[size];

    // get_sanitized_input returns 1 for success, not the actual value!
    if (get_sanitized_input(choice_char, size)) {
        const int choice = atoi(choice_char);

        if (choice == 1) {
            current_interface = &cli_interface;
            printf("Terminal interface selected!\n");
        } else if (choice == 2) {
            printf("SDL interface not yet implemented!\nDefaulting to terminal.\n");
            current_interface = &cli_interface;  // ← Don't exit, just default
        } else {
            printf("Invalid choice, defaulting to terminal.\n");
            current_interface = &cli_interface;
        }
    } else {
        printf("Invalid input, defaulting to terminal.\n");
        current_interface = &cli_interface;
    }

    return current_interface;
}