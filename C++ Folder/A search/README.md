# A* Search C++ Project

Compile:
```powershell
g++ main.cpp -o astar -std=c++17 -O2
```

Run:
```powershell
.\astar.exe as-skitter.txt 0 0
```

This creates `results.csv`, `runtime_graph.svg`, and `path_length_graph.svg`.

Since as-Skitter has no coordinates, the heuristic is 0, so A* behaves like Dijkstra.
