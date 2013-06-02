Name: fin
Version: 1.51
Release: 1%{?dist}
Summary: The command line personal finances manager
License: GPL3
URL: https://github.com/fuzzgun/%{name}
Packager: Bob Mottram <bob@robotics.uk.to>   
Source0: https://launchpad.net/%{name}/trunk/v%{version}/+download/%{name}_%{version}.orig.tar.gz
Group: Applications/Utilities

Requires: bcrypt gnuplot
%if 0%{?mandriva_version}
BuildRequires: libsqlite3-devel
Requires: sqlite3
%else
BuildRequires: sqlite-devel
Requires: sqlite
%endif

%description
Keeping track of personal finances is an incredibly boring task, and if you
don't have much money then juggling it can be tricky.  When looking at the
existing software out there to manage accounts it's frequently highly
complex and top heavy.  Fin - an abbreviation of "financials" - is an
attempt to do the opposite of what most accounting software does, and
that's to make things incredibly simple and quick.  Logging a transaction
or viewing your current balance should take no more than a few seconds.

This means less time wasted on bureaucracy and more time spent on what you
really want to be doing.  Fin can import and export to CSV files, Emacs
org mode tables and GnuCash.

%prep
%setup -q

%build
%configure
make %{?_smp_mflags}

%install
# Create some directories to install into
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
make install DESTDIR=%{buildroot}

%files
%doc README.md LICENSE
%defattr(-,root,root,-)
%{_bindir}/*
%{_mandir}/man1/*

%changelog
* Sun Jun 02 2013  Bob Mottram <bob@robotics.uk.to>
- A better spec file created
* Fri Jan 20 2012  Bob Mottram <bob@robotics.uk.to>
- Spec file created
