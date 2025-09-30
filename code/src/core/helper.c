//
// Created by cantonio on 29/09/2025.
//

#include "../../Include/core/helper.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int get_sanitized_input(char *buffer, size_t max_len) {
    if (buffer == NULL || max_len == 0) {
        return 0;
    }

    // Read input with fgets
    if (fgets(buffer, max_len, stdin) == NULL) {
        return 0; // EOF or error
    }

    // Remove trailing newline if present
    // if not present use has input size greater than max_len
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    } else if (len == max_len - 1) {
        // Input was too long, clear remaining input from stdin
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return 0; // Input exceeded buffer
    }

    // Skip leading whitespace
    size_t start = 0;
    while (buffer[start] && isspace((unsigned char)buffer[start])) {
        start++;
    }

    // Trim trailing whitespace
    while (len > start && isspace((unsigned char)buffer[len - 1])) {
        len--;
    }

    // Shift string if needed
    if (start > 0) {
        memmove(buffer, buffer + start, len - start + 1);
        len = len - start;
    }
    buffer[len] = '\0';

    // Check if empty after trimming
    if (len == 0) {
        return 0;
    }

    return 1;
}
