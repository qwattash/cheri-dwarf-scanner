# Cheri DWARF scanner

This tool is designed to scan DWARF information of CHERI programs to extract
various information.
The output is stored in a sqlite3 database for later use and post-processing.

Currently the scanner extracts information about imprecise sub-object bounds in
data structures.
