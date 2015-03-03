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

#include "lock.h"

int is_locked(char * directory)
{
  FILE * fp;
  char filename[STRING_BLOCK];
  int locked = 0;

  sprintf((char*)filename,"%s/settings.txt.bfe",directory);
  fp = fopen(filename,"r");
  if (fp!=0) {
    locked = 1;
    fclose(fp);
  }

  if (locked==0) {
    sprintf((char*)filename,"%s/current.sqlite3.bfe",directory);
    fp = fopen(filename,"r");
    if (fp!=0) {
      locked = 1;
      fclose(fp);
    }
  }

  return locked;
}

int lock()
{
  char directory[STRING_BLOCK];
  char command[STRING_BLOCK];
  int retval=0;

  database_directory(directory);

  if (is_locked(directory)==0) {
    sprintf((char*)command,"bcrypt %s/*", directory);
    retval = system(command);
  }
  return retval;
}

int unlock()
{
  char directory[STRING_BLOCK];
  char command[STRING_BLOCK];

  database_directory(directory);
  if (is_locked(directory)!=0) {
    sprintf((char*)command,"bcrypt %s/*.bfe", directory);
    if (system(command)!=0) {
      printf("Command failed\n");
    }
  }
  return is_locked(directory);
}
