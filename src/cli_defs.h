#ifndef _CLI_DEFS_H_
#define _CLI_DEFS_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_ARGS            30
#define CMD_TERMINATOR      '\n'    /* Delimitor denoting end of cmd */

typedef enum
{
    CLI_OK,                 /* API execution successful.                */
    CLI_E_NULL_PTR,         /* Null pointer error.                      */
    CLI_E_IO,
    CLI_E_CMD_NOT_FOUND,    /* Command name not found in command table. */
    CLI_E_INVALID_ARGS,     /* Invalid function parameters/arguments.   */
    CLI_E_BUF_FULL,         /* CLI buffer full.                         */
    CLI_E_CMD_NOT_READY
} cli_status_t;

/*!
 * @brief Function type declarations.
 */
typedef cli_status_t (*cmd_func_ptr_t)(int argc, char **argv);
typedef void (*println_func_ptr_t)(const char * format, ...);

/*!
 * @brief Command structure, consisting of a name and function pointer.
 */
typedef struct
{
    char *cmd;           /* Command name.                            */
    cmd_func_ptr_t func; /* Function pointer to associated function. */
    char *help_text;    /* Help text.                                */
} cmd_t;

/*!
 * @brief Command-line interface handle structure.
 */
typedef struct
{
    println_func_ptr_t println; /* Function pointer to user defined println function.      */
    cmd_t *cmd_tbl;             /* Pointer to series of commands which are to be accepted. */
    size_t cmd_cnt;             /* Number of commands in cmd_tbl.                          */
} cli_t;

/*!
 * @brief Struct for holding received data
 */
typedef struct
{
    uint8_t *buf_ptr;               /* Pointer to Rx byte-buffer */
    uint16_t buf_size;              /* Size of buffer */
    uint16_t current_buf_length;    /* Current length of buffer */
    bool is_ready;                  /* Is a command fully received and ready to be processed */
} rx_data_t;

#endif