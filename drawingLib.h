#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <iostream>
#include <thread>
#include <cstdlib>
#include <chrono>
#include <assert.h>
class drawingLib{
    public:
        drawingLib(int width,int height){
            drawingLib::width=width;
            drawingLib::height=height;
            char black_bits[]="#000000";
            char white_bits[]="#FFFFFF";
            char red_bits[]="#FF0000";
            char green_bits[]="#00FF00";
            char blue_bits[]="#0000FF";
            char yellow_bits[]="#FFFF00";
            
            dpy=XOpenDisplay(NULL);
            assert(dpy);

            colormap=DefaultColormap(dpy,0);
            XParseColor(dpy,colormap, black_bits, &colours[0]);
            XAllocColor(dpy,colormap,&colours[0]);

            XParseColor(dpy,colormap,white_bits,&colours[1]);
            XAllocColor(dpy,colormap,&colours[1]);

            XParseColor(dpy,colormap,red_bits,&colours[2]);
            XAllocColor(dpy,colormap,&colours[2]);

            XParseColor(dpy,colormap,green_bits,&colours[3]);
            XAllocColor(dpy,colormap,&colours[3]);

            XParseColor(dpy,colormap,blue_bits,&colours[4]);
            XAllocColor(dpy,colormap,&colours[4]);

            XParseColor(dpy,colormap,yellow_bits,&colours[5]);
            XAllocColor(dpy,colormap,&colours[5]);

            int blackColor=BlackPixel(dpy,DefaultScreen(dpy));
            w=XCreateSimpleWindow(dpy,DefaultRootWindow(dpy),0,0,width,height,0,blackColor,blackColor);

            XSelectInput(dpy,w,StructureNotifyMask|ButtonPressMask|KeyPressMask);
            XMapWindow(dpy,w);
            for(;;){
                XEvent e;
                XNextEvent(dpy,&e);
                if(e.type==MapNotify)break;
            }
            gc=XCreateGC(dpy,w,0,NULL);
            //std::thread keyPressThread(F);
        }
        void close(){
            XCloseDisplay(dpy);
        }
        void drawLine(int x,int y,int endX,int endY,int colour){
            //0=black, 1=white, 2=red, 3=green, 4=blue, 5=yellow
            XSetForeground(dpy,gc,colours[colour].pixel);
            XDrawLine(dpy,w,gc,x,y,endX,endY);
            XFlush(dpy);
        }
        void fillRect(int x, int y, int width, int height, int colour){
            XSetForeground(dpy,gc,colours[colour].pixel);
            XFillRectangle(dpy,w,gc,x,y,width,height);
            XFlush(dpy);
        }
        void sleepM(int n){
            std::this_thread::sleep_for(std::chrono::milliseconds(n));
        }
        int getWidth(){return width;}
        int getHeight(){return height;}
    private:
        Display* dpy;
        GC gc;
        //XColor black_col,white_col,red_col,green_col,blue_col,yellow_col;
        XColor colours[6];
        Colormap colormap;
        Window w;
        int width,height;
        //std::thread keyPressThread;
};
