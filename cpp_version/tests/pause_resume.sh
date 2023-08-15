#! /bin/bash

. funcs.sh

function test_pause_resume {
	echo "Testing pause and resume 7 secs with $1 threads"
	compile src/pause_resume.cpp
	real_secs=$(time_exec ./test "$1")
	threshold=1.00
	ret=$(python -c "print((${real_secs} - 7) <= ${threshold})")
	if [[ "$ret" == "True" ]]; then
		return
	fi
	err "Elapsed ${real_secs} which is more than allowed"
	exit 1
}

# Run test
test_pause_resume 4

echo "pause_resume_thpool, No errors"
