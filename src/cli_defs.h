/*
 * MIT License
 *
 * Copyright (c) 2019 Sean Farrelly
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef _CLI_DEFS_H_
#define _CLI_DEFS_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_ARGS       30
#define CMD_TERMINATOR '\n' /* Delimitor denoting end of cmd */

typedef enum
{
    CLI_OK,              /* API execution successful. */
    CLI_E_NULL_PTR,      /* Null pointer error. */
    CLI_E_CMD_NOT_FOUND, /* Command name not found in command table. */
    CLI_E_INVALID_ARGS,  /* Invalid function parameters/arguments.   */
    CLI_E_BUF_FULL,      /* CLI buffer full.                         */
    CLI_E_CMD_NOT_READY, /* CLI command not ready. */
    CLI_MAX_STATUS       /* Maximum number of status codes. */
} cli_status_t;

/*!
 * @brief Function type declarations.
 */
typedef cli_status_t (*cmd_func_ptr_t)(int argc, char **argv);
typedef void (*println_func_ptr_t)(const char *format, ...);

/*!
 * @brief Command structure, consisting of a name and function pointer.
 */
typedef struct
{
    char *cmd;           /* Command name. */
    cmd_func_ptr_t func; /* Function pointer to associated function. */
    char *help_text;     /* Help text. */
} cmd_t;

/*!
 * @brief Command-line interface handle structure.
 */
typedef struct
{
    println_func_ptr_t println; /* Pointer to user defined println function.*/
    cmd_t *cmd_tbl; /* Pointer to series of commands to be accepted. */
    size_t cmd_cnt; /* Number of commands in cmd_tbl. */
} cli_t;

/*!
 * @brief Struct for holding received data
 */
typedef struct
{
    uint8_t *buf_ptr;            /* Pointer to Rx byte-buffer */
    uint16_t buf_size;           /* Size of buffer */
    uint16_t current_buf_length; /* Current length of buffer */
    bool is_ready; /* Command fully received and ready to be processed */
} rx_data_t;

#endif /* _CLI_DEFS_H_ */
