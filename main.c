#include "cli.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static cli_status_t help_func(int argc, char **argv);
static cli_status_t gpio_func(int argc, char **argv);
static cli_status_t adc_func(int argc, char **argv);

cmd_t cmd_tbl[3] = {
    {
        .cmd = "help",
        .func = help_func
    },
    {
        .cmd = "gpio",
        .func = gpio_func
    },
    {
        .cmd = "adc",
        .func = adc_func
    }
};
cli_t cli;
cli_status_t rslt = CLI_OK;

cli_status_t help_func(int argc, char **argv)
{
    cli_status_t ok = CLI_OK;
    cli.println("<cli> CLI HELP. Available commands:\n  gpio (-set or -get)\n  adc (get_sample)\n\n");
    return ok;
}

cli_status_t gpio_func(int argc, char **argv)
{
    cli_status_t ok = CLI_OK;
    cli.println("<cli> gpio [need to implement]\n");
    return ok;
}

cli_status_t adc_func(int argc, char **argv)
{
    cli_status_t ok = CLI_OK;
    cli.println("<cli> adc [need to implement]\n");
    return ok;
}

void user_uart_println(char *string)
{
    printf("%s", string);
}

int main(void)
{
    cli.println = user_uart_println;
    cli.cmd_tbl = cmd_tbl;
    cli.cmd_cnt = sizeof(cmd_tbl)/sizeof(cmd_t);

    if((rslt = cli_init(&cli)) != CLI_OK)
    {
        printf("CLI: Failed to initialise");
    }

    while(1)
    {
        char in = fgetc(stdin);
        cli_put(&cli, in);
        cli_process(&cli);
    }

    return 0;
}