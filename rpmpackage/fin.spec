Name: fin
Version: 1.51
Release: 1%{?dist}
Summary: Command line personal finances manager
License: GPL3
URL: https://github.com/fuzzgun/fin
Packager: Bob Mottram (4096 bits) <bob@robotics.uk.to>
Source0: http://yourdomainname.com/src/%{name}_%{version}.orig.tar.gz
Group: Utility/ConsoleOnly

%if 0%{?mandriva_version}
Requires: sqlite3
%else
Requires: sqlite
%endif

%if 0%{?mandriva_version}
BuildRequires: libsqlite3-devel
%else
BuildRequires: sqlite-devel
%endif

%description
Keeping track of personal finances is an incredibly boring task, and if you
don't have much money then juggling it can be tricky. When looking at the
existing software out there to manage accounts it's frequently highly
complex and top heavy. Fin - an abbreviation of "financials" - is an
attempt to do the opposite of what most accounting software does, and
that's to make things incredibly simple and quick. Logging a transaction or
viewing your current balance should take no more than a few sec

%prep
%setup -q

%build
%configure
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}
mkdir -p %{buildroot}/etc
mkdir -p %{buildroot}/etc/%{name}
mkdir -p %{buildroot}/usr
mkdir -p %{buildroot}/usr/bin
mkdir -p %{buildroot}/usr/share
mkdir -p %{buildroot}/usr/share/man
mkdir -p %{buildroot}/usr/share/man/man1
# Make install but to the RPM BUILDROOT directory
make install -B DESTDIR=%{buildroot} PREFIX=/usr

%files
%doc README.md LICENSE
%defattr(-,root,root,-)
%{_bindir}/*
%{_mandir}/man1/*

%changelog
* Sat May 04 2013 Bob Mottram (4096 bits) <bob@robotics.uk.to> - 1.51-1
- Compile instructions in README and manpage for OpenSUSE and bcrypt
- Monthly and yearly totals when searching
- Colour changes for better readability
- Summary of accounts

* Sat Mar 09 2013 Bob Mottram (4096 bits) <bob@robotics.uk.to> - 1.50-1
- Adjustments description spans multiple lines
- Backup and restore commands
- Bug fix for balance value in monthly listing for a given year
- Monthly and yearly transaction volumes
- Removed version control files from source package
- Transaction volume totals

* Tue Oct 09 2012 Bob Mottram (4096 bits) <bob@robotics.uk.to> - 1.41-1
- Import transactions from GnuCash
- Edits and deletions
  are recorded in an adjustments table
- Time of transaction included with date
- Pie charts can now be plotted

* Tue Oct 02 2012 Bob Mottram (4096 bits) <bob@robotics.uk.to> - 1.40-1
- Improvements to import function
  Data can be exported from Paypal as csv and imported into
  fin with no editing required. Also a bug related to the
  transaction numbers during the import process was fixed.
- Multiple search criteria are now allowed
  Search string can contain multiple fields separated by commas
- Spending distribution graph
- Colour coding
  Of positive/negative incoming/outgoing figures
- Long descriptions
  Are now displayed over multiple rows
- Three digit record numbers
  Should be enough to cover most situations
- More unit test coverage
- Short status messages
  Including forecasting of when funds will be depleted
- General code tidying
  Try to keep most lines to less than 80 characters
- Debian packaging improvements
  Now passes lintian tests
