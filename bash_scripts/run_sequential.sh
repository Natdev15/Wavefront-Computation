#!/bin/bash

EXEC="../sequential"
OUT="../outputs/csv/res_sequential.csv"
SIZES=(400 800 1200 1600 2000 2400 2800 3200)
ITER=5

[[ -f "$EXEC" ]] || { echo "Error: Executable not found at $EXEC"; exit 1; }
chmod +x "$EXEC"
echo "Matrix Size,Execution Time (ms)" > "$OUT"

for size in "${SIZES[@]}"; do
    sum=0
    for ((i=0; i<ITER; i++)); do
        output=$(srun "$EXEC" "$size" 1)
        elapsed=$(echo "$output" | grep -i "elapsed time" | head -n1 | grep -oE '[0-9]+(\.[0-9]+)?')
        [[ "$elapsed" =~ ^[0-9]+(\.[0-9]+)?$ ]] || { echo "Error: Invalid elapsed time ($elapsed) for matrix size $size"; exit 1; }
        sum=$(echo "$sum + $elapsed" | bc)
    done
    avg=$(echo "scale=6; $sum / $ITER" | bc)
    echo "$size,$avg" >> "$OUT"
done

echo "Sequential tests complete. Results stored in $OUT."


