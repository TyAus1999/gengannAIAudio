#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include "genann.h"
#include "AudioFile.h"
#include <string.h>
#include <chrono>
#include <ctime>
using namespace std;
const int amountOfPress=78;
const int amountOfNotPress=65;
AudioFile<double> allFiles[240];
AudioFile<double> press[amountOfPress];
AudioFile<double> notPress[amountOfNotPress];
int sampleSize=100;
double learnRate=0.01;
double fSig(double in){
    return in/(1+abs(in));
}
void addingTest(){
    const char* fName="gandat.ann";
    FILE *gandat=fopen(fName,"r");
    genann*adder;
    if(!gandat){
        adder=genann_init(2,1,3,1);
        cout<<"INIT"<<endl;
    }
    else{
        adder=genann_read(gandat);
        cout<<"Read"<<endl;
        fclose(gandat);
    }
    //adder=genann_init(2,1,3,1);
    double * inputs=(double*)malloc(sizeof(double)*2);
    double * outputa=(double*)malloc(sizeof(double)*1);
    unsigned long p;
    unsigned long max=0xfffffff;
    int prevValue=-1;
    for(p=0;p<max;p++){
        *inputs=(double)rand()/(double)RAND_MAX;
        *(inputs+1)=(double)rand()/(double)RAND_MAX;
        *outputa=*(inputs)+*(inputs+1);
        genann_train(adder,inputs,outputa,0.1);
        double test=(double)p/(double)max;
        test*=100;
        test=round(test);
        if((int)test%5==0 && prevValue!=test){
            prevValue=test;
            cout<<test<<"%"<<endl;
        }
        
    }
    *inputs=-0.3;
    *(inputs+1)=0.2;
    cout<<"-0.3+0.2=-0.1, "<<*genann_run(adder,inputs)<<endl;
    *inputs=0.5;
    *(inputs+1)=0.2;
    cout<<"0.5+0.2=0.7, "<<*genann_run(adder,inputs)<<endl;
    *inputs=0.9;
    *(inputs+1)=0.2;
    cout<<"0.9+0.2=1.1, "<<*genann_run(adder,inputs)<<endl;
    FILE*gandat2=fopen(fName,"w");
    genann_write(adder,gandat2);
    fclose(gandat2);
    free(inputs);
    free(outputa);
    genann_free(adder);
}
bool testBigEndianess(){//returns true if big endian
    unsigned short test=0x0001;
    unsigned char * e=(unsigned char*)&test;
    if (*e==1)return true;
    return false;
}

void loadAllData(){
    
    for(int i=0;i<240;i++){
        string temp=to_string((i+1));
        //char*temp=to_string((i+1)+".wav");
        if(i<9){
            allFiles[i].load("SplitFile/Machine-0"+temp + ".wav");
        }
        else{
            allFiles[i].load("SplitFile/Machine-"+temp + ".wav");
        }
    }
}
genann* ginit(){
    const char* fName="gandataudio.ann";
    FILE* gda=fopen(fName,"r");
    genann*ann;
    if(!gda){
        ann=genann_init(sampleSize,4,20,1);
        cout<<"Init"<<endl;
    }
    else{
        ann=genann_read(gda);
        fclose(gda);
        cout<<"Read"<<endl;
    }
    return ann;
}
void closeG(genann * ann){
    const char* fName="gandataudio.ann";
    FILE* gda=fopen(fName,"w");
    genann_write(ann,gda);
    fclose(gda);
    genann_free(ann);
}
void loadTraining(){
    for(int i=0;i<amountOfPress;i++){
        string temp=to_string(i+1);
        press[i].load("Good Samples/Press" + temp + ".wav");
    }
    for(int i=0;i<amountOfNotPress;i++){
        string temp=to_string(i+1);
        notPress[i].load("Compare/NotPress"+temp+".wav");
    }
}
void train(genann*ann,unsigned int max){
    unsigned int i,prevValue=-1,p,np,choi;
    for(i=0;i<max;i++){
        choi=rand()%2;
        //cout<<choi<<endl;
        if(choi==0){//p
            p=rand()%amountOfPress;
            int maxS=press[p].getNumSamplesPerChannel();
            double*samp=(double*)malloc(sizeof(double)*sampleSize);
            double*output=(double*)malloc(sizeof(double)*1);
            *output=1.0;
            int value=0;
            for(int pr=0;pr<maxS;pr++){
                *(samp+value)=press[p].samples[0][pr];
                *(samp+value)+=1;
                *(samp+value)/=2;
                value++;
                if(value>=sampleSize){
                    value=0;
                    genann_train(ann,samp,output,learnRate);
                    //cout<<"Train 0"<<endl;
                }
            }
            value=0;
            for(int pr=0;pr<maxS;pr++){
                *(samp+value)=press[p].samples[1][pr];
                *(samp+value)+=1;
                *(samp+value)/=2;
                value++;
                if(value>=sampleSize){
                    value=0;
                    genann_train(ann,samp,output,learnRate);
                    //cout<<"Train 1"<<endl;
                }
            }
            free(samp);
            free(output);
        }
        else{//np
            np=rand()%amountOfNotPress;
            int maxS=notPress[np].getNumSamplesPerChannel();
            double*samp=(double*)malloc(sizeof(double)*sampleSize);
            double*output=(double*)malloc(sizeof(double)*1);
            *output=0.0;
            int value=0;
            for(int pr=0;pr<maxS;pr++){
                *(samp+value)=notPress[np].samples[0][pr];
                *(samp+value)+=1;
                *(samp+value)/=2;
                value++;
                if(value>=sampleSize){
                    value=0;
                    genann_train(ann,samp,output,learnRate);
                    //cout<<"Train np 0"<<endl;
                }
            }
            value=0;
            for(int pr=0;pr<maxS;pr++){
                *(samp+value)=notPress[np].samples[1][pr];
                *(samp+value)+=1;
                *(samp+value)/=2;
                value++;
                if(value>=sampleSize){
                    value=0;
                    genann_train(ann,samp,output,learnRate);
                    //cout<<"Train np 1"<<endl;
                }
            }
            free(samp);
            free(output);
        }
        double test=(double)i/(double)max;
        test*=100;
        test=round(test);
        if((int)test%1==0 && prevValue!=test){
            prevValue=test;
            cout<<test<<"%"<<endl;
        }

    }
}
void whileTrain(genann*ann,unsigned long durationS){
    unsigned int p,np,choi;
    unsigned int prevValue=0;
    auto start = std::chrono::system_clock::now();
    while(true){
        choi=rand()%2;
        //cout<<choi<<endl;
        if(choi==0){//p
            p=rand()%amountOfPress;
            int maxS=press[p].getNumSamplesPerChannel();
            double*samp=(double*)malloc(sizeof(double)*sampleSize);
            double*output=(double*)malloc(sizeof(double)*1);
            *output=1.0;
            int value=0;
            for(int pr=0;pr<maxS;pr++){
                *(samp+value)=press[p].samples[0][pr];
                *(samp+value)+=1;
                *(samp+value)/=2;
                value++;
                if(value>=sampleSize){
                    value=0;
                    genann_train(ann,samp,output,learnRate);
                    //cout<<"Train 0"<<endl;
                }
            }
            value=0;
            for(int pr=0;pr<maxS;pr++){
                *(samp+value)=press[p].samples[1][pr];
                *(samp+value)+=1;
                *(samp+value)/=2;
                value++;
                if(value>=sampleSize){
                    value=0;
                    genann_train(ann,samp,output,learnRate);
                    //cout<<"Train 1"<<endl;
                }
            }
            free(samp);
            free(output);
        }
        else{//np
            np=rand()%amountOfNotPress;
            int maxS=notPress[np].getNumSamplesPerChannel();
            double*samp=(double*)malloc(sizeof(double)*sampleSize);
            double*output=(double*)malloc(sizeof(double)*1);
            *output=0.0;
            int value=0;
            for(int pr=0;pr<maxS;pr++){
                *(samp+value)=notPress[np].samples[0][pr];
                *(samp+value)+=1;
                *(samp+value)/=2;
                value++;
                if(value>=sampleSize){
                    value=0;
                    genann_train(ann,samp,output,learnRate);
                    //cout<<"Train np 0"<<endl;
                }
            }
            value=0;
            for(int pr=0;pr<maxS;pr++){
                *(samp+value)=notPress[np].samples[1][pr];
                *(samp+value)+=1;
                *(samp+value)/=2;
                value++;
                if(value>=sampleSize){
                    value=0;
                    genann_train(ann,samp,output,learnRate);
                    //cout<<"Train np 1"<<endl;
                }
            }
            free(samp);
            free(output);
        }
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;

        //std::cout << "finished computation at " << std::ctime(&end_time)
              //<< "elapsed time: " << elapsed_seconds.count() << "s\n";
        if(prevValue!=(unsigned int)elapsed_seconds.count()){
            cout<<"Time left: " << (unsigned int)(durationS-elapsed_seconds.count())<<"s"<<endl;
            prevValue=(unsigned int)elapsed_seconds.count();
        }
        
        if(elapsed_seconds.count()>durationS){
            cout<<"Time: " << elapsed_seconds.count()<<"s"<<endl;
            break;
        }
    }
}
void testAnn(genann*ann){
    //Go through each of the files and sample from there
    //print which ones the ai thinks is a press
    vector<double> thoughts;
    for(int file=0;file<240;file++){
        int maxS=allFiles[file].getNumSamplesPerChannel();
        double*samp=(double*)malloc(sizeof(double)*sampleSize);
        int value=0;
        double average=0;
        for(int i=0;i<maxS;i++){
            *(samp+value)=allFiles[file].samples[0][i];
            *(samp+value)+=1;
            *(samp+value)/=2;
            value++;
            if(value>=sampleSize){
                value=0;
                average+=*genann_run(ann,samp);
            }
        }
        value=0;
        for(int i=0;i<maxS;i++){
            *(samp+value)=allFiles[file].samples[1][i];
            *(samp+value)+=1;
            *(samp+value)/=2;
            value++;
            if(value>=sampleSize){
                value=0;
                average+=*genann_run(ann,samp);
            }
        }
        //average/=maxS;
        //average/=2;
        //cout<<average<<endl;
        //average=average/(double)2;
        thoughts.push_back(average);
        //average*=100;
        if(average>150)
            cout<<"The value for Machine-" << (file+1)<< " is " <<average<<endl;
    }
    
}
void testAnnPickedOut(genann*ann){
    AudioFile<double> test[5];
    test[0].load("Test1.wav");
    test[1].load("Test2.wav");
    test[2].load("Test3.wav");
    test[3].load("Test4.wav");
    test[4].load("Test5.wav");

    vector<double> confidence;

    for(int i=0;i<5;i++){
        int maxS=test[i].getNumSamplesPerChannel();
        double*samp=(double*)malloc(sizeof(double)*sampleSize);
        int value=0;
        double average=0;
        for(int i2=0;i2<maxS;i2++){
            *(samp+value)=test[i].samples[0][i2];
            *(samp+value)+=1;
            *(samp+value)/=2;
            value++;
            if(value>=sampleSize){
                value=0;
                average+=*genann_run(ann,samp);
            }
        }
        value=0;
        for(int i2=0;i2<maxS;i2++){
            *(samp+value)=test[i].samples[1][i2];
            *(samp+value)+=1;
            *(samp+value)/=2;
            value++;
            if(value>=sampleSize){
                value=0;
                average+=*genann_run(ann,samp);
            }
        }
        //average=fSig(average);
        confidence.push_back(average);
        cout<<"Value for Test"<<(i+1)<<".wav is: "<<average<<endl;
    }
    double totalConfidence=0;
    for(int i=0;i<confidence.size();i++){
        totalConfidence+=confidence[i];
    }
    cout<<"Total confidence: " << totalConfidence<<endl;
    for (int i=0;i<confidence.size();i++){
        cout<<confidence[i]/totalConfidence<<endl;
    }
}

int main(int argc, char *argv[])
{
    loadAllData();
    loadTraining();
    srand(time(NULL));
    //Load data
    genann*ann=ginit();

    //train(ann,100);
    //whileTrain(ann,2400);
    //testAnn(ann);
    testAnnPickedOut(ann);
    
    closeG(ann);

    
    
    /*
    unsigned int r,choice,max=5000,prevValue=-1;
    for(unsigned int i=0;i<max;i++){
        r=rand()%2;
        choice=rand()%10;
        if(r==0){//p
            int nSamples=p[choice].getNumSamplesPerChannel();
            double*samp=(double*)malloc(sizeof(double)*100);
            double*output=(double*)malloc(sizeof(double)*1);
            *output=1.0;
            int value=0;

            for(int it=0;it<nSamples;it++){
                *(samp+value)=p[choice].samples[0][nSamples];
                *(samp+value)+=1;
                *(samp+value)=*(samp+value)/2;
                value++;
                if(value==100){
                    value=0;
                    genann_train(ann,samp,output,0.1);
                }
            }
            free(output);
            free(samp);
        }
        else if(r==1){//np
            int nSamples=np[choice].getNumSamplesPerChannel();
            double*samp=(double*)malloc(sizeof(double)*100);
            double*output=(double*)malloc(sizeof(double)*1);
            *output=0.0;
            int value=0;

            for(int it=0;it<nSamples;it++){
                *(samp+value)=p[choice].samples[0][nSamples];
                *(samp+value)+=1;
                *(samp+value)=*(samp+value)/2;
                value++;
                if(value==100){
                    value=0;
                    genann_train(ann,samp,output,0.1);
                }
            }
            free(output);
            free(samp);
        }
        else{
            cout<<"Bad Number"<<endl;
        }
        double test=(double)i/(double)max;
        test*=100;
        test=round(test);
        if((int)test%10==0 && prevValue!=test){
            prevValue=test;
            cout<<test<<"%"<<endl;
        }
    }*/

    /*
    cout<<"Testing with good sample"<<endl;
    AudioFile<double> test;
    //test.load("Test/Test2.wav");
    test.load("Compare/WholeThing.wav");
    int testS=test.getNumSamplesPerChannel();
    double*samp=(double*)malloc(sizeof(double)*100);
    int value=0;
    vector<double> testValues;
    for(int i=0;i<testS;i++){
        *(samp+value)=p[0].samples[0][i];
        *(samp+value)+=1;
        *(samp+value)=*(samp+value)/2;
        value++;
        if(value==100){
            value=0;
            double temp=*genann_run(ann,samp);
            //cout<<temp<<endl;
            testValues.push_back(temp);
        }
    }
    double sum=0;
    for(unsigned int i=0;i<testValues.size();i++){
        sum+=testValues[i];
    }
    sum/=testValues.size();
    sum*=100;
    free(samp);
    cout<<"Guess: " <<sum<<"%"<<endl;
    */

    

    /* 
    AudioFile<double> af;
    af.load("Good Samples/Press1.wav");
    af.printSummary();
    //End load data

    int numberOfSamples=af.getNumSamplesPerChannel();
    int numChannels=af.getNumChannels();
    for(int i=0;i<numberOfSamples;i++){
        double currentSample=af.samples[0][i];
        cout<<currentSample<<endl;
    }
    */

    return 0;
}