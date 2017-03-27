/*
  Fin - a simple personal accounts manager
  Copyright (C) 2012-2015  Bob Mottram <bob@robotics.uk.to>

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

#include "yearly.h"

int show_yearly_summary(char * account, int max_months,
                        char * export_filename, char separator,
                        char * currency, char column_separator)
{
    FILE * fp=0;
    int retval,col,cols,row,i;
    char database_filename[STRING_BLOCK];
    char receive_str[STRING_BLOCK],str[STRING_BLOCK];
    char query[STRING_BLOCK_LONG];
    double total_receive=0,total_spend=0;
    char spend_str[STRING_BLOCK];
    char balance_str[STRING_BLOCK];
    char curr_balance_str[STRING_BLOCK];
    const int line_length = 65;
    double bal=0,curr_balance;
    double current_balance = get_current_balance(account,currency,0);
    double prev_receive=0,prev_spend=0;
    double receive_change=0, spend_change=0;
    sqlite3_stmt *stmt;
    sqlite3 *handle;

    sprintf(query,
            "select substr(date,1,4) as year, sum(receive) " \
            "as monthreceive, sum(spend) as monthspend, " \
            "sum(receive-spend) as monthbalance from " \
            "transactions where (currency is \"%s\") " \
            "group by year order by year desc;",currency);

    if (export_filename != 0) {
        if (strlen(export_filename) > 0) {
            fp = fopen(export_filename,"w");
            if (fp != 0) {
                sprintf((char*)str,"%s",
                        get_text_from_identifier(EXPORT_YEARLY_HEADER));
                for (i = 0; i < strlen(str); i++) {
                    if (str[i] == ',')
                        str[i] = separator;
                }
                fprintf(fp,"%s\n",str);
            }
        }
    }

    /* if no account is specified use the default one */
    if (strlen(account) == 0) {
        settings_set_account(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT));
        if (strlen(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT)) == 0) {
            printf("Unable to get default account name\n");
            return 0;
        }
        account = settings_get_account();
    }

    if (strlen(account) == 0) {
        printf("No account specified\n");
        return 0;
    }
    database_name(account, (char*)database_filename);
    retval = sqlite3_open_v2((char*)database_filename,
                             &handle,
                             SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
                             NULL);
    /* If connection failed, handle returns NULL */
    if (retval) {
        printf("Sqlite3 database connection failed\n");
        sqlite3_close(handle);
        return 0;
    }

    retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
    if (retval) {
        printf("Selecting recent transactions " \
               "from %s account failed\n",account);
        sqlite3_close(handle);
        return 0;
    }

    cols = sqlite3_column_count(stmt);

    if (fp == 0) {
        /* title */
        sprintf((char*)str,
                get_text_from_identifier(TITLE_YEARLY_SUMMARY),
                account);
        str[0] = toupper(str[0]);
        if (column_separator == ' ')
            printf("%s\n",str);
        else
            printf("* %s\n",str);

        if (column_separator == ' ')
            for (i = 0;i < line_length; i++)
                printf("-");

        printf("\n");

        if (column_separator != ' ')
            printf("%c",column_separator);

        printf(get_text_from_identifier(HEADER_YEARLY_SUMMARY),
               column_separator,column_separator,column_separator,
               column_separator,column_separator,column_separator,
               column_separator);
        printf("\n");

        if (column_separator != ' ')
            printf("%c",column_separator);

        for (i = 0; i < line_length; i++) {
            if ((column_separator == ' ') ||
                ((i != 5) && (i != 15) && (i != 25) &&
                 (i != 35) && (i != 45) && (i != 55)))
                printf("-");
            else
                printf("+");
        }
        printf("%c\n",column_separator);
    }

    curr_balance = current_balance;

    row = 0;
    while (1) {
        retval = sqlite3_step(stmt);

        if (retval == SQLITE_ROW) {

            if (fp == 0) {
                if (column_separator != ' ')
                    printf("%c",column_separator);

#ifdef USE_COLOURS
                if (column_separator == ' ') {
                    if (atof((char*)sqlite3_column_text(stmt,3))>=0)
                        printf("%s",
                               settings_get_theme_colour(COLOUR_POSITIVE));
                    else
                        printf("%s",
                               settings_get_theme_colour(COLOUR_NEGATIVE));
                }
#endif

                for (col = 0; col < cols; col++) {
                    char * val = (char*)sqlite3_column_text(stmt,col);

                    if (col == 1) {
                        if (is_value(val) != 0) {

                            /* percent change in received value */
                            if (prev_receive != 0) {
                                receive_change =
                                    (atof(val) - prev_receive)*
                                    100/prev_receive;
                            }
                            prev_receive = atof(val);

                            total_receive += atof(val);
                        }
                        else break;
                    }
                    if (col == 2) {
                        if (is_value(val) != 0) {
                            /* percent change in spend value */
                            if (prev_spend != 0) {
                                spend_change =
                                    (atof(val) -
                                     prev_spend)*100/prev_spend;
                            }
                            prev_spend = atof(val);

                            total_spend += atof(val);
                        }
                    }

                    if (col > 0) {
                        pad_value(val, (char *)receive_str,
                                  LEADING_SPACES, TRAILING_ZEROS);
                        printf("%s %c",receive_str,column_separator);
                    }
                    else {
                        if (val != 0)
                            printf("%s %c",val,column_separator);
                        else
                            break;
                    }

                    if ((col == 3) && (is_value(val) != 0))
                        bal = atof(val);
                }
                if (col == cols) {

                    sprintf((char*)str,"%.2f",curr_balance);
                    pad_value(str, (char *)receive_str,
                              LEADING_SPACES, TRAILING_ZEROS);
                    printf("%s %c",receive_str,column_separator);

                    sprintf((char*)str,"%.2f",receive_change);
                    pad_value(str, (char *)receive_str,
                              LEADING_SPACES, TRAILING_ZEROS);
                    printf("%s %c",receive_str,column_separator);

                    sprintf((char*)str,"%.2f",spend_change);
                    pad_value(str, (char *)receive_str,
                              LEADING_SPACES, TRAILING_ZEROS);
                    printf("%s%c",receive_str,column_separator);

                }
#ifdef USE_COLOURS
                if (column_separator == ' ')
                    printf("%s",NORMAL);
#endif
                printf("\n");

            }
            else {
                for (col = 0; col < cols; col++) {
                    char * val = (char*)sqlite3_column_text(stmt,col);

                    if (col == 1) {
                        if (is_value(val) != 0) {

                            /* percent change in received value */
                            if (prev_receive != 0)
                                receive_change =
                                    (atof(val) -
                                     prev_receive)*100/prev_receive;

                            prev_receive = atof(val);
                        }
                        else
                            break;
                    }
                    if (col == 2) {
                        if (is_value(val) != 0) {
                            /* percent change in spend value */
                            if (prev_spend != 0)
                                spend_change =
                                    (atof(val) -
                                     prev_spend)*100/prev_spend;

                            prev_spend = atof(val);
                        }
                    }

                    if (col > 0) {
                        if (is_value(val) != 0)
                            fprintf(fp,"%s%c",val,separator);
                        else
                            break;
                    }
                    else {
                        if (val != 0)
                            fprintf(fp,"%s%c",val,separator);
                        else
                            break;
                    }
                    if (col == 3) {
                        if (is_value(val) != 0)
                            bal = atof(val);
                        else
                            bal = 0;
                    }
                }
                if (col == cols)
                    fprintf(fp,"%.5f%c%.2f%c%.2f\n",
                            curr_balance,separator,
                            receive_change,separator,spend_change);
            }
            curr_balance -= bal;
            row++;
            if (row >= max_months)
                break;
        }
        else if (retval == SQLITE_DONE) {
            break;
        }
        else {
            printf("Some error encountered while running monthly query\n");
            sqlite3_close(handle);
            return 0;
        }
    }

    sqlite3_close(handle);

    if (fp == 0) {
        if (column_separator == ' ')
            for (i = 0;i < line_length; i++)
                printf("-");

        printf("\n");

        sprintf((char*)str,"%.2f", total_receive);
        pad_value(str, (char *)receive_str,
                  LEADING_SPACES, TRAILING_ZEROS);

        sprintf((char*)str,"%.2f", total_spend);
        pad_value(str, (char *)spend_str,
                  LEADING_SPACES, TRAILING_ZEROS);

        sprintf((char*)str,"%.2f", total_receive - total_spend);
        pad_value(str, (char *)balance_str,
                  LEADING_SPACES, TRAILING_ZEROS);

        sprintf((char*)str,"%.2f", current_balance);
        pad_value(str, (char *)curr_balance_str,
                  LEADING_SPACES, TRAILING_ZEROS);

        printf("%s   %s  %s  %s  %s\n",
               currency,(char*)receive_str,
               (char*)spend_str,(char*)balance_str,
               (char*)curr_balance_str);
    }
    else {
        fclose(fp);
        reverse_file(export_filename);
        printf(get_text_from_identifier(EXPORT_COMPLETED),export_filename);
        printf("\n");
    }

    return 1;
}
