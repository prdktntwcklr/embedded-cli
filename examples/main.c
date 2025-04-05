#include "cli.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(x) ((void)(x))

static cli_status_t help_func(int argc, char **argv);
static cli_status_t gpio_func(int argc, char **argv);
static cli_status_t adc_func(int argc, char **argv);
static uint8_t cli_buffer[256] = {0};

cmd_t cmd_tbl[3] = {
    {
        .cmd = "help",
        .func = help_func,
    },
    {
        .cmd = "gpio",
        .func = gpio_func,
    },
    {
        .cmd = "adc",
        .func = adc_func,
    },
};
cli_t cli;

cli_status_t help_func(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    cli_status_t ok = CLI_OK;
    cli.println("CLI HELP. Available commands:\n");
    cli.println("  gpio (-set or -get)\n");
    cli.println("  adc (get_sample)\n");
    return ok;
}

cli_status_t gpio_func(int argc, char **argv)
{
    cli_status_t ok = CLI_OK;

    if(argc >= 3)
    {
        if(strcmp(argv[1], "-set") == 0 && argc == 4)
        {
            int pin = (int)strtol(argv[2], NULL, 10);
            int val = (int)strtol(argv[3], NULL, 10);
            cli.println("gpio setting pin %d to: %d\n", pin, val);
            return ok;
        }
        else if(strcmp(argv[1], "-get") == 0 && argc == 3)
        {
            int pin = (int)strtol(argv[2], NULL, 10);
            cli.println("gpio getting pin: %d\n", pin);
            return ok;
        }
    }

    cli.println("arguments missing or unknown\n");

    return ok;
}

cli_status_t adc_func(int argc, char **argv)
{
    cli_status_t ok = CLI_OK;

    if(argc == 3)
    {
        if(strcmp(argv[1], "-sample_channel") == 0 && argc == 3)
        {
            int channel = (int)strtol(argv[2], NULL, 10);
            cli.println("adc sample channel %d\n", channel);
            return ok;
        }
    }

    cli.println("arguments missing or unknown\n");

    return CLI_E_INVALID_ARGS;
}

int user_uart_println(char *format, ...)
{
    printf("[cli] ");

    va_list args;
    va_start(args, format);

    // NOLINTNEXTLINE(clang-diagnostic-format-nonliteral)
    int chars_written = vprintf(format, args);

    va_end(args);

    return chars_written;
}

int main(void)
{
    cli.println = user_uart_println;
    cli.cmd_tbl = cmd_tbl;
    cli.cmd_cnt = sizeof(cmd_tbl) / sizeof(cmd_t);

    cli_status_t result = cli_init(&cli, cli_buffer, sizeof(cli_buffer));

    if(result != CLI_OK)
    {
        printf("ERROR: Failed to initialise cli.");

        return 1;
    }

    printf("Welcome to the cli. Type 'help' to get a list of commands.\n");

    while(1)
    {
        char in = (char)fgetc(stdin);
        cli_put(&cli, in);
        cli_process(&cli);
    }

    return 0;
}
