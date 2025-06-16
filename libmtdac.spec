Name:		libmtdac
Version:	1.1.0
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

Currently supports the Income Tax Self Assessment (ITSA) & VAT APIs


%prep
%setup -q


%build
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
install -Dp -m0644 -t $RPM_BUILD_ROOT/%{_includedir}/libmtdac/ include/libmtdac/*.h
install -Dp -m0755 src/libmtdac.so.%{version} $RPM_BUILD_ROOT/%{_libdir}/libmtdac.so.%{version}
install -Dp -m0644 -t $RPM_BUILD_ROOT/%{_mandir}/man3/ docs/man/man3/*.3
install -Dp -m0644 -t $RPM_BUILD_ROOT/%{_mandir}/man3type/ docs/man/man3type/*.3type
gzip $RPM_BUILD_ROOT/%{_mandir}/man3/*.3 $RPM_BUILD_ROOT/%{_mandir}/man3type/*.3type
cd $RPM_BUILD_ROOT/%{_libdir}
ln -s libmtdac.so.1 libmtdac.so
cd -

%post -p /sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc README.rst COPYING CodingStyle.rst Contributing.rst docs/*.rst
%{_mandir}/man3/*.gz
%{_mandir}/man3type/*.gz
%{_libdir}/libmtdac.*
%{_includedir}/libmtdac/*.h


%changelog

* Mon Jun 16 2025 Andrew Clayton <ac@sigsegv.uk> - 1.1.0-1
- Add support for more endpoints; Individuals Pensions Income,
  Self Assessment Assist, Self Assessment Test Support,
  Individuals Employments Income, Self Assessment Accounts API &
  Individuals Expenses
- Support application-restricted endpoints that require a scope

* Tue May 20 2025 Andrew Clayton <ac@sigsegv.uk> - 1.0.0-1
- Re-add VAT support
- Convert markdown docs to reStructuredText

* Wed May 07 2025 Andrew Clayton <ac@sigsegv.uk> - 0.90.0-1
- Big API update, both in terms MTD APIs and libmtdac API
- Big docs update markdown -> reStructuredText and covering API changes

* Tue Apr 05 2022 Andrew Clayton <andrew@digital-domain.net> - 0.61.0-1
- Per API credentials
- FreeBSD port
- API updates

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
