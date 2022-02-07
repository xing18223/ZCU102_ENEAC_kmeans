#include "kernelkmeans.h"
#include "KMEANS.h"

class Body{
public:
    bool firsttime;
public:
    void OperatorGPU(int begin, int end, int id){
        int chunk_size = end-begin;
        for(int i=0; i!=chunk_size; i++){
            array_in[i*5] = points[begin+i].get_x();
            array_in[(i*5)+1] = points[begin+i].get_y();
            array_in[(i*5)+2] = points[begin+i].get_z();
            array_in[(i*5)+3] = points[begin+i].get_cluster_id();
            array_in[(i*5)+4] = points[begin+i].get_min_distance();
        }

        kernel_kmeans(array_in, array_out, cur_cluster->get_cluster_id(), cur_cluster->get_x(), cur_cluster->get_y(), cur_cluster->get_z(), begin, end);

        for(int j=0; j!=chunk_size; j++){
            if(array_out[(j*2)+1]<points[begin+j].get_min_distance()){
                points[begin+j].update_cluster_id(array_out[j*2]);
                points[begin+j].update_min_distance(array_out[(j*2)+1]);
            }
        }
    }
    
    void OperatorCPU(int begin, int end){
        kmeans_CPU(begin, end);
    }
};


