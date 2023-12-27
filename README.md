# Polygon_operations
* Ubuntu 20.04 , Gcc/G++ version 5.3.1, Cmake version 3.20.0 
    These versions are used to run the given shell script. 
    It should work on windows as well due to Cmake's cross compilation capability.
    It should work for othe linux and gcc versions as well.

* Function parameters are used to pass the vertices of polygons. (By Default 3 polygons of different sides).

* User can provide the number of polygons and vertices of each polygon as well to get the results.

* Code is handling the cases where the result is an empty polygon.

* Exceptions are handled.

* Code is optimized using OMP library for multithreading if the number of polygons are greater than 10. (For just Intersection function to let you know about my ability).

* Comments are just provided for main.cpp i can provide it for other files but i am not considering it as necessary.
