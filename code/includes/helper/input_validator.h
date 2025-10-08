//
// Created by cantonio on 29/09/2025.
//

#ifndef OCEANDEPTHS_INPUT_VALIDATOR_H
#define OCEANDEPTHS_INPUT_VALIDATOR_H
#include <stddef.h>

/**
 * @brief Handles input validation and returns 1 on success, 0 on failure.
 *        On success, the buffer is populated with the sanitized input.
 * @param buffer The buffer to store the input (must be pre-allocated).
 * @param max_len The maximum length of the input (including null terminator).
 * @return 1 on success, 0 on failure (invalid input, EOF, or error).
 */

int get_sanitized_input(char *buffer, size_t max_len);

#endif //OCEANDEPTHS_INPUT_VALIDATOR_H