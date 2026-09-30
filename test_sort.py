import sys
from pathlib import Path
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1]/"src"))
from sort import ALGORITHMS, Stats, make_input, is_stable

class SortingTests(unittest.TestCase):
    def test_inputs_and_stability(self):
        for n in range(201):
            for shape in ("random", "sorted", "reverse", "duplicates", "equal"):
                original = make_input(n, shape)
                expected = sorted(x[0] for x in original)
                for name, fn in ALGORITHMS:
                    with self.subTest(n=n, shape=shape, algorithm=name):
                        a, stats = original.copy(), Stats()
                        fn(a, stats)
                        self.assertEqual([x[0] for x in a], expected)
                        if name != "heap":
                            self.assertTrue(is_stable(a))

    def test_heap_stability_counterexample(self):
        a = [(7, 0), (7, 1), (7, 2)]
        ALGORITHMS[2][1](a, Stats())
        self.assertFalse(is_stable(a))

if __name__ == "__main__":
    unittest.main()
