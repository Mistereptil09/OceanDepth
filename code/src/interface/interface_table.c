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
    printf("Choix Interface:\n");
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
            printf("Terminal selectionne!\n");
        } else if (choice == 2) {
            printf("SDL non implemente!\nPassage au terminal.\n");
            current_interface = &cli_interface;
        } else {
            printf("Choix invalide, passage au terminal.\n");
            current_interface = &cli_interface;
        }
    } else {
        printf("Choix invalide, passage au terminal.\n");
        current_interface = &cli_interface;
    }

    return current_interface;
}