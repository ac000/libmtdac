Name:		libmtdac
Version:	0.50.0
Release:	1%{?dist}
Summary:	Library to interface with HMRC MTD API

Group:		System Environment/Libraries
License:	LGPLv2.1
URL:		https://github.com/ac000/libmtdac
Source0:	libmtdac-%{version}.tar
BuildRoot:	%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

BuildRequires:	glibc-devel libcurl-devel jansson-devel
Requires:	libcurl jansson

%description
libmtdac is a library for interfacing with the UK's HMRC Make Tax Digital API.

Currently just supports the Self-Assessment & VAT APIs


%prep
%setup -q


%build
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
install -Dp -m0644 -t $RPM_BUILD_ROOT/%{_includedir}/libmtdac/ include/libmtdac/*.h
install -Dp -m0755 src/libmtdac.so.%{version} $RPM_BUILD_ROOT/%{_libdir}/libmtdac.so.%{version}
install -Dp -m0644 -t $RPM_BUILD_ROOT/%{_mandir}/man3/ man/man3/*.gz
cd $RPM_BUILD_ROOT/%{_libdir}
ln -s libmtdac.so.0 libmtdac.so
cd -

%post -p /sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc README.md COPYING CodingStyle.md Contributing.md
%{_mandir}/man3/*.gz
%{_libdir}/libmtdac.*
%{_includedir}/libmtdac/*.h


%changelog

* Thu Apr 08 2021 Andrew Clayton <andrew@digital-domain.net> - 0.50.0-1
- Many cleanups & fixes

* Sun Jan 10 2021 Andrew Clayton <andrew@digital-domain.net> - 0.18.0-1
- Cleanups
- Fix the Gov-Client-User-Agent for version 3.0 of the FPH API.

* Thu Jan 07 2021 Andrew Clayton <andrew@digital-domain.net> - 0.17.0-1
- Cleanups & fixes
- Various fraud prevention header work

* Wed Sep 30 2020 Andrew Clayton <andrew@digital-domain.net> - 0.16.0-1
- Add support for more endpoints; Individual Income/Employment, VAT
- Can now use the production API
- Re-work config directory

* Wed Sep 16 2020 Andrew Clayton <andrew@digital-domain.net> - 0.15.0-1
- Some API updates (API break)

* Wed Sep 09 2020 Andrew Clayton <andrew@digital-domain.net> - 0.14.0-1
- mtd_sa_se_list_periods() should take a query_string (API break)

* Fri Jul 24 2020 Andrew Clayton <andrew@digital-domain.net> - 0.13.0-1
- Fix the prototype of mtd_il_lc_update_loss_order() (API break...)

* Fri Jun 26 2020 Andrew Clayton <andrew@digital-domain.net> - 0.12.0-1
- Some API re-working for POST/PUT endpoints (API break...)

* Wed Jun 10 2020 Andrew Clayton <andrew@digital-domain.net> - 0.11.0-1
- Some API re-working (introduce mtd_global_init(), API break...)

* Sun Jun 07 2020 Andrew Clayton <andrew@digital-domain.net> - 0.10.0-1
- mtd_ic_list_calculations() should take a query_string (API break)

* Mon Jun 01 2020 Andrew Clayton <andrew@digital-domain.net> - 0.9.0-2
- Specfile fix, be sure to install all header files

* Mon Jun 01 2020 Andrew Clayton <andrew@digital-domain.net> - 0.9.0-1
- Fix mtd_sa_cr_crystallise() to take an input file (API break)

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
