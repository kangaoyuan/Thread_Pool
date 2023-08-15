#! /bin/bash

. funcs.sh

function test_wait_each_job { # ./executable num_jobs num_threads 1(wait_each_job)or0(not_wait_each_job) sleep_per_job(default 1s)
	echo "Will test waiting for each job ($1 threads, $2 jobs)"
	compile src/wait.cpp
	real_secs=$(time_exec ./test $2 $1 1)
	echo "Consume time = $real_secs"
	threshold=1.00
	ret=$(python -c "print(abs($real_secs-$2) <= $threshold)")
	if [[ "$ret" == "True" ]]; then
		return
	fi
	err "Elapsed $real_secs which is more than allowed"
}

function test_wait_pool { # ./executable num_jobs num_threads 1(wait_each_job)or0(not_wait_each_job) sleep_per_job(default 1s)
	echo "Will test waiting for whole thread pool ($1 threads, $2 jobs)"
	compile src/wait.cpp
	real_secs=$(time_exec ./test $2 $1 0)
	echo "Consume time = $real_secs"
	threshold=1.00
	expected_secs=$(python -c "import math; print(math.ceil(${2}/${1}.0))")
	ret=$(python -c "print(abs($real_secs-$expected_secs) <= $threshold)")
	if [[ "$ret" == "True" ]]; then
		return
	fi
	err "Elapsed $real_secs which is too different from what expected $expected_secs"
}

# Run tests
# test_wait_each_job 1 4
test_wait_each_job 4 4
test_wait_each_job 4 10
test_wait_pool 1 4
test_wait_pool 8 2
test_wait_pool 4 4
test_wait_pool 4 20

echo "test wait, No errors"
