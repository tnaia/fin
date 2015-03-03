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

#include "narrator.h"

static char * get_currency_description(char * currency)
{
  int i;

  if (strlen(currency)!=3) {
    return 0;
  }

  /* ensure that currency is upper case */
  for (i = 0; i < strlen(currency); i++) {
    currency[i] = toupper(currency[i]);
  }

  if ((strcmp(currency,"GBP")==0) || (strcmp(currency,"UKP")==0)) {
    return get_text_from_identifier(CURRENCY_POUNDS);
  }
  if ((strcmp(currency,"USD")==0) || (strcmp(currency,"AUD")==0)) {
    return get_text_from_identifier(CURRENCY_DOLLARS);
  }
  if (strcmp(currency,"EUR")==0) {
    return get_text_from_identifier(CURRENCY_EUROS);
  }
  if (strcmp(currency,"RUB")==0) {
    return get_text_from_identifier(CURRENCY_RUBLES);
  }
  if (strcmp(currency,"CNY")==0) {
    return get_text_from_identifier(CURRENCY_YUAN);
  }
  if (strcmp(currency,"JPY")==0) {
    return get_text_from_identifier(CURRENCY_YEN);
  }
  return 0;
}

/* produces natural language string suitable for text to speech */
void narrate_balance(char * account, char * currency)
{
  char * str;
  double balance = get_current_balance(account, currency, 0);
  char * currencystr = get_currency_description(currency);

  if (currencystr==0) {
    printf("Unrecognised currency type %s\n", currency);
    return;
  }

  if (balance>=0) {
    str = get_text_from_identifier(NARRATOR_BALANCE);
  }
  else {
    str = get_text_from_identifier(NARRATOR_BALANCE_NEGATIVE);
    balance = -balance;
  }

  printf(str,account,(int)balance,currencystr);
  printf("\n");
}

void narrate_income(char * account, char * currency)
{
  char * currencystr = get_currency_description(currency);
  time_t rawtime;
  struct tm * timeinfo;
  int year, month;
  double total_receive=0,total_spend=0;
  double average_income = 0;
  char * str = 0;

  if (currencystr==0) {
    printf("Unrecognised currency type %s\n", currency);
    return;
  }

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );

  month = timeinfo->tm_mon + 1; 
  year = timeinfo->tm_year + YEAR_BASELINE;

  if (get_annual_cashflow(account, currency,
                          year, month,
                          &total_receive, &total_spend)!=0) {
    average_income = total_receive / 12.0;
    str = get_text_from_identifier(NARRATOR_AVERAGE_INCOME);
    if (str!=0) {
      printf(str,account,(int)average_income,currencystr);
      printf("\n");
    }
  }
}

/* returns the time to broke (positive balance)
   or time to repayment (negative balance) in months
   with the given average interest and inflation rates. */
static int calc_ETA(double income_rate_per_month,
                    double spending_rate_per_month,
                    double balance,
                    double interest_rate,
                    double inflation_rate)
{
  float bal = balance;
  int ETA = 0;
  const int max_years = 80;

  while ((((balance>0) && (bal>0)) ||
          ((balance<0) && (bal<0)))
         && (ETA<max_years*12)) {
    bal = bal - spending_rate_per_month + income_rate_per_month;
    /* inflate spending */
    spending_rate_per_month *= (100.0+(inflation_rate/12.0))/100.0;
    /* accrue interest or debt */
    bal *= (100.0+(interest_rate/12.0))/100.0;
    ETA++;
  }
  return ETA;
}

void narrate_spending(char * account, char * currency,
                      double loans_interest_rate,
                      double savings_interest_rate,
                      double inflation_rate)
{
  double balance = get_current_balance(account, currency, 0);
  char * currencystr = get_currency_description(currency);
  time_t rawtime;
  struct tm * timeinfo;
  int year, month, months_remaining;
  double total_receive=0,total_spend=0;
  double average_income = 0;
  double average_spending = 0;
  char * str = 0;

  if (currencystr==0) {
    printf("Unrecognised currency type %s\n", currency);
    return;
  }

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );

  month = timeinfo->tm_mon + 1; 
  year = timeinfo->tm_year + YEAR_BASELINE;

  if (get_annual_cashflow(account, currency,
                          year, month,
                          &total_receive, &total_spend)!=0) {
    average_income = total_receive / 12.0;
    average_spending = total_spend / 12.0;
    if (average_spending>=1) {
      str = get_text_from_identifier(NARRATOR_AVERAGE_SPENDING);
      if (str!=0) {
        /* report current spending */
        printf(str,account,(int)average_spending,currencystr);
        if (balance>0) {
          /* estimate time to broke */
          months_remaining = calc_ETA(average_income,
                                      average_spending, balance,
                                      savings_interest_rate,
                                      inflation_rate);
          if (months_remaining <= 12) {
            /* less than a year to broke */
            str = get_text_from_identifier(NARRATOR_MONTHS_TO_BROKE);
            if (str!=0) {
              printf("\n");
              printf(str,months_remaining);
            }
          }
          else {
            if (months_remaining < MAX_PREDICTION_YEARS*12) {
              /* number of years until broke */
              str = get_text_from_identifier(NARRATOR_YEARS_TO_BROKE);
              if (str!=0) {
                printf("\n");
                printf(str,months_remaining/12,months_remaining%12);
              }
            }
          }
        }
        else {
          /* negative balance */
          if ((int)average_income>0) {

            /* estimate time to full repayment */
            months_remaining = calc_ETA(average_income,
                                        average_spending,
                                        balance,
                                        loans_interest_rate,
                                        inflation_rate);
            if (months_remaining <= 12) {
              /* less than a year to broke */
              str = get_text_from_identifier(NARRATOR_MONTHS_TO_REPAID);
              if (str != 0) {
                printf(".  ");
                printf(str,months_remaining);
              }
            }
            else {
              if (months_remaining < MAX_PREDICTION_YEARS*12) {
                /* number of years until broke */
                str = get_text_from_identifier(NARRATOR_YEARS_TO_REPAID);
                if (str != 0) {
                  printf(".\n");
                  printf(str,months_remaining/12,months_remaining%12);
                }
              }
              else {
                /* spending rate is indefinitely sustainable */
                str = get_text_from_identifier(NARRATOR_DELINQUENT);
                if (str != 0) {
                  printf(".\n%s", str);
                }
              }
            }
          }
        }
        printf("\n");
      }
    }
  }
}

void narrate_relative_cashflow(char * account, char * currency)
{
  char * currencystr = get_currency_description(currency);
  time_t rawtime;
  struct tm * timeinfo;
  int year, month, m;
  char * str = 0;
  double prev_month_spend=0, month_spend=0;
  double percent_change;
  double receive, spend;
  int spend_hits=0;
  int prev_spend_hits=0;

  if (currencystr==0) {
    printf("Unrecognised currency type %s\n", currency);
    return;
  }

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );

  month = timeinfo->tm_mon + 1; 
  year = timeinfo->tm_year + YEAR_BASELINE;

  /* take an average of the three previous months */
  for (m = 0; m < 3; m++) {
    /* this year */
    if (get_month_totals(account, currency,
                         year,  month,
                         &receive, &spend)>-1) {
      month_spend += spend;
      spend_hits++;
    }

    /* previous year */
    if (get_month_totals(account, currency,
                         year-1,  month,
                         &receive, &spend)>-1) {
      prev_month_spend += spend;
      prev_spend_hits++;
    }
    month--;
    if (month<1) {
      month += 12;
      year--;
    }
  }

  if ((prev_spend_hits > 0) && (spend_hits > 0) &&
      ((int)month_spend > 0) && ((int)prev_month_spend > 0)) {

    /* calculate averages */
    month_spend /= spend_hits;
    prev_month_spend /= prev_spend_hits;

    percent_change =
      (month_spend-prev_month_spend)*100/prev_month_spend;
    if ((int)percent_change > 0) {
      str = get_text_from_identifier(NARRATOR_SPENT_MORE);
      printf(str,(int)percent_change);
      printf("\n");
    }
    if ((int)percent_change < 0) {
      str = get_text_from_identifier(NARRATOR_SPENT_LESS);
      printf(str,-(int)percent_change);
      printf("\n");
    }
    if ((int)percent_change == 0) {
      str = get_text_from_identifier(NARRATOR_SPENT_SAME);
      printf("%s\n",str);
    }
  }
}
