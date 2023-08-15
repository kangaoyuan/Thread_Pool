#! /bin/bash

. funcs.sh

function test_nonzero_heap_stack {
	echo "Testing for non-zero heap and stack"	
	compile src/nonzero_heap_stack.cpp	
	output=$(timeout 1 ./test)
	if [[ $? != 0 ]]; then
		err "Fail running on nonzero heap and stack" "$output"
		exit 1;
	fi
}

# Run test
test_nonzero_heap_stack 

echo "nonzero_heap_stack in thread_pool, No errors"
