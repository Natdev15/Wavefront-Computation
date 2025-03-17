#!/bin/bash

EXEC="../ff_parallelfor"
OUT="../outputs/csv/res_fastflow.csv"
WORKERS=(1 2 4 8 12 16 20)
SIZES=(400 800 1200 1600 2000 2400 2800 3200)
ITER=5

[[ -f "$EXEC" ]] || { echo "Error: $EXEC not found"; exit 1; }
chmod +x "$EXEC"

echo "Matrix Size,Number of Workers,Execution Time (ms)" > "$OUT"

for workers in "${WORKERS[@]}"; do
    for size in "${SIZES[@]}"; do
        sum=0
        for ((i=0; i<ITER; i++)); do
            result=$(srun --nodes=1 --ntasks=1 "$EXEC" "$size" "$workers" | grep "Elapsed time" | awk '{print $3}')
            [[ "$result" =~ ^[0-9]+(\.[0-9]+)?$ ]] || { echo "Error: Invalid output ($result) for workers=$workers, size=$size"; exit 1; }
            sum=$(echo "$sum + $result" | bc)
        done
        avg=$(echo "scale=6; $sum / $ITER" | bc)
        echo "$size,$workers,$avg" >> "$OUT"
    done
done

echo "FastFlow tests complete. Results stored in $OUT."

