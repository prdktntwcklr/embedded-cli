#include "cli.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
cli_status_t rslt = CLI_OK;

cli_status_t help_func(int argc, char **argv)
{
    cli_status_t ok = CLI_OK;
    cli.println("[cli] CLI HELP. Available commands:\n");
    cli.println("  gpio (-set or -get)\n");
    cli.println("  adc (get_sample)\n");
    return ok;
}

cli_status_t gpio_func(int argc, char **argv)
{
    cli_status_t ok = CLI_OK;
    if(strcmp(argv[1], "-set") == 0 && argc == 4)
    {
        int pin = atoi(argv[2]);
        int val = atoi(argv[3]);
        cli.println("[cli] gpio set pin %d to %d\n", pin, val);
    }
    else if(strcmp(argv[1], "-get") == 0 && argc == 3)
    {
        int pin = atoi(argv[2]);
        cli.println("[cli] gpio get pin %d\n", pin);
    }
    else
    {
        cli.println("missing arguments\n");
    }
    return ok;
}

cli_status_t adc_func(int argc, char **argv)
{
    cli_status_t ok = CLI_OK;
    if(argc == 3)
    {
        if(strcmp(argv[1], "-sample_channel") == 0 && argc == 3)
        {
            int channel = atoi(argv[2]);
            cli.println("[cli] adc sample channel %d\n", channel);
            return ok;
        }
    }

    cli.println("missing arguments\n");
    return CLI_E_INVALID_ARGS;
}

int user_uart_println(char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

int main(void)
{
    cli.println = user_uart_println;
    cli.cmd_tbl = cmd_tbl;
    cli.cmd_cnt = sizeof(cmd_tbl) / sizeof(cmd_t);

    if((rslt = cli_init(&cli, cli_buffer, sizeof(cli_buffer))) != CLI_OK)
    {
        printf("CLI: Failed to initialise");
    }

    printf("Welcome to the cli. Type 'help' to get a list of commands.\n");

    while(1)
    {
        char in = fgetc(stdin);
        cli_put(&cli, in);
        cli_process(&cli);
    }

    return 0;
}
