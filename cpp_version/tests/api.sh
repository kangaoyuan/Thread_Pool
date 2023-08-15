#! /bin/bash

. funcs.sh

function test_api {
	echo "Testing API calls..."
	compile src/api.cpp
	output=$(./test)
	if [[ $? != 0 ]]; then
		err "$output" "$output"
		exit 1
	fi
}

# Run test
test_api

echo "thread_pool API calls, No errors"
