Name:		libmtdac
Version:	0.8.0
Release:	1%{?dist}
Summary:	Library to interface with HMRC MTD API

Group:		System Environment/Libraries
License:	LGPLv2.1
URL:		https://github.com/ac000/libmtdac
Source0:	libmtdac-%{version}.tar
BuildRoot:	%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

BuildRequires:	glibc-devel curl-devel jansson-devel

%description
libmtdac is a library for interfacing with the UK's HMRC Make Tax Digital API.

Currently just supports the Self-Assessment API


%prep
%setup -q


%build
make -C src/ %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
install -Dp -m644 include/libmtdac/mtd.h $RPM_BUILD_ROOT/%{_includedir}/libmtdac/mtd.h
install -Dp -m644 include/libmtdac/mtd-sa.h $RPM_BUILD_ROOT/%{_includedir}/libmtdac/mtd-sa.h
install -Dp -m644 include/libmtdac/mtd-saac.h $RPM_BUILD_ROOT/%{_includedir}/libmtdac/mtd-saac.h
install -Dp -m644 include/libmtdac/mtd-ic.h $RPM_BUILD_ROOT/%{_includedir}/libmtdac/mtd-ic.h
install -Dp -m644 include/libmtdac/mtd-il.h $RPM_BUILD_ROOT/%{_includedir}/libmtdac/mtd-il.h
install -Dp -m644 include/libmtdac/mtd-ni.h $RPM_BUILD_ROOT/%{_includedir}/libmtdac/mtd-ni.h
install -Dp -m644 include/libmtdac/mtd-test-cu.h $RPM_BUILD_ROOT/%{_includedir}/libmtdac/mtd-test-cu.h
install -Dp -m644 include/libmtdac/mtd-test-fph.h $RPM_BUILD_ROOT/%{_includedir}/libmtdac/mtd-test-fph.h
install -Dp -m0755 src/libmtdac.so.%{version} $RPM_BUILD_ROOT/%{_libdir}/libmtdac.so.%{version}
cd $RPM_BUILD_ROOT/%{_libdir}
ln -s libmtdac.so.0 libmtdac.so
cd -

%post -p /sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc README.md COPYING CodingStyle.md Contributing.md
%{_libdir}/libmtdac.*
%{_includedir}/libmtdac/*.h


%changelog

* Fri May 29 2020 Andrew Clayton <andrew@digital-domain.net> - 0.8.0-1
- Internal re-woring but an exported enum value got removed (API break)

* Fri May 29 2020 Andrew Clayton <andrew@digital-domain.net> - 0.7.0-1
- Split out NINO (API break)

* Wed May 27 2020 Andrew Clayton <andrew@digital-domain.net> - 0.6.0-1
- Fix a fucntion prototype (API break)

* Wed May 27 2020 Andrew Clayton <andrew@digital-domain.net> - 0.5.0-1
- Rename a function (API break)

* Sat May 23 2020 Andrew Clayton <andrew@digital-domain.net> - 0.4.0-1
- Change in JSON output (API break)

* Fri May 22 2020 Andrew Clayton <andrew@digital-domain.net> - 0.3.0-1
- Namespace Self-Assessment API (API break)

* Thu May 21 2020 Andrew Clayton <andrew@digital-domain.net> - 0.2.0-1
- Change how query string parameters are passed in (API break)

* Tue May 12 2020 Andrew Clayton <andrew@digital-domain.net> - 0.1.0-1
- Support multiple employments (API break)

* Mon May 11 2020 Andrew Clayton <andrew@digital-domain.net> - 0.0.0-1
- Initial version.
