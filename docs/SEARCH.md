### Searching Code
in order to find a specific function, implementation, class, or other source code information for extension or debugging purposes, the following grep may prove helpful:

    grep: USAGE (from man pages)

    grep [OPTION...] PATTERNS [FILE...]

    grep [OPTION...] -e PATTERNS ... [FILE...]

    grep [OPTION...] -f PATTERN_FILE ... [FILE...]


# Most Used Commands
grep 'yoursearchterm' <filename> 
Searches a specific file for your term

grep -r 'yoursearchterm' .
Searches every file in the directory and all subdirectories for your term

grep -r --exclude-dir=(your directory to exclude) 'yoursearchterm' . 
Searches every file in the directory and all subdirectories **EXCEPT for the one excluded** in the directory for your term. This is useful when folders like .ccls\_cache or other caches are generated. 

# Other Useful Flags
-i - ignores the case of the search term
-E - extended regex
-G - basic regex
-e - Followed by PATTERN, do regex search


