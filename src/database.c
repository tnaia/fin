/*
  Fin - a simple personal accounts manager
  Copyright (C) 2012-2013  Bob Mottram <bob@robotics.uk.to>

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

#include "database.h"

void database_directory(char * directory)
{
  sprintf(directory,"%s/.fin",getenv("HOME"));
}

void database_name(char * account, char * filename)
{
  char directory[STRING_BLOCK];

  database_directory(directory);
  sprintf(filename,"%s/%s.sqlite3",directory,account);
}

int file_exists(char * name)
{
  FILE * fp = fopen(name,"r");
  if (fp!=0) {
    fclose(fp);
    return 1;
  }
  return 0;
}

int directory_exists(char * name)
{
  struct stat sts;
  if ((stat(name, &sts)) == -1) {
    return 0;
  }
  return 1;
}

int database_backup(char * backup_filename, char * lock)
{
  FILE * fp;
  int retval,i,contains_path;
  char directory[STRING_BLOCK];
  char command[STRING_BLOCK];
  char * script_filename = "temp_fin_script.sh";

  /* get the directory of the database files */
  database_directory(directory);

  /* save a script */
  fp = fopen(script_filename, "w");
  if (fp) {
    /* does the backup filename contain a full path? */
    contains_path = 0;
    for (i = 0; i < strlen(backup_filename); i++) {
      if (backup_filename[i] == '/') {
	contains_path = 1;
	break;
      }
    }

    /* save a script */
    fprintf(fp, "%s", "#!/bin/bash\n\n");
    if (contains_path == 0) {
      fprintf(fp, "%s", "CURRENT_FIN_DIR=$PWD\n");
    }
    fprintf(fp, "cd %s\n", directory);
    fprintf(fp, "tar -czf %s *\n", backup_filename);
    if (contains_path == 0) {
      fprintf(fp, "mv %s $CURRENT_FIN_DIR\n",
	      backup_filename);
    }
    fclose(fp);

    sprintf(command, "chmod +x %s", script_filename);
    retval = system(command);

    sprintf(command, "./%s", script_filename);
    if (lock == 0) {
      retval = system(command);

      /* remove the temporary script */
      sprintf(command, "rm %s", script_filename);
      retval = system(command);
      return retval;
    }
    /* encrypt the backup */
    if (system(command) == 0) {
      /* remove the temporary script */
      sprintf(command, "rm %s", script_filename);
      retval = system(command);

      sprintf(command, "bcrypt %s",
	      backup_filename);
      retval = system(command);
      return retval;
    }
  }
  return -1;
}

int database_restore(char * backup_filename)
{
  char directory[STRING_BLOCK];
  char command[STRING_BLOCK];
  int len = strlen(backup_filename);
  int unlocked = 0;
  char temp_filename[STRING_BLOCK];

  if (file_exists(backup_filename)==0) {
    /* file was not found */
    return -1;
  }

  if (len > 4) {
    if ((backup_filename[len-1]=='e') &&
	(backup_filename[len-2]=='f') &&
	(backup_filename[len-3]=='b') &&
	(backup_filename[len-4]=='.')) {

      /* copy the backup file to a temporary file */
      sprintf(temp_filename, "%s",
	      "temp_fin_backup.bfe");
      sprintf(command, "cp %s %s",
	      backup_filename, temp_filename);
      len = system(command);

      /* unlock */
      sprintf(command, "bcrypt %s", temp_filename);
      len = system(command);
      temp_filename[strlen(temp_filename)-4] = 0;
      unlocked = 1;
    }
  }

  database_directory(directory);
  if (unlocked == 0) {
    sprintf(command, "tar -C %s -zxpf %s",
	    directory, backup_filename);
    len = system(command);
  }
  else {
    sprintf(command, "tar -C %s -zxpf %s",
	    directory, temp_filename);
    len = system(command);

    /* delete the unencrypted file */
    sprintf(command, "shred -u %s",
	    temp_filename);
    len = system(command);
  }
  return 1;
}

int account_exists(char * name)
{
  FILE * fp;
  char str[STRING_BLOCK];
  char directory[STRING_BLOCK];

  database_directory(directory);
  sprintf((char*)str,"%s/%s.sqlite3",directory, name);
  fp = fopen(str,"r");
  if (fp!=0L) {
    fclose(fp);
    return 1;
  }
  return 0;
}

int create_sqlite_adjustments_table(char * account)
{
  int retval;
  char database_filename[128];

  /* Create a handle for database connection, create a pointer to sqlite3 */
  sqlite3 *handle;

  char create_adjustments_table[] =
    "CREATE TABLE IF NOT EXISTS adjustments (" \
    "adjustment_date DATETIME," \
    "adjustment_type CHAR(1)," \
    "transaction_number TEXT," \
    "date DATETIME," \
    "qty DECIMAL," \
    "description TEXT," \
    "spend MONEY,"  \
    "receive MONEY," \
    "recur INTEGER," \
    "currency CHAR(3)," \
    "status TEXT," \
    "invoice_number TEXT," \
    "sales_number TEXT," \
    "purchase_number TEXT," \
    "works_number TEXT," \
    "job_number TEXT," \
    "serial_number TEXT," \
    "part_number TEXT," \
    "tracking_number TEXT," \
    "progress DECIMAL," \
    "latitude DECIMAL," \
    "longitude DECIMAL," \
    "altitude DECIMAL," \
    "supplier TEXT," \
    "customer TEXT," \
    "vatrate DECIMAL" \
    ")";

  /* try to create the database. If it doesnt exist, it would be created
     pass a pointer to the pointer to sqlite3, in short sqlite3 */
  database_name(account, (char*)database_filename);
  retval = sqlite3_open_v2(database_filename,
			   &handle,
			   SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
			   NULL);
  /* If connection failed, handle returns NULL */
  if (retval) {
    printf("Sqlite3 database connection failed\n");
    return 0;
  }

  retval = sqlite3_exec(handle,create_adjustments_table,0,0,0);
  if (retval) {
    printf("Adjustments table creation failed. %s\n%s\n",
	   sqlite3_errmsg(handle),create_adjustments_table);
    sqlite3_close(handle);
    return 0;
  }

  sqlite3_close(handle);
  return 1;
}

int create_sqlite_account(char * name)
{
  int retval;
  char database_filename[128];

  /* Create a handle for database connection, create a pointer to sqlite3 */
  sqlite3 *handle;

  char create_transactions_table[] =
    "CREATE TABLE IF NOT EXISTS transactions (" \
    "transaction_number TEXT PRIMARY KEY," \
    "date DATETIME," \
    "qty DECIMAL," \
    "description TEXT," \
    "spend MONEY,"  \
    "receive MONEY," \
    "recur INTEGER," \
    "currency CHAR(3)," \
    "status TEXT," \
    "invoice_number TEXT," \
    "sales_number TEXT," \
    "purchase_number TEXT," \
    "works_number TEXT," \
    "job_number TEXT," \
    "serial_number TEXT," \
    "part_number TEXT," \
    "tracking_number TEXT," \
    "progress DECIMAL," \
    "last_modified DATETIME," \
    "latitude DECIMAL," \
    "longitude DECIMAL," \
    "altitude DECIMAL," \
    "supplier TEXT," \
    "customer TEXT," \
    "vatrate DECIMAL" \
    ")";

  /* try to create the database. If it doesnt exist, it would be created
     pass a pointer to the pointer to sqlite3, in short sqlite3 */
  database_name(name, (char*)database_filename);
  retval = sqlite3_open_v2(database_filename,
			   &handle,
			   SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
			   NULL);
  /* If connection failed, handle returns NULL */
  if (retval) {
    printf("Sqlite3 database connection failed\n");
    return 0;
  }

  retval = sqlite3_exec(handle,create_transactions_table,0,0,0);
  if (retval) {
    printf("Transactions table creation failed. %s\n%s\n",
	   sqlite3_errmsg(handle),create_transactions_table);
    sqlite3_close(handle);
    return 0;
  }

  sqlite3_close(handle);

  /* create the adjustments table */
  return create_sqlite_adjustments_table(name);
}


int create_sqlite_database()
{
  int retval=0;
  char str[STRING_BLOCK];
  char directory[STRING_BLOCK];

  database_directory(directory);

  if (directory_exists(directory)==0) {
    sprintf((char*)str,"mkdir %s",directory);
    retval = system(str);
  }
  create_sqlite_account("current");
  create_sqlite_account("savings");
  return retval;
}

void get_current_datetime(char * result)
{
  time_t now;
  struct tm *d;

  time(&now);
  d = localtime(&now);
  sprintf(result,"%04d-%02d-%02d %02d:%02d:%02d",
	  d->tm_year+YEAR_BASELINE, d->tm_mon+1, d->tm_mday,
	  d->tm_hour, d->tm_min, d->tm_sec);
}

void get_transaction_identifier(char * spend, char * receive,
				char * description,
				char * transaction_identifier)
{
  int i,j,tot;
  char str[16];
  char nums[128];
  time_t now;
  int randval = (int)(rand()%999999);
  struct tm *d;

  j=0;
  for (i=0;i<strlen(spend);i++) {
    if ((spend[i]>='0') && (spend[i]<='9')) {
      nums[j++] = spend[i];
    }
  }
  for (i=0;i<strlen(receive);i++) {
    if ((receive[i]>='0') && (receive[i]<='9')) {
      nums[j++] = receive[i];
    }
  }
  nums[j]=0;
  time(&now);
  d = localtime(&now);
  strftime(str, 16, "%Y%m%d%H%M%S", d);
  tot = 0;
  for (i=0;i<strlen(description);i++) {
    tot += (int)description[i];
  }
  sprintf(transaction_identifier,"%s%06d%s%03d",str,randval,nums,tot%1000);
}

int database_save_transaction(
			      char * account, char * date_format,
			      char * quantity, char * description,
			      char * spend, char * receive,
			      char * date, char * timestr, int recur,
			      char * currency, char * status,
			      char * invoice_number, char * sales_number,
			      char * purchase_number, char * works_number,
			      char * job_number, char * serial_number,
			      char * part_number, char * tracking_number,
			      float progress, char * latitude,
			      char * longitude, char * altitude,
			      char * supplier, char * customer, char * VATrate)
{
  int retval;
  char database_filename[STRING_BLOCK];
  char query[STRING_BLOCK_LONG];
  char lat[32],lng[32],alt[32];
  char transaction_identifier[STRING_BLOCK];
  char curr[STRING_BLOCK],date_str[STRING_BLOCK];
  char datetime[STRING_BLOCK];

  /* Create a handle for database connection, create a pointer to sqlite3 */
  sqlite3 *handle;

  if ((date==0) || (strlen(date)==0)) {
    printf("No date specified\n");
    return 0;
  }
  if (is_date(date, date_format)==0) {
    printf("%s is not a valid date format\n", date);
    return 0;
  }

  /* if no account is specified use the default one */
  if (strlen(account)==0) {
    settings_set_account(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT));
    if (strlen(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT))==0) {
      printf("Unable to get default account name\n");
      return 0;
    }
    account = settings_get_account();
  }

  remove_currency_symbol(spend);
  remove_currency_symbol(receive);

  /* if no currency is given then use the default */
  curr[0]=0;
  if (strlen(currency)==0) {
    settings_set_currency(get_text_from_identifier(SETTINGS_DEFAULT_CURRENCY));
    if (strlen(get_text_from_identifier(SETTINGS_DEFAULT_CURRENCY))==0) {
      printf("Unable to get default currency value\n");
      return 0;
    }
    sprintf(curr,"%s",settings_get_currency());
  }
  else {
    sprintf((char*)curr,"%s",currency);
  }

  create_sqlite_database();

  if (strlen(account)==0) {
    printf("No account specified\n");
    return 0;
  }

  create_sqlite_account(account);

  database_name(account, (char*)database_filename);
  retval = sqlite3_open_v2(database_filename,
			   &handle,
			   SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
			   NULL);
  /* If connection failed, handle returns NULL */
  if (retval) {
    printf("Sqlite3 database connection failed\n");
    return 0;
  }

  sprintf(lat,"%s",latitude);
  if (strlen(latitude)==0) {
    sprintf(lat,"%s","0");
  }
  sprintf(lng,"%s",longitude);
  if (strlen(longitude)==0) {
    sprintf(lng,"%s","0");
  }
  sprintf(alt,"%s",altitude);
  if (strlen(altitude)==0) {
    sprintf(alt,"%s","0");
  }

  convert_date_to_standard(date,date_format,date_str);

  get_transaction_identifier(spend,receive,
			     description,
			     (char*)transaction_identifier);

  if (strlen(timestr)>0) {
    sprintf(datetime,"%s %s",date_str,timestr);
  }
  else {
    sprintf(datetime,"%s",date_str);
  }

  sprintf((char*)query,
	  "INSERT INTO transactions " \
	  "VALUES(\"%s\",\"%s\",%s,\"%s\",%s,%s,%d,\"%s\"," \
	  "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"," \
	  "\"%s\",\"%s\",%.3f,\"%s\",%s,%s,%s,\"%s\",\"%s\",%s);",
	  transaction_identifier,(char*)datetime,quantity,
	  description,spend,receive,recur,curr,status,
	  invoice_number,sales_number,purchase_number,
	  works_number,job_number,serial_number,
	  part_number,tracking_number,progress,(char*)datetime,
	  lat,lng,
	  alt,supplier,customer,VATrate);

  retval = sqlite3_exec(handle,query,0,0,0);
  if (retval) {
    printf("Failed to insert record into transactions table. " \
	   "%s\nTransaction %s\n%s\n",sqlite3_errmsg(handle),
	   transaction_identifier,query);
    sqlite3_close(handle);
    return 0;
  }

  sqlite3_close(handle);
  return 1;
}

int database_save_adjustment(
			     char * account,
			     char * current_datetime,
			     char adjustment_type,
			     char * transaction_identifier,
			     char * quantity, char * description,
			     char * spend, char * receive,
			     char * date, char * timestr, int recur,
			     char * currency, char * status,
			     char * invoice_number, char * sales_number,
			     char * purchase_number, char * works_number,
			     char * job_number, char * serial_number,
			     char * part_number, char * tracking_number,
			     float progress, char * latitude,
			     char * longitude, char * altitude,
			     char * supplier, char * customer, char * VATrate)
{
  int retval;
  char database_filename[STRING_BLOCK];
  char query[STRING_BLOCK_LONG];
  char lat[32],lng[32],alt[32];
  char curr[STRING_BLOCK];
  char datetime[STRING_BLOCK];

  /* Create a handle for database connection, create a pointer to sqlite3 */
  sqlite3 *handle;

  if ((date==0) || (strlen(date)==0)) {
    printf("No date specified\n");
    return 0;
  }

  /* date is expected to be in the standard format */
  if (is_date(date, "%Y-%m-%d")==0) {
    printf("%s is not a valid date format\n", date);
    return 0;
  }

  /* if no account is specified use the default one */
  if (strlen(account)==0) {
    settings_set_account(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT));
    if (strlen(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT))==0) {
      printf("Unable to get default account name\n");
      return 0;
    }
    account = settings_get_account();
  }

  remove_currency_symbol(spend);
  remove_currency_symbol(receive);

  /* if no currency is given then use the default */
  curr[0]=0;
  if (strlen(currency)==0) {
    settings_set_currency(get_text_from_identifier(SETTINGS_DEFAULT_CURRENCY));
    if (strlen(get_text_from_identifier(SETTINGS_DEFAULT_CURRENCY))==0) {
      printf("Unable to get default currency value\n");
      return 0;
    }
    sprintf(curr,"%s",settings_get_currency());
  }
  else {
    sprintf((char*)curr,"%s",currency);
  }

  create_sqlite_database();

  if (strlen(account)==0) {
    printf("No account specified\n");
    return 0;
  }

  create_sqlite_account(account);

  database_name(account, (char*)database_filename);
  retval = sqlite3_open_v2(database_filename,
			   &handle,
			   SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
			   NULL);
  /* If connection failed, handle returns NULL */
  if (retval) {
    printf("Sqlite3 database connection failed\n");
    return 0;
  }

  sprintf(lat,"%s",latitude);
  if (strlen(latitude)==0) {
    sprintf(lat,"%s","0");
  }
  sprintf(lng,"%s",longitude);
  if (strlen(longitude)==0) {
    sprintf(lng,"%s","0");
  }
  sprintf(alt,"%s",altitude);
  if (strlen(altitude)==0) {
    sprintf(alt,"%s","0");
  }

  if (strlen(timestr)>0) {
    sprintf(datetime,"%s %s",date,timestr);
  }
  else {
    sprintf(datetime,"%s",date);
  }

  sprintf((char*)query,
	  "INSERT INTO adjustments VALUES(" \
	  "\"%s\",\"%c\"," \
	  "\"%s\",\"%s\",%s,\"%s\",%s,%s,%d,\"%s\"," \
	  "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"," \
	  "\"%s\",\"%s\",%.3f,%s,%s,%s,\"%s\",\"%s\",%s);",
	  current_datetime, adjustment_type,
	  transaction_identifier,(char*)datetime,quantity,
	  description,spend,receive,recur,curr,status,
	  invoice_number,sales_number,purchase_number,
	  works_number,job_number,serial_number,
	  part_number,tracking_number,progress,
	  lat,lng,
	  alt,supplier,customer,VATrate);

  retval = sqlite3_exec(handle,query,0,0,0);
  if (retval) {
    printf("Failed to insert record into adjustments table. " \
	   "%s\n%s\n",sqlite3_errmsg(handle),
	   query);
    sqlite3_close(handle);
    return 0;
  }

  sqlite3_close(handle);
  return 1;
}

int get_balance(
		char * account,
		char ** currency, char ** total_spent,
		char ** total_received, char ** balance)
{
  int retval,col,cols;
  char database_filename[STRING_BLOCK];
  char query[] = "select currency, sum(spend) as totalspend," \
    " sum(receive) as totalreceive, sum(receive-spend) as " \
    "balance from transactions group by currency;";
  int row=0;

  /* A prepered statement for fetching tables */
  sqlite3_stmt *stmt;

  /* Create a handle for database connection, create a pointer to sqlite3 */
  sqlite3 *handle;

  if (account_exists(account)==0) {
    return 0;
  }

  /* if no account is specified use the default one */
  if (strlen(account)==0) {
    settings_set_account(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT));
    if (strlen(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT))==0) {
      printf("Unable to get default account name\n");
      return 0;
    }
    account = settings_get_account();
  }

  create_sqlite_database();

  if (strlen(account)==0) {
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
  if(retval) {
    printf("Unable to retrieve balance information. %s\n",
	   sqlite3_errmsg(handle));
    sqlite3_close(handle);
    return 0;
  }

  cols = sqlite3_column_count(stmt);

  row = 0;
  while(1) {
    retval = sqlite3_step(stmt);

    if (retval == SQLITE_ROW) {
      for(col=0; col < cols; col++) {
	const char *val = (const char*)sqlite3_column_text(stmt,col);
	if (col==0) {
	  sprintf((char*)&currency[row],"%s",val);
	}
	if (col==1) {
	  sprintf((char*)&total_spent[row],"%s",val);
	}
	if (col==2) {
	  sprintf((char*)&total_received[row],"%s",val);
	}
	if (col==3) {
	  sprintf((char*)&balance[row],"%s",val);
	}
      }
      row++;
      ((char*)&total_spent[row])[0]=0;
      ((char*)&total_received[row])[0]=0;
      ((char*)&balance[row])[0]=0;
      if (row>=MAX_CURRENCIES-1) break;
    }
    else if(retval == SQLITE_DONE) {
      break;
    }
    else {
      printf("Some error encountered while running balance query\n");
      sqlite3_close(handle);
      return 0;
    }
  }

  retval = sqlite3_finalize(stmt);
  if (retval != SQLITE_OK) {
    printf("get_balance finalize error %d\n",retval);
  }
  sqlite3_close(handle);
  return 1;
}

int clear_transaction_numbers()
{
  FILE * fp;
  char filename[STRING_BLOCK];
  char directory[STRING_BLOCK];

  database_directory(directory);

  sprintf((char*)filename,"%s/transaction_numbers",directory);

  fp = fopen(filename,"w");
  if (fp!=0L) {
    fclose(fp);
  }

  return 1;
}

int save_transaction_number(char * transaction_number)
{
  FILE * fp;
  char filename[STRING_BLOCK];
  char directory[STRING_BLOCK];

  database_directory(directory);

  sprintf((char*)filename,"%s/transaction_numbers",directory);

  fp = fopen(filename,"a");
  if (fp!=0L) {
    fprintf(fp,"%s\n",transaction_number);
    fclose(fp);
  }

  return 1;
}

int get_transaction_number_from_record_number(int record_number,
					      char * transaction_number)
{
  FILE * fp;
  char filename[STRING_BLOCK];
  char str[STRING_BLOCK];
  int rec=1;
  int found = 0;
  char directory[STRING_BLOCK];

  database_directory(directory);

  transaction_number[0]=0;

  sprintf((char*)filename,"%s/transaction_numbers",directory);

  fp = fopen(filename,"r");
  if (fp!=0L) {
    while (!feof(fp)) {
      if (fgets(str,64,fp) != NULL) {
	if(rec==record_number) {
	  sprintf(transaction_number,"%s",str);
	  /* remove carriage return */
	  transaction_number[strlen(transaction_number)-1]=0;
	  found=1;
	  break;
	}
	rec++;
      }
    }
    fclose(fp);
  }
  return found;
}

int database_edit_record(
			 char * account,
			 int record_number,
			 char * prev_date, char * prev_timestr,
			 char * prev_status, char * prev_description,
			 char * prev_currency, char *prev_VATrate,
			 float prev_progress, char * prev_supplier,
			 char * prev_receive, char * prev_spend,
			 char * date, char * timestr,
			 char * status, char * description,
			 char * currency, char *VATrate,
			 float progress, char * supplier,
			 char * receive, char * spend,
			 char * date_format)
{
  int retval,tries,max_tries=4;
  char database_filename[STRING_BLOCK];
  char query[STRING_BLOCK_LONG];
  char transaction_identifier[STRING_BLOCK];
  char current_datetime[STRING_BLOCK];
  char datetime[STRING_BLOCK];

  /* Create a handle for database connection, create a pointer to sqlite3 */
  sqlite3 *handle;

  if (account_exists(account)==0) {
    return 0;
  }

  /* get the transaction number */
  if (get_transaction_number_from_record_number(record_number,
						(char*)transaction_identifier)==0) {
    return 0;
  }

  if (strlen(timestr)>0) {
    sprintf(datetime,"%s %s",date,timestr);
  }
  else {
    sprintf(datetime,"%s",date);
  }

  sprintf((char*)query,
	  "update transactions set status=\"%s\"," \
	  "date=\"%s\"," \
	  "description=\"%s\",currency='%s'," \
	  "receive=%s,spend=%s,progress=%f where " \
	  "transaction_number = \"%s\"",
	  status,datetime,description,currency,receive,spend,
	  progress,transaction_identifier);

  if (strlen(account)==0) {
    printf("No account specified\n");
    return 0;
  }
  database_name(account, (char*)database_filename);
  retval = sqlite3_open((char*)database_filename,&handle);
  /* If connection failed, handle returns NULL */
  if (retval) {
    printf("Sqlite3 database connection failed\n");
    sqlite3_close(handle);
    return 0;
  }

  tries=0;
  while (tries<max_tries) {
    retval = sqlite3_exec(handle,query,0,0,0);
    if (retval == 5) {
      fflush(stdout);
      sleep(1);
    }
    else {
      if (retval) {
	printf("Unable to run edit query.  %s\n",
	       sqlite3_errmsg(handle));
	printf("%s\n",(char*)query);
	sqlite3_close(handle);
	return 0;
      }
      else {
	break;
      }
    }
    tries++;
  }

  if (tries==max_tries) {
    printf("%s\n",sqlite3_errmsg(handle));
    printf("%s\n",(char*)query);
    sqlite3_close(handle);
    return 0;
  }
  sqlite3_close(handle);

  /* save a corresponding adjustment */

  /* ensure that the adjustments table exists */
  create_sqlite_adjustments_table(account);

  /* get the current date and time */
  get_current_datetime(current_datetime);

  /* add a record for the before state */
  database_save_adjustment_short(
				 account,
				 current_datetime,
				 ADJUSTMENT_TYPE_BEFORE,
				 transaction_identifier,
				 prev_date, prev_timestr,
				 prev_status, prev_description,
				 prev_currency, prev_VATrate,
				 prev_progress, prev_supplier,
				 prev_receive, prev_spend);

  /* add a record for the after state */
  database_save_adjustment_short(
				 account,
				 current_datetime,
				 ADJUSTMENT_TYPE_AFTER,
				 transaction_identifier,
				 date, timestr,
				 status, description,
				 currency, VATrate,
				 progress, supplier,
				 receive, spend);

  return 1;
}


int database_delete_record(char * account, int record_number)
{
  int retval,tries,max_tries=4;
  char database_filename[STRING_BLOCK];
  char query[STRING_BLOCK_LONG];
  char transaction_identifier[STRING_BLOCK];

  /* Create a handle for database connection,
     create a pointer to sqlite3 */
  sqlite3 *handle;
  char current_datetime[STRING_BLOCK];
  char prev_date[STRING_BLOCK];
  char prev_timestr[STRING_BLOCK];
  char prev_status[STRING_BLOCK];
  char prev_description[STRING_BLOCK];
  char prev_currency[STRING_BLOCK];
  char prev_VATrate[STRING_BLOCK];
  char prev_progress[STRING_BLOCK];
  char prev_supplier[STRING_BLOCK];
  char prev_receive[STRING_BLOCK];
  char prev_spend[STRING_BLOCK];

  if (account_exists(account)==0) {
    return 0;
  }

  /* get the transaction number */
  if (get_transaction_number_from_record_number(record_number,
						(char*)transaction_identifier)==0) {
    return 0;
  }

  if (database_get_record(account,record_number,
			  (char*)prev_date, (char*)prev_timestr,
			  (char*)transaction_identifier,
			  (char*)prev_status,(char*)prev_description,
			  (char*)prev_currency,(char*)prev_VATrate,
			  (char*)prev_progress,
			  (char*)prev_supplier,
			  (char*)prev_receive,
			  (char*)prev_spend)==0) {
    printf("Failed to get transaction\n");
    return 0;
  }

  sprintf((char*)query,
	  "delete from transactions where " \
	  "transaction_number = \"%s\"",transaction_identifier);

  if (strlen(account)==0) {
    printf("No account specified\n");
    return 0;
  }
  database_name(account, (char*)database_filename);
  retval = sqlite3_open((char*)database_filename,&handle);
  /* If connection failed, handle returns NULL */
  if (retval) {
    printf("Sqlite3 database connection failed\n");
    sqlite3_close(handle);
    return 0;
  }

  tries=0;
  while (tries<max_tries) {
    retval = sqlite3_exec(handle,query,0,0,0);
    if (retval == 5) {
      fflush(stdout);
      sleep(1);
    }
    else {
      if (retval) {
	printf("Unable to run delete query.  %s\n",
	       sqlite3_errmsg(handle));
	printf("%s\n",(char*)query);
	sqlite3_close(handle);
	return 0;
      }
      else {
	break;
      }
    }
    tries++;
  }

  if (tries==max_tries) {
    printf("%s\n",sqlite3_errmsg(handle));
    printf("%s\n",(char*)query);
    sqlite3_close(handle);
    return 0;
  }
  sqlite3_close(handle);

  /* save a corresponding adjustment */

  /* ensure that the adjustments table exists */
  create_sqlite_adjustments_table(account);

  /* get the current date and time */
  get_current_datetime(current_datetime);

  /* add a record for the before state */
  database_save_adjustment_short(
				 account,
				 current_datetime,
				 ADJUSTMENT_TYPE_DELETE,
				 transaction_identifier,
				 prev_date, prev_timestr,
				 prev_status, prev_description,
				 prev_currency, prev_VATrate,
				 atof(prev_progress), prev_supplier,
				 prev_receive, prev_spend);

  return 1;
}

int database_get_record(
			char * account,
			int record_number,
			char * date, char * timestr,
			char * transaction_number,
			char * status, char * description,
			char * currency, char *VATrate,
			char * progress, char * supplier,
			char * receive, char * spend)
{
  int retval,col,cols;
  char database_filename[STRING_BLOCK];
  char query[STRING_BLOCK_LONG];
  sqlite3_stmt *stmt;
  sqlite3 *handle;

  if (account_exists(account)==0) {
    return 0;
  }

  /* get the transaction number */
  if (get_transaction_number_from_record_number(record_number,
						(char*)transaction_number)==0) {
    printf("Failed to get transaction number\n");
    return 0;
  }

  sprintf((char*)query,
	  "select date,status,description,currency,vatrate," \
	  "progress,supplier,receive,spend from " \
	  "transactions where (status is not \"%s\") and " \
	  "(transaction_number=\"%s\");",
	  get_text_from_identifier(CANCEL_TRANSACTION_STATUS),
	  transaction_number);

  if (strlen(account)==0) {
    printf("No account specified\n");
    return 0;
  }
  database_name(account, (char*)database_filename);
  retval = sqlite3_open_v2((char*)database_filename,
			   &handle,SQLITE_OPEN_READONLY,NULL);
  /* If connection failed, handle returns NULL */
  if (retval) {
    printf("Sqlite3 database connection failed\n");
    sqlite3_close(handle);
    return 0;
  }

  retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
  if(retval) {
    printf("Unable to retrieve balance information. %s\n",
	   sqlite3_errmsg(handle));
    sqlite3_close(handle);
    return 0;
  }

  cols = sqlite3_column_count(stmt);

  while(1) {
    retval = sqlite3_step(stmt);

    if (retval == SQLITE_ROW) {
      for(col=0; col < cols; col++) {
	char *val = (char*)sqlite3_column_text(stmt,col);
	if (col==0) {
	  if (strlen(val)>10) {
	    strncpy((char*)date,val,10);
	    strncpy((char*)timestr,&val[11],8);
	  }
	  else {
	    sprintf((char*)date,"%s",val);
	    sprintf((char*)timestr,"%s","00:00:00");
	  }
	}
	if (col==1) {
	  sprintf((char*)status,"%s",val);
	}
	if (col==2) {
	  sprintf((char*)description,"%s",val);
	}
	if (col==3) {
	  sprintf((char*)currency,"%s",val);
	}
	if (col==4) {
	  sprintf((char*)VATrate,"%s",val);
	}
	if (col==5) {
	  sprintf((char*)progress,"%s",val);
	}
	if (col==6) {
	  sprintf((char*)supplier,"%s",val);
	}
	if (col==7) {
	  sprintf((char*)receive,"%s",val);
	}
	if (col==8) {
	  sprintf((char*)spend,"%s",val);
	}
      }
    }
    else if(retval == SQLITE_DONE) {
      break;
    }
    else {
      printf("Some error encountered while getting transaction record\n");
      sqlite3_close(handle);
      return 0;
    }
  }

  retval = sqlite3_finalize(stmt);
  if (retval != SQLITE_OK) {
    printf("database_get_record finalize error %d\n",retval);
  }
  sqlite3_close(handle);
  return 1;
}


/* A shorter version of transaction save */
int database_save_short(char * account,
			char * date, char * timestr,
			char * date_format,
			char * status, char * description,
			char * currency, char *VATrate,
			float progress, char * supplier,
			char * receive, char * spend)
{
  return database_save_transaction(
				   account, date_format, "1", description,
				   spend, receive, date, timestr, -1,
				   currency, status,
				   "", "", "", "", "", "",
				   "", "", progress, "0", "0", "0",
				   "", "", VATrate);
}

int database_save_adjustment_short(char * account,
				   char * current_datetime,
				   char adjustment_type,
				   char * transaction_identifier,
				   char * date, char * timestr,
				   char * status, char * description,
				   char * currency, char *VATrate,
				   float progress, char * supplier,
				   char * receive, char * spend)
{
  return database_save_adjustment(
				  account,
				  current_datetime,
				  adjustment_type,
				  transaction_identifier,
				  "1", description,
				  spend, receive, date, timestr, -1,
				  currency, status,
				  "", "", "", "", "", "",
				  "", "", progress, "0", "0", "0",
				  "", "", VATrate);
}


/* returns whether a transaction exists.
   This is used when importing, and it's not infallible since multiple
   transactions of the same amount could be made on the same day */
int database_transaction_exists(
				char * account,
				char * date, char * timestr,
				char * receive, char * spend,
				char * description,
				char * currency)
{
  int retval,exists=0;
  char database_filename[STRING_BLOCK];
  char query[STRING_BLOCK_LONG];
  char datetime[STRING_BLOCK];

  /* A prepered statement for fetching tables */
  sqlite3_stmt *stmt;

  /* Create a handle for database connection, create a pointer to sqlite3 */
  sqlite3 *handle;

  if (account_exists(account)==0) {
    return 0;
  }

  if (timestr[0]==0) {
    sprintf(datetime,"%s",date);
  }
  else {
    if (strlen(timestr)>0) {
      sprintf(datetime,"%s %s",date,timestr);
    }
    else {
      sprintf(datetime,"%s",date);
    }
  }

  if (strlen(datetime)==10) {
    sprintf((char*)query,
	    "select * from transactions where "						\
	    "(strftime(\"%%Y-%%m-%%d\",date)=\"%s\") and (receive=%s) " \
	    "and (spend=%s);",datetime,receive,spend);
  }
  else {
    sprintf((char*)query,
	    "select * from transactions where "						\
	    "(strftime(\"%%Y-%%m-%%d %%H:%%M:%%S\",date)=\"%s\") and " \
	    "(receive=%s) and " \
	    "(spend=%s);",datetime,receive,spend);
  }

  if (strlen(account)==0) {
    printf("No account specified\n");
    return 0;
  }
  database_name(account, (char*)database_filename);
  retval = sqlite3_open_v2((char*)database_filename,
			   &handle,SQLITE_OPEN_READONLY,
			   NULL);
  /* If connection failed, handle returns NULL */
  if (retval) {
    printf("Sqlite3 database connection failed\n");
    sqlite3_close(handle);
    return 0;
  }

  retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
  if(retval) {
    printf("Unable to run query. %s\n",sqlite3_errmsg(handle));
    sqlite3_close(handle);
    return 0;
  }

  while(1) {
    retval = sqlite3_step(stmt);

    if (retval == SQLITE_ROW) {
      exists=1;
    }
    else if(retval == SQLITE_DONE) {
      break;
    }
    else {
      printf("Some error encountered while " \
	     "getting transaction record\n");
      sqlite3_close(handle);
      return 0;
    }
  }

  retval = sqlite3_finalize(stmt);
  if (retval != SQLITE_OK) {
    printf("database_transaction_exists " \
	   "finalize error %d\n",retval);
  }
  sqlite3_close(handle);

  return exists;
}

int get_annual_cashflow(char * account, char * currency,
			int year, int month,
			double * total_receive, double * total_spend)
{
  int retval,col,cols,row;
  char database_filename[STRING_BLOCK];
  char query[STRING_BLOCK_LONG];
  sqlite3_stmt *stmt;
  sqlite3 *handle;

  *total_receive=0;
  *total_spend=0;

  if (account_exists(account)==0) {
    return 0;
  }

  sprintf(query,
	  "select ((substr(date,1,4)*100)+substr(date,6,2)) " \
	  "as yearmonth, sum(receive) as monthreceive, " \
	  "sum(spend) as monthspend, sum(receive-spend) " \
	  "as monthbalance from transactions where ("	\
	  "(yearmonth >= %d%02d)"	\
	  " and (currency is \"%s\")) group by yearmonth " \
	  "order by yearmonth desc;",year-1,month,currency);

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
  if(retval) {
    printf("Selecting recent transactions from " \
	   "%s account failed\n%s\n",account,query);
    sqlite3_close(handle);
    return 0;
  }

  cols = sqlite3_column_count(stmt);

  row=0;
  while(1) {
    retval = sqlite3_step(stmt);

    if(retval == SQLITE_ROW) {
      for(col=0;col<cols;col++) {
	char * val = (char*)sqlite3_column_text(stmt,col);

	if (col==1) {
	  if (is_value(val)!=0) {
	    *total_receive += atof(val);
	  }
	  else {
	    break;
	  }
	}
	if (col==2) {
	  if (is_value(val)!=0) {
	    *total_spend += atof(val);
	  }
	}

      }
      row++;
    }
    else if(retval == SQLITE_DONE) {
      break;
    }
    else {
      printf("Some error encountered while running monthly query\n");
      sqlite3_close(handle);
      return 0;
    }
  }

  sqlite3_close(handle);

  return 1;
}

/* returns the distribution of spending */
int get_distribution(char * account, char * currency,
		     int start_year, int end_year,
		     int * distribution, double maximum,
		     int distribution_size,
		     char * search_string)
{
  int retval,index;
  char database_filename[STRING_BLOCK];
  char query[STRING_BLOCK_LONG],search_sql[STRING_BLOCK*10];
  sqlite3_stmt *stmt;
  sqlite3 *handle;
  int	perform_search = 0;
  double increment = maximum / distribution_size;
  int total_transactions = 0;

  if (account_exists(account)==0) {
    return 0;
  }

  /* clear the distribution array */
  for (index = 0; index < distribution_size; index++) {
    distribution[index] = 0;
  }

  /* has a search string been specified? */
  if (search_string != 0) {
    if (strlen(search_string) > 0) {
      perform_search = 1;
    }
  }
  /* create query */
  if (perform_search == 0) {
    /* no search criteria */
    sprintf(query,
	    "select transaction_number,spend," \
	    "(substr(date,1,4)*1) as yr from transactions where ("	\
	    "(yr >= %d) and (yr <= %d) and " \
	    "(currency is \"%s\"));",
	    start_year,end_year,currency);
  }
  else {
    /* apply single search criteria */
    search_string_to_sql(search_string, search_sql,0,0);
    sprintf(query,
	    "select transaction_number,spend," \
	    "(substr(date,1,4)*1) as yr from transactions where ("	\
	    "(yr >= %d) and (yr <= %d) and (%s)" \
	    " and (currency is \"%s\"));",
	    start_year,end_year,
	    search_sql,	currency);
  }

  /* open database */
  database_name(account, (char*)database_filename);
  retval = sqlite3_open_v2((char*)database_filename,
			   &handle,
			   SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
			   NULL);
  if (retval) {
    printf("Sqlite3 database connection failed\n");
    sqlite3_close(handle);
    return 0;
  }

  /* run query */
  retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
  if(retval) {
    printf("Selecting recent transactions from " \
	   "%s account failed\n%s\n",account,query);
    sqlite3_close(handle);
    return 0;
  }

  if (sqlite3_column_count(stmt) == 0) {
    /* no data */
    return 0;
  }

  /* step through results */
  while(1) {
    retval = sqlite3_step(stmt);

    if(retval == SQLITE_ROW) {
      char * val = (char*)sqlite3_column_text(stmt,1);
      if (is_value(val)!=0) {
	index = (int)(atof(val)/increment);
	if ((index > 0) && (index < distribution_size)) {
	  distribution[index]++;
	  total_transactions++;
	}
      }
    }
    else if(retval == SQLITE_DONE) {
      break;
    }
    else {
      printf("Some error encountered while " \
	     "running spending distribution query\n");
      sqlite3_close(handle);
      return 0;
    }
  }

  /* close database */
  sqlite3_close(handle);

  if (total_transactions > 0) {
    return 1;
  }
  printf("%s\n",get_text_from_identifier(NO_TRANSACTIONS));
  return 0;
}

/* show spending distribution either in CSV or emacs table format */
void show_distribution(int * distribution, int distribution_size,
		       double maximum,
		       char field_separator, char column_separator)
{
  int i;
  double distribution_increment = maximum / distribution_size;

  if (column_separator==' ') {
    /* show as CSV */
    printf("Amount%c Frequency\n", field_separator);
    for (i = 0; i < distribution_size; i++) {
      printf("%.2f%c %d\n",
	     i*distribution_increment,
	     field_separator,
	     distribution[i]);
    }
  }
  else {
    /* show in emacs style */
    printf("%c Amount %c Frequency %c\n", column_separator,
	   column_separator, column_separator);
    for (i = 0; i < distribution_size; i++) {
      printf("%c %.2f %c %d %c\n",
	     column_separator,
	     i*distribution_increment,
	     column_separator,
	     distribution[i],
	     column_separator);
    }
  }
}

/* return the total spent and received in the given month */
int get_month_totals(char * account, char * currency,
		     int year, int month,
		     double * month_receive, double * month_spend)
{
  int retval;
  char database_filename[STRING_BLOCK];
  char query[STRING_BLOCK_LONG];
  sqlite3_stmt *stmt;
  sqlite3 *handle;
  int total_transactions = 0;

  if (account_exists(account)==0) {
    return 0;
  }

  /* create query */
  sprintf(query,
	  "select ((substr(date,1,4)*100)+substr(date,6,2)) " \
	  "as yearmonth,sum(spend) as totalspend," \
	  "sum(receive) as totalreceive "	\
	  "from transactions where " \
	  "((yearmonth == %d%02d) and (currency is \"%s\")) " \
	  "group by yearmonth;",
	  year,month,currency);

  /* open database */
  database_name(account, (char*)database_filename);
  retval = sqlite3_open_v2((char*)database_filename,
			   &handle,
			   SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
			   NULL);
  if (retval) {
    printf("Sqlite3 database connection failed\n");
    sqlite3_close(handle);
    return 0;
  }

  /* run query */
  retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
  if(retval) {
    printf("Selecting monthly totals from " \
	   "%s account failed\n%s\n",account,query);
    sqlite3_close(handle);
    return 0;
  }

  if (sqlite3_column_count(stmt) == 0) {
    /* no data */
    return -1;
  }

  /* step through results */
  while(1) {
    retval = sqlite3_step(stmt);

    if(retval == SQLITE_ROW) {
      char * val = (char*)sqlite3_column_text(stmt,1);
      if (is_value(val)!=0) {
	*month_spend = atof(val);
	total_transactions++;
      }
      val = (char*)sqlite3_column_text(stmt,2);
      if (is_value(val)!=0) {
	*month_receive = atof(val);
	total_transactions++;
      }
    }
    else if(retval == SQLITE_DONE) {
      break;
    }
    else {
      printf("Some error encountered while " \
	     "running monthly totals query\n");
      sqlite3_close(handle);
      return 0;
    }
  }

  /* close database */
  sqlite3_close(handle);

  if (total_transactions >= 2) {
    return 1;
  }
  return -1;
}

int transaction_volume(char * account, char * currency, int year,
		       int monthly, int max_transactions,
		       char field_separator, char column_separator,
		       char * search_string, char * export_filename)
{
  int retval,cols,col,row=0,i,total_transactions=0;
  int width_columns = 22;
  int separator_column = 11;
  char database_filename[STRING_BLOCK];
  char query[STRING_BLOCK_LONG], search_sql[STRING_BLOCK_LONG];
  sqlite3_stmt *stmt;
  sqlite3 *handle;
  FILE * fp_export=NULL;
  char datestr[STRING_BLOCK];
  char volumestr[STRING_BLOCK];

  if (account_exists(account)==0) {
    return 0;
  }

  if (strlen(search_string)!=0) {
    search_string_to_sql(search_string, search_sql, year, 0);
  }

  /* open file for export */
  if (export_filename!=0) {
    if (strlen(export_filename)>2) {
      fp_export = fopen(export_filename,"w");
      if (fp_export!=NULL) {
	if (column_separator == ' ') {
	  fprintf(fp_export,"%s","Date, Volume\n");
	}
	else {
	  fprintf(fp_export,"%s","| Date | Volume |\n");
	}
      }
    }
  }

  sprintf(datestr, "%s",
	  get_text_from_identifier(DATE));
  datestr[0] = toupper(datestr[0]);
  sprintf(volumestr, "%s",
	  get_text_from_identifier(VOLUME));
  volumestr[0] = toupper(volumestr[0]);

  if (fp_export==NULL) {
    if (column_separator == ' ') {
      /* show title */
      for (i = 0; i < width_columns; i++) {
	printf("-");
      }
      printf("\n");

      printf(" %s", datestr);
      for (i = strlen(datestr)+1;
	   i < separator_column; i++) {
	printf(" ");
      }
      printf(" %s\n", volumestr);

      for (i = 0; i < width_columns; i++) {
	printf("-");
      }
      printf("\n");
    }
    else {
      /* emacs style title */
      printf("| %s", datestr);
      for (i = strlen(datestr)+2;
	   i < separator_column; i++) {
	printf(" ");
      }
      printf("| %s", volumestr);
      i += strlen(volumestr)+2;
      while (i < width_columns) {
	printf(" ");
	i++;
      }
      printf("|\n");
      printf("|");
      for (i = 1; i < separator_column; i++) {
	printf("-");
      }
      printf("+");
      for (i = separator_column+1;
	   i < width_columns; i++) {
	printf("-");
      }
      printf("|\n");
    }
  }

  /* create query */
  if (monthly != 0) {
    if (strlen(search_string)==0) {
      if (year < 1) {
	sprintf(query,
		"select substr(date,1,7) " \
		"as yearmonth, sum(1) as volume " \
		"from transactions where " \
		"(currency is \"%s\") " \
		"group by yearmonth order by yearmonth desc;",
		currency);
      }
      else {
	sprintf(query,
		"select substr(date,1,7) " \
		"as yearmonth, sum(1) as volume " \
		"from transactions where " \
		"((currency is \"%s\") and " \
		"(CAST(substr(date,1,4) as INT) is %d))" \
		"group by yearmonth order by yearmonth desc;",
		currency, year);
      }
    }
    else {
      if (year < 1) {
	sprintf(query,
		"select substr(date,1,7) " \
		"as yearmonth, sum(1) as volume " \
		"from transactions where " \
		"((currency is \"%s\") and " \
		"(%s)) " \
		"group by yearmonth order by yearmonth desc;",
		currency, search_sql);
      }
      else {
	sprintf(query,
		"select substr(date,1,7) " \
		"as yearmonth, sum(1) as volume " \
		"from transactions where " \
		"((currency is \"%s\") and " \
		"(%s) and " \
		"(CAST(substr(date,1,4) as INT) is %d)) " \
		"group by yearmonth order by yearmonth desc;",
		currency, search_sql, year);
      }
    }
  }
  else {
    if (strlen(search_string)==0) {
      if (year < 1) {
	sprintf(query,
		"select substr(date,1,4) "	\
		"as year, sum(1) as volume " \
		"from transactions where " \
		"(currency is \"%s\") " \
		"group by year order by year desc;",
		currency);
      }
      else {
	sprintf(query,
		"select substr(date,1,4) "	\
		"as year, sum(1) as volume " \
		"from transactions where " \
		"((currency is \"%s\") and " \
		"(CAST(substr(date,1,4) as INT) is %d))" \
		"group by year order by year desc;",
		currency, year);
      }
    }
    else {
      if (year < 1) {
	sprintf(query,
		"select substr(date,1,4) " \
		"as year, sum(1) as volume " \
		"from transactions where " \
		"((currency is \"%s\") and " \
		"(%s)) " \
		"group by year order by year desc;",
		currency, search_sql);
      }
      else {
	sprintf(query,
		"select substr(date,1,4) " \
		"as year, sum(1) as volume " \
		"from transactions where " \
		"((currency is \"%s\") and " \
		"(%s) and " \
		"(CAST(substr(date,1,4) as INT) is %d)) " \
		"group by year order by year desc;",
		currency, search_sql, year);
      }
    }
  }

  /* open database */
  database_name(account, (char*)database_filename);
  retval = sqlite3_open_v2((char*)database_filename,
			   &handle,
			   SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
			   NULL);
  if (retval) {
    printf("Sqlite3 database connection failed\n");
    sqlite3_close(handle);
    return 0;
  }

  /* run query */
  retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
  if(retval) {
    printf("Selecting volume from " \
	   "%s account failed\n%s\n",account,query);
    sqlite3_close(handle);
    return 0;
  }

  if (sqlite3_column_count(stmt) == 0) {
    /* no data */
    return -1;
  }

  cols = sqlite3_column_count(stmt);

  /* step through results */
  while(1) {
    retval = sqlite3_step(stmt);

    if(retval == SQLITE_ROW) {
      for(col=0; col < cols; col++) {
	char * val = (char*)sqlite3_column_text(stmt,col);

	if (fp_export==NULL) {
	  /* print to console */
	  if (column_separator != ' ') {
	    if (col > 0) {
	      printf(" %c ",column_separator);
	    }
	    else {
	      printf("%c ",column_separator);
	    }
	  }

	  if ((col == 0) &&
	      (column_separator == ' ')) {
	    printf(" ");
	  }

	  printf("%s",val);

	  if (col == 0) {
	    for (i = strlen(val)+3;
		 i < separator_column; i++) {
	      printf(" ");
	    }
	  }
	  else {
	    total_transactions += atoi(val);
	    while (i<width_columns-3-strlen(val)) {
	      printf(" ");
	      i++;
	    }
	  }

	  if (column_separator != ' ') {
	    if (col == cols-1) {
	      printf(" %c",column_separator);
	    }
	  }
	  else {
	    if (col < cols-1) {
	      printf("   ");
	    }
	  }
	}
	else {
	  /* export to file */
	  if (column_separator != ' ') {
	    if (col > 0) {
	      fprintf(fp_export," %c ",column_separator);
	    }
	    else {
	      fprintf(fp_export,"%c ",column_separator);
	    }
	  }

	  fprintf(fp_export,"%s",val);
	  if (column_separator != ' ') {
	    if (col == cols-1) {
	      fprintf(fp_export," %c",column_separator);
	    }
	  }
	  else {
	    if (col < cols-1) {
	      fprintf(fp_export,"%c ",field_separator);
	    }
	  }
	}
      }
      if (fp_export==NULL) {
	printf("\n");
      }
      else {
	fprintf(fp_export,"%s","\n");
      }
    }
    else if(retval == SQLITE_DONE) {
      break;
    }
    else {
      printf("Some error encountered while " \
	     "running volumes query\n");
      sqlite3_close(handle);
      return 0;
    }
    row++;
    if (row >= max_transactions) break;
  }

  if ((fp_export==NULL) &&
      (column_separator == ' ')) {
    /* show footer */
    for (i = 0; i < width_columns; i++) {
      printf("-");
    }
    printf("\n");
    for (i = 0; i <= separator_column; i++) {
      printf(" ");
    }
    printf("%d\n\n",total_transactions);
  }

  /* close database */
  sqlite3_close(handle);

  if (fp_export!=NULL) {
    fclose(fp_export);
  }

  return 1;
}

/* returns total spend and receive values for the given search string */
int get_totals(char * account, char * search_string,
	       int month_number, int year,
	       char * currency,
	       double * total_receive, double * total_spend)
{
  int retval, col, cols;
  char database_filename[STRING_BLOCK];
  char query[STRING_BLOCK_LONG];
  char fields[STRING_BLOCK];
  char search_sql[STRING_BLOCK];
  sqlite3_stmt *stmt;
  sqlite3 *handle;

  *total_receive=0;
  *total_spend=0;

  sprintf((char*)fields,"%s",
	  "sum(receive) as TotalReceive, sum(spend) as TotalSpend");

  search_string_to_sql(search_string, search_sql, year, month_number);

  sprintf((char*)query,
	  "select %s from transactions where " \
	  "(currency is \"%s\") and (%s);",
	  fields,currency,search_sql);

  /* if no account is specified use the default one */
  if (strlen(account)==0) {
    settings_set_account(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT));
    if (strlen(get_text_from_identifier(SETTINGS_DEFAULT_CURRENT_ACCOUNT))==0) {
      printf("Unable to get default account name\n");
      return 0;
    }
    account = settings_get_account();
  }

  if (strlen(account)==0) {
    printf("No account specified\n");
    return 0;
  }
  sprintf((char*)database_filename,
	  "%s/.fin/%s.sqlite3",getenv("HOME"),account);
  retval = sqlite3_open_v2(database_filename,
			   &handle,
			   SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
			   NULL);
  /* If connection failed, handle returns NULL */
  if (retval) {
    printf("Sqlite3 database connection failed\n");
    return 0;
  }

  retval = sqlite3_prepare_v2(handle,query,-1,&stmt,0);
  if (retval) {
    printf("Unable to retrieve search information.  " \
	   "Does this account '%s' exist?\n",account);
    return 0;
  }

  cols = sqlite3_column_count(stmt);

  while(1) {
    retval = sqlite3_step(stmt);

    if(retval == SQLITE_ROW) {

      for(col = 0; col < cols; col++) {
	char *val = (char*)sqlite3_column_text(stmt,col);
	if (val != 0) {
	  if (col == 0) {
	    *total_receive += atof(val);
	  }
	  if (col == 1) {
	    *total_spend += atof(val);
	  }
	}
      }
    }
    else if(retval == SQLITE_DONE) {
      break;
    }
  }

  sqlite3_close(handle);

  return 1;
}
