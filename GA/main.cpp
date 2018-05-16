





#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>
#include <unistd.h>
using namespace std;

int const geneLength = 50;
int const individualCount = 200;
int const evolveIteration = 100;
class individual{
private:
    int gene[geneLength];
    
public:
    individual(){
        initialize();
    }
    
    void initialize(){
        for (int i=0; i<geneLength; i++) {
            if (rand()%2==0) gene[i] = 0;
            else gene[i] = 1;
        }
    }
    
    unsigned long int eval(int type){
        int tmp = 0;
        for (int i=0; i<geneLength; i++) tmp = tmp + gene[i];
        return tmp+1000;
    }
    
    int* genotype(){
        return gene;
    }
    
    void operator = (individual &a){
        for (int i=0; i<geneLength; i++) this->gene[i] = a.gene[i];
    }
    
    void crossOver(individual &a, int crossingPoint){
        int tmp;
        for (int i=crossingPoint; i<geneLength; i++) {
            tmp = this->gene[i];
            this->gene[i] = a.gene[i];
            a.gene[i] = tmp;
        }
    }
};

class speicies{
private:
    int* genotype[individualCount];
    individual IND[individualCount];
    individual nextGen[individualCount];
    double IND_selected_prob[individualCount];
    
    void random_shuffle(){
        int x, y;
        for (int i=0; i<1000; i++) {
            x = rand()%individualCount;
            y = rand()%individualCount;
            individual tmp;
            tmp = nextGen[x];
            nextGen[x] = nextGen[y];
            nextGen[y] = tmp;
        }
    }
    
    
    void calculateSelectedProb(){
        unsigned long int sum = 0;
        unsigned long int tmpProb[individualCount];
        for (int i=0; i<individualCount; i++) {
            tmpProb[i] = IND[i].eval(0);
            sum = sum + tmpProb[i];
        }
        for (int i=0; i<individualCount; i++) IND_selected_prob[i] = (double)tmpProb[i]/(double)sum;
    }
    
    void selectParent(int method){
        if (method==0){
            double tmp[individualCount];
            tmp[0] = IND_selected_prob[0];
            for (int i=1; i<individualCount; i++) {
                tmp[i] = IND_selected_prob[i] + tmp[i-1];
            }
            printf("\n");
            for (int i=0; i<individualCount; i++) {
                double lottery = (double)rand()/(double)RAND_MAX;
                for (int j=0; j<individualCount; j++) {
                    if (lottery<tmp[j]) {
                        nextGen[i] = IND[j];
                        break;
                    }
                }
            }
        }
        
        else if(method==1){
            float p = 0.6;
            int x, y;
            for (int i=0; i<individualCount; i++) {
                x = rand()%individualCount;
                y = rand()%individualCount;
                if (IND_selected_prob[x] > IND_selected_prob[y]) {
                    if (rand()/(float)RAND_MAX<=p) nextGen[i] = IND[x];
                    else nextGen[i] = IND[y];
                }
                else{
                    if (rand()/(float)RAND_MAX<=p) nextGen[i] = IND[y];
                    else nextGen[i] = IND[x];
                }
            }
        }
        random_shuffle();
        
    }
    
    void crossOver(int NPoint, float prob){
        //currently only support NPoint = 1
        for (int i=0; i<individualCount; i=i+2) {
            int crossPoint = rand()%geneLength;
            nextGen[i].crossOver(nextGen[i+1], crossPoint);
        }
    }
    
    void replaceParent(){
        for (int i=0; i<individualCount; i++) IND[i] = nextGen[i];
    }
    
    
public:
    void initialize(){
        for (int i=0; i<individualCount; i++) {
            IND[i].initialize();
        }
    }
    void evolve(int iteration, int parentSelectionMethod){
        FILE *fp;
        fp = fopen("/Users/eric/Desktop/output/output.csv", "a+");
        for (int i=0; i<iteration; i++) {
            fprintf(fp, "%d,", best()-1000);
            calculateSelectedProb();
            selectParent(parentSelectionMethod);
            crossOver(1, 1.0);
            replaceParent();
        }
        fprintf(fp, "\n");
        fclose(fp);
    }
    
    unsigned long int best(){
        unsigned long int high = 0;
        unsigned long int tmp = 0;
        for (int i=0; i<individualCount; i++) {
            tmp = IND[i].eval(1);
            if (high < tmp) {
                high = tmp;
            }
        }
        return high;
    }
};



int main() {
    speicies s;
    for (int i=0; i<10; i++) {
        srand (time(NULL));
        usleep(1000000);
        s.initialize();
        s.evolve(evolveIteration, 1);
    }
    
    return 0;
}
