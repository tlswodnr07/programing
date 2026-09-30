import csv
import sys
from statistics import median
from time import process_time
from sort import ALGORITHMS, Stats, make_input, is_stable

def main():
    writer = csv.writer(sys.stdout)
    writer.writerow(["n", "shape", "algorithm", "median_ms", "comparisons", "moves", "stable_observed"])
    for n in (1000, 2000, 4000, 8000):
        for shape in ("random", "sorted", "reverse", "duplicates", "equal"):
            original = make_input(n, shape)
            expected = sorted(x[0] for x in original)
            for name, function in ALGORITHMS:
                times = []
                for run in range(-1, 5):
                    a, stats = original.copy(), Stats()
                    start = process_time()
                    function(a, stats)
                    elapsed = (process_time()-start)*1000
                    assert [x[0] for x in a] == expected
                    if run >= 0:
                        times.append(elapsed)
                writer.writerow([n, shape, name, f"{median(times):.6f}", stats.comparisons, stats.moves, int(is_stable(a))])

if __name__ == "__main__":
    main()
