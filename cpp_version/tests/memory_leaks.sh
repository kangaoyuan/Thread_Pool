#! /bin/bash

. funcs.sh

function __test_thread_alloc_free {
	output=$(valgrind --leak-check=full --track-origins=yes ./test $1 2>&1 > /dev/null)
	heap_usage=$(echo "${output}" | grep "total heap usage")
	allocs=$(extra_num "[0-9]* allocs" "${heap_usage}")
	frees=$(extra_num "[0-9]* frees" "${heap_usage}")
	if (( "$allocs" == 0 )); then
		err "Allocated 0 times. Something is wrong..." "$output"
		return
	fi
	if (( "$allocs" != "$frees" )); then
		err "Allocated $allocs times but freed only $frees" "$output"
		return
	fi
}

function test_thread_alloc_free {
	echo "Testing creation and destruction of threads = $1"
	compile src/no_work.cpp
	__test_thread_alloc_free "$1"
}

function test_thread_alloc_free_multi {
	echo "Testing multiple threads creation and destruction in pool of threads = $1, times = $2"
	compile src/no_work.cpp
	pids=()
	parallels="${3:-10}"
	for(( i = 1; i <= $2; ++i )); do
		python -c "import sys; sys.stdout.write('${i}/${2}\r')"
		__test_thread_alloc_free "${1}" &
		# Add the process ID (PID) of the last background command executed to array.
		pids+=($!)
		if(( "${i}" % 10 == 0 )); then
			for pid in "${pids[@]}"; do
				# Wait for background command to complete, which allows synchronizing. Providing a way to pause the script until the specified background processes are done.
				wait $pid
				if (( $? != 0 )); then
					err "Testing failed" "Testing failed"
				fi
			done
			pids=()
		fi
	done
	echo
}

# Run tests
ensure_installed_valgrind
test_thread_alloc_free 1
test_thread_alloc_free 2
test_thread_alloc_free 4
test_thread_alloc_free 8
test_thread_alloc_free 20
test_thread_alloc_free_multi 4 20
# test_thread_free_multi 3 1000  # Takes way too long
test_thread_alloc_free_multi 8 100
# test_thread_free_multi 100 100  # Takes way too long
test_thread_alloc_free_multi 100 20 1

echo "test memory leaks, No errors"
