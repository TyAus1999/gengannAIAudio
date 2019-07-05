#include <random>
class Paddle{
    public:
        Paddle(int x,int y,int amount, drawingLib * dhandle){
            Paddle::x=x;
            Paddle::y=y;
            Paddle::amount=amount;
            Paddle::dhandle=dhandle;
            dhandle->fillRect(x,y,width,height,1);
            atMin=false;
            atMax=false;
        }
        void moveUp(){
            dhandle->fillRect(x,y,width,height,0);
            y-=amount;
            if(y<0){
                y=0;
                atMin=true;
                atMax=false;
            }
            else atMin=false;
            dhandle->fillRect(x,y,width,height,rand()%5+1);
        }
        void moveDown(){
            dhandle->fillRect(x,y,width,height,0);
            y+=amount;
            if(y>(dhandle->getHeight()-height)){
                y=dhandle->getHeight()-height;
                atMax=true;
                atMin=false;
            }
            else atMax=false;
            dhandle->fillRect(x,y,width,height,rand()%5+1);
        }
        int getX(){return x;}
        int getY(){return y;}
        int getWidth(){return width;}
        int getHeight(){return height;}
        bool isAtMax(){return atMax;}
        bool isAtMin(){return atMin;}
        void clear(){
            dhandle->fillRect(x,y,width,height,0);
        }
    private:
        int x,y,amount;
        drawingLib * dhandle;
        bool atMin,atMax;
        int width=10,height=50;

};
class rBall{
    public:
        rBall(int x,int y,drawingLib* dhandle,Paddle*leftP){
            rBall::x=x;
            rBall::y=y;
            rBall::dhandle=dhandle;
            rBall::leftP=leftP;
            rBall::xDir=0;
            rBall::yDir=0;
            rBall::hitWall=false;
            rBall::hits=0;
            //rBall::speedX=(rand()%8)+1;
        }
        void move(){
            if(!hitWall){
                dhandle->fillRect(x,y,width,height,0);
                if(xDir==1){
                    x+=speedX;
                    if(x>dhandle->getWidth()-width){
                        x=dhandle->getWidth()-width;
                        xDir=0;
                        //rBall::speedX=(rand()%8)+1;
                    }
                }
                else{
                    x-=speedX;
                    if(x<leftP->getX()+leftP->getWidth()&&
                    y<=leftP->getY()+leftP->getHeight() && 
                    y>=leftP->getY()&&
                    y+height<=leftP->getY()+leftP->getHeight()&&
                    y+height>=leftP->getY()){
                        x=leftP->getX()+leftP->getWidth();
                        xDir=1;
                        hits++;
                    }
                    else if(x<0){
                        x=0;
                        xDir=1;
                        hitWall=true;
                    }
                }

                if(yDir==1){
                    y+=speedY;
                    if(y>dhandle->getHeight()-height){
                        y=dhandle->getHeight()-height;
                        yDir=0;
                    }
                }
                else{
                    y-=speedY;
                    if(y<0){
                        y=0;
                        yDir=1;
                    }
                }
            }
            dhandle->fillRect(x,y,width,height,rand()%5+1);
        }
        int getX(){return x;}
        int getY(){return y;}
        int getWidth(){return width;}
        int getHeight(){return height;}
        bool didHitWall(){return hitWall;}
        void clear(){
            dhandle->fillRect(x,y,width,height,0);
        }
        void changexDir(int value){xDir=value;}
        void changeyDir(int value){yDir=value;}
        unsigned long getHits(){return hits;}
    private:
        int yDir,xDir;
        int x,y;
        int width=5,height=5;
        int speedY=1,speedX=2;
        drawingLib* dhandle;
        Paddle*leftP;
        bool hitWall;
        unsigned long hits;
};
class Ball{
    public:
        Ball(int x,int y,drawingLib* dhandle,Paddle*leftP, Paddle*rightP){
            Ball::x=x;
            Ball::y=y;
            Ball::dhandle=dhandle;
            Ball::leftP=leftP;
            Ball::rightP=rightP;
            Ball::xDir=0;
            Ball::yDir=0;
            Ball::hitWall=false;
        }
        void clear(){
            dhandle->fillRect(x,y,width,height,0);
        }
        void move(){
            if(!hitWall){
                dhandle->fillRect(x,y,width,height,0);
                if(xDir==1){
                    x+=speedX;
                    if(x>rightP->getX()-width && 
                    y<=rightP->getY()+rightP->getHeight() && 
                    y>=rightP->getY()&&
                    y+height<=rightP->getY()+rightP->getHeight()&&
                    y+height>=rightP->getY()){
                        x=rightP->getX();
                        xDir=0;
                    }
                    else if(x>dhandle->getWidth()-width){
                        x=dhandle->getWidth()-width;
                        xDir=0;
                        hitWall=true;
                    }
                }
                else{
                    x-=speedX;
                    if(x<leftP->getX()+leftP->getWidth()&&
                    y<=leftP->getY()+leftP->getHeight() && 
                    y>=leftP->getY()&&
                    y+height<=leftP->getY()+leftP->getHeight()&&
                    y+height>=leftP->getY()){
                        x=leftP->getX()+leftP->getWidth();
                        xDir=1;
                    }
                    else if(x<0){
                        x=0;
                        xDir=1;
                        hitWall=true;
                    }
                }

                if(yDir==1){
                    y+=speedY;
                    if(y>dhandle->getHeight()-height){
                        y=dhandle->getHeight()-height;
                        yDir=0;
                    }
                }
                else{
                    y-=speedY;
                    if(y<0){
                        y=0;
                        yDir=1;
                    }
                }
            }
            dhandle->fillRect(x,y,width,height,rand()%5+1);
        }
        int getX(){return x;}
        int getY(){return y;}
        int getWidth(){return width;}
        int getHeight(){return height;}
        bool didHitWall(){return hitWall;}
    private:
        int yDir,xDir;
        int x,y;
        int width=5,height=5;
        int speedY=1,speedX=3;
        drawingLib* dhandle;
        Paddle*leftP;
        Paddle*rightP;
        bool hitWall;
};
