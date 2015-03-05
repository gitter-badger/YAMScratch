Slicer
============
Command line program to slice STL files and use ezdxf to write each layer to
a seperate DXF. The program creates a directory to place the layer DXF files.


Dependencies
================
You need the following python packages:

ezdxf version 0.6.2   <https://pypi.python.org/pypi/ezdxf/0.6.2>

There is a later version available, 0.6.5, which should work as the API is the same.

Roadmap
===========
-[ ] Add slicing along user defined direction
-[ ] Add sheet packing output
-[ ] Replace brute force O(n^3) slicing with elegant O(n^2)