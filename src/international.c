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

#include "international.h"

int current_language = ENGLISH_UK;

/*Eng (uk),   Eng (US),      French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese, Swedish */
char * international_text0[] = {
  "supplier",   "supplier",    "",      "",      "",       "",       "",     "",      "",     "",    "",    "supplier",
  "supply",     "supply",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "supply",

  "lock",       "lock",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "lock",
  "encrypt",    "encrypt",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "encrypt",

  "edit",       "edit",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "edit",
  "modify",     "modify",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "modify",

  "version",    "version",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "version",
  "--version",  "--version",   "",      "",      "",       "",       "",     "",      "",     "",    "",    "--version",

  "help",       "help",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "help",
  "--help",     "--help",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "--help",

  "language",   "language",    "",      "",      "",       "",       "",     "",      "",     "",    "",    "language",
  "lang",       "lang",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "lang",

  "to",         "to",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "to",
  "into",       "into",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "into",

  "dateformat", "dateformat",  "",      "",      "",       "",       "",     "",      "",     "",    "",    "dateformat",
  "df",         "df",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "df",

  "transfer",   "transfer",    "",      "",      "",       "",       "",     "",      "",     "",    "",    "transfer",
  "move",       "move",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "move",

  "push",       "push",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "push",
  "upload",     "upload",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "upload",

  "pull",       "pull",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "pull",
  "download",   "download",    "",      "",      "",       "",       "",     "",      "",     "",    "",    "download",

  "at",         "at",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "at",
  "in",         "in",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "in",

  "search",     "search",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "search",
  "find",       "find",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "find",

  "speak",      "speak",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "speak",
  "say",        "say",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "say"
};

/*Eng (uk),   Eng (US),      French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese, Swedish */
char * international_text1[] = {
  "adjustments","adjustments", "",      "",      "",       "",       "",     "",      "",     "",    "",    "adjustments",
  "edits",      "edits",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "edits",

  "time",       "time",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "time",
  "time",       "time",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "time",

  "volume",     "volume",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "volume",
  "vol",        "vol",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "vol",

  "maximum",    "maximum",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "maximum",
  "max",        "max",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "max",

  "distribution","distribution","",      "",      "",       "",       "",     "",      "",     "",    "",    "distribution",
  "distr",       "distr",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "distr",

  "interest",   "interest",    "",      "",      "",       "",       "",     "",      "",     "",    "",    "interest",
  "int",        "int",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "int",

  "usury",      "usury",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "usury",
  "apr",        "apr",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "apr",

  "theme",      "theme",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "theme",
  "colours",    "colors",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "colours",

  "inflation",  "inflation",   "",      "",      "",       "",       "",     "",      "",     "",    "",    "inflation",
  "inf",        "inf",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "inf",

  "transactions","transactions", "",    "",      "",       "",       "",     "",      "",     "",    "",    "transactions",
  "trans",       "trans",        "",    "",      "",       "",       "",     "",      "",     "",    "",    "trans",

  "records",    "records",     "",    "",      "",       "",       "",     "",      "",     "",    "",    "records",
  "recs",       "recs",        "",    "",      "",       "",       "",     "",      "",     "",    "",    "recs",

  "VAT",        "VAT",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "VAT",
  "vat",        "vat",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "vat",

  "customer",   "customer",    "",      "",      "",       "",       "",     "",      "",     "",    "",    "customer",
  "cust",       "cust",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "cust",

  "latitude",   "latitude",    "",      "",      "",       "",       "",     "",      "",     "",    "",    "latitude",
  "lat",        "lat",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "lat",

  "longitude",  "longitude",   "",      "",      "",       "",       "",     "",      "",     "",    "",    "longitude",
  "long",       "long",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "long",

  "altitude",   "altitude",    "",      "",      "",       "",       "",     "",      "",     "",    "",    "altitude",
  "alt",        "alt",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "alt",

  "progress",   "progress",    "",      "",      "",       "",       "",     "",      "",     "",    "",    "progress",
  "prog",       "prog",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "prog",

  "tracking",   "tracking",    "",      "",      "",       "",       "",     "",      "",     "",    "",    "tracking",
  "track",      "track",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "track",

  "currency",   "currency",    "",      "",      "",       "",       "",     "",      "",     "",    "",    "currency",
  "curr",       "curr",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "curr",

  "status",     "status",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "status",
  "stat",       "stat",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "qty",

  "quantity",   "quantity",    "",      "",      "",       "",       "",     "",      "",     "",    "",    "quantity",
  "qty",        "qty",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "qty",

  "number",     "number",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "number",
  "num",        "num",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "num"
};

char * international_text2[] = {
  "account",    "account",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "account",
  "acc",        "acc",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "acc",

  "on",         "on",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "on",
  "from",       "from",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "from",

  "invoice",    "invoice",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "invoice",
  "IN",         "IN",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "IN",

  "part",       "part",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "part",
  "PN",         "PN",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "PN",

  "sale",       "sale",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "sale",
  "SON",        "SON",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "SON",

  "purchase",   "purchase",    "",      "",      "",       "",       "",     "",      "",     "",    "",    "purchase",
  "PON",        "PON",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "PON",

  "works",      "works",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "works",
  "WO",         "WO",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "WO",

  "job",        "job",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "job",
  "JN",         "JN",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "JN",

  "serial",     "serial",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "serial",
  "SN",         "SN",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "SN",

  "date",       "date",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "date",
  "date",       "date",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "date"
};

/*Eng (uk),   Eng (US),      French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese, Swedish */
char * international_text3[] = {
  "spend",     "spend",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "spend",
  "outgoing",  "outgoing",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "outgoing",
  "paid",      "paid",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "paid",
  "buy",       "buy",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "buy",
  "bought",    "bought",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "bought",
  "purchase",  "purchase",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "purchase",
  "purchased", "purchased",    "",      "",      "",       "",       "",     "",      "",     "",    "",    "purchased",
  "spent",     "spent",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "spent",

  "sell",      "sell",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "sell",
  "sold",      "sold",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "sold",
  "income",    "income",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "income",
  "earn",      "earn",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "earn",
  "earned",    "earned",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "earned",
  "earnings"   "earnings",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "earnings",
  "receive",   "receive",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "receive",
  "received",  "received",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "received"
};

/*Eng (uk),   Eng (US),      French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese */
char * international_text4[] = {
  "import",    "import",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "import",

  "export",    "export",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "export",

  "report",    "report",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "report",

  "account",   "account",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "account",

  "list",      "list",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "list",
  "ls",        "ls",           "",      "",      "",       "",       "",     "",      "",     "",    "",    "ls",

  "calendar",  "calendar",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "calendar",

  "tests",     "tests",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "tests",

  "recur",     "recur",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "recur"
};

/*Eng (uk),   Eng (US),      French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese, Swedish */
char * international_text5[] = {
  "daily",     "daily",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "daily",
  "2daily",    "2daily",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "2daily",
  "3daily",    "3daily",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "3daily",
  "4daily",    "4daily",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "4daily",
  "5daily",    "5daily",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "5daily",
  "6daily",    "6daily",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "6daily",
  "weekly",    "weekly",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "weekly",
  "biweekly",  "biweekly",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "biweekly",
  "2weekly",   "2weekly",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "2weekly",
  "monthly",   "monthly",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "monthly",
  "2monthly",  "2monthly",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "2monthly",
  "3monthly",  "3monthly",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "3monthly",
  "6monthly",  "6monthly",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "6monthly",
  "yearly",    "yearly",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "yearly",
  "annual",    "annual",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "annual",
  "annually",  "annually",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "annually",
  "2yearly",   "2yearly",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "2yearly"
};

/*Eng (uk),         Eng (US),            French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese, Swedish */
char * international_text6[] = {
  "settings.txt",     "settings.txt",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "settings.txt",
  "Server",           "Server",            "",      "",      "",       "",       "",     "",      "",     "",    "",    "Server",
  "Date Format",      "Date Format",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "Date Format",
  "Default VAT rate", "Default VAT rate",  "",      "",      "",       "",       "",     "",      "",     "",    "",    "Default VAT rate",
  "Default Account",  "Default Account",   "",      "",      "",       "",       "",     "",      "",     "",    "",    "Default Account",
  "Default Currency", "Default Currency",  "",      "",      "",       "",       "",     "",      "",     "",    "",    "Default Currency",
  "current",          "current",           "",      "",      "",       "",       "",     "",      "",     "",    "",    "current",
  "savings",          "savings",           "",      "",      "",       "",       "",     "",      "",     "",    "",    "savings",
  "GBP",              "USD",               "EUR",   "EUR",   "EUR",    "EUR",    "EUR",  "RUB",   "EUR",  "EUR", "CNY",    "SEK",
  "20",               "0",                 "0",     "0",     "0",      "0",      "0",    "0",     "0",    "0",   "0",    "0",
  "%d/%m/%Y",         "%m/%d/%Y",          "%m/%d/%Y",  "%m/%d/%Y", "%m/%d/%Y", "%m/%d/%Y", "%m/%d/%Y", "%m/%d/%Y", "%m/%d/%Y", "%m/%d/%Y", "%m/%d/%Y",    "%d/%m/%Y",

  "normal",           "normal",            "",      "",      "",       "",       "",     "",      "",     "",    "",    "normal",
  "default",          "default",           "",      "",      "",       "",       "",     "",      "",     "",    "",    "default",
  "dark",             "dark",              "",      "",      "",       "",       "",     "",      "",     "",    "",    "dark",
  "light",            "light",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "light",
  "green",            "green",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "green",
  "black",            "black",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "black",
  "white",            "white",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "white",
  "blue",             "blue",              "",      "",      "",       "",       "",     "",      "",     "",    "",    "blue",
  "Theme set to %s",  "Theme set to %s",   "",      "",      "",       "",       "",     "",      "",     "",    "",    "Theme set to %s",
  "%s is not a known theme",  "%s is not a known theme",   "",      "",      "",       "",       "",     "",      "",     "",    "",    "%s is not a known theme",

  "cancel",           "cancel",            "",      "",      "",       "",       "",     "",      "",     "",    "",    "cancel",
  "remove",           "remove",            "",      "",      "",       "",       "",     "",      "",     "",    "",    "remove",
  "reverse",          "reverse",           "",      "",      "",       "",       "",     "",      "",     "",    "",    "reverse",
  "delete",           "delete",            "",      "",      "",       "",       "",     "",      "",     "",    "",    "delete",
  "kill",             "kill",              "",      "",      "",       "",       "",     "",      "",     "",    "",    "kill",
  "undo",             "undo",              "",      "",      "",       "",       "",     "",      "",     "",    "",    "undo",

  "%s spent",         "%s spent",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "%s spent",
  "%s received",      "%s received",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "%s received",
  "%s is not a valid amount","%s is not a valid amount",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "%s is not a valid amount",
  "No %cDate       %c    In   %c    Out  %cStatus   %cDescription%c","No %cDate       %c    In   %c    Out  %cStatus   %cDescription%c",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "No %cDate       %c    In   %c    Out  %cStatus   %cDescription%c",
  "Date    %c     In  %c    Out  %c","Date    %c     In  %c    Out  %c",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "Date    %c     In  %c    Out  %c",
  "%s Account Summary","%s Account Summary",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "%s Account Summary",
  "%s Account Monthly Summary","%s Account Monthly Summary",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "%s Account Monthly Summary",
  "%s Account Annual Summary","%s Account Annual Summary",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "%s Account Annual Summary",
  "Month   %c    In   %c    Out  %c    Net  %c Balance%c","Month   %c    In   %c    Out  %c    Net  %c Balance%c",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "Month   %c    In   %c    Out  %c    Net  %c Balance%c",
  "Year %c    In   %c    Out  %c    Net  %c Balance %c    In %% %c    Out %%%c","Year %c    In   %c    Out  %c    Net  %c Balance %c    In %% %c    Out %%%c",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "Year %c    In   %c    Out  %c    Net  %c Balance %c    In %% %c    Out %%%c",
  "%s Account Search Results","%s Account Search Results",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "%s Account Search Results",
  "%s\tIn: %s\tOut: %s\tBalance: %s","%s\tIn: %s\tOut: %s\tBalance: %s",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "%s\tIn: %s\tOut: %s\tBalance: %s",
  "Totals",           "Totals",            "",      "",      "",       "",       "",     "",      "",     "",    "",    "Totals",
  "No server has been specified",          "No server has been specified",  "",      "",      "",       "",       "",     "",      "",     "",    "",    "No server has been specified"
};

/*Eng (uk),         Eng (US),            French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese, Swedish */
char * international_text7[] = {
  "month",            "month",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "month",
  "mon",              "mon",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "mon",

  "year",             "year",              "",      "",      "",       "",       "",     "",      "",     "",    "",    "year",
  "startyear",        "startyear",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "startyear",

  "endyear",          "endyear",           "",      "",      "",       "",       "",     "",      "",     "",    "",    "endyear",
  "endyr",            "endyr",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "endyr",

  "january",          "january",           "",      "",      "",       "",       "",     "",      "",     "",    "",    "january",
  "jan",              "jan",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "jan",

  "feburary",         "feburary",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "feburary",
  "feb",              "feb",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "feb",

  "march",            "march",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "march",
  "mar",              "mar",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "mar",

  "april",            "april",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "april",
  "apr",              "apr",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "apr",

  "may",              "may",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "may",
  "may",              "may",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "may",

  "june",             "june",              "",      "",      "",       "",       "",     "",      "",     "",    "",    "june",
  "jun",              "jun",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "jun",

  "july",             "july",              "",      "",      "",       "",       "",     "",      "",     "",    "",    "july",
  "jul",              "jul",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "jul",

  "august",           "august",            "",      "",      "",       "",       "",     "",      "",     "",    "",    "august",
  "aug",              "aug",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "aug",

  "september",        "september",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "september",
  "sep",              "sep",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "sep",

  "october",          "october",           "",      "",      "",       "",       "",     "",      "",     "",    "",    "october",
  "oct",              "oct",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "oct",

  "november",         "november",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "november",
  "nov",              "nov",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "nov",

  "december",         "december",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "december",
  "dec",              "dec",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "dec",

  "Account changed to %s","Account changed to %s", "",      "",      "",       "",       "",     "",      "",     "",    "",    "Account changed to %s",
  "Currency changed to %s","Currency changed to %s", "",      "",      "",       "",       "",     "",      "",     "",    "",    "Currency changed to %s",
  "Currency must contain three letters (eg. USD)","Currency must contain three letters (eg. USD)", "",      "",      "",       "",       "",     "",      "",     "",    "",    "Currency must contain three letters (eg. USD)",
  "#CANCEL %s",       "#CANCEL %s",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "#CANCEL %s",
  "X",                "X",                 "",      "",      "",       "",       "",     "",      "",     "",    "",    "X",
  "Date format changed to %s", "Date format changed to %s", "",      "",      "",       "",       "",     "",      "",     "",    "",    "Date format changed to %s",
  "Data exported to %s", "Data exported to %s", "",      "",      "",       "",       "",     "",      "",     "",    "",    "Data exported to %s",
  "separator",        "separator",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "separator",
  "tab",              "tab",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "tab",
  "Month,In,Out,Net,Balance", "Month,In,Out,Net,Balance", "",      "",      "",       "",       "",     "",      "",     "",    "",    "Month,In,Out,Net,Balance",
  "Year,In,Out,Net,Balance,In % Change,Out % Change", "Year,In,Out,Net,Balance,In % Change,Out % Change", "",      "",      "",       "",       "",     "",      "",     "",    "",    "Year,In,Out,Net,Balance,In % Change,Out % Change",
  "#REF %s #TRANSFER to %s", "#REF %s #TRANSFER to %s", "",      "",      "",       "",       "",     "",      "",     "",    "",    "#REF %s #TRANSFER to %s",
  "#REF %s #TRANSFER from %s", "#REF %s #TRANSFER from %s", "",      "",      "",       "",       "",     "",      "",     "",    "",    "#REF %s #TRANSFER from %s",
  "Transferred %s from %s to %s", "Transferred %s from %s to %s", "",      "",      "",       "",       "",     "",      "",     "",    "",    "Transferred %s from %s to %s",
  "Language",         "Language",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "Language",
  "Theme",            "Theme",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "Theme",
  "Language changed to %s", "Language changed to %s", "",      "",      "",       "",       "",     "",      "",     "",    "",    "Language changed to %s",
  "kml",              "kml", "",      "",      "",       "",       "",     "",      "",     "",    "",    "kml",
  "Version %.3f",     "Version %.3f", "",      "",      "",       "",       "",     "",      "",     "",    "",    "Version %.3f",
  "%d transactions imported from %s",     "%d transactions imported from %s", "",      "",      "",       "",       "",     "",      "",     "",    "",    "%d transactions imported from %s"
};

/*Eng (uk),         Eng (US),            French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese, Swedish */
char * international_text8[] = {
  "money in",         "money in",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "money in",
  "money out",        "money out",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "money out",
  "credit",           "credit",            "",      "",      "",       "",       "",     "",      "",     "",    "",    "credit",
  "debit",            "debit",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "debit",
  "amount",           "amount",            "",      "",      "",       "",       "",     "",      "",     "",    "",    "amount",
  "memo",             "memo",              "",      "",      "",       "",       "",     "",      "",     "",    "",    "memo",
  "notes",            "notes",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "notes",
  "category",         "category",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "category",
  "subcategory",      "subcategory",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "subcategory",
  "description",      "description",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "description",
  "name",             "name",              "",      "",      "",       "",       "",     "",      "",     "",    "",    "name",
  "type",             "type",              "",      "",      "",       "",       "",     "",      "",     "",    "",    "type",
  "in",               "in",                "",      "",      "",       "",       "",     "",      "",     "",    "",    "in",
  "out",              "out",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "out",
  "balance",          "balance",           "",      "",      "",       "",       "",     "",      "",     "",    "",    "balance",

  "Nothing found to plot","Nothing found to plot",   "",      "",      "",       "",       "",     "",      "",     "",    "",    "Nothing found to plot",
  "Spending",         "Spending",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "Spending",
  "Spending for %d",  "Spending for %s",   "",      "",      "",       "",       "",     "",      "",     "",    "",    "Spending for %d",
  "Income",           "Income",            "",      "",      "",       "",       "",     "",      "",     "",    "",    "Income",
  "Income for %d",    "Income for %s",     "",      "",      "",       "",       "",     "",      "",     "",    "",    "Income for %d",
  "Monthly Volumes",  "Monthly Volumes",   "",      "",      "",       "",       "",     "",      "",     "",    "",    "Monthly Volumes",
  "Yearly Volumes",   "Yearly Volumes",    "",      "",      "",       "",       "",     "",      "",     "",    "",    "Yearly Volumes",
  "Spending on %s for %s account",         "Spending on %s for %s account", "",      "",      "",       "",       "",     "",      "",     "",    "",    "Spending on %s for %s account",
  "Balance for %s account",                "Balance for %s account",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "Balance for %s account",
  "Record number %s undone",               "Record number %s undone",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "Record number %s undone",
  "Record number %s edited",               "Record number %s edited",       "",      "",      "",       "",       "",     "",      "",     "",    "",    "Record number %s edited",
  "Exported image %s",                     "Exported image %s",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "Exported image %s",

  "No transactions found",                 "No transactions found",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "No transactions found",
  "Spending Distribution for %d", "Spending Distribution for %d",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "Spending Distribution for %d",
  "Spending Distribution for %d-%d", "Spending Distribution for %d-%d",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "Spending Distribution for %d-%d"
};

/*Eng (uk),         Eng (US),            French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese, Swedish */
char * international_text9[] = {
  "%s Account",       "%s Account",        "",      "",      "",       "",       "",     "",      "",     "",    "",    "%s Account",
  "am",               "am",                "",      "",      "",       "",       "",     "",      "",     "",    "",    "am",
  "pm",               "pm",                "",      "",      "",       "",       "",     "",      "",     "",    "",    "pm",
  "Frequency",        "Frequency",         "",      "",      "",       "",       "",     "",      "",     "",    "",    "Frequency",
  "spending",         "spending",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "spending",
  "currency conversion", "currency conversion", "",      "",      "",       "",       "",     "",      "",     "",    "",    "currency conversion",
  "pounds",           "pounds",            "",      "",      "",       "",       "",     "",      "",     "",    "",    "pounds",
  "dollars",          "dollars",           "",      "",      "",       "",       "",     "",      "",     "",    "",    "dollars",
  "euros",            "euros",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "euros",
  "rubles",           "rubles",            "",      "",      "",       "",       "",     "",      "",     "",    "",    "rubles",
  "yuan",             "yuan",              "",      "",      "",       "",       "",     "",      "",     "",    "",    "yuan",
  "yen",              "yen",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "yen",
  "title",            "title",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "title",
  "username",         "username",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "username",
  "piechart",         "piechart",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "piechart",
  "percent",          "percent",           "",      "",      "",       "",       "",     "",      "",     "",    "",    "percent",
  "Other",            "Other",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "Other",
  "No income found",  "No income found",   "",      "",      "",       "",       "",     "",      "",     "",    "",    "No income found",
  "Name of the GnuCash account to import: ","Name of the GnuCash account to import: ",   "",      "",      "",       "",       "",     "",      "",     "",    "",    "Name of the GnuCash account to import: ",
  "Recent Edits",     "Recent Edits",      "",      "",      "",       "",       "",     "",      "",     "",    "",    "Recent Edits",
  " %cDate       %c    In   %c    Out  %cStatus   %cDescription%c","No %cDate       %c    In   %c    Out  %cStatus   %cDescription%c",     "",      "",      "",       "",       "",     "",      "",     "",    "",    " %cDate       %c    In   %c    Out  %cStatus   %cDescription%c",
  "style",            "style",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "style",
  "sty",              "sty",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "sty",
  "backup",           "backup",            "",      "",      "",       "",       "",     "",      "",     "",    "",    "backup",
  "restore",          "restore",           "",      "",      "",       "",       "",     "",      "",     "",    "",    "restore",
  "Backup created %s\n","Backup created %s\n","",      "",      "",       "",       "",     "",      "",     "",    "",    "Backup created %s\n",
  "No backup filename specified\n","No backup filename specified\n","",      "",      "",       "",       "",     "",      "",     "",    "",    "No backup filename specified\n",
  "Restored from %s\n","Restored from %s\n","",      "",      "",       "",       "",     "",      "",     "",    "",    "Restored from %s\n",
  "No restore filename specified\n","No restore filename specified\n","",      "",      "",       "",       "",     "",      "",     "",    "",    "No restore filename specified\n",
  "File %s not found\n","File %s not found\n","",      "",      "",       "",       "",     "",      "",     "",    "",    "File %s not found\n",

  "summary",          "summary",           "",      "",      "",       "",       "",     "",      "",     "",    "",    "summary",
  "accounts",         "accounts",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "accounts",

  "emacs",            "emacs",             "",      "",      "",       "",       "",     "",      "",     "",    "",    "emacs",
  "org",              "org",               "",      "",      "",       "",       "",     "",      "",     "",    "",    "org",
  "org-mode",         "org-mode",          "",      "",      "",       "",       "",     "",      "",     "",    "",    "org-mode"
};

char * international_narrator[] = {
  /* balance (positive)*/
  "The balance of your %s account is %d %s", /*Eng (uk)*/
  "The balance of your %s account is %d %s", /*Eng (US)*/
  "The balance of your %s account is %d %s", /*French*/
  "The balance of your %s account is %d %s", /*German*/
  "The balance of your %s account is %d %s", /*Italian*/
  "The balance of your %s account is %d %s", /*Spanish*/
  "The balance of your %s account is %d %s", /*Greek*/
  "The balance of your %s account is %d %s", /*Russian*/
  "The balance of your %s account is %d %s", /*Polish*/
  "The balance of your %s account is %d %s", /*Dutch*/
  "The balance of your %s account is %d %s", /*Chinese*/
  "Saldot på kontot %s är %d %s", /*Swedish*/

  /* balance (negative)*/
  "Your %s account is %d %s overdrawn", /*Eng (uk)*/
  "Your %s account is %d %s overdrawn", /*Eng (US)*/
  "Your %s account is %d %s overdrawn", /*French*/
  "Your %s account is %d %s overdrawn", /*German*/
  "Your %s account is %d %s overdrawn", /*Italian*/
  "Your %s account is %d %s overdrawn", /*Spanish*/
  "Your %s account is %d %s overdrawn", /*Greek*/
  "Your %s account is %d %s overdrawn", /*Russian*/
  "Your %s account is %d %s overdrawn", /*Polish*/
  "Your %s account is %d %s overdrawn", /*Dutch*/
  "Your %s account is %d %s overdrawn", /*Chinese*/
  "Saldot på kontot %s är minus %d %s", /*Swedish*/

  /* average income */
  "Average income to your %s account is %d %s per month", /*Eng (uk)*/
  "Average income to your %s account is %d %s per month", /*Eng (US)*/
  "Average income to your %s account is %d %s per month", /*French*/
  "Average income to your %s account is %d %s per month", /*German*/
  "Average income to your %s account is %d %s per month", /*Italian*/
  "Average income to your %s account is %d %s per month", /*Spanish*/
  "Average income to your %s account is %d %s per month", /*Greek*/
  "Average income to your %s account is %d %s per month", /*Russian*/
  "Average income to your %s account is %d %s per month", /*Polish*/
  "Average income to your %s account is %d %s per month", /*Dutch*/
  "Average income to your %s account is %d %s per month", /*Chinese*/
  "Snittinkomsten på kontot %s är  %d %s per månad", /*Swedish*/

  /* average spending */
  "Average outgoings from your %s account are %d %s per month", /*Eng (uk)*/
  "Average outgoings from your %s account are %d %s per month", /*Eng (US)*/
  "Average outgoings from your %s account are %d %s per month", /*French*/
  "Average outgoings from your %s account are %d %s per month", /*German*/
  "Average outgoings from your %s account are %d %s per month", /*Italian*/
  "Average outgoings from your %s account are %d %s per month", /*Spanish*/
  "Average outgoings from your %s account are %d %s per month", /*Greek*/
  "Average outgoings from your %s account are %d %s per month", /*Russian*/
  "Average outgoings from your %s account are %d %s per month", /*Polish*/
  "Average outgoings from your %s account are %d %s per month", /*Dutch*/
  "Average outgoings from your %s account are %d %s per month", /*Chinese*/
  "Snittförbrukningen på %s kontoen är %d %s per månad", /*Swedish*/

  /* months remaining */
  "Funds will be expended in %d months", /*Eng (uk)*/
  "Funds will be expended in %d months", /*Eng (US)*/
  "Funds will be expended in %d months", /*French*/
  "Funds will be expended in %d months", /*German*/
  "Funds will be expended in %d months", /*Italian*/
  "Funds will be expended in %d months", /*Spanish*/
  "Funds will be expended in %d months", /*Greek*/
  "Funds will be expended in %d months", /*Russian*/
  "Funds will be expended in %d months", /*Polish*/
  "Funds will be expended in %d months", /*Dutch*/
  "Funds will be expended in %d months", /*Chinese*/
  "Funds will be expended in %d months", /*Swedish*/

  /* years and months remaining */
  "Funds will be expended in %d years and %d months", /*Eng (uk)*/
  "Funds will be expended in %d years and %d months", /*Eng (US)*/
  "Funds will be expended in %d years and %d months", /*French*/
  "Funds will be expended in %d years and %d months", /*German*/
  "Funds will be expended in %d years and %d months", /*Italian*/
  "Funds will be expended in %d years and %d months", /*Spanish*/
  "Funds will be expended in %d years and %d months", /*Greek*/
  "Funds will be expended in %d years and %d months", /*Russian*/
  "Funds will be expended in %d years and %d months", /*Polish*/
  "Funds will be expended in %d years and %d months", /*Dutch*/
  "Funds will be expended in %d years and %d months", /*Chinese*/
  "Funds will be expended in %d years and %d months", /*Swedish*/

  /* Indefinitely sustainable spending rate -
     a sort of financial escape velocity */
  "This is indefinitely sustainable", /*Eng (uk)*/
  "This is indefinitely sustainable", /*Eng (US)*/
  "This is indefinitely sustainable", /*French*/
  "This is indefinitely sustainable", /*German*/
  "This is indefinitely sustainable", /*Italian*/
  "This is indefinitely sustainable", /*Spanish*/
  "This is indefinitely sustainable", /*Greek*/
  "This is indefinitely sustainable", /*Russian*/
  "This is indefinitely sustainable", /*Polish*/
  "This is indefinitely sustainable", /*Dutch*/
  "This is indefinitely sustainable", /*Chinese*/
  "This is indefinitely sustainable", /*Swedish*/


  /* months remaining */
  "Debt/overdraft will be repaid in %d months", /*Eng (uk)*/
  "Debt/overdraft will be repaid in %d months", /*Eng (US)*/
  "Debt/overdraft will be repaid in %d months", /*French*/
  "Debt/overdraft will be repaid in %d months", /*German*/
  "Debt/overdraft will be repaid in %d months", /*Italian*/
  "Debt/overdraft will be repaid in %d months", /*Spanish*/
  "Debt/overdraft will be repaid in %d months", /*Greek*/
  "Debt/overdraft will be repaid in %d months", /*Russian*/
  "Debt/overdraft will be repaid in %d months", /*Polish*/
  "Debt/overdraft will be repaid in %d months", /*Dutch*/
  "Debt/overdraft will be repaid in %d months", /*Chinese*/
  "Debt/overdraft will be repaid in %d months", /*Swedish*/

  /* years and months remaining */
  "Debt/overdraft will be repaid in %d years and %d months", /*Eng (uk)*/
  "Debt/overdraft will be repaid in %d years and %d months", /*Eng (US)*/
  "Debt/overdraft will be repaid in %d years and %d months", /*French*/
  "Debt/overdraft will be repaid in %d years and %d months", /*German*/
  "Debt/overdraft will be repaid in %d years and %d months", /*Italian*/
  "Debt/overdraft will be repaid in %d years and %d months", /*Spanish*/
  "Debt/overdraft will be repaid in %d years and %d months", /*Greek*/
  "Debt/overdraft will be repaid in %d years and %d months", /*Russian*/
  "Debt/overdraft will be repaid in %d years and %d months", /*Polish*/
  "Debt/overdraft will be repaid in %d years and %d months", /*Dutch*/
  "Debt/overdraft will be repaid in %d years and %d months", /*Chinese*/
  "Debt/overdraft will be repaid in %d years and %d months", /*Swedish*/

  /* Indefinitely sustainable spending rate -
     a sort of financial escape velocity */
  "The debt/overdraft will never be repaid", /*Eng (uk)*/
  "The debt/overdraft will never be repaid", /*Eng (US)*/
  "The debt/overdraft will never be repaid", /*French*/
  "The debt/overdraft will never be repaid", /*German*/
  "The debt/overdraft will never be repaid", /*Italian*/
  "The debt/overdraft will never be repaid", /*Spanish*/
  "The debt/overdraft will never be repaid", /*Greek*/
  "The debt/overdraft will never be repaid", /*Russian*/
  "The debt/overdraft will never be repaid", /*Polish*/
  "The debt/overdraft will never be repaid", /*Dutch*/
  "The debt/overdraft will never be repaid", /*Chinese*/
  "The debt/overdraft will never be repaid", /*Swedish*/

  /* spent more */
  "You have spent %d%% more than the same time last year", /*Eng (uk)*/
  "You have spent %d%% more than the same time last year", /*Eng (US)*/
  "You have spent %d%% more than the same time last year", /*French*/
  "You have spent %d%% more than the same time last year", /*German*/
  "You have spent %d%% more than the same time last year", /*Italian*/
  "You have spent %d%% more than the same time last year", /*Spanish*/
  "You have spent %d%% more than the same time last year", /*Greek*/
  "You have spent %d%% more than the same time last year", /*Russian*/
  "You have spent %d%% more than the same time last year", /*Polish*/
  "You have spent %d%% more than the same time last year", /*Dutch*/
  "You have spent %d%% more than the same time last year", /*Chinese*/
  "You have spent %d%% more than the same time last year", /*Swedish*/

  /* spent less */
  "You have spent %d%% less than the same time last year", /*Eng (uk)*/
  "You have spent %d%% less than the same time last year", /*Eng (US)*/
  "You have spent %d%% less than the same time last year", /*French*/
  "You have spent %d%% less than the same time last year", /*German*/
  "You have spent %d%% less than the same time last year", /*Italian*/
  "You have spent %d%% less than the same time last year", /*Spanish*/
  "You have spent %d%% less than the same time last year", /*Greek*/
  "You have spent %d%% less than the same time last year", /*Russian*/
  "You have spent %d%% less than the same time last year", /*Polish*/
  "You have spent %d%% less than the same time last year", /*Dutch*/
  "You have spent %d%% less than the same time last year", /*Chinese*/
  "You have spent %d%% less than the same time last year", /*Swedish*/

  /* spent same */
  "You have spent a similar amount to this time last year", /*Eng (uk)*/
  "You have spent a similar amount to this time last year", /*Eng (US)*/
  "You have spent a similar amount to this time last year", /*French*/
  "You have spent a similar amount to this time last year", /*German*/
  "You have spent a similar amount to this time last year", /*Italian*/
  "You have spent a similar amount to this time last year", /*Spanish*/
  "You have spent a similar amount to this time last year", /*Greek*/
  "You have spent a similar amount to this time last year", /*Russian*/
  "You have spent a similar amount to this time last year", /*Polish*/
  "You have spent a similar amount to this time last year", /*Dutch*/
  "You have spent a similar amount to this time last year", /*Chinese*/
  "You have spent a similar amount to this time last year" /*Swedish*/
};

void set_language(int index)
{
  current_language = index;
}

void set_language_from_string(char * language)
{
  current_language = valid_language(language)-1;
  if (current_language<0) current_language=0;
}

char * get_text_from_identifier(int identifier)
{
  char ** lookup = international_text0;

  if (identifier < ADJUSTMENTS) {
    return lookup[identifier*LANGUAGES + current_language];
  }

  if ((identifier >= ADJUSTMENTS) && (identifier < ACCOUNT)) {
    lookup = international_text1;
    identifier -= ADJUSTMENTS;
    return lookup[identifier*LANGUAGES + current_language];
  }

  if ((identifier >= ACCOUNT) && (identifier < COMMAND_SPEND)) {
    lookup = international_text2;
    identifier -= ACCOUNT;
    return lookup[identifier*LANGUAGES + current_language];
  }

  if ((identifier >= COMMAND_SPEND) && (identifier < COMMAND_IMPORT)) {
    lookup = international_text3;
    identifier -= COMMAND_SPEND;
    return lookup[identifier*LANGUAGES + current_language];
  }

  if ((identifier >= COMMAND_IMPORT) && (identifier < PERIOD_DAILY)) {
    lookup = international_text4;
    identifier -= COMMAND_IMPORT;
    return lookup[identifier*LANGUAGES + current_language];
  }

  if ((identifier >= PERIOD_DAILY) && (identifier < SETTINGS_FILENAME)) {
    lookup = international_text5;
    identifier -= PERIOD_DAILY;
    return lookup[identifier*LANGUAGES + current_language];
  }

  if ((identifier >= SETTINGS_FILENAME) && (identifier < COMMAND_MONTH)) {
    lookup = international_text6;
    identifier -= SETTINGS_FILENAME;
    return lookup[identifier*LANGUAGES + current_language];
  }

  if ((identifier >= COMMAND_MONTH) && (identifier < MONEY_IN)) {
    lookup = international_text7;
    identifier -= COMMAND_MONTH;
    return lookup[identifier*LANGUAGES + current_language];
  }

  if ((identifier >= MONEY_IN) && (identifier < TITLE_ACCOUNT)) {
    lookup = international_text8;
    identifier -= MONEY_IN;
    return lookup[identifier*LANGUAGES + current_language];
  }

  if ((identifier >= TITLE_ACCOUNT) && (identifier < NARRATOR_BALANCE)) {
    lookup = international_text9;
    identifier -= TITLE_ACCOUNT;
    return lookup[identifier*LANGUAGES + current_language];
  }

  lookup = international_narrator;
  identifier -= NARRATOR_BALANCE;
  return lookup[identifier*LANGUAGES + current_language];
}
