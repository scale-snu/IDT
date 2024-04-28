#clear-caches
echo "=== 30 ===" > random.txt
numactl -N0 -m0 -- ./gups-random 16 1000000000 30 8 30
