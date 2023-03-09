#ifndef UTILS_H
#define UTILS_H

char *readLine();
int stringToInt(const char* string, int *j);
int getInt(int *i, int(*verify)(int));
void appendString(char **target, const char *string);
void dropStringBeforeChar(char **target, char symbol);
int pressEnterToContinue();
void clearOutput();
int isPositive(int i);

#endif //UTILS_H
