#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
// #include "immintrin.h"
//#include "arm_neon.h"
//#include "tbb/tbb.h"
//#include "tbb/parallel_for.h"
//#include "tbb/blocked_range.h"

#include "Point.h"
#include "Cluster.h"

using namespace std;

int n;
int k;
int max_range;
int epochs;
bool convergence;
int iter;

Point *points;
Cluster *clusters;
Cluster *cur_cluster;
float *array_in;
float *array_out;

void output(){
	ofstream myfile;
	myfile.open("output.csv");
	for(int i=0; i<n; i++){
		myfile << points[i].get_x() << "," << points[i].get_y() << "," << points[i].get_z() << "," << points[i].get_cluster_id() << endl;
	}
	myfile.close();
}

void input(){
	string line;
	ifstream file("input1.csv");
	int i = 0;

	while(getline(file, line)) {
		stringstream lineStream(line);
		string bit;
		float x, y, z;
		getline(lineStream, bit, ',');
		x = stof(bit);
		getline(lineStream, bit, ',');
		y = stof(bit);
		getline(lineStream, bit, '\n');
		z = stof(bit);

		points[i] = Point(x,y,z);
		i++;
	}

}

void init_cluster(){
	// srand(time(NULL));
	// for(int i=0; i<k; i++){
	// 	clusters[i] = Cluster(rand()%max_range, rand()%max_range, rand()%max_range, i);
	// 	// cout << clusters[i].get_x() << "," << clusters[i].get_y() << "," << clusters[i].get_z() << "," << clusters[i].get_cluster_id() << endl;
	// }
    clusters[0] = Cluster(50,40,55,0);
    clusters[1] = Cluster(59,71,11,1);
    clusters[2] = Cluster(29,98,88,2);
    clusters[3] = Cluster(45,28,32,3);
    clusters[4] = Cluster(54,63,46,4);
}

bool update_clusters(){
	bool convergence = true;
	for(int i=0; i<n; i++){
		clusters[points[i].get_cluster_id()].add_point(points[i]);
	}

	for(int j=0; j<k; j++){
		convergence = convergence && clusters[j].update_cluster_coord();
		clusters[j].reset();
	}
	return convergence;
}

float eucl_dist_CPU(Cluster *cluster, Point *point){
    // __m256 vec1 = {point->get_x(), point->get_y(), point->get_z()};
    // __m256 vec2 = {cluster->get_x(), cluster->get_y(), cluster->get_z()};
    // __m256 diff = _mm256_sub_ps(vec1, vec2);
    // __m256 square = _mm256_mul_ps(diff, diff);
    // float distance = sqrt(square[0]+square[1]+square[2]);
//	float32x4_t vec1 = {point->get_x(), point->get_y(), point->get_z()};
//    float32x4_t vec2 = {cluster->get_x(), cluster->get_y(), cluster->get_z()};
//    float32x4_t diff = vsubq_f32(vec1, vec2);
//    float32x4_t square = vmulq_f32(diff, diff);
//    float distance = sqrt(square[0]+square[1]+square[2]);
	float distance =
	    sqrt(pow(cluster->get_x() - point ->get_x(), 2)
	    + pow(cluster->get_y() - point ->get_y(), 2)
	    + pow(cluster->get_z() - point ->get_z(), 2));
    return distance;
}

void kmeans_CPU(int begin, int end){
    for(int i=begin; i!=end; i++){
        Point *p = &points[i];
        float distance = eucl_dist_CPU(cur_cluster, p);
        if(distance < p->get_min_distance()){
            p->update_min_distance(distance);
            p->update_cluster_id(cur_cluster->get_cluster_id());
        }
    }
}
