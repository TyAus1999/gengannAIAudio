#include "genann.h"
class gennanInterface{
    public:
        gennanInterface(int sampleSize,int hiddenLayers,int nodesInHidden,int output){
            gennanInterface::sampleSize=sampleSize;
            gennanInterface::hiddenLayers=hiddenLayers;
            gennanInterface::nodesInHidden=nodesInHidden;
            gennanInterface::output=output;
        }
        genann* ginit(const char* fileName){
            gennanInterface::fileName=fileName;
            FILE* gda=fopen(fileName,"r");
            genann*ann;
            if(!gda){
                ann=genann_init(sampleSize,hiddenLayers,nodesInHidden,output);
            }
            else{
                ann=genann_read(gda);
                fclose(gda);
            }
            return ann;
        }
        void closeG(genann*ann){
            FILE* gda=fopen(fileName,"w");
            genann_write(ann,gda);
            fclose(gda);
            genann_free(ann);
        }
    private:
        int sampleSize,hiddenLayers,nodesInHidden,output;
        const char* fileName;
        
};