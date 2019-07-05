#include <iostream>
#include "genann.h"
#include "drawingLib.h"
#include "AIGamePong.h"

using namespace std;
void closeG(genann * ann){
    const char* fName="gandatPong.ann";
    FILE* gda=fopen(fName,"w");
    genann_write(ann,gda);
    fclose(gda);
    genann_free(ann);
}
genann* ginit(){
    const char* fName="gandatPong.ann";
    FILE* gda=fopen(fName,"r");
    genann*ann;
    if(!gda){
        ann=genann_init(4,6,10,2);
        cout<<"Init"<<endl;
    }
    else{
        ann=genann_read(gda);
        fclose(gda);
        cout<<"Read"<<endl;
    }
    return ann;
}

int main(void)
{
    
    drawingLib dhandle(750,500);
    Paddle p1(0,275,2,&dhandle);
    //Paddle p2(dhandle.getWidth()-p1.getWidth(),0,1,&dhandle);
    //Ball b(250,275,&dhandle,&p1,&p2);
    rBall rb(250,275,&dhandle,&p1);

    genann*ann;
    ann=ginit();
    double*inputs=(double*)malloc(sizeof(double)*4);
    //0=rbx,1=rxy,2=px,3=py
    double*output=(double*)malloc(sizeof(double)*2);

    double*desire=(double*)malloc(sizeof(double)*2);
    //0=down,1=up

    //bool goingUp=false;
    int times=0;
    unsigned long maxHits=0,totalHits=0,maxGens=25;
    double averageHits=0;
    while(times<maxGens){
        cout<<"Times: " << times<<endl;
        dhandle.fillRect(0,0,750,500,0);
        int startX=rand()%200+(750-200);
        int startY=rand()%500;
        int paddleStartY=startY;
        if(paddleStartY>500-p1.getHeight())paddleStartY-=p1.getHeight();
        Paddle p1(0,paddleStartY,2,&dhandle);
        rBall rb(startX,startY,&dhandle,&p1);
        while(!rb.didHitWall()){
            rb.move();

            //make AI ball to counter pong paddle
            *inputs=(double)rb.getX();
            *(inputs+1)=(double)rb.getY();
            *(inputs+2)=(double)p1.getX();
            *(inputs+3)=(double)p1.getY();
            double*tempP=(double*)genann_run(ann,inputs);
            *output=*tempP;
            *(output+1)=*(tempP+1);
            
            
            if(*output>=0.7 && *(output+1)<0.7){
                p1.moveDown();//Make paddle move by percentage
            }
            else if(*output<0.7 && *(output+1)>=0.7){
                p1.moveUp();
            }
            /*
            if(*output>*(output+1)){
                p1.moveDown();
            }
            else if(*output<*(output+1)){
                p1.moveUp();
            }*/
            cout<<"Up:"<<*output<<" Down:"<<*(output+1)<<" Times: " << times<<" Max hits: " << maxHits<<" Average Hits: "<<averageHits<< endl;

            if(rb.getY()+(rb.getHeight()/2)<p1.getY()+(p1.getHeight()/2)){//higher
                *desire=0;
                *(desire+1)=1;
                genann_train(ann,inputs,desire,0.1);
            }
            else if(rb.getY()+(rb.getHeight()/2)>p1.getY()+(p1.getHeight()/2)){//lower
                *desire=1;
                *(desire+1)=0;
                genann_train(ann,inputs,desire,0.1);
            }
            /*
            if(goingUp){
                p1.moveUp();
                if(p1.isAtMin()){
                    goingUp=false;
                }
            }
            else{
                p1.moveDown();
                if(p1.isAtMax()){
                    goingUp=true;
                }
            }
            b.move();
            */
        dhandle.sleepM(5);
        }
        if(rb.getHits()>maxHits)maxHits=rb.getHits();
        totalHits+=rb.getHits();
        times++;
        averageHits=(double)totalHits/(double)times;
    }
    dhandle.close();
    closeG(ann);
    free(inputs);
    free(output);
    free(desire);
    return 0;
 }