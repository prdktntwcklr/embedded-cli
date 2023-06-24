#include "unity.h"

#include "cli.h"
#include "cli_defs.h"

#include <stdarg.h>

static cli_status_t help_func(int argc, char **argv);
static cli_status_t gpio_func(int argc, char **argv);
static cli_status_t adc_func(int argc, char **argv);
static cli_status_t big_func(int argc, char **argv);
static uint8_t cli_buffer[256] = {0};
static char test_buffer[256] = {0};

cmd_t cmd_tbl[4] = {
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
    {
        .cmd = "big",
        .func = big_func,
    },
};

cli_t cli;

cli_status_t help_func(int argc, char **argv)
{
    cli_status_t ok = CLI_OK;
    cli.println("[cli] CLI HELP. Available commands:\n  gpio (-set or -get)\n  "
                "adc (get_sample)\n\n");
    return ok;
}

cli_status_t gpio_func(int argc, char **argv)
{
    cli_status_t ok = CLI_OK;
    cli.println("[cli] gpio [need to implement]\n");
    return ok;
}

cli_status_t adc_func(int argc, char **argv)
{
    cli_status_t ok = CLI_OK;
    cli.println("[cli] adc [need to implement]\n");
    return ok;
}

static cli_status_t big_func(int argc, char **argv)
{
    cli_status_t ok = CLI_OK;
    // 212 characters
    cli.println("0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCD"
                "EF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789AB"
                "CDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789"
                "ABCDEF0123456789ABCDEF-END\n");
    return ok;
}

void user_uart_println(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    vsprintf(test_buffer, format, args);
    va_end(args);
}

void setUp(void)
{
    cli_status_t cli_result = CLI_MAX_STATUS;
    cli.println = user_uart_println;
    cli.cmd_tbl = cmd_tbl;
    cli.cmd_cnt = sizeof(cmd_tbl) / sizeof(cmd_t);
    if((cli_result = cli_init(&cli, cli_buffer, sizeof(cli_buffer))) != CLI_OK)
    {
        printf("CLI: Failed to initialise");
        TEST_FAIL();
    }
}

void tearDown(void) {}

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

void test_cli_cmd_big(void)
{
    char cmd[] = {'b', 'i', 'g', '\n'};
    for(int i = 0; i < sizeof(cmd); i++)
    {
        cli_put(&cli, cmd[i]);
    }

    cli_status_t result = cli_process(&cli);
    TEST_ASSERT_EQUAL_INT(CLI_OK, result);

    // Make sure we get all the way to the end
    TEST_ASSERT_EQUAL_UINT8('D', test_buffer[211]);
}
