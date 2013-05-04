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
	char value_format_str[STRING_BLOCK];
	double balance, abstotal = 0, total = 0;
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

	/* org mode header */
	if (column_separator == '|') {
		fprintf(fp, "%s", "| ");
		sprintf((char*)balance_str, "%s",
				get_text_from_identifier(ACCOUNT));
		fprintf(fp, "%s", balance_str);

		for (i = strlen(balance_str);
			 i < max_account_name_length+1; i++) {
			fprintf(fp, "%c", ' ');
		}
		fprintf(fp, "%c", '|');
		sprintf((char*)balance_str, "%s",
				get_text_from_identifier(BALANCE));
		for (i = strlen(balance_str);
			 i < TRAILING_ZEROS+LEADING_SPACES+2; i++) {
			fprintf(fp, "%c", ' ');
		}
		fprintf(fp, "%s", balance_str);
		fprintf(fp, "%s", " |\n");

		fprintf(fp, "%c", '|');
		for (i = 0;
			 i < max_account_name_length+6+
				 TRAILING_ZEROS+LEADING_SPACES; i++) {
			if (i == max_account_name_length+2) {
				fprintf(fp, "%c", '+');
			}
			else {
				fprintf(fp, "%c", '-');
			}
		}
		fprintf(fp,"%s", "|\n");
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
				abstotal += fabs(balance);

#ifdef USE_COLOURS
				if (column_separator == ' ') {
					if (balance >= 0) {
						printf("%s",COLOUR_POSITIVE);
					}
					else {
						printf("%s",COLOUR_NEGATIVE);
					}
				}
#endif				
				if (column_separator == '|') {
					fprintf(fp,"%c ", column_separator);
				}
				fprintf(fp,"%s", account);
				for (i = strlen(account) + 1;
					 i < max_account_name_length+2; i++) {
					fprintf(fp, "%c", ' ');
				}
				fprintf(fp,"%c ", column_separator);
				sprintf(value_format_str,"%%.%df", TRAILING_ZEROS);
				sprintf(balance_str, value_format_str, balance);
				for (i = strlen(balance_str)-TRAILING_ZEROS-1;
					 i < LEADING_SPACES; i++) {
					fprintf(fp,"%c", ' ');
				}
				fprintf(fp,"%s", balance_str);
				if (column_separator == '|') {
					fprintf(fp, " %c", column_separator);
				}

#ifdef USE_COLOURS
				if (column_separator == ' ') {
					printf("%s",NORMAL);
				}
#endif
				fprintf(fp,"%s","\n");
			}
		}
		closedir(dir);
	}

	/* don't show a total if there isn't any */
	if (abstotal == 0) return;

	/* total line */
	if (column_separator == ' ') {
		for (i = 0;
			 i < max_account_name_length+4+
				 TRAILING_ZEROS+LEADING_SPACES; i++) {
			fprintf(fp, "%c", '-');
		}
		fprintf(fp,"%s", "\n");
	}

	/* org mode footer */
	if (column_separator == '|') {
		fprintf(fp, "%s", "|");
		for (i = 0;
			 i < max_account_name_length+6+
				 TRAILING_ZEROS+LEADING_SPACES; i++) {
			if (i == max_account_name_length+2) {
				fprintf(fp, "%c", '+');
			}
			else {
				fprintf(fp, "%c", '-');
			}
		}
		fprintf(fp,"%s", "|\n");
	}

#ifdef USE_COLOURS
	if (column_separator == ' ') {
		if (total >= 0) {
			printf("%s",COLOUR_POSITIVE);
		}
		else {
			printf("%s",COLOUR_NEGATIVE);
		}
	}
#endif				

	/* total */
	if (column_separator == '|') {
		fprintf(fp,"%c", column_separator);
	}
	for (i = 0;
		 i < max_account_name_length+2; i++) {
		fprintf(fp, "%c", ' ');
	}
	fprintf(fp,"%c ", column_separator);
	sprintf(balance_str, value_format_str, total);
	for (i = strlen(balance_str)-TRAILING_ZEROS-1;
		 i < LEADING_SPACES; i++) {
		fprintf(fp,"%c", ' ');
	}
	fprintf(fp,"%s", balance_str);
	if (column_separator == '|') {
		fprintf(fp, " %c", column_separator);
	}
#ifdef USE_COLOURS
	if (column_separator == ' ') {
		printf("%s",NORMAL);
	}
#endif
	fprintf(fp,"%s","\n");
}
