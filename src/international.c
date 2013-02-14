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

#include "international.h"

int current_language = ENGLISH_UK;

/*Eng (uk),   Eng (US),      French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese */
char * international_text0[] = {
	"supplier",   "supplier",    "",      "",      "",       "",       "",     "",      "",     "",    "",
	"supply",     "supply",      "",      "",      "",       "",       "",     "",      "",     "",    "",

	"lock",       "lock",        "",      "",      "",       "",       "",     "",      "",     "",    "",
	"encrypt",    "encrypt",     "",      "",      "",       "",       "",     "",      "",     "",    "",

	"edit",       "edit",        "",      "",      "",       "",       "",     "",      "",     "",    "",
	"modify",     "modify",      "",      "",      "",       "",       "",     "",      "",     "",    "",

	"version",    "version",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"--version",  "--version",   "",      "",      "",       "",       "",     "",      "",     "",    "",

	"help",       "help",        "",      "",      "",       "",       "",     "",      "",     "",    "",
	"--help",     "--help",      "",      "",      "",       "",       "",     "",      "",     "",    "",

	"language",   "language",    "",      "",      "",       "",       "",     "",      "",     "",    "",
	"lang",       "lang",        "",      "",      "",       "",       "",     "",      "",     "",    "",

	"to",         "to",          "",      "",      "",       "",       "",     "",      "",     "",    "",
	"into",       "into",        "",      "",      "",       "",       "",     "",      "",     "",    "",

	"dateformat", "dateformat",  "",      "",      "",       "",       "",     "",      "",     "",    "",
	"df",         "df",          "",      "",      "",       "",       "",     "",      "",     "",    "",

	"transfer",   "transfer",    "",      "",      "",       "",       "",     "",      "",     "",    "",
	"move",       "move",        "",      "",      "",       "",       "",     "",      "",     "",    "",

	"push",       "push",        "",      "",      "",       "",       "",     "",      "",     "",    "",
	"upload",     "upload",      "",      "",      "",       "",       "",     "",      "",     "",    "",

	"pull",       "pull",        "",      "",      "",       "",       "",     "",      "",     "",    "",
	"download",   "download",    "",      "",      "",       "",       "",     "",      "",     "",    "",

	"at",         "at",          "",      "",      "",       "",       "",     "",      "",     "",    "",
	"in",         "in",          "",      "",      "",       "",       "",     "",      "",     "",    "",

	"search",     "search",      "",      "",      "",       "",       "",     "",      "",     "",    "",
	"find",       "find",        "",      "",      "",       "",       "",     "",      "",     "",    "",

	"speak",      "speak",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"say",        "say",         "",      "",      "",       "",       "",     "",      "",     "",    ""
};

char * international_text1[] = {
	"adjustments","adjustments", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"edits",      "edits",       "",      "",      "",       "",       "",     "",      "",     "",    "",

	"time",       "time",        "",      "",      "",       "",       "",     "",      "",     "",    "",
	"time",       "time",        "",      "",      "",       "",       "",     "",      "",     "",    "",

	"volume",     "volume",      "",      "",      "",       "",       "",     "",      "",     "",    "",
	"vol",        "vol",         "",      "",      "",       "",       "",     "",      "",     "",    "",

	"maximum",    "maximum",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"max",        "max",         "",      "",      "",       "",       "",     "",      "",     "",    "",

	"distribution","distribution","",      "",      "",       "",       "",     "",      "",     "",    "",
	"distr",       "distr",       "",      "",      "",       "",       "",     "",      "",     "",    "",

	"interest",   "interest",    "",      "",      "",       "",       "",     "",      "",     "",    "",
	"int",        "int",         "",      "",      "",       "",       "",     "",      "",     "",    "",

	"usury",      "usury",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"apr",        "apr",         "",      "",      "",       "",       "",     "",      "",     "",    "",

	"inflation",  "inflation",   "",      "",      "",       "",       "",     "",      "",     "",    "",
	"inf",        "inf",         "",      "",      "",       "",       "",     "",      "",     "",    "",

	"transactions","transactions", "",    "",      "",       "",       "",     "",      "",     "",    "",
	"trans",       "trans",        "",    "",      "",       "",       "",     "",      "",     "",    "",

	"records",    "records",     "",    "",      "",       "",       "",     "",      "",     "",    "",
	"recs",       "recs",        "",    "",      "",       "",       "",     "",      "",     "",    "",

	"VAT",        "VAT",         "",      "",      "",       "",       "",     "",      "",     "",    "",
	"vat",        "vat",         "",      "",      "",       "",       "",     "",      "",     "",    "",

	"customer",   "customer",    "",      "",      "",       "",       "",     "",      "",     "",    "",
	"cust",       "cust",        "",      "",      "",       "",       "",     "",      "",     "",    "",

	"latitude",   "latitude",    "",      "",      "",       "",       "",     "",      "",     "",    "",
	"lat",        "lat",         "",      "",      "",       "",       "",     "",      "",     "",    "",

	"longitude",  "longitude",   "",      "",      "",       "",       "",     "",      "",     "",    "",
	"long",       "long",        "",      "",      "",       "",       "",     "",      "",     "",    "",

	"altitude",   "altitude",    "",      "",      "",       "",       "",     "",      "",     "",    "",
	"alt",        "alt",         "",      "",      "",       "",       "",     "",      "",     "",    "",

	"progress",   "progress",    "",      "",      "",       "",       "",     "",      "",     "",    "",
	"prog",       "prog",        "",      "",      "",       "",       "",     "",      "",     "",    "",

	"tracking",   "tracking",    "",      "",      "",       "",       "",     "",      "",     "",    "",
	"track",      "track",       "",      "",      "",       "",       "",     "",      "",     "",    "",

	"currency",   "currency",    "",      "",      "",       "",       "",     "",      "",     "",    "",
	"curr",       "curr",        "",      "",      "",       "",       "",     "",      "",     "",    "",

	"status",     "status",      "",      "",      "",       "",       "",     "",      "",     "",    "",
	"stat",       "stat",        "",      "",      "",       "",       "",     "",      "",     "",    "",

	"quantity",   "quantity",    "",      "",      "",       "",       "",     "",      "",     "",    "",
	"qty",        "qty",         "",      "",      "",       "",       "",     "",      "",     "",    "",

	"number",     "number",      "",      "",      "",       "",       "",     "",      "",     "",    "",
	"num",        "num",         "",      "",      "",       "",       "",     "",      "",     "",    ""
};

char * international_text2[] = {
	"account",    "account",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"acc",        "acc",         "",      "",      "",       "",       "",     "",      "",     "",    "",

	"on",         "on",          "",      "",      "",       "",       "",     "",      "",     "",    "",
	"from",       "from",        "",      "",      "",       "",       "",     "",      "",     "",    "",

	"invoice",    "invoice",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"IN",         "IN",          "",      "",      "",       "",       "",     "",      "",     "",    "",

	"part",       "part",        "",      "",      "",       "",       "",     "",      "",     "",    "",
	"PN",         "PN",          "",      "",      "",       "",       "",     "",      "",     "",    "",

	"sale",       "sale",        "",      "",      "",       "",       "",     "",      "",     "",    "",
	"SON",        "SON",         "",      "",      "",       "",       "",     "",      "",     "",    "",

	"purchase",   "purchase",    "",      "",      "",       "",       "",     "",      "",     "",    "",
	"PON",        "PON",         "",      "",      "",       "",       "",     "",      "",     "",    "",

	"works",      "works",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"WO",         "WO",          "",      "",      "",       "",       "",     "",      "",     "",    "",

	"job",        "job",         "",      "",      "",       "",       "",     "",      "",     "",    "",
	"JN",         "JN",          "",      "",      "",       "",       "",     "",      "",     "",    "",

	"serial",     "serial",      "",      "",      "",       "",       "",     "",      "",     "",    "",
	"SN",         "SN",          "",      "",      "",       "",       "",     "",      "",     "",    "",

	"date",       "date",        "",      "",      "",       "",       "",     "",      "",     "",    "",
	"date",       "date",        "",      "",      "",       "",       "",     "",      "",     "",    ""
};

/*Eng (uk),   Eng (US),      French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese */
char * international_text3[] = {
	"spend",     "spend",        "",      "",      "",       "",       "",     "",      "",     "",    "",
	"outgoing",  "outgoing",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"paid",      "paid",         "",      "",      "",       "",       "",     "",      "",     "",    "",
	"buy",       "buy",          "",      "",      "",       "",       "",     "",      "",     "",    "",
	"bought",    "bought",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"purchase",  "purchase",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"purchased", "purchased",    "",      "",      "",       "",       "",     "",      "",     "",    "",
	"spent",     "spent",        "",      "",      "",       "",       "",     "",      "",     "",    "",

	"sell",      "sell",         "",      "",      "",       "",       "",     "",      "",     "",    "",
	"sold",      "sold",         "",      "",      "",       "",       "",     "",      "",     "",    "",
	"income",    "income",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"earn",      "earn",         "",      "",      "",       "",       "",     "",      "",     "",    "",
	"earned",    "earned",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"earnings"   "earnings",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"receive",   "receive",      "",      "",      "",       "",       "",     "",      "",     "",    "",
	"received",  "received",     "",      "",      "",       "",       "",     "",      "",     "",    ""
};

/*Eng (uk),   Eng (US),      French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese */
char * international_text4[] = {
	"import",    "import",       "",      "",      "",       "",       "",     "",      "",     "",    "",

	"export",    "export",       "",      "",      "",       "",       "",     "",      "",     "",    "",

	"report",    "report",       "",      "",      "",       "",       "",     "",      "",     "",    "",

	"account",   "account",      "",      "",      "",       "",       "",     "",      "",     "",    "",

	"list",      "list",         "",      "",      "",       "",       "",     "",      "",     "",    "",
	"ls",        "ls",           "",      "",      "",       "",       "",     "",      "",     "",    "",

	"calendar",  "calendar",     "",      "",      "",       "",       "",     "",      "",     "",    "",

	"tests",     "tests",        "",      "",      "",       "",       "",     "",      "",     "",    "",

	"recur",     "recur",        "",      "",      "",       "",       "",     "",      "",     "",    ""
};

/*Eng (uk),   Eng (US),      French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese */
char * international_text5[] = {
	"daily",     "daily",        "",      "",      "",       "",       "",     "",      "",     "",    "",
	"2daily",    "2daily",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"3daily",    "3daily",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"4daily",    "4daily",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"5daily",    "5daily",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"6daily",    "6daily",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"weekly",    "weekly",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"biweekly",  "biweekly",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"2weekly",   "2weekly",      "",      "",      "",       "",       "",     "",      "",     "",    "",
	"monthly",   "monthly",      "",      "",      "",       "",       "",     "",      "",     "",    "",
	"2monthly",  "2monthly",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"3monthly",  "3monthly",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"6monthly",  "6monthly",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"yearly",    "yearly",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"annual",    "annual",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"annually",  "annually",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"2yearly",   "2yearly",      "",      "",      "",       "",       "",     "",      "",     "",    ""
};

/*Eng (uk),         Eng (US),            French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese */
char * international_text6[] = {
	"settings.txt",     "settings.txt",      "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Server",           "Server",            "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Date Format",      "Date Format",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Default VAT rate", "Default VAT rate",  "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Default Account",  "Default Account",   "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Default Currency", "Default Currency",  "",      "",      "",       "",       "",     "",      "",     "",    "",
	"current",          "current",           "",      "",      "",       "",       "",     "",      "",     "",    "",
	"savings",          "savings",           "",      "",      "",       "",       "",     "",      "",     "",    "",
	"GBP",              "USD",               "EUR",   "EUR",   "EUR",    "EUR",    "EUR",  "RUB",   "EUR",  "EUR", "CNY",
	"20",               "0",                 "0",     "0",     "0",      "0",      "0",    "0",     "0",    "0",   "0",
	"%d/%m/%Y",         "%m/%d/%Y",          "%m/%d/%Y",  "%m/%d/%Y", "%m/%d/%Y", "%m/%d/%Y", "%m/%d/%Y", "%m/%d/%Y", "%m/%d/%Y", "%m/%d/%Y", "%m/%d/%Y",

	"cancel",           "cancel",            "",      "",      "",       "",       "",     "",      "",     "",    "",
	"remove",           "remove",            "",      "",      "",       "",       "",     "",      "",     "",    "",
	"reverse",          "reverse",           "",      "",      "",       "",       "",     "",      "",     "",    "",
	"delete",           "delete",            "",      "",      "",       "",       "",     "",      "",     "",    "",
	"kill",             "kill",              "",      "",      "",       "",       "",     "",      "",     "",    "",
	"undo",             "undo",              "",      "",      "",       "",       "",     "",      "",     "",    "",

	"%s spent",         "%s spent",          "",      "",      "",       "",       "",     "",      "",     "",    "",
	"%s received",      "%s received",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"%s is not a valid amount","%s is not a valid amount",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"No %cDate       %c    In   %c    Out  %cStatus   %cDescription%c","No %cDate       %c    In   %c    Out  %cStatus   %cDescription%c",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Date    %c     In  %c    Out  %c","Date    %c     In  %c    Out  %c",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"%s Account Summary","%s Account Summary",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"%s Account Monthly Summary","%s Account Monthly Summary",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"%s Account Annual Summary","%s Account Annual Summary",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Month   %c    In   %c    Out  %c    Net  %c Balance%c","Month   %c    In   %c    Out  %c    Net  %c Balance%c",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Year %c    In   %c    Out  %c    Net  %c Balance %c    In %% %c    Out %%%c","Year %c    In   %c    Out  %c    Net  %c Balance %c    In %% %c    Out %%%c",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"%s Account Search Results","%s Account Search Results",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"%s\tIn: %s\tOut: %s\tBalance: %s","%s\tIn: %s\tOut: %s\tBalance: %s",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Totals",           "Totals",            "",      "",      "",       "",       "",     "",      "",     "",    "",
	"No server has been specified",          "No server has been specified",  "",      "",      "",       "",       "",     "",      "",     "",    ""
};

/*Eng (uk),         Eng (US),            French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese */
char * international_text7[] = {
	"month",            "month",             "",      "",      "",       "",       "",     "",      "",     "",    "",
	"mon",              "mon",               "",      "",      "",       "",       "",     "",      "",     "",    "",

	"year",             "year",              "",      "",      "",       "",       "",     "",      "",     "",    "",
	"startyear",        "startyear",         "",      "",      "",       "",       "",     "",      "",     "",    "",

	"endyear",          "endyear",           "",      "",      "",       "",       "",     "",      "",     "",    "",
	"endyr",            "endyr",             "",      "",      "",       "",       "",     "",      "",     "",    "",

	"january",          "january",           "",      "",      "",       "",       "",     "",      "",     "",    "",
	"jan",              "jan",               "",      "",      "",       "",       "",     "",      "",     "",    "",

	"feburary",         "feburary",          "",      "",      "",       "",       "",     "",      "",     "",    "",
	"feb",              "feb",               "",      "",      "",       "",       "",     "",      "",     "",    "",

	"march",            "march",             "",      "",      "",       "",       "",     "",      "",     "",    "",
	"mar",              "mar",               "",      "",      "",       "",       "",     "",      "",     "",    "",

	"april",            "april",             "",      "",      "",       "",       "",     "",      "",     "",    "",
	"apr",              "apr",               "",      "",      "",       "",       "",     "",      "",     "",    "",

	"may",              "may",               "",      "",      "",       "",       "",     "",      "",     "",    "",
	"may",              "may",               "",      "",      "",       "",       "",     "",      "",     "",    "",

	"june",             "june",              "",      "",      "",       "",       "",     "",      "",     "",    "",
	"jun",              "jun",               "",      "",      "",       "",       "",     "",      "",     "",    "",

	"july",             "july",              "",      "",      "",       "",       "",     "",      "",     "",    "",
	"jul",              "jul",               "",      "",      "",       "",       "",     "",      "",     "",    "",

	"august",           "august",            "",      "",      "",       "",       "",     "",      "",     "",    "",
	"aug",              "aug",               "",      "",      "",       "",       "",     "",      "",     "",    "",

	"september",        "september",         "",      "",      "",       "",       "",     "",      "",     "",    "",
	"sep",              "sep",               "",      "",      "",       "",       "",     "",      "",     "",    "",

	"october",          "october",           "",      "",      "",       "",       "",     "",      "",     "",    "",
	"oct",              "oct",               "",      "",      "",       "",       "",     "",      "",     "",    "",

	"november",         "november",          "",      "",      "",       "",       "",     "",      "",     "",    "",
	"nov",              "nov",               "",      "",      "",       "",       "",     "",      "",     "",    "",

	"december",         "december",          "",      "",      "",       "",       "",     "",      "",     "",    "",
	"dec",              "dec",               "",      "",      "",       "",       "",     "",      "",     "",    "",

	"Account changed to %s","Account changed to %s", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Currency changed to %s","Currency changed to %s", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Currency must contain three letters (eg. USD)","Currency must contain three letters (eg. USD)", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"#CANCEL %s",       "#CANCEL %s",        "",      "",      "",       "",       "",     "",      "",     "",    "",
	"X",                "X",                 "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Date format changed to %s", "Date format changed to %s", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Data exported to %s", "Data exported to %s", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"separator",        "separator",         "",      "",      "",       "",       "",     "",      "",     "",    "",
	"tab",              "tab",               "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Month,In,Out,Net,Balance", "Month,In,Out,Net,Balance", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Year,In,Out,Net,Balance,In % Change,Out % Change", "Year,In,Out,Net,Balance,In % Change,Out % Change", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"#REF %s #TRANSFER to %s", "#REF %s #TRANSFER to %s", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"#REF %s #TRANSFER from %s", "#REF %s #TRANSFER from %s", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Transferred %s from %s to %s", "Transferred %s from %s to %s", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Language",         "Language", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Language changed to %s", "Language changed to %s", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"kml",              "kml", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Version %.3f",     "Version %.3f", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"%d transactions imported from %s",     "%d transactions imported from %s", "",      "",      "",       "",       "",     "",      "",     "",    ""
};

/*Eng (uk),         Eng (US),            French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese */
char * international_text8[] = {
	"money in",         "money in",          "",      "",      "",       "",       "",     "",      "",     "",    "",
	"money out",        "money out",         "",      "",      "",       "",       "",     "",      "",     "",    "",
	"credit",           "credit",            "",      "",      "",       "",       "",     "",      "",     "",    "",
	"debit",            "debit",             "",      "",      "",       "",       "",     "",      "",     "",    "",
	"amount",           "amount",            "",      "",      "",       "",       "",     "",      "",     "",    "",
	"memo",             "memo",              "",      "",      "",       "",       "",     "",      "",     "",    "",
	"notes",            "notes",             "",      "",      "",       "",       "",     "",      "",     "",    "",
	"category",         "category",          "",      "",      "",       "",       "",     "",      "",     "",    "",
	"subcategory",      "subcategory",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"description",      "description",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"name",             "name",              "",      "",      "",       "",       "",     "",      "",     "",    "",
	"type",             "type",              "",      "",      "",       "",       "",     "",      "",     "",    "",
	"in",               "in",                "",      "",      "",       "",       "",     "",      "",     "",    "",
	"out",              "out",               "",      "",      "",       "",       "",     "",      "",     "",    "",
	"balance",          "balance",           "",      "",      "",       "",       "",     "",      "",     "",    "",

	"Nothing found to plot","Nothing found to plot",   "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Spending",         "Spending",          "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Spending for %d",  "Spending for %s",   "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Income",           "Income",            "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Income for %d",    "Income for %s",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Monthly Volumes",  "Monthly Volumes",   "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Yearly Volumes",   "Yearly Volumes",    "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Spending on %s for %s account",         "Spending on %s for %s account", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Balance for %s account",                "Balance for %s account",        "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Record number %s undone",               "Record number %s undone",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Record number %s edited",               "Record number %s edited",       "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Exported image %s",                     "Exported image %s",             "",      "",      "",       "",       "",     "",      "",     "",    "",

	"No transactions found",                 "No transactions found",         "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Spending Distribution for %d", "Spending Distribution for %d",         "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Spending Distribution for %d-%d", "Spending Distribution for %d-%d",         "",      "",      "",       "",       "",     "",      "",     "",    ""
};

/*Eng (uk),         Eng (US),            French,  German,  Italian,  Spanish,  Greek,  Russian, Polish, Dutch, Chinese */
char * international_text9[] = {
	"%s Account",       "%s Account",        "",      "",      "",       "",       "",     "",      "",     "",    "",
	"am",               "am",                "",      "",      "",       "",       "",     "",      "",     "",    "",
	"pm",               "pm",                "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Frequency",        "Frequency",         "",      "",      "",       "",       "",     "",      "",     "",    "",
	"spending",         "spending",          "",      "",      "",       "",       "",     "",      "",     "",    "",
	"currency conversion", "currency conversion", "",      "",      "",       "",       "",     "",      "",     "",    "",
	"pounds",           "pounds",            "",      "",      "",       "",       "",     "",      "",     "",    "",
	"dollars",          "dollars",           "",      "",      "",       "",       "",     "",      "",     "",    "",
	"euros",            "euros",             "",      "",      "",       "",       "",     "",      "",     "",    "",
	"rubles",           "rubles",            "",      "",      "",       "",       "",     "",      "",     "",    "",
	"yuan",             "yuan",              "",      "",      "",       "",       "",     "",      "",     "",    "",
	"yen",              "yen",               "",      "",      "",       "",       "",     "",      "",     "",    "",
	"title",            "title",             "",      "",      "",       "",       "",     "",      "",     "",    "",
	"username",         "username",          "",      "",      "",       "",       "",     "",      "",     "",    "",
	"piechart",         "piechart",          "",      "",      "",       "",       "",     "",      "",     "",    "",
	"percent",          "percent",           "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Other",            "Other",             "",      "",      "",       "",       "",     "",      "",     "",    "",
	"No income found",  "No income found",   "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Name of the GnuCash account to import: ","Name of the GnuCash account to import: ",   "",      "",      "",       "",       "",     "",      "",     "",    "",
	"Recent Edits",     "Recent Edits",      "",      "",      "",       "",       "",     "",      "",     "",    "",
	" %cDate       %c    In   %c    Out  %cStatus   %cDescription%c","No %cDate       %c    In   %c    Out  %cStatus   %cDescription%c",     "",      "",      "",       "",       "",     "",      "",     "",    "",
	"style",            "style",             "",      "",      "",       "",       "",     "",      "",     "",    "",
	"sty",              "sty",               "",      "",      "",       "",       "",     "",      "",     "",    "",
	"emacs",            "emacs",             "",      "",      "",       "",       "",     "",      "",     "",    "",
	"org",              "org",               "",      "",      "",       "",       "",     "",      "",     "",    "",
	"org-mode",         "org-mode",          "",      "",      "",       "",       "",     "",      "",     "",    ""
};

char * international_narrator[] = {
	/* balance (positive)*/
	"The balance of your %s account is %d %s", /*Eng (uk)*/
	"The balance of your %s account is %d %s", /*Eng (US)*/
	"", /*French*/
	"", /*German*/
	"", /*Italian*/
	"", /*Spanish*/
	"", /*Greek*/
	"", /*Russian*/
	"", /*Polish*/
	"", /*Dutch*/
	"", /*Chinese*/

	/* balance (negative)*/
	"Your %s account is %d %s overdrawn", /*Eng (uk)*/
	"Your %s account is %d %s overdrawn", /*Eng (US)*/
	"", /*French*/
	"", /*German*/
	"", /*Italian*/
	"", /*Spanish*/
	"", /*Greek*/
	"", /*Russian*/
	"", /*Polish*/
	"", /*Dutch*/
	"", /*Chinese*/

	/* average income */
	"Average income to your %s account is %d %s per month", /*Eng (uk)*/
	"Average income to your %s account is %d %s per month", /*Eng (US)*/
	"", /*French*/
	"", /*German*/
	"", /*Italian*/
	"", /*Spanish*/
	"", /*Greek*/
	"", /*Russian*/
	"", /*Polish*/
	"", /*Dutch*/
	"", /*Chinese*/

	/* average spending */
	"Average outgoings from your %s account are %d %s per month", /*Eng (uk)*/
	"Average outgoings from your %s account are %d %s per month", /*Eng (US)*/
	"", /*French*/
	"", /*German*/
	"", /*Italian*/
	"", /*Spanish*/
	"", /*Greek*/
	"", /*Russian*/
	"", /*Polish*/
	"", /*Dutch*/
	"", /*Chinese*/

	/* months remaining */
	"Funds will be expended in %d months", /*Eng (uk)*/
	"Funds will be expended in %d months", /*Eng (US)*/
	"", /*French*/
	"", /*German*/
	"", /*Italian*/
	"", /*Spanish*/
	"", /*Greek*/
	"", /*Russian*/
	"", /*Polish*/
	"", /*Dutch*/
	"", /*Chinese*/

	/* years and months remaining */
	"Funds will be expended in %d years and %d months", /*Eng (uk)*/
	"Funds will be expended in %d years and %d months", /*Eng (US)*/
	"", /*French*/
	"", /*German*/
	"", /*Italian*/
	"", /*Spanish*/
	"", /*Greek*/
	"", /*Russian*/
	"", /*Polish*/
	"", /*Dutch*/
	"", /*Chinese*/

	/* Indefinitely sustainable spending rate -
	   a sort of financial escape velocity */
	"This is indefinitely sustainable", /*Eng (uk)*/
	"This is indefinitely sustainable", /*Eng (US)*/
	"", /*French*/
	"", /*German*/
	"", /*Italian*/
	"", /*Spanish*/
	"", /*Greek*/
	"", /*Russian*/
	"", /*Polish*/
	"", /*Dutch*/
	"", /*Chinese*/


	/* months remaining */
	"Debt/overdraft will be repaid in %d months", /*Eng (uk)*/
	"Debt/overdraft will be repaid in %d months", /*Eng (US)*/
	"", /*French*/
	"", /*German*/
	"", /*Italian*/
	"", /*Spanish*/
	"", /*Greek*/
	"", /*Russian*/
	"", /*Polish*/
	"", /*Dutch*/
	"", /*Chinese*/

	/* years and months remaining */
	"Debt/overdraft will be repaid in %d years and %d months", /*Eng (uk)*/
	"Debt/overdraft will be repaid in %d years and %d months", /*Eng (US)*/
	"", /*French*/
	"", /*German*/
	"", /*Italian*/
	"", /*Spanish*/
	"", /*Greek*/
	"", /*Russian*/
	"", /*Polish*/
	"", /*Dutch*/
	"", /*Chinese*/

	/* Indefinitely sustainable spending rate -
	   a sort of financial escape velocity */
	"The debt/overdraft will never be repaid", /*Eng (uk)*/
	"The debt/overdraft will never be repaid", /*Eng (US)*/
	"", /*French*/
	"", /*German*/
	"", /*Italian*/
	"", /*Spanish*/
	"", /*Greek*/
	"", /*Russian*/
	"", /*Polish*/
	"", /*Dutch*/
	"", /*Chinese*/

	/* spent more */
	"You have spent %d%% more than the same time last year", /*Eng (uk)*/
	"You have spent %d%% more than the same time last year", /*Eng (US)*/
	"", /*French*/
	"", /*German*/
	"", /*Italian*/
	"", /*Spanish*/
	"", /*Greek*/
	"", /*Russian*/
	"", /*Polish*/
	"", /*Dutch*/
	"", /*Chinese*/

	/* spent less */
	"You have spent %d%% less than the same time last year", /*Eng (uk)*/
	"You have spent %d%% less than the same time last year", /*Eng (US)*/
	"", /*French*/
	"", /*German*/
	"", /*Italian*/
	"", /*Spanish*/
	"", /*Greek*/
	"", /*Russian*/
	"", /*Polish*/
	"", /*Dutch*/
	"", /*Chinese*/

	/* spent same */
	"You have spent a similar amount to this time last year", /*Eng (uk)*/
	"You have spent a similar amount to this time last year", /*Eng (US)*/
	"", /*French*/
	"", /*German*/
	"", /*Italian*/
	"", /*Spanish*/
	"", /*Greek*/
	"", /*Russian*/
	"", /*Polish*/
	"", /*Dutch*/
	"" /*Chinese*/
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
