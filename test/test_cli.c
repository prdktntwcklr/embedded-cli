#include "unity.h"

#include "cli.h"
#include "cli_defs.h"

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

void setUp(void)
{
    cli_status_t cli_result = CLI_E_IO;
    cli.println = user_uart_println;
    cli.cmd_tbl = cmd_tbl;
    cli.cmd_cnt = sizeof(cmd_tbl)/sizeof(cmd_t);
    if((cli_result = cli_init(&cli)) != CLI_OK)
    {
        printf("CLI: Failed to initialise");
        TEST_FAIL();
    }
}

void tearDown(void)
{
}

void test_cli_cmd_help(void)
{
    char cmd[] = {'h', 'e', 'l', 'p', '\n'};
    for(int i = 0; i < sizeof(cmd); i++)
    {
        cli_put(&cli, cmd[i]);
    }
    cli_status_t result = cli_process(&cli);
    TEST_ASSERT_EQUAL_INT(CLI_OK, result);
}

void test_cli_cmd_gpio(void)
{
    char cmd[] = {'g', 'p', 'i', 'o', '\n'};
    for(int i = 0; i < sizeof(cmd); i++)
    {
        cli_put(&cli, cmd[i]);
    }
    cli_status_t result = cli_process(&cli);
    TEST_ASSERT_EQUAL_INT(CLI_OK, result);
}

void test_cli_cmd_adc(void)
{
    char cmd[] = {'a', 'd', 'c', '\n'};
    for(int i = 0; i < sizeof(cmd); i++)
    {
        cli_put(&cli, cmd[i]);
    }
    cli_status_t result = cli_process(&cli);
    TEST_ASSERT_EQUAL_INT(CLI_OK, result);
}

void test_cli_cmd_bad(void)
{
    char cmd[] = {'n', 'o', '\n'};
    for(int i = 0; i < sizeof(cmd); i++)
    {
        cli_put(&cli, cmd[i]);
    }
    cli_status_t result = cli_process(&cli);
    TEST_ASSERT_EQUAL_INT(CLI_E_CMD_NOT_FOUND, result);
}

void test_cli_cmd_no_terminator(void)
{
    char cmd[] = {'h', 'e', 'l', 'p'};
    for(int i = 0; i < sizeof(cmd); i++)
    {
        cli_put(&cli, cmd[i]);
    }
    cli_status_t result = cli_process(&cli);
    TEST_ASSERT_EQUAL_INT(CLI_E_CMD_NOT_READY, result);

    cli_put(&cli, '\n');
    result = cli_process(&cli);
    TEST_ASSERT_EQUAL_INT(CLI_OK, result);
}
