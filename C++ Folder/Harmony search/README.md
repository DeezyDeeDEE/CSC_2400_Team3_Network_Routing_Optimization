# Harmony Search C++ Project

Compile:
```powershell
g++ main.cpp -o harmony_search -std=c++17 -O2
```

Run:
```powershell
.\harmony_search.exe as-skitter.txt 0 100000 5000 50
```

Arguments: dataset source maxEdges iterations harmonyMemorySize.

This creates `results.csv`, `runtime_graph.svg`, and `path_length_graph.svg`.
