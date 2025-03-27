#include "my_string.h"

int my_sprintf(char *str, const char *format, ...) {
  int res = 0;
  int i = 0;
  va_list args;
  va_start(args, format);
  for (; format[i]; ++i) {
    if (format[i] != '%') {
      str[res++] = format[i];
    } else {
      param param = parse_param(format + i, &args);
      i += param.amount;
      if (format[i] == 'd' || format[i] == 'i') {
        put_int(str, &res, &param, va_arg(args, long), START_FUNC);
      } else if (format[i] == 's') {
        put_str(str, &res, &param, va_arg(args, const char *), START_FUNC);
      } else if (format[i] == 'c') {
        put_char(str, &res, &param, va_arg(args, int));
      } else if (format[i] == 'n') {
        *(va_arg(args, int *)) = res;
      } else if (format[i] == '%') {
        put_char(str, &res, &param, '%');
      } else if (format[i] == 'f') {
        distibutiont_of_arguments_for_float(str, &res, &param, &args);
      } else if (format[i] == 'x') {
        put_switch_base(str, &res, &param, va_arg(args, long), SMALL_LET, HEX);
      } else if (format[i] == 'X') {
        put_switch_base(str, &res, &param, va_arg(args, long), CAP_LET, HEX);
      } else if (format[i] == 'u') {
        put_uns_int(str, &res, &param, va_arg(args, unsigned long));
      } else if (format[i] == 'o') {
        put_switch_base(str, &res, &param, va_arg(args, long), SMALL_LET, OCT);
      } else if (format[i] == 'p') {
        put_ptr(str, &res, &param, va_arg(args, long int *));
      } else if (format[i] == 'e') {
        distibutiont_of_arguments_for_exp(str, &res, &param, &args, SMALL_LET,
                                          (void *)put_exponent);
      } else if (format[i] == 'E') {
        distibutiont_of_arguments_for_exp(str, &res, &param, &args, CAP_LET,
                                          (void *)put_exponent);
      } else if (format[i] == 'g') {
        distibutiont_of_arguments_for_exp(str, &res, &param, &args, SMALL_LET,
                                          put_greater);
      } else if (format[i] == 'G') {
        distibutiont_of_arguments_for_exp(str, &res, &param, &args, CAP_LET,
                                          put_greater);
      }
    }
  }
  va_end(args);
  str[res] = '\0';
  return res;
}

void distibutiont_of_arguments_for_float(char *buff, int *res, param *param,
                                         va_list *args) {
  if (param->lenght == LONG_DBL) {
    put_float(buff, res, param, va_arg(*args, long double));
  } else {
    put_float(buff, res, param, (long double)va_arg(*args, double));
  }
}

void distibutiont_of_arguments_for_exp(char *buff, int *res, param *paramt,
                                       va_list *args, int mode,
                                       void (*fun)(char *, int *, param *,
                                                   long double, const int)) {
  if (paramt->lenght == LONG_DBL) {
    fun(buff, res, paramt, va_arg(*args, long double), mode);
  } else {
    fun(buff, res, paramt, (long double)va_arg(*args, double), mode);
  }
}

void put_char(char *buff, int *res, param *param, const char c) {
  if (PFLAGS.minus) {
    buff[(*res)++] = c;
    __builtin_put_zero_or_space(buff, res, param, false, !PFLAGS.space, true);
  } else {
    __builtin_put_zero_or_space(buff, res, param, false, !PFLAGS.space, true);
    buff[(*res)++] = c;
  }
}

void put_str(char *buff, int *res, param *param, const char *str,
             bool fun_flag) {
  my_size_t len = 0;
  if (!fun_flag) {
    len = (my_strlen(str));
  } else {
    len = (my_strlen(str));
    if (len > param->prec) {
      len = param->prec;
    }
  }
  if (PFLAGS.minus) {
    for (my_size_t i = 0; str[i] && (i < len); ++i) {
      buff[(*res)++] = str[i];
    }
    __builtin_put_zero_or_space(buff, res, param, false, (int)len, true);
  } else {
    __builtin_put_zero_or_space(buff, res, param, false, (int)len, true);
    for (my_size_t i = 0; str[i] && (i < len); ++i) {
      buff[(*res)++] = str[i];
    }
  }
}

bool flag_enter(param *param) {
  return (PFLAGS.hash || PFLAGS.minus || PFLAGS.plus || PFLAGS.space ||
          PFLAGS.zero || !param->prec_flag);
}

int dig_for_ptr(unsigned long num) {
  int i = 0;
  do {
    num /= HEX;
    ++i;
  } while (num != 0);
  return i;
}

void put_ptr(char *buff, int *res, param *param, void *ptr) {
  const char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                      '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  unsigned long num = (uintptr_t)ptr;
  int arr[128] = {0};
  int i = 0;
  PFLAGS.hash = 1;
  int digits = dig_for_ptr(num) - PFLAGS.space + 2;
  distribution_of_hash_and_space(buff, res, param, digits, SMALL_LET, HEX,
                                 !PFLAGS.minus);
  do {
    arr[i++] = num % HEX;
    num /= HEX;
  } while (num != 0);
  for (--i; i >= 0; --i) {
    buff[(*res)++] = hex[arr[i]];
  }
  __builtin_put_zero_or_space(buff, res, param, false, digits, PFLAGS.minus);
}

param parse_param(const char *format, va_list *args) {
  param param = {0};
  param.prec = 6;
  while (true) {
    param.amount++;
    if (*(format + param.amount) == '-') {
      param.flags.minus = true;
    } else if (*(format + param.amount) == '+') {
      param.flags.plus = true;
    } else if (*(format + param.amount) == ' ') {
      param.flags.space = true;
    } else if (*(format + param.amount) == '#') {
      param.flags.hash = true;
    } else if (*(format + param.amount) == '0') {
      param.flags.zero = true;
    } else if (*(format + param.amount) == '.') {
      param.amount++;
      param.prec_flag = true;
      if (*(format + param.amount) == '*') {
        param.prec = va_arg(*args, int);
      } else {
        param.prec = process_parse(format, &param);
      }
    } else if (*(format + param.amount) == '*') {
      param.width = va_arg(*args, int);
      if (param.width < 0) {
        param.flags.minus = true;
        param.width *= -1;
      }
    } else if (*(format + param.amount) <= '9' &&
               *(format + param.amount) > '0') {
      param.width = process_parse(format, &param);
    } else if (*(format + param.amount) == 'l') {
      param.lenght = LONG_INT;
    } else if (*(format + param.amount) == 'h') {
      param.lenght = SHORT_INT;
    } else if (*(format + param.amount) == 'L') {
      param.lenght = LONG_DBL;
    } else {
      intger_fun(format, param.amount) &&!param.prec_flag ? param.prec = 0 : 0;
      *(format + param.amount) == 's' && !param.prec_flag ? param.prec = 1 << 15
                                                          : 0;
      return param;
    }
  }
}
int process_parse(const char *format, param *param) {
  int res = 0;
  while (*(format + param->amount) <= '9' && *(format + param->amount) >= '0') {
    res *= 10;
    res += *(format + param->amount) - '0';
    param->amount++;
  }
  param->amount--;
  return res;
}

void fill_str(char *buff, int *res, char c, my_size_t len) {
  for (my_size_t i = 0; i < len; ++i) {
    buff[(*res)++] = c;
  }
}

void __builtin_put_zero_or_space(char *buff, int *res, param *param, bool sign,
                                 int digits, bool minus_flag) {
  char mode = PFLAGS.zero & ~PFLAGS.minus ? '0' : ' ';
  if (minus_flag && param->width > (short)(digits + sign)) {
    param->width -= digits + (sign | PFLAGS.space);
    fill_str(buff, res, mode, param->width);
  }
}

bool intger_fun(const char *format, my_usint_t amount) {
  return (*(format + amount) == 'd' || *(format + amount) == 'i' ||
          *(format + amount) == 'u' || *(format + amount) == 'o' ||
          *(format + amount) == 'x' || *(format + amount) == 'X');
}

void put_greater(char *buff, int *res, param *param, long double num,
                 const int mode) {
  lenght_check_dbl(&num, param);
  char *arr = (char *)calloc(256, sizeof(char));
  char *arr1 = (char *)calloc(256, sizeof(char));
  int res_cpy = 0;
  int res_cpy1 = 0;
  bool float_flag = false;
  int digits_int = calculate_digits((long)num, DEC);
  if ((int)param->prec < digits_int) {
    param->prec = 0;
  } else {
    param->prec -= digits_int;
  }
  my_usint_t hash_cpy = PFLAGS.hash;
  put_float(arr, &res_cpy, param, num);
  PFLAGS.hash = hash_cpy;
  put_exponent(arr1, &res_cpy1, param, num, mode);
  if (PFLAGS.hash == 0) {
    float_flag = processing_of_greater(arr, &res_cpy, false);
    processing_of_greater(arr1, &res_cpy1, true);
  }
  if ((res_cpy <= res_cpy1 && (float_flag || num == 0.l)) ||
      (param->prec == 0 && num == 0.l)) {
    put_str(buff, res, param, arr, 0);
  } else {
    put_str(buff, res, param, arr1, 0);
  }
  free(arr);
  free(arr1);
}

bool processing_of_greater(char *buff, int *res, bool func_flag) {
  bool result = 0;
  int i = *res - 1;
  int i_for_exp = 0;
  int point_flag = false;
  if (func_flag) {
    for (; (buff[i] != 'e' && buff[i] != 'E') && i >= 0; --i);
    i_for_exp = i;
    i--;
  }
  bool cycle_flag = true;
  for (; cycle_flag && i >= 1; --i) {
    if (buff[i] > '0' && buff[i] <= '9') {
      result = true;
      cycle_flag = false;
    } else if (buff[i] == '0' || buff[i] == '.') {
      buff[i] == '.' ? point_flag = true : 0;
      buff[i] = '\0';
    } else if (buff[i] == '-' || buff[i] == '+' || buff[i] == ' ') {
    } else {
      cycle_flag = false;
    }
  }
  if (func_flag) {
    for (; buff[i_for_exp] != '\0'; ++i_for_exp) {
      buff[++i] = buff[i_for_exp];
    }
    buff[++i] = '\0';
  }
  *res = i - point_flag;
  return result;
}

bool almost_equal(long double a, long double b, double epsilon) {
  return fabsl(a - b) < (long double)epsilon;
}

int lenght_check_int(long *num, param *param) {  // ret???
  int res = LONG_INT;
  if (param->lenght == SHORT_INT) {
    *num = (short)*num;
    res = SHORT_INT;
  } else if (param->lenght == 0) {
    *num = (int)*num;
    res = INT_INT;
  }
  return res;
}

void convert_number_to_base(int *arr, int *i, long num, unsigned long base) {
  while (num != 0) {
    arr[(*i)++] = num % base;
    num /= base;
  }
}

void put_switch_base(char *buff, int *res, param *param, long num,
                     const int mode, const unsigned long base) {
  int type = lenght_check_int(&num, param);
  const char hex[] = {'0',        '1',        '2',        '3',
                      '4',        '5',        '6',        '7',
                      '8',        '9',        'a' + mode, 'b' + mode,
                      'c' + mode, 'd' + mode, 'e' + mode, 'f' + mode};
  int converted_digits[64] = {0};
  for (int i = 0; i < 64; i++) {
    converted_digits[i] = -1;
  }
  int i = 0;
  bool sign = false;
  num == 0 ? (PFLAGS.hash = 0) : 0;
  PFLAGS.hash = ((num == 0)                     ? 0
                 : (base == HEX && PFLAGS.hash) ? 2
                                                : PFLAGS.hash);
  int digits = calculate_digits(num, base) - PFLAGS.space + (PFLAGS.hash);
  convert_number_to_base(converted_digits, &i, num, base);
  base == OCT ? (converted_digits[10] -= (type == LONG_INT ? 0 : 4)) : 0;
  if (num < 0) {
    i = type + (base == OCT ? (type == LONG_INT ? 6 : 3) : 0);
    digits = i - PFLAGS.space + PFLAGS.hash;
  }
  if (!(param->prec == 0 && num == 0 && !flag_enter(param))) {
    distribution_of_hash_and_space(
        buff, res, param, rl_len ? param->prec : digits, mode, base, num);

    if (rl_len) {
      fill_str(buff, res, '0', param->prec - digits);
    }
    for (--i; i >= 0; --i) {
      buff[(*res)++] = hex[(16 + converted_digits[i]) % 16];
    }
    put_zero_or_space(digits, PFLAGS.minus);
  }
}

bool __builtin_dig_or_prec(int digits, my_usint_t prec) {
  return (prec > digits);
}

void distribution_of_hash_and_space(char *buff, int *res, param *param,
                                    int digits, const int mode,
                                    const unsigned int base, long num) {
  bool sign = 0;
  if (num == 0) {
    put_zero_or_space(digits, !PFLAGS.minus);
    buff[(*res)++] = '0';
  } else if (PFLAGS.hash) {
    !PFLAGS.zero ? put_zero_or_space(digits, !PFLAGS.minus) : 0;
    buff[(*res)++] = '0';
    base == HEX ? (buff[(*res)++] = 'x' + mode) : 0;
    PFLAGS.zero ? put_zero_or_space(digits, !PFLAGS.minus) : 0;
  } else {
    put_zero_or_space(digits, !PFLAGS.minus);
  }
  param->prec += PFLAGS.hash;
}

int calculate_digits(long num, const unsigned int base) {
  int digits = 0;
  do {
    num /= (long)base;
    digits++;
  } while (num != 0);
  return digits;
}

void put_uns_int(char *buff, int *res, param *param, unsigned long num) {
  if (param->lenght == SHORT_INT) {
    num = (unsigned short)num;
  } else if (param->lenght == 0) {
    num = (unsigned)num;
  }
  unsigned long tmp = num;
  int digits = 0;
  bool sign = false;
  do {
    tmp /= DEC;
    digits++;
  } while (tmp > (unsigned)0);
  if (rl_len) {
    fill_str(buff, res, '0', param->prec - digits);
  }
  put_zero_or_space(digits - PFLAGS.space, !PFLAGS.minus);
  if (param->prec == 0 && num == 0 && !flag_enter(param)) {
    digits = 0;
  }
  for (int i = digits - 1; i >= 0; i--) {
    buff[(*res) + i] = (num % 10) + '0';
    num /= 10;
  }
  *res += digits;
  put_zero_or_space(digits - PFLAGS.space, PFLAGS.minus);
}

long int long_abs(long int num) { return (num < 0 ? -num : num); }

void put_sign(char *buff, int *res, param *param, long *num, bool sign) {
  if (sign) {
    buff[(*res)++] = (*num < 0l) ? '-' : '+';
    *num = long_abs(*num);
  } else if (PFLAGS.space) {
    buff[(*res)++] = ' ';
  }
}

void put_sign_float(char *buff, int *res, param *param, long double *num,
                    bool sign) {
  if (sign) {
    buff[(*res)++] = (*num < 0) ? '-' : '+';
    *num = fabsl(*num);
  } else if (PFLAGS.space) {
    buff[(*res)++] = ' ';
  }
  PFLAGS.space = 0;
  PFLAGS.plus = 0;
}

void put_int(char *buff, int *res, param *param, long int num, bool prec) {
  lenght_check_int(&num, param);
  bool sign = num < 0 || PFLAGS.plus;
  int digits = calculate_digits(num, DEC);
  digits += rl_len && prec ? param->prec - digits : 0;
  if (param->prec == 0 && num == 0 && prec && !flag_enter(param)) {
    digits = 0;
  }
  if (PFLAGS.zero) {
    put_sign(buff, res, param, &num, sign);
    put_zero_or_space(digits, !PFLAGS.minus);
    if (rl_len && prec) {
      fill_str(buff, res, '0', param->prec - digits);
    }
  } else {
    put_zero_or_space(digits, !PFLAGS.minus);
    put_sign(buff, res, param, &num, sign);
    if (rl_len && prec) {
      fill_str(buff, res, '0', param->prec - digits);
    }
  }
  for (int i = digits - 1; i >= 0; --i) {
    buff[*res + i] = (num % 10) + '0';
    num /= 10;
  }

  *res += digits;
  put_zero_or_space(digits, PFLAGS.minus);
}

void processing_of_fraction(char *buff, int *res, param *param, long double num,
                            my_size_t res_cpy) {
  PFLAGS.hash ? buff[(*res)++] = '.' : 0;
  for (my_size_t i = 0; i < param->prec; ++i) {
    num *= 10;
    buff[(*res)++] = (int)num + '0';
    num -= (int)num;
  }
  num *= 10;
  if ((num > 5) || (((int)num / 10) % 2 == 1)) {
    (buff[*res - 1] == '.') ? (buff[*res - 2] += 1) : (buff[*res - 1] += 1);
    for (my_size_t i = *res - 1; i > res_cpy; --i) {
      if (buff[i] > '9') {
        buff[i] = '0';
        buff[i - 1] == '.' ? i-- : 0;
        buff[i - 1] += 1;
      }
    }
  }
  if (buff[res_cpy] > '9') {
    for (my_size_t i = *res; i > res_cpy; --i) {
      buff[i] = buff[i - 1];
    }
    buff[res_cpy] = '1';
    *res += 1;
  }
  for (my_size_t i = *res - 1; i > res_cpy; --i) {
    if (buff[i] == ':') {
      buff[i] = '0';
    }
  }
}

my_usint_t width_for_float(param *param) {
  short res = param->width;
  if (PFLAGS.minus) {
    param->width = 0;
  }
  return res;
}

int lenght_check_dbl(long double *num, param *param) {
  if (param->lenght == LONG_DBL) {
    return LONG_DBL;
  }
  *num = (double)*num;
  return DBL_DBL;
}

void put_float(char *buff, int *res, param *param, long double num) {
  lenght_check_dbl(&num, param);
  bool sign = num < 0 || PFLAGS.plus;
  short width_cpy = 0;
  if (param->width > (param->prec + 1)) {
    param->width -= (short)(param->prec + 1);
    width_cpy = width_for_float(param) - (sign | PFLAGS.space);
  } else {
    param->width = 0;
  }
  my_size_t res_cpy = (my_size_t)*res;
  PFLAGS.hash = PFLAGS.hash || (!param->prec == 0);
  param->width += (short)((param->prec == 0) && !PFLAGS.hash);
  put_int(buff, res, param, (long)num, 0);
  num = fabsl(num);
  num -= (long)num;
  PFLAGS.plus = 0;
  processing_of_fraction(buff, res, param, num, res_cpy);
  width_cpy -= width_cpy > 0;
  if (PFLAGS.minus) {
    param->width = width_cpy;
    put_zero_or_space(0, true);
  }
}

int put_exponent(char *buff, int *res, param *param, long double num,
                 const int mode) {
  lenght_check_dbl(&num, param);
  int rank = 0;
  const double epsilon = 1e-9;
  bool sign = (num < (long double)0) | PFLAGS.plus;
  int res_cpy = 0;
  char *arr = (char *)calloc(256, sizeof(char));
  put_sign_float(arr, &res_cpy, param, &num, sign);
  num *= num < (long double)0 ? -1 : 1;
  while (num >= 10.0 - epsilon || num <= -10.0 + epsilon) {
    num /= 10;
    rank++;
  }
  while (num < 1 && num != (double)0) {
    num *= 10;
    rank--;
  }
  if (almost_equal(num, 10.0, epsilon)) {
    num = 1.0;
    rank++;
  } else if (almost_equal(num, 1.0, epsilon)) {
    num = 1.0;
  }
  get_str_for_exponent(arr, res_cpy, param, num, mode, rank);
  put_str(buff, res, param, arr, 0);
  free(arr);
  return rank;
}

void get_str_for_exponent(char *arr, int res, param *orig, long double num,
                          const int mode, int rank) {
  param new = {0};
  new.prec = orig->prec;
  new.flags.hash = orig->flags.hash;
  put_float(arr, &res, &new, num);
  arr[(res)++] = 'e' + mode;
  arr[(res)++] = (rank < 0) ? '-' : '+';
  abs(rank) < 10 ? arr[(res)++] = '0' : 0;
  put_int(arr, &res, &new, abs(rank), 0);
}

my_size_t my_strlen(const char *str) {
  my_size_t count = 0;
  for (; str[count] != '\0'; count++) {
    count++;
  }
  return count;
}