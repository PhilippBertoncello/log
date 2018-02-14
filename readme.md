log.cpp
This "library" is in reality not a library.
To use these functions, you only have to download the file "log.cpp"
and to tell the linker of your compiler that you want to use it. In 
g++ you can do that by adding the flag "-Llog.cpp". If you save the 
library in another directory than you're compiling it in, then you 
also have to include the path into the above stated flag. For example,
if the file "log.cpp" is located in the directory "./src/lib/ext/",
you have to use the flag "-L./src/lib/ext".
