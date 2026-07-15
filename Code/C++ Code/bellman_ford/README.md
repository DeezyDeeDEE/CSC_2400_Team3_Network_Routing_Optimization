# Bellman-Ford C++ Project

Compile:
```powershell
g++ main.cpp -o bellman_ford -std=c++17 -O2
```

Run:
```powershell
.\bellman_ford.exe as-skitter.txt 0 10000
```

This creates `results.csv`, `runtime_graph.svg`, and `path_length_graph.svg`.

Bellman-Ford is slow, so use a smaller edge limit.
