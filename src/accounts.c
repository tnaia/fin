/*
    Fin - a simple personal accounts manager
    Copyright (C) 2012-2013  Bob Mottram <bob@sluggish.dyndns.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "accounts.h"

/* returns the account name from the given filename,
   which ends with .sqlite3 */
static void account_name_from_filename(char * filename,
									   char * account)
{
	sprintf(account, "%s", filename);
	account[strlen(filename)-8] = 0;
}

/* returns 1 if the given filename is an account database */
static int is_account_database(char * filename)
{
	if (strlen(filename) < 9) return 0;

	if ((filename[strlen(filename)-1]=='3') &&
		(filename[strlen(filename)-2]=='e') &&
		(filename[strlen(filename)-3]=='t') &&
		(filename[strlen(filename)-4]=='i') &&
		(filename[strlen(filename)-5]=='l') &&
		(filename[strlen(filename)-6]=='q') &&
		(filename[strlen(filename)-7]=='s') &&
		(filename[strlen(filename)-8]=='.')) {
		return 1;
	}
	return 0;
}

/* shows a summary of all accounts */
void summary_of_accounts(FILE * fp, char * directory,
						 char * currency, int year,
						 char column_separator)
{
    DIR *dir;
    struct dirent *ent;
	char filename[STRING_BLOCK];
	char account[STRING_BLOCK];
	char balance_str[STRING_BLOCK];
	double balance, total = 0;
	int i, max_account_name_length = 1;

	/* get the maximum length of the account name */
    if ((dir = opendir(directory)) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			sprintf(filename,"%s",ent->d_name);
			if (is_account_database(filename) != 0) {
				account_name_from_filename(filename,account);
				if (strlen(account) > max_account_name_length) {
					max_account_name_length = strlen(account);
				}
			}
		}
		closedir(dir);
	}

    if ((dir = opendir(directory)) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			sprintf(filename,"%s",ent->d_name);
			if (is_account_database(filename) != 0) {
				/* extract the account name from the filename */
				account_name_from_filename(filename,account);

				/* ignore any test account */
				if (strcmp(account,"test")==0) continue;

				/* get values for this account */
				balance = get_current_balance(account,currency,year);

				/* don't show accounts with zero balance */
				if (balance == 0) continue;

				/* update the overall total */
				total += balance;

				if (column_separator == '|') {
					fprintf(fp,"%c", column_separator);
				}
				fprintf(fp,"%s", account);
				for (i = strlen(account) + 1;
					 i < max_account_name_length+2; i++) {
					fprintf(fp, "%c", ' ');
				}
				fprintf(fp,"%c ", column_separator);
				sprintf(balance_str, "%.2f", balance);
				for (i = strlen(balance_str)-TRAILING_ZEROS-1;
					 i < LEADING_SPACES; i++) {
					fprintf(fp,"%c", ' ');
				}
				fprintf(fp,"%s", balance_str);
				if (column_separator == '|') {
					fprintf(fp, " %c", column_separator);
				}
				fprintf(fp,"%s","\n");
			}
		}
		closedir(dir);
	}
}
