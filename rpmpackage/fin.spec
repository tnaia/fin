Name: fin
Version: 1.51
Release: 1%{?dist}
Summary: Command line personal finances manager
License: GPL3
URL: https://github.com/fuzzgun/fin
Packager: Bob Mottram (4096 bits) <bob@robotics.uk.to>
Source0: http://yourdomainname.com/src/%{name}_%{version}.orig.tar.gz
Group: Utility/ConsoleOnly

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
make install -B DESTDIR=%{buildroot}

%files
%doc README.md LICENSE
%defattr(-,root,root,-)
%{_bindir}/*
%{_mandir}/man1/*

%changelog
* Sun Jun 30 2013  Bob Mottram (4096 bits) <bob@robotics.uk.to>
- Spec file created
