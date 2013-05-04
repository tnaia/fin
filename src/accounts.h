#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "summary.h"

void summary_of_accounts(FILE * fp, char * directory,
                         char * currency, int year,
                         char column_separator);

#endif
