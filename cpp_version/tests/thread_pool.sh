#! /bin/bash

. funcs.sh

function test_multi_process_addition {
	echo "Testing addition up to $1 with $2 threads..."
	compile src/conc_increment.cpp
	output=$(./test $1 $2)
	num=$(echo $output | awk '{print $(NF)}')
	echo "num = $num"
	if [[ $num == $1 ]]; then
		return
	fi
	err "Expected $1 but got $output" "$output"
	exit 1
}

# Run tests
test_multi_process_addition 100 4
test_multi_process_addition 100 1000
test_multi_process_addition 100000 1000

echo "thread_pool, No errors"
