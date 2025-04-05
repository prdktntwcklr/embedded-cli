#ifndef FAKE_PRINTF_H
#define FAKE_PRINTF_H

int fake_printf(char *format, ...);
char *fake_printf_get_last_message(void);
void fake_printf_reset(void);
void fake_printf_delete_file(void);

#endif /* FAKE_PRINTF_H */
