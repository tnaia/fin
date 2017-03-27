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

#include "help.h"

void show_help_english()
{
    printf("%s","\nFin: The command line personal finances manager\n");
    printf("%s","-----------------------------------------------\n\n");

    printf("%s","To log an amount of money spent:\n\n");

    printf("%s","   fin spend <amount> on <thing purchased>\n\n");

    printf("%s","   fin spend 2.99 on cheese\n\n");

    printf("%s","Currency symbols such as $ or £ should not be included in the amount.\n\n");

    printf("%s","You might also wish to log where money was spent, so that you can use that as a search criteria later.\n\n");

    printf("%s","   fin spend 25.39 on groceries at supermarket\n\n");

    printf("%s","To receive an amount of money:\n\n");

    printf("%s","   fin receive <amount> from <source>\n\n");

    printf("%s","   fin receive 100 from \"balance brought forward\"\n\n");

    printf("%s","To show recent transactions just type \"fin\".  The most recent is at the top.  If you want to list more transactions:\n\n");

    printf("%s","   fin transactions <number>\n\n");

    printf("%s","You can also see aggregated figures:\n\n");

    printf("%s","   fin monthly\n\n");

    printf("%s","   fin yearly\n\n");

    printf("%s","For more details see the README file.\n\n");
}

void show_help()
{
    char * language = settings_get_language();

    if ((strcmp(language,"english uk") == 0) ||
        (strcmp(language,"english us") == 0))
        show_help_english();
}
