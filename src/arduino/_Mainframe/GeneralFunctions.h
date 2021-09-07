int boolean_to_integer(bool bol[]) {
  int result = 0;
  for (int i = 0; i < 8; i++) {
    result += bol[i] * (1 << (7 - i));
  }
  return result;
}

byte boolarr_to_byte(bool bol[]) {
  int result = 0;
  for (int i = 0; i < 8; i++) {
    if (bol[i]) {
      result |= (1 << (7 - i));
    }
  }
  return result;
}

void appendCharArray(char* s, char c) {
  int len = strlen(s);

  s[len] = c;
  s[len + 1] = '\0';
}

void appendCsv(char* s) {
  int len = strlen(s);
  char end[5] = ".csv";
  for (int i = 0; i < 5; i++)
  {
    s[len + i] = end[i];
  }

  s[len + 5] = '\0';
}


unsigned long ToLong(bool b[])
{
  unsigned long c = 0;
  for (int i = 0; i < MODULE_COUNT; ++i) {
    if (b[i])
      c |= 1L << i;
  }
  return c;
}