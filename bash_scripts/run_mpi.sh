#!/bin/bash

EXE="../mpi"
OUT="../outputs/csv/res_mpi.csv"
MATRIX=(400 800 1200 1600 2000 2400 2800 3200)
PROCS=(1 2 4 8 12 16 20)
MAX_NODES=6
ITER=5

[[ -f "$EXE" ]] || { echo "Error: $EXE not found."; exit 1; }
echo "Matrix Size,Nodes,Number of Processes,Execution Time (ms)" > "$OUT"

for p in "${PROCS[@]}"; do
  for n in "${MATRIX[@]}"; do
    sum=0
    # Set NODES to be the smaller of P and MAX_NODES
    nodes=$(( p < MAX_NODES ? p : MAX_NODES ))
    for i in $(seq $ITER); do
      RAW=$(srun --mpi=pmix --nodes="$nodes" --ntasks="$p" "$EXE" "$n")
      OUTVAL=$(echo "$RAW" | sed -nE 's/.*(Elapsed|Execution) time( is)?:[[:space:]]*([0-9]+(\.[0-9]+)?)( in ms)?.*/\3/p' | head -n1)
      [[ -n "$OUTVAL" ]] || { echo "Error: No elapsed time found for n=$n, p=$p"; exit 1; }
      sum=$(echo "$sum + $OUTVAL" | bc -l)
    done
    avg=$(echo "scale=6; $sum / $ITER" | bc -l)
    echo "$n,$nodes,$p,$avg" >> "$OUT"
  done
done

echo "MPI tests complete. Results stored in $OUT. "

