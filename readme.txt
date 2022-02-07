6 Feb 2022
ACTUAL working k-means benchmark on the heterogeneous system (without custom interrupt)
Verified using the following criterias:
-Loading different bitstreams. If a bitstream from another benchmark is loaded, k-means hangs when running the binary
-Comparing output solution with verif_output.csv 

The bitstream if in the sdsoc_kmeans_host folder
