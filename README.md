*FIN: The command line personal finances manager*

Keeping track of personal finances is an incredibly boring task, and if you don't have much money then juggling it can be tricky.  When looking at the existing software out there to manage accounts it's frequently highly complex and top heavy.  Fin - an abbreviation of "financials" - is an attempt to do the opposite of what most accounting software does, and that's to make things incredibly simple and quick.  Logging a transaction or viewing your current balance should take no more than a few seconds.  This means less time wasted on bureaucracy and more time spent on what you really want to be doing.

* _Installation_

The easiest way to install is via a pre-compiled package.  You can also compile from source as follows:

    sudo apt-get install libsqlite3-dev bcrypt gnuplot
    make
    sudo make install

You will need to have the sqlite3 development package installed to be able to compile, and to get the full functionality you'll also need to have gnuplot and bcrypt installed.


* _Language_

First set the language:

   fin language "English US"

   fin language "English UK"

Possible languages are: English UK, English US, French, German, Italian, Spanish, Greek, Russian, Polish, Dutch, Chinese

Currently only English UK and English US are implemented.  For translation text see international.c


* _Basic Usage_

To log an amount of money spent:

   fin spend <amount> on <thing purchased>

   fin spend 2.99 on cheese

   fin spend 4.50 date 02/07/2011 on bread

Currency symbols such as $ or £ should not be included in the amount.

You might also wish to log where money was spent, so that you can use that as a search criteria later.

   fin spend 25.39 on groceries at supermarket

To receive an amount of money:

   fin receive <amount> from <source>

   fin receive 100 from "balance brought forward"

To show recent transactions just type "fin".  The most recent is at the top.  If you want to list more transactions:

   fin transactions <number>

You can also see aggregated figures:

   fin monthly

   fin yearly

and combine these to view however many transactions you want:

   fin monthly transactions 24

You can also use the word 'records' instead of 'transactions'.

By default transactions will be timestamped according to the system time when they are entered.  Optionally it is possible to adjust the time using the *time* option.  For example:

   fin receive 2.50 from "back of sofa" time 3.45pm


* _Brief Status Messages_

It's also possible to produce brief status messages suitable either for including into *.bashrc* files, or for sending to a text-to-speech system.

** Balance

To report the balance for the currently selected account.


  fin say

  fin say | espeak


** Average Income

To see what your average income has been over the previous year.


  fin say income

  fin say income | espeak


** Average Spending

To see what your average spending rate has been over the previous year.


  fin say spending

  fin say spend | espeak


The spending status message will also provide an estimate of when your funds will run out, or if the balance is negative how long it will take to pay off the debt or overdraft at the current average rate of income and spending.  This can help to provide an indication of whether your current rate of income and spending are sustainable or not, based upon the previous years transactions.  If you want to improve this time estimate you can also include average inflation and interest rates.

So for example in the case of a savings account where the balance is positive.


  fin say spend interest 1.5 inflation 2.0


Sets an average savings interest rate of 1.5% per year and an average inflation rate of 2% per year.  Or for a current account which is overdrawn and has a usury rate (APR) of 18%.


  fin say spend apr 18 inflation 2.0

  fin say spending usury 18 inflation 2.0 | espeak


** Personal Inflation Rate

Indepentently from national inflation rates you can also get an idea of your own personal inflation rate.


  fin say inflation

  fin say inflation | espeak


This compares spending within the previous three months with the same three months a year ago, and will report the percentage by which these values have changed.


* _Searching_

To search for transactions with particular words:


   fin find <words>


Note that multiple words should be surrounded by quotes, such as:


   fin find "cheese sandwich"


You can also use wildcards:


   fin find "*cheese*"


Or with multiple strings:


   fin find "cheese, bread, butter"


It's also possible to search by months or years:


   fin find "*" month jan
   fin find "*" year 2011


As an alternative to "find" you can also use the word "search".

You can also aggregate figures on a monthly basis.


   fin find "*groceries*" monthly



* _Deleting transactions_

To delete a transaction first locate the record number on the far left hand side by typing "fin" or doing a search, then type:


   fin undo <record number>



* _Editing transactions_

The status or description for a transaction can be edited as follows:


   fin edit <record number> status <new status> description <new description>

   fin edit 2 status "complete"

   fin edit 3 description "groceries"

   fin edit 1 date "15/08/2016"


The record number is the number shown to the far left when you type "fin" or do a search.  If you wish to change the date or values then use the undo command and create a new transaction.

All edits are recorded, and can be viewed by typing:


  fin edits


So if you make a change by mistake it's always possible to see what the previous values were.  The letter on the left hand side indicates different types of edit.


  B    The state of the transaction before the edit was made

  A    The state of the transaction after the edit was made

  D    A deleted transaction


If necessary you can also search through the edits.


  fin edits search "*cheese*"

  fin edits search "*" year 2012

  fin edits search "*" month dec



* _Multiple Accounts_

By default any transactions will be added to an account called "current".

To create other accounts, or to switch to another account use the following


   fin account <name>


For example:


   fin account savings
   fin account expenses


All account data is stored in the directory called ".fin" in your home directory (~/.fin).

Or just to send a particular transaction to a particular account:


   fin spend 2.99 on chips account:expenses



* _Transfering between accounts_

To transfer an ammount from one account to another:


   fin transfer <amount> to <account name>


For example:


   fin transfer 10.29 to savings


This will transfer 10.29 from whatever is the currently active account (you can check that by typing "fin") to the savings account.


* _Synchronising across multiple machines_

Often people use more than one computer, and want to be able to transfer data.  Fin does this using the scp command, in order to push or pull data from an ssh server.

To push the data to a server:


   fin push <domain>:<directory>

   fin push mydomainname.com:/home/myusername


Optionally you can also include the username for the remote system if it differs from your local username.  If you get a /"permission denied"/ message when attempting to push or pull then try one of the following:


   fin username <myusername> push mydomainname.com:/home/myusername

   fin username <myusername> pull mydomainname.com:/home/myusername



Subsequently you can then just type:


   fin push


The same also applies to retrieve data from a server:


   fin pull



* _Changing currencies_

It's possible to log amounts in multiple currencies, for example:


    fin spend 3.40 on milk curr:EUR


means that the amount spent was in Euros.  Currencies are always given as a three character code (GBP, USD, BTC, etc).

You can also change the default currency:


    fin currency USD


with any subsequent transactions then occurring in that currency by default.


* _Importing data_

** From CSV

Data can be imported from a CSV file, which is a common way of exporting bank transactions from online accounts.  The first line of the file is assumed to be a header containing field names.

The expected field names are "Date", "Money in", "Money out", "Spent", "Spend", "Amount", "Memo", "Description", "Subcategory".  Not all of those field names need to be present for the import to be successful, and fields such as "description" and "memo" mean the same thing.  The currency type and date format are assumed to be whatever has been set as the defaults.

To import:


    fin import <filename>

    fin import mybankstatement.csv


The filename should have a /.csv/ extension.

In case the same file is imported more than once the system attempts to avoid adding duplicate transactions.


** From Emacs org mode tables

Tables created with Emacs org mode, such as a shopping list or parts list, can also be imported.

As with CSV, the expected fields can include "Date", "Money in", "Money out", "Spent", "Spend", "Amount", "Memo", "Description" or "Subcategory".

Here's an example table:

  | Date       | Description     | Spent |
  |------------+-----------------+-------|
  | 03/05/2012 | Cheese sandwich |  1.50 |
  | 03/05/2012 | Teacup          |  2.00 |
  | 04/05/2012 | Butter          |  1.20 |

Note that if you use the "Amount" field name then a positive value means money received and a negative value indicates money spent.


    fin import mynotes.txt



** From GnuCash

To import from GnuCash first create a new account if you need one.


  fin account [account name]


Then import the *.gnucash* file.


  fin import myaccounts.gnucash


The GnuCash file will be examined and any active accounts will be listed.  You should then enter the name of the account that you wish to import.

If you subsequently need to export back to GnuCash then you can use the CSV export feature described below.


* _Exporting data_

** CSV

You can export data in CSV format for viewing in a spreadsheet or import into other systems.


    fin find "*" export myexportfilename.csv


Obviously you can use search strings to narrow down the data set.  It's also possible to specify the separator used between fields.


    fin find "*" export myexportfilename.csv separator tab

    fin find "*" export myexportfilename.csv separator ,


You can also export aggregated figures:


    fin export myexportfilename.csv monthly

    fin export myexportfilename.csv yearly


** Emacs

Data can be exported to Emacs in the form of org mode formatted tables.

To show results in tabular form add the option /style emacs/.

 When performing a search:


    fin search "*shopping*" style emacs > shopping_list.txt


Monthly:


    fin style emacs monthly > monthly.txt


Or yearly


    fin style emacs yearly > yearly.txt



* _Date formats_

You can change the date format used in the following way:


    fin dateformat "%d/%m/%Y"

    fin dateformat "%m/%d/%Y"


Only the %d, %m and %Y fields are supported.


* _Plotting_

** Balance

You can plot monthly balance figures using the following command:


    fin monthly <image filename>

    fin monthly balance.png transactions 100


The image which is output is in PNG format, and gnuplot needs to be installed for this to work.

Perhaps even more useful is to plot particular kinds of expenditure over time by using the search function.  For example:


    fin find "*groceries*" monthly groceries.png


Will output an image showing spending on groceries on a monthly basis over time. The "*" wildcards can be used to search for any description field containing a particular word.

** Spending Distribution

You can plot the overall distribution of amounts spent in the following way:


  fin distribution spending.png startyear 2007 endyear 2012 maximum 200


or for an individual year.


  fin distribution spending.png year 2012 maximum 100


Where /startyear/ and /endyear/ specify the range of years, and /maximum/ denotes the maximum amount.  If an image file is not specified then the results will be listed in CSV format, or emacs org-mode table format if you use the /style emacs/ option. You can also apply a search string to show the spending distribution for a particular type of item.


  fin distribution spending.png year 2015 maximum 100 search "*supermarket*,*groceries*"


** Transaction Volumes

The number of transactions per month can be shown as follows:

  fin volume

or plotted in a graph:

  fin volume mytransactions.png

As usual you can also apply search criteria and increase or decrease the number of months shown using the *transactions* option.

  fin volume mytransactions.png search "*supermarket*" transactions 50

** Pie Charts

Another way of looking at spending is to find out what percentage of income was spent on various things.  This can be done with the /piechart/ command:


    fin piechart year 2008 search "*fuel*,*supermarket*,*car*"

    fin piechart year 2009 search "*fuel*|Petrol,*supermarket*|Food,*car*|Car repairs/insurance,*savings*"


The "|" character is used to specify an alias name corresponding to each search.  You can also export the data to a CSV or text file, which will then allow you to create pie charts using LibreOffice or similar software.


    fin piechart spending.csv year 2008 search "*fuel*,*supermarket*,*car*"

    fin piechart spending.csv month july search "*fuel*|Petrol,*supermarket*|Food,*car*|Car repairs/insurance,*savings*"


Or you can also plot to an image.


    fin piechart chart.png year 2010 search "*fuel*|Petrol,*supermarket*|Food,*car*|Car repairs/insurance,*savings*"


A subtlety here is that you can combine searches into a single category using the "+" sign.  For example, to merge "car" and "fuel" into a single category:


    fin piechart chart.png year 2010 search "*car*+*fuel*|Total Car,*supermarket*|Food,*savings*"



* _Geolocation_

Although it's not typical at the time of writing it is expected that embedded devices such as phones will eventually become a replacement for conventional wallets and physical cash, so it might also be useful to be able to log the locations of transactions.  Optionally, a geolocation can be associated with each transaction.


    fin spend 9.99 on book long:1.2 lat:41.6 alt:52.1


This information can subsequently be exported as part of a search:


    fin find "*" kml <filename>

    fin find "*" year 2011 kml <filename>


* _Security_

Ordinarily the "data at rest" within the ~/.fin directory is not encrypted.  Since financial data may often be confidential there is an easy way to encrypt the data when you're not using it.


    fin lock


You will be prompted to enter a password, then the files will be encrypted.  The next time you run fin you will then be prompted for the password.
