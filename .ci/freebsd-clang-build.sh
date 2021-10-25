#!/usr/bin/env bash

# This script is for CI environments and is to get around the
# problem of linking with -zdefs on FreeBSD and the environ
# variable (which we use in libmtdac) which unlike in Linux/glibc
# is _not_ defined in libc but in /usr/lib/crt1.o which is only
# linked into executables.
#
# That means linking libmtdac always produces
#
#   ld: warning: undefined symbol: environ
#
# Seeing as there is no way currently (that I could find) to tell
# the linker to not warn about certain symbols, we use this script
# to wrap the compile/linking process, look for undefined symbols,
# if we get no undefined symbols or only environ, return 0 (success)
# if we get any other undefined symbols return 1 (failure).

error=

while read line
do
	if [[ $line =~ "undefined symbol:" ]]; then
		if [[ $line =~ " environ" ]]; then
			echo "  IGNORE undefined symbol: environ"
		else
			error=1
		fi
	fi
done <<<$(CFLAGS=-Werror gmake CC=clang GIT_VERSION=\\\"v0.0.0\\\" V=1 -C src/ 3>&1 1>&2 2>&3 | tee /dev/stderr)

# The crazy command above basically displays the make output
# to the console while sending stderr through the while loop.
# See https://sebthom.de/158-bash-capturing-stderr-variable/

# If $error is set, we got at least one undefined symbol that
# wasn't 'environ' so we need to fail the build.
if test -n "$error"
then
	exit 1
fi

exit 0
