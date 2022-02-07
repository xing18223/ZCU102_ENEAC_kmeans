#include "../sdsoc_heter_5_host/schedulers/MultiDynamic.h"
#include <chrono>
#include "sds_lib.h"
#include "Body.h"

using namespace std;
using namespace tbb;
using chrono::high_resolution_clock;
using chrono::duration_cast;
using chrono::duration;
using chrono::milliseconds;

int main(){
	n = 200;
	k = 5;
	max_range = 140;
    epochs = 100;

    Body body;
    Params p;
    p.numcpus = 1;
    p.numgpus = 1;
    p.gpuChunk = 50;
    Dynamic *hs = Dynamic::getInstance(&p);
    
    points = (Point *)sds_alloc(n*sizeof(Point));
	clusters = (Cluster *)sds_alloc(k*sizeof(Cluster));
	array_in = (float *)sds_alloc(5*p.gpuChunk*sizeof(float)); //Chunk_size
	array_out = (float *)sds_alloc(2*p.gpuChunk*sizeof(float));

    auto t1 = high_resolution_clock::now();
    input();
    auto t2 = high_resolution_clock::now();
	init_cluster();
	auto t3 = high_resolution_clock::now();
    convergence = false;
    iter = 0;
    while(!convergence && iter < epochs){
        iter ++;
        for(int i=0; i<k; i++){
            cur_cluster = &clusters[i];
            hs->heterogeneous_parallel_for(0, n, &body);
        }
        convergence = update_clusters();
    }
    auto t4 = high_resolution_clock::now();
	output();
	auto t5 = high_resolution_clock::now();
	duration<double, milli> ms_double_a = t2 - t1;
	duration<double, milli> ms_double_b = t3 - t2;
	duration<double, milli> ms_double_c = t4 - t3;
	duration<double, milli> ms_double_d = t5 - t4;
	cout << "k: " << k << endl;
	cout << "n :" << n << endl;
	cout << "max coord value: " << max_range << endl;
	cout << "Input: " << ms_double_a.count() << "ms" << endl;
	cout << "Init clusters: " << ms_double_b.count() << "ms" << endl;
	cout << "Compute: " << ms_double_c.count() << "ms" << endl;
	cout << "Output: " << ms_double_d.count() << "ms" << endl;
	cout << "Total iterations: " << iter << endl;

	sds_free(points);
	sds_free(clusters);
	sds_free(array_in);
	sds_free(array_out);

    return 0;
}
