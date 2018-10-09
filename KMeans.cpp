#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <random>
#include <time.h>

//type to represent a cluster
typedef std::vector<double *> cluster;

//euclidean distance function
double distance(double * x, double * y, int len){
    double d=0;
    for(int i=0;i<len;i++){
        d+= (x[i]-y[i])*(x[i]-y[i]);
    }
    return sqrt(d);

}

double SSE(cluster * Cset, double ** means, int attr, int k){

    double sse=0;
    for(int j=0; j<k;j++){
        for(int l=0;l<attr;l++){
            for(int i=0; i<Cset[j].size();i++){
                sse += pow(distance(Cset[j].at(i),means[j], attr),2);
            }

        }
    }
    return sse;
}

/*
* Kmeans function receives a set of examples x and divides
* them to k clusters
*/
cluster * Kmeans(double ** x,int attr, int examples, int k){
    cluster * Cset; //set to hold resulting clusters

    //initializing means
    double ** means = new double * [k];
    for(int j=0; j<k;j++){
        means[j] = new double[attr];
        //pick random sample form x for each mean
        int indx = rand() % examples;
        for(int i=0;i<attr;i++){
            means[j][i]= x[indx][i];
        }

    }
    bool converge = false;
    while(!converge){
        Cset = new cluster[k];

        //assignment step
        for(int i=0;i<examples;i++){
            int index=-1;           //index of closest mean
            double mind=99999999;   //minimum distance
            for(int j=0; j<k;j++){
                //finding closest mean for point x[i]
                double d = distance(means[j],x[i],attr);
                if(d<=mind){
                    mind=d;
                    index=j;
                }
            }
            Cset[index].push_back(x[i]);
        }

        //update step
        double * tempMean = new double [attr];
        for(int j=0; j<k;j++){
            //if cluster isn't empty then update its mean
            if(Cset[j].size()>0){
                for(int l=0;l<attr;l++){
                    tempMean[l]= 0;
                    for(int i=0; i<Cset[j].size();i++){
                        //mean of cluster j attribute l +=
                        //attribute l of example i in cluster j
                        tempMean[l] += Cset[j].at(i)[l];       //summing each x value of attribute l
                    }
                    tempMean[l] *= 1.0/(double)Cset[j].size();  //finding mean of attribute l
                    if(means[j][l]== tempMean[l]){
                        converge=true;
                    }else{
                        means[j][l]=tempMean[l];
                        converge = false;   //if any mean changes then it did not converge
                    }
                }
            }
        }

        if(!converge)
            delete[] Cset;  //reset set
    }
    //clean up memory
    for(int i=0;i<k;i++){
        delete[] means[i];
    }
    delete[] means;
    return Cset;
}

double ** readCSVData(char * fileName, int rows, int cols){
    std::ifstream inf;
    inf.open(fileName);
    std::string s;
    double ** data = new double * [rows];
    for(int i=0; i<rows;i++){
        data[i] = new double [cols];
    }
   for(int i=0;i<rows;i++){
        inf >> s;
        char * temp,* temp2 = new char [s.length()-1];
        strcpy(temp2,s.c_str());
        temp = strtok(temp2,",\n");
        data[i][0] = atof(temp);
        for(int j=1;j<cols;j++){
            data[i][j] = atof(strtok(NULL,",\n"));
        }
    }
    inf.close();
    return data;
}
void printClusterSet(cluster * set,int instances, int attributes, int k){
    for(int i=0; i<k;i++){
        std::cout<<"\nCluster "<<i+1<<"\n";
        for(int j=0; j< set[i].size();j++){
            for(int k=0; k<attributes;k++){
                std::cout<<set[i].at(j)[k]<< " ";
            }
        std::cout<<"\n";
        }
    }
}
int main(){

    int attributes = 2;
    int instances = 400;
    int k=4;
    double ** train_data = readCSVData("cluster-data-for-k-means.csv",instances,attributes);
    srand(time(NULL));
    cluster * set = Kmeans(train_data,attributes,instances,k);
    printClusterSet(set,instances,attributes,k);
    return 0;
}

