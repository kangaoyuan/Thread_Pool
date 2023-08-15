#!/bin/bash

function compile {
	g++ -g $COMPILATION_FLAGS "$1" ../thread_pool.hpp -o test
}

# NB: the sequence about redirecton is important, the error stram is still print
function time_exec {
	real_secs=$(/usr/bin/time -f '%e' "$@" 2>&1 > /dev/null)
	echo "${real_secs}"
}

function err {
	echo "-------------------ERROR-------------------"
	echo "$1"
	echo "$2" >> error.log
	exit 1
}

function ensure_installed_valgrind {
	valgrind --version &> /dev/null
	if(( $? != 0)); then
		msg="Valgrind seems not to be installed"
		err "${msg}" "${msg}"
	fi
}

function needle {
	python -c "import re; print(re.search(r'$1', '$2').group(0))"
	if(( $? != 0)); then
		msg="Python script to regular search error"
		err "${msg}" "${msg}"
	fi
}

function extra_num {
	string=$(needle "$1" "$2")
	needle "[0-9]*" "${string}"
	if(( $? != 0 )); then
		msg="extra_num error"
		err "${msg}" "${msg}"
	fi	
}
