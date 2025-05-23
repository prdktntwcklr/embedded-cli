#ifdef TEST

#include "unity.h"

#include "cli.h"

#include "check_assert.h"
#include "fake_printf.h"

#include <stdarg.h>
#include <string.h>

#define BUFFER_SIZE (256)

static cli_status_t help_func(int argc, char **argv);
static cli_status_t args_func(int argc, char **argv);
static uint8_t cli_buffer[BUFFER_SIZE] = {0};
static cli_t cli;

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

void initialize_cli(void)
{
    cli_status_t cli_result = CLI_MAX_STATUS;
    cli.println = fake_printf;
    cli.cmd_tbl = cmd_tbl;
    cli.cmd_cnt = (sizeof(cmd_tbl) / sizeof(cmd_t));

    /* clear buffer */
    memset(cli_buffer, 0, BUFFER_SIZE);

    cli_result = cli_init(&cli, cli_buffer, sizeof(cli_buffer));

    if(cli_result != CLI_OK)
    {
        TEST_FAIL_MESSAGE("CLI failed to initialize.");
    }

    TEST_ASSERT_EQUAL_STRING("cli_init() ok.\n",
                             fake_printf_get_last_message());
}

void setUp(void)
{
    initialize_cli();

    // clear "cli_init() ok" from fake printf
    fake_printf_clear();
}

void tearDown(void)
{
    fake_printf_delete_file();
}

/**
 * @brief Helper function to send a cmd of a certain length to the CLI.
 */
void send_cmd(char *cmd, int length)
{
    for(int i = 0; i < length; i++)
    {
        cli_put(&cli, cmd[i]);
    }
}

void test_cli_put_should_addCharacterToBuffer(void)
{
    char cmd[] = {'h', 'e', 'l', 'p'};

    /* check if buffer is empty originally */
    TEST_ASSERT_EQUAL(0, cli_buffer[0]);
    TEST_ASSERT_EQUAL(0, cli_buffer[1]);
    TEST_ASSERT_EQUAL(0, cli_buffer[2]);
    TEST_ASSERT_EQUAL(0, cli_buffer[3]);
    TEST_ASSERT_EQUAL(0, cli_buffer[4]);

    send_cmd(cmd, sizeof(cmd));

    TEST_ASSERT_EQUAL_CHAR('h', cli_buffer[0]);
    TEST_ASSERT_EQUAL_CHAR('e', cli_buffer[1]);
    TEST_ASSERT_EQUAL_CHAR('l', cli_buffer[2]);
    TEST_ASSERT_EQUAL_CHAR('p', cli_buffer[3]);
    TEST_ASSERT_EQUAL(0, cli_buffer[4]);
}

void test_cli_put_should_returnFullIfBufferIsFull(void)
{
    for(int i = 0; i < BUFFER_SIZE; i++)
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

    send_cmd(cmd, sizeof(cmd));

    cli_status_t result = cli_process(&cli);

    TEST_ASSERT_EQUAL_INT(CLI_E_CMD_NOT_READY, result);
}

void test_cli_process_should_returnOkIfTerminatorFound(void)
{
    char cmd[] = {'h', 'e', 'l', 'p', '\n'};

    send_cmd(cmd, sizeof(cmd));

    cli_status_t result = cli_process(&cli);

    TEST_ASSERT_EQUAL_INT(CLI_OK, result);
}

void test_cli_process_should_returnNotFoundIfUnknownCommand(void)
{
    char cmd[] = {'b', 'e', 'e', 'f', '\n'};

    send_cmd(cmd, sizeof(cmd));

    cli_status_t result = cli_process(&cli);

    TEST_ASSERT_EQUAL_INT(CLI_E_CMD_NOT_FOUND, result);
}

void test_cli_process_should_callHelpIfHelpCmdReceived(void)
{
    char cmd[] = {'h', 'e', 'l', 'p', '\n'};

    send_cmd(cmd, sizeof(cmd));

    cli_status_t result = cli_process(&cli);

    TEST_ASSERT_EQUAL_INT(CLI_OK, result);
    TEST_ASSERT_EQUAL_STRING("cli help func called\n",
                             fake_printf_get_last_message());
}

void test_cli_process_should_callArgsWithArgsIfArgsCmdReceived(void)
{
    char cmd[] = {'a', 'r', 'g', 's', ' ', 'a', 'r', 'g',
                  '1', ' ', 'p', 'a', 'r', '2', '\n'};

    send_cmd(cmd, sizeof(cmd));

    cli_status_t result = cli_process(&cli);

    TEST_ASSERT_EQUAL_INT(CLI_OK, result);
    TEST_ASSERT_EQUAL_STRING("cli args func called with 3 args: arg1 par2\n",
                             fake_printf_get_last_message());
}

void test_cli_init_should_failIfNullPointerPassedForCli(void)
{
    cli_t bad_cli;

    cli_status_t cli_result = CLI_MAX_STATUS;
    bad_cli.println = fake_printf;
    bad_cli.cmd_tbl = cmd_tbl;
    bad_cli.cmd_cnt = (sizeof(cmd_tbl) / sizeof(cmd_t));

    SHOULD_FAIL_ASSERT(cli_init(NULL, cli_buffer, sizeof(cli_buffer)));
}

void test_cli_init_should_failIfNullPointerPassedForBufferPtr(void)
{
    cli_t bad_cli;

    cli_status_t cli_result = CLI_MAX_STATUS;
    bad_cli.println = fake_printf;
    bad_cli.cmd_tbl = cmd_tbl;
    bad_cli.cmd_cnt = (sizeof(cmd_tbl) / sizeof(cmd_t));

    SHOULD_FAIL_ASSERT(cli_init(&bad_cli, NULL, sizeof(cli_buffer)));
}

void test_cli_init_should_failIfBufSizeIsZero(void)
{
    cli_t bad_cli;

    cli_status_t cli_result = CLI_MAX_STATUS;
    bad_cli.println = fake_printf;
    bad_cli.cmd_tbl = cmd_tbl;
    bad_cli.cmd_cnt = (sizeof(cmd_tbl) / sizeof(cmd_t));

    SHOULD_FAIL_ASSERT(cli_init(&bad_cli, cli_buffer, 0));
}

void test_cli_process_should_returnNotFoundIfEmptyString(void)
{
    char cmd[] = {'\n'};

    send_cmd(cmd, sizeof(cmd));

    cli_status_t result = cli_process(&cli);

    TEST_ASSERT_EQUAL_INT(CLI_E_CMD_NOT_FOUND, result);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_cli_put_should_addCharacterToBuffer);
    RUN_TEST(test_cli_put_should_returnFullIfBufferIsFull);
    RUN_TEST(test_cli_process_should_returnNotReadyIfNoTerminatorFound);
    RUN_TEST(test_cli_process_should_returnOkIfTerminatorFound);
    RUN_TEST(test_cli_process_should_returnNotFoundIfUnknownCommand);
    RUN_TEST(test_cli_process_should_callHelpIfHelpCmdReceived);
    RUN_TEST(test_cli_process_should_callArgsWithArgsIfArgsCmdReceived);
    RUN_TEST(test_cli_init_should_failIfNullPointerPassedForCli);
    RUN_TEST(test_cli_init_should_failIfNullPointerPassedForBufferPtr);
    RUN_TEST(test_cli_init_should_failIfBufSizeIsZero);
    RUN_TEST(test_cli_process_should_returnNotFoundIfEmptyString);
    return UNITY_END();
}
#endif /* TEST */
