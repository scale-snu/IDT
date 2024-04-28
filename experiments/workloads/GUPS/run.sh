THREADS=16
UPDATES=10000000
MEM=36
DATESIZE=8
HOT=25

#clear-caches
echo "=== $HOT/$MEM ===" >> results.txt
numactl -N0 -c0 -- ./gups-pebs $THREADS $UPDATES 34 8 $HOT >> results.txt
