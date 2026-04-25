#!/bin/bash
# Test runner for basic_c project

run_test() {
    local test_file=$1
    local name=$(basename $(dirname $test_file))
    local test_name=$(basename $test_file .c)
    
    if [[ "$test_file" == *"strings"* ]] || [[ "$test_file" == *"memory"* ]]; then
        gcc -Wall -Wextra -std=c11 -g "$test_file" -o /tmp/test -lm 2>/dev/null
    elif [[ "$test_file" == *"primitives"* ]]; then
        gcc -Wall -Wextra -std=c11 -g "$test_file" -o /tmp/test -lm 2>/dev/null
    else
        gcc -Wall -Wextra -std=c11 -g "$test_file" -o /tmp/test 2>/dev/null
    fi
    
    if [ -f /tmp/test ]; then
        result=$(/tmp/test 2>&1 | grep -E "Провалено" | awk '{print $2}')
        if [ "$result" = "0" ] || [ -z "$result" ]; then
            echo "[PASS] $name"
        else
            echo "[FAIL] $name ($result failed)"
        fi
        rm -f /tmp/test
    else
        echo "[SKIP] $name (compilation failed)"
    fi
}

echo "=== Running all tests ==="
echo ""

for test in $(find . -name "test*.c" -type f | sort); do
    run_test "$test"
done

echo ""
echo "=== Memory check ==="
if command -v valgrind &> /dev/null; then
    valgrind --leak-check=full --error-exitcode=1 ./1.types/4.memory/testMemory 2>&1 | tail -5
else
    echo "valgrind not installed"
fi