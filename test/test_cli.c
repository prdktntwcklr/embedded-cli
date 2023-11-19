#ifdef TEST

#include "unity.h"

#include "cli.h"
#include "fake_printf.h"

#include <stdarg.h>

static cli_status_t help_func(int argc, char **argv);
static cli_status_t args_func(int argc, char **argv);
static uint8_t cli_buffer[256] = {0};

cmd_t cmd_tbl[2] = {
    {
        .cmd = "help",
        .func = help_func,
    },
    {
        .cmd = "args",
        .func = args_func,
    },
};

cli_t cli;

cli_status_t help_func(int argc, char **argv)
{
    cli.println("cli help func called\n");
    return CLI_OK;
}

cli_status_t args_func(int argc, char **argv)
{
    cli.println("cli args func called with %d args:", argc);

    for(int i = 1; i < argc; i++)
    {
        cli.println(" %s", argv[i]);
    }

    cli.println("\n");

    return CLI_OK;
}

void setUp(void)
{
    cli_status_t cli_result = CLI_MAX_STATUS;
    cli.println = fake_printf;
    cli.cmd_tbl = cmd_tbl;
    cli.cmd_cnt = (sizeof(cmd_tbl) / sizeof(cmd_t));

    cli_result = cli_init(&cli, cli_buffer, sizeof(cli_buffer));

    if(cli_result != CLI_OK)
    {
        TEST_FAIL_MESSAGE("CLI failed to initialize.");
    }

    TEST_ASSERT_EQUAL_STRING("cli_init() ok.\n",
                             fake_printf_get_last_message());

    fake_printf_reset();
}

void tearDown(void)
{
    fake_printf_delete_file();
}

void test_cli_put_should_addCharacterToBuffer(void)
{
    char cmd[] = {'h', 'e', 'l', 'p'};

    TEST_ASSERT_EQUAL(0, cli_buffer[0]);
    TEST_ASSERT_EQUAL(0, cli_buffer[1]);
    TEST_ASSERT_EQUAL(0, cli_buffer[2]);
    TEST_ASSERT_EQUAL(0, cli_buffer[3]);
    TEST_ASSERT_EQUAL(0, cli_buffer[4]);

    for(int i = 0; i < sizeof(cmd); i++)
    {
        cli_put(&cli, cmd[i]);
    }

    TEST_ASSERT_EQUAL('h', cli_buffer[0]);
    TEST_ASSERT_EQUAL('e', cli_buffer[1]);
    TEST_ASSERT_EQUAL('l', cli_buffer[2]);
    TEST_ASSERT_EQUAL('p', cli_buffer[3]);
    TEST_ASSERT_EQUAL(0, cli_buffer[5]);
}

void test_cli_put_should_returnFullIfBufferIsFull(void)
{
    for(int i = 0; i < 256; i++)
    {
        TEST_ASSERT_EQUAL(CLI_OK, cli_put(&cli, 'a'));
    }

    TEST_ASSERT_EQUAL(CLI_E_BUF_FULL, cli_put(&cli, 'a'));

    /* sending terminator when buffer is full should work */
    TEST_ASSERT_EQUAL(CLI_OK, cli_put(&cli, '\n'));

    /* buffer should no longer be full */
    TEST_ASSERT_EQUAL(CLI_OK, cli_put(&cli, 'a'));
}

void test_cli_process_should_returnNotReadyIfNoTerminatorFound(void)
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

void test_cli_process_should_returnNotFoundIfUnknownCommand(void)
{
    char cmd[] = {'n', 'o', '\n'};

    for(int i = 0; i < sizeof(cmd); i++)
    {
        cli_put(&cli, cmd[i]);
    }

    cli_status_t result = cli_process(&cli);

    TEST_ASSERT_EQUAL_INT(CLI_E_CMD_NOT_FOUND, result);

    TEST_ASSERT_EQUAL_STRING("CLI ERROR: Command not recognized\n",
                             fake_printf_get_last_message());
}

void test_cli_process_should_callHelpIfHelpCmdReceived(void)
{
    char cmd[] = {'h', 'e', 'l', 'p', '\n'};

    for(int i = 0; i < sizeof(cmd); i++)
    {
        cli_put(&cli, cmd[i]);
    }

    cli_status_t result = cli_process(&cli);

    TEST_ASSERT_EQUAL_INT(CLI_OK, result);

    TEST_ASSERT_EQUAL_STRING("cli help func called\n",
                             fake_printf_get_last_message());
}

void test_cli_process_should_callArgsWithArgsIfArgsCmdReceived(void)
{
    char cmd[] = {'a', 'r', 'g', 's', ' ', 'a', 'r', 'g',
                  '1', ' ', 'p', 'a', 'r', '2', '\n'};

    for(int i = 0; i < sizeof(cmd); i++)
    {
        cli_put(&cli, cmd[i]);
    }

    cli_status_t result = cli_process(&cli);

    TEST_ASSERT_EQUAL_INT(CLI_OK, result);

    TEST_ASSERT_EQUAL_STRING("cli args func called with 3 args: arg1 par2\n",
                             fake_printf_get_last_message());
}

void test_cli_init_should_failIfNullPointerPassed(void)
{
    cli_t bad_cli;

    cli_status_t cli_result = CLI_MAX_STATUS;
    bad_cli.println = fake_printf;
    bad_cli.cmd_tbl = cmd_tbl;
    bad_cli.cmd_cnt = (sizeof(cmd_tbl) / sizeof(cmd_t));

    cli_result = cli_init(&bad_cli, NULL, sizeof(cli_buffer));

    TEST_ASSERT_EQUAL(CLI_E_NULL_PTR, cli_result);
}

void test_cli_init_should_failIfBufSizeIsZero(void)
{
    cli_t bad_cli;

    cli_status_t cli_result = CLI_MAX_STATUS;
    bad_cli.println = fake_printf;
    bad_cli.cmd_tbl = cmd_tbl;
    bad_cli.cmd_cnt = (sizeof(cmd_tbl) / sizeof(cmd_t));

    cli_result = cli_init(&bad_cli, cli_buffer, 0);

    TEST_ASSERT_EQUAL(CLI_E_INVALID_ARGS, cli_result);
}

void test_cli_process_should_returnNotFoundIfEmptyString(void)
{
    char cmd[] = {'\n'};

    for(int i = 0; i < sizeof(cmd); i++)
    {
        cli_put(&cli, cmd[i]);
    }

    cli_status_t result = cli_process(&cli);

    TEST_ASSERT_EQUAL_INT(CLI_E_CMD_NOT_FOUND, result);
}
#endif /* TEST */
