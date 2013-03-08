#
# rpm spec for fin
#

%define        __spec_install_post %{nil}
%define          debug_package %{nil}
%define        __os_install_post %{_dbpath}/brp-compress

Summary: A commandline personal finances manager
Name: fin
Version: 1.50
Release: 1
License: GPL3
Group: Utils
SOURCE0 : %{name}-%{version}.tar.gz
URL: https://launchpad.net/fin
Packager: Bob Mottram <bob@sluggish.dyndns.org>
Requires: sqlite, sqlite-devel, gnuplot, bcrypt
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

%description
 Keeping track of personal finances is an incredibly boring task, and if
 you don't have much money then juggling it can be tricky.  When looking
 at the existing software out there to manage accounts it's frequently
 highly complex and top heavy.  Fin - an abbreviation of 'financials' -
 is an attempt to do the opposite of what most accounting software does,
 and that's to make things incredibly simple and quick.  Logging a
 transaction or viewing your current balance should take no more than a
 few seconds.  This means less time wasted on bureaucracy and more time
 spent on what you really want to be doing.  Fin can import and export
 to CSV files, Emacs org mode tables and GnuCash.

%prep
%setup -q

%build
 # Empty section.

%install
rm -rf %{buildroot}
mkdir -p  %{buildroot}

# in builddir
cp -a * %{buildroot}

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%config(noreplace) %{_sysconfdir}/%{name}/%{name}.conf
%{_bindir}/*
%attr(644,root,root) /usr/share/man/man1/fin.1.gz

%changelog
* Fri Jan 20 2012  Bob Mottram <bob@sluggish.dyndns.org>
- Spec file created

