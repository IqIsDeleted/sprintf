#ifndef STRING
#define STRING

#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 512

#define S21_NULL ((void *)0)
#define CAP_LET ('A' - 'a')
#define SMALL_LET 0
#define HEX 16
#define DEC 10
#define OCT 8
#define PFLAGS param->flags
#define MAX_INT -1 ^ (1 << 31)
#define PTR_WIDTH 14
#define SHORT_INT 4
#define INT_INT 8
#define DBL_DBL 8
#define LONG_INT 16
#define LONG_DBL 16
#define START_FUNC 1

#define put_zero_or_space(digits, bool) \
  __builtin_put_zero_or_space(buff, res, param, sign, digits, bool)
#define rl_len __builtin_dig_or_prec(digits, param->prec)

typedef unsigned long int my_size_t;
typedef unsigned short int my_usint_t;
typedef char *my_string_t;

typedef struct flags_bit_field {
  my_usint_t zero : 1;
  my_usint_t plus : 1;
  my_usint_t minus : 1;
  my_usint_t space : 1;
  my_usint_t hash : 2;
} flags;

typedef struct string_parameters {
  my_usint_t amount;
  flags flags;
  short int width;
  my_usint_t prec;
  my_usint_t lenght : 5;
  bool prec_flag : 1;
} param;

int my_sprintf(char *, const char *, ...);
param parse_param(const char *, va_list *);
int process_parse(const char *, param *);
bool intger_fun(const char *, my_usint_t);
void distibutiont_of_arguments_for_float(char *, int *, param *, va_list *);
void distibutiont_of_arguments_for_exp(char *buff, int *res, param *paramt,
                                       va_list *args, int mode,
                                       void (*fun)(char *, int *, param *,
                                                   long double, const int));
bool flag_enter(param *);
void put_int(char *, int *, param *, long int n, bool);
bool __builtin_dig_or_prec(int digits, my_usint_t prec);
long int long_abs(long int n);
void put_str(char *, int *, param *, const char *, bool);
void put_char(char *, int *, param *, const char c);
int lenght_check_dbl(long double *, param *);
void put_float(char *, int *, param *, long double n);
void put_uns_int(char *, int *, param *, unsigned long n);
void put_switch_base(char *, int *, param *, long, const int mode,
                     const unsigned long base);
void convert_number_to_base(int *arr, int *i, long num, unsigned long base);
int dig_for_ptr(unsigned long n);
void put_ptr(char *, int *, param *, void *);
bool almost_equal(long double a, long double b, double);
int put_exponent(char *, int *, param *, long double, const int mode);
void fill_str(char *, int *, char, my_size_t);
void put_sign(char *, int *, param *, long *, bool);
void put_sign_float(char *, int *, param *, long double *, bool);
void __builtin_put_zero_or_space(char *, int *, param *, bool, int, bool);
int calculate_digits(long, const unsigned int base);
void distribution_of_hash_and_space(char *buff, int *res, param *param,
                                    int digits, const int mode,
                                    const unsigned int base, long num);
void get_str_for_exponent(char *, int, param *, long double n, const int, int);
bool processing_of_greater(char *buff, int *res, bool);
void put_greater(char *, int *, param *, long double n, const int m);
my_size_t my_strlen(const char *str);

#if defined(__APPLE__) || defined(__MACH__)
#define FORM_NULL "0"
#define S21_ERRLIST_SIZE 107
#elif defined(__linux__)
#define S21_ERRLIST_SIZE 134
#define FORM_NULL "nil"
#endif

#endif