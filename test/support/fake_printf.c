#include "fake_printf.h"

#include "unity.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define FILE_NAME  "fake_printf.txt"
#define MAX_LENGTH (64)

static char message[MAX_LENGTH] = "";

int fake_printf(char *format, ...)
{
    FILE *file = fopen(FILE_NAME, "a");

    if(file == NULL)
    {
        fprintf(
            stderr,
            "Failed to create fake_printf file for write: %s (errno = %d)\n",
            strerror(errno), errno);
        TEST_FAIL();
    }

    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);

    fclose(file);
}

char *fake_printf_get_last_message(void)
{
    FILE *file = fopen(FILE_NAME, "r");

    if(file == NULL)
    {
        fprintf(stderr,
                "Failed to open fake_printf file for read: %s (errno = %d)\n",
                strerror(errno), errno);
        TEST_FAIL();
    }

    /* read line from file into message */
    fgets(message, MAX_LENGTH, file);

    fclose(file);

    return message;
}

void fake_printf_reset(void)
{
    /* open file in write mode to clear contents */
    FILE *file = fopen(FILE_NAME, "w");

    if(file == NULL)
    {
        fprintf(
            stderr,
            "Failed to create fake_printf file for clear: %s (errno = %d)\n",
            strerror(errno), errno);
    }

    fclose(file);
}

void fake_printf_delete_file(void)
{
    int err_no = remove(FILE_NAME);

    if(err_no != 0)
    {
        fprintf(stderr, "Failed to delete fake_printf file: %s (errno = %d)\n",
                strerror(errno), errno);
        TEST_FAIL();
    }
}