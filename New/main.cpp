#include "genannInterface.h"
#include "AudioFile.h"
#include <iostream>
#include <chrono>
#include <math.h>
#include <thread>
using namespace std;

const unsigned int sampleSize=100,trainingFiles=108,yaxisvibAmount=12;
const unsigned int notWantedSize=70,testDataSize=15,notWanted2Size=78;
const double learnRate=0.01;
AudioFile<double> transfer[trainingFiles];
AudioFile<double> yaxisvib[yaxisvibAmount];
AudioFile<double> notWanted[notWantedSize];
AudioFile<double> notWanted2[notWanted2Size];
AudioFile<double> testData[testDataSize];
string testFileNames[testDataSize];
const char* saveFile="transferData.ann";
genann* gann;
gennanInterface gi(sampleSize,4,25,2);
double map(double value,double from1,double to1,double from2,double to2){
    return (value-from1)/(to1-from1)*(to2-from2)+from2;
}
void loadTransferData(){
    for(unsigned int i=0;i<trainingFiles;i++){
        string temp=to_string(i);
        transfer[i].load("/home/tauslitz/Documents/Audio for Training/TransferSounds/" + temp + ".wav");
    }
    for(unsigned int i=0;i<yaxisvibAmount;i++){
        string temp=to_string(i);
        yaxisvib[i].load("/home/tauslitz/Documents/Audio for Training/TransferSounds/vibyaxis" + temp + ".wav");
    }
    for(unsigned int i=0;i<notWantedSize;i++){
        string temp=to_string(i+1);
        notWanted[i].load("/home/tauslitz/Documents/Genann/Compare/NotPress" + temp + ".wav");
    }
    for(unsigned int i=0;i<notWanted2Size;i++){
        string temp=to_string(i+1);
        notWanted2[i].load("/home/tauslitz/Documents/Genann/Good Samples/Press" + temp + ".wav");
    }
}
void loadTestData(){
    testFileNames[0]="/home/tauslitz/Documents/Audio for Training/TransferSounds/20.wav";
    testFileNames[1]="/home/tauslitz/Documents/Audio for Training/TransferSounds/26.wav";
    testFileNames[2]="/home/tauslitz/Documents/Audio for Training/TransferSounds/35.wav";
    testFileNames[3]="/home/tauslitz/Documents/Audio for Training/TransferSounds/60.wav";
    testFileNames[4]="/home/tauslitz/Documents/Audio for Training/TransferSounds/55.wav";
    testFileNames[5]="/home/tauslitz/Documents/Genann/Good Samples/Press1.wav";
    testFileNames[6]="/home/tauslitz/Documents/Genann/Good Samples/Press6.wav";
    testFileNames[7]="/home/tauslitz/Documents/Genann/Good Samples/Press51.wav";
    testFileNames[8]="/home/tauslitz/Documents/Genann/Good Samples/Press71.wav";
    testFileNames[9]="/home/tauslitz/Documents/Genann/Good Samples/Press78.wav";
    testFileNames[10]="/home/tauslitz/Documents/Genann/Compare/NotPress38.wav";
    testFileNames[11]="/home/tauslitz/Documents/Genann/Compare/NotPress45.wav";
    testFileNames[12]="/home/tauslitz/Documents/Audio for Training/TransferSounds/67.wav";
    testFileNames[13]="/home/tauslitz/Documents/Audio for Training/TransferSounds/12.wav";
    testFileNames[14]="/home/tauslitz/Documents/Audio for Training/TransferSounds/vibyaxis1.wav";

    for(unsigned int i=0;i<testDataSize;i++){
        testData[i].load(testFileNames[i]);
    }
}
void trainOne(genann* gann,AudioFile<double> in[],unsigned int inSize,double*inputs,double*output){
    unsigned int channel=rand()%2,indexToUse=rand()%inSize,value=0;
    for(int i=0;i<in[indexToUse].getNumSamplesPerChannel();i++){
        *(inputs+value)=in[indexToUse].samples[channel][i];
        *(inputs+value)+=1;
        *(inputs+value)/=2;
        value++;
        if(value>=sampleSize){
            value=0;
            genann_train(gann,inputs,output,learnRate);
        }
    }
}
void whileTrainGann(genann* gann,unsigned long durationS){
    double* inputs=(double*)malloc(sizeof(double)*sampleSize);
    double* output=(double*)malloc(sizeof(double)*1);
    unsigned int prevValue=0,choice;
    unsigned long iterations=0;
    auto start=std::chrono::system_clock::now();
    while(true){

        choice=rand()%4;
        if(choice==0){
            *output=1.0;
            trainOne(gann,transfer,trainingFiles,inputs,output);
        }
        else if(choice==1){
            *output=1.0;
            trainOne(gann,yaxisvib,yaxisvibAmount,inputs,output);
        }
        else if(choice==2){
            *output=0;
            trainOne(gann,notWanted,notWantedSize,inputs,output);
        }
        else if(choice==3){
            *output=0;
            trainOne(gann,notWanted2,notWanted2Size,inputs,output);
        }
        iterations++;
        auto end=std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds=end-start;
        if(prevValue!=(unsigned int)elapsed_seconds.count()){
            cout<<"Time left: " << (unsigned int) (durationS-elapsed_seconds.count())<<"s"<<endl;
            prevValue=(unsigned int)elapsed_seconds.count();
        }

        if(elapsed_seconds.count()>durationS){
            cout<<"Time: " <<elapsed_seconds.count()<<"s"<<endl;
            cout<<"Amount of iterations: " << iterations<<endl;
            break;
        }
    }
    free(inputs);
    free(output);
}
void testFuncc(double inp){
    cout<<"testFunc: "<<inp<<endl;
}
void batchTrain(genann*gann,unsigned long amount){
    unsigned int prevValue=0,choice;
    //double* inputs=(double*)malloc(sizeof(double)*sampleSize);
    //double* output=(double*)malloc(sizeof(double)*1);

    double* in0=(double*)malloc(sizeof(double)*sampleSize);
    double* in1=(double*)malloc(sizeof(double)*sampleSize);
    double* in2=(double*)malloc(sizeof(double)*sampleSize);
    double* in3=(double*)malloc(sizeof(double)*sampleSize);

    double* o0=(double*)malloc(sizeof(double)*2);
    double* o1=(double*)malloc(sizeof(double)*2);
    double* o2=(double*)malloc(sizeof(double)*2);
    double* o3=(double*)malloc(sizeof(double)*2);

    //thread trainThread;
    thread thread0,thread1,thread2,thread3;
    bool t0=true,t1=true,t2=true,t3=true;
    //bool first=true;
    auto start=std::chrono::system_clock::now();
    for(unsigned long i=0;i<amount;i++){
        choice=rand()%4;
        /*
        if(!first){
            trainThread.join();
            //cout<<"Joined Thread"<<endl;
        }
        else{
            first=false;
            //cout<<"Set False"<<endl;
        }
        */
        if(choice==0){
            //*output=1.0;
            *o0=1.0;
            *(o0+1)=0;
            if(!t0)
                thread0.join();
            else
                t0=false;
            thread0=thread(trainOne,gann,transfer,trainingFiles,in0,o0);
            //trainThread=thread(why);
            //trainThread.join();
            //trainOne(gann,transfer,trainingFiles,inputs,output);
        }
        else if(choice==1){
            //*output=1.0;
            *o1=1.0;
            *(o1+1)=1.0;
            if(!t1)
                thread1.join();
            else
                t1=false;
            thread1=thread(trainOne,gann,yaxisvib,yaxisvibAmount,in1,o1);
            //trainOne(gann,yaxisvib,yaxisvibAmount,inputs,output);
        }
        else if(choice==2){
            //*output=0;
            *o2=0.0;
            *(o2+1)=0.0;
            if(!t2)
                thread2.join();
            else
                t2=false;
            thread2=thread(trainOne,gann,notWanted,notWantedSize,in2,o2);
            //trainOne(gann,notWanted,notWantedSize,inputs,output);
        }
        else if(choice==3){
            //*output=0;
            *o3=0.0;
            *(o3+1)=0.0;
            if(!t3)
                thread3.join();
            else
                t3=false;
            thread3=thread(trainOne,gann,notWanted2,notWanted2Size,in3,o3);
            //trainOne(gann,notWanted2,notWanted2Size,inputs,output);
        }
        //cout<<"Choice "<<choice<<endl;
        double test=(double)i/(double)amount;
        test*=100;
        test=round(test);
        if((unsigned int)test!=prevValue){
            prevValue=test;
            cout<<test<<"%"<<endl;
        }
    }
    auto end=std::chrono::system_clock::now();
    //trainThread.join();
    thread0.join();
    thread1.join();
    thread2.join();
    thread3.join();
    std::chrono::duration<double> elapsed_seconds=end-start;
    cout<<"Time taken: " << elapsed_seconds.count()<<"s"<<endl;
    //free(inputs);
    //free(output);
    free(in0);
    free(in1);
    free(in2);
    free(in3);
    free(o0);
    free(o1);
    free(o2);
    free(o3);
}
void test(genann*ann){
    double*input=(double*)malloc(sizeof(double)*sampleSize);
    vector<double> runValues;
    for(unsigned int i=0;i<testDataSize;i++){
        int samplesPerChannel=testData[i].getNumSamplesPerChannel();
        unsigned int value=0;
        double runValue=0;
        for(int g=0;g<samplesPerChannel;g++){
            *(input+value)=testData[i].samples[0][g];
            *(input+value)+=1;
            *(input+value)/=2;
            value++;
            if(value>=sampleSize){
                value=0;
                runValue+=*genann_run(ann,input);
            }
        }
        //cout<<"File " << i << " has the value of: " << runValue<<endl;
        runValues.push_back(runValue);
    }
    free(input);
    double min=5000,max=0;
    for(unsigned int i=0;i<runValues.size();i++){
        if(runValues[i]<min)
            min=runValues[i];
        else if(runValues[i]>max)
            max=runValues[i];
    }
    cout<<"Min: " << min << " Max: " <<max<<" Difference: " << (max-min)<<endl;
    //low2+(value-low1)*(high2-low2)/(high1-low1)
    for(unsigned int i=0;i<runValues.size();i++){
        double percentage=(runValues[i]-min)/(max-min);
        percentage*=100;
        cout<<"File " << i << " is " <<percentage<<"% a transfer"<<endl;
    }
}
void test2(genann*ann){
    double*input=(double*)malloc(sizeof(double)*sampleSize);
    vector<double> testValueTransfer,testValueYAxisVibration;
    for(unsigned int i=0;i<testDataSize;i++){
        int samplesPerChannel=testData[i].getNumSamplesPerChannel();
        unsigned int value=0;
        double tvT=0,tvV=0;
        for(int g=0;g<samplesPerChannel;g++){
            *(input+value)=testData[i].samples[0][g];
            *(input+value)+=1;
            *(input+value)/=2;
            value++;
            if(value>=sampleSize){
                value=0;
                double* retValue=(double*)genann_run(ann,input);
                tvT+=*retValue;
                tvV+=*(retValue+1);
            }
        }
        testValueTransfer.push_back(tvT);
        testValueYAxisVibration.push_back(tvV);
        tvT=0;
        tvV=0;
    }
    free(input);
    double minT=__DBL_MAX__,maxT=0,minV=__DBL_MAX__,maxV=0;
    for(unsigned int i=0;i<testValueTransfer.size();i++){
        if(testValueTransfer[i]<minT)
            minT=testValueTransfer[i];
        else if(testValueTransfer[i]>maxT)
            maxT=testValueTransfer[i];
    }
    for(unsigned int i=0;i<testValueYAxisVibration.size();i++){
        if(testValueYAxisVibration[i]<minV)
            minV=testValueYAxisVibration[i];
        else if(testValueYAxisVibration[i]>maxV)
            maxV=testValueYAxisVibration[i];
    }
    cout<<"Min for Transfer: " << minT << " Max for Transfer: " << maxT<<endl;
    cout<<"Min for Vibration: " <<minV<< " Max for Vibration: " << maxV<<endl;
    for(unsigned int i=0;i<testValueTransfer.size();i++){
        double pTestT=(testValueTransfer[i]-minT)/(maxT-minT);
        double pTestV=(testValueYAxisVibration[i]-minV)/(maxV-minV);
        pTestT*=100;
        pTestV*=100;
        cout<<"File " << i << " is a transfer:"<<pTestT<<"%\tHas Y axis vibration:"<<pTestV<<"%"<<endl;
    }
}
int main(){
    srand(time(NULL));
    gann=gi.ginit(saveFile);

    loadTransferData();
    loadTestData();
    cout<<"Training Data Loaded"<<endl;
    cout<<"Enter Which (1,0): ";
    int which;
    cin>>which;
    cout<<"Enter how many: ";
    int amount;
    cin>>amount;

    if(which==0)
        batchTrain(gann,amount);
    else if(which==1)
        whileTrainGann(gann,amount);
    //batchTrain(gann,amount);
    //whileTrainGann(gann,amount);
    cout<<endl<<"Which test (0,1): ";
    cin>>which;
    if(which==0)
        test(gann);
    else if(which==1)
        test2(gann);

    gi.closeG(gann);
    return 0;
}