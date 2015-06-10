#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WHITE 0xFFFFFF
#define BLACK 0x000000

typedef struct system
{
    Display *display;
    Window window;
    XEvent event;
    XWindowAttributes attributes;
    int screen;
    int fontsize;
    int choice;
    int font[64][64];
    int status;
    int pen;
}SYSTEM;

typedef struct div_button
{
    int x;
    int y;
    int width;
    int height;
    int visible;
    int chosen;
    char caption[4096];
    void (*div_draw)(SYSTEM *p_system,struct div_button *p_div);
}DIV_BUTTON;

typedef struct div_message
{
    int x;
    int y;
    int visible;
    char caption[4096];
    void (*div_draw)(SYSTEM *p_system,struct div_message *p_div);
}DIV_MESSAGE;

typedef struct div_shape
{
    int x;
    int y;
    int radius;
    int visible;
    void (*div_draw)(SYSTEM *p_system,struct div_shape *p_div);
}DIV_SHAPE;

typedef struct div_shape2
{
    int x;
    int y;
    int width;
    int height;
    int visible;
    void (*div_draw)(SYSTEM *p_system,struct div_shape2 *p_div);
}DIV_SHAPE2;

typedef struct ui
{
    DIV_BUTTON S;
    DIV_BUTTON M;
    DIV_BUTTON L;
    DIV_BUTTON pen;
    DIV_BUTTON save;
    DIV_BUTTON test;
    DIV_MESSAGE message;
    DIV_SHAPE shape;
    DIV_SHAPE2 mesh;
    DIV_SHAPE2 font;
    DIV_SHAPE2 font_disp;
}UI;


//主函数
void init_ui(UI *p_ui);
void init_system(SYSTEM *p_system);
void std_view(SYSTEM *p_system,UI *p_ui);
void std_button_press(SYSTEM *p_system,UI *p_ui);
void std_button_release(SYSTEM *p_system,UI *p_ui);
void std_motion_notify(SYSTEM *p_system,UI *p_ui);
//显示相关函数
void div_button(SYSTEM *p_system,DIV_BUTTON *p_div);
void div_button2(SYSTEM *p_system,DIV_BUTTON *p_div);
void div_message(SYSTEM *p_system,DIV_MESSAGE *p_div);
void div_shape(SYSTEM *p_system,DIV_SHAPE *p_div);
void div_mesh(SYSTEM *p_system,DIV_SHAPE2 *p_div);
void div_font(SYSTEM *p_system,DIV_SHAPE2 *p_div);
void div_font_display(SYSTEM *p_system,DIV_SHAPE2 *p_div);
//字体处理函数
void clear_font(SYSTEM *p_system);
void save_font(SYSTEM *p_system);
//测试相关函数
void test_module(SYSTEM *p_system);
void color_view(SYSTEM *p_system);

int main()
{
    UI ui;
    SYSTEM system;
    //init ui
    init_ui(&ui);
    //init system;
    init_system(&system);
    while(1)
    {
        XNextEvent(system.display,&system.event);
        if(system.event.type==Expose)
            std_view(&system,&ui);
        if(system.event.type==KeyPress)
            break;
        if(system.event.type==ButtonPress)
        {
            system.status=2;
            std_button_press(&system,&ui);
        }
        else if(system.event.type==ButtonRelease)
        {
            system.status=1;
            std_button_release(&system,&ui);
        }
        else if(system.event.type==MotionNotify)
        {
            if(system.status==2)
                std_motion_notify(&system,&ui);
        }
    }
    //close display
    XCloseDisplay(system.display);

    return 0;
}

void init_ui(UI *p_ui)
{
    p_ui->S.x=350;
    p_ui->S.y=100;
    p_ui->S.width=30;
    p_ui->S.height=20;
    p_ui->S.visible=1;
    p_ui->S.chosen=1;
    strcpy(p_ui->S.caption,"S");
    p_ui->S.div_draw=div_button2;

    p_ui->M.x=380;
    p_ui->M.y=100;
    p_ui->M.width=30;
    p_ui->M.height=20;
    p_ui->M.visible=1;
    p_ui->M.chosen=0;
    strcpy(p_ui->M.caption,"M");
    p_ui->M.div_draw=div_button2;

    p_ui->L.x=410;
    p_ui->L.y=100;
    p_ui->L.width=30;
    p_ui->L.height=20;
    p_ui->L.visible=1;
    p_ui->L.chosen=0;
    strcpy(p_ui->L.caption,"L");
    p_ui->L.div_draw=div_button2;

    p_ui->pen.x=410;
    p_ui->pen.y=130;
    p_ui->pen.width=30;
    p_ui->pen.height=20;
    p_ui->pen.visible=1;
    p_ui->pen.chosen=0;
    strcpy(p_ui->pen.caption,"");
    p_ui->pen.div_draw=div_button2;

    p_ui->save.x=410;
    p_ui->save.y=320;
    p_ui->save.width=30;
    p_ui->save.height=20;
    p_ui->save.visible=1;
    p_ui->save.chosen=0;
    strcpy(p_ui->save.caption,"save");
    p_ui->save.div_draw=div_button2;

    p_ui->test.x=410;
    p_ui->test.y=290;
    p_ui->test.width=30;
    p_ui->test.height=20;
    p_ui->test.visible=1;
    p_ui->test.chosen=0;
    strcpy(p_ui->test.caption,"test");
    p_ui->test.div_draw=div_button2;

    p_ui->message.x=350;
    p_ui->message.y=145;
    p_ui->message.visible=1;
    strcpy(p_ui->message.caption,"Pen");
    p_ui->message.div_draw=div_message;

    p_ui->shape.x=380;
    p_ui->shape.y=130;
    p_ui->shape.radius=20;
    p_ui->shape.visible=1;
    p_ui->shape.div_draw=div_shape;

    p_ui->mesh.x=20;
    p_ui->mesh.y=20;
    p_ui->mesh.width=320;
    p_ui->mesh.height=320;
    p_ui->mesh.visible=1;
    p_ui->mesh.div_draw=div_mesh;

    p_ui->font.x=p_ui->mesh.x;
    p_ui->font.y=p_ui->mesh.y;
    p_ui->font.width=p_ui->mesh.width;
    p_ui->font.height=p_ui->mesh.height;
    p_ui->font.visible=1;
    p_ui->font.div_draw=div_font;

    p_ui->font_disp.x=350;
    p_ui->font_disp.y=20;
    p_ui->font_disp.width=64+2;
    p_ui->font_disp.height=64+2;
    p_ui->font_disp.visible=1;
    p_ui->font_disp.div_draw=div_font_display;
}

void init_system(SYSTEM *p_system)
{
    int i,j;
    p_system->fontsize=16;
    p_system->choice=1;
    p_system->status=1;
    p_system->pen=1;
    for(i=0;i<64;i++)
    {
        for(j=0;j<64;j++)
        {
            p_system->font[i][j]=0;
        }
    }
    p_system->display=XOpenDisplay(NULL);
    if(p_system->display==NULL)
    {
        fprintf(stderr,"cannot open display");
        exit(1);
    }
    p_system->screen=DefaultScreen(p_system->display);
    p_system->window=XCreateSimpleWindow(p_system->display,RootWindow(p_system->display,p_system->screen),0,10,500,400,1,BlackPixel(p_system->display,p_system->screen),WhitePixel(p_system->display,p_system->screen));
    XSelectInput(p_system->display,p_system->window,ExposureMask|KeyPressMask|ButtonPress|ButtonReleaseMask|PointerMotionMask);
    XMapWindow(p_system->display,p_system->window);
}

void std_view(SYSTEM *p_system,UI *p_ui)
{
    int i,j,size=p_system->fontsize;
    char buffer[4096]="";
    //div mesh (Framework & Mesh)
    p_ui->mesh.div_draw(p_system,&p_ui->mesh);
    //div font (Font)
    p_ui->font.div_draw(p_system,&p_ui->font);
    //div font (Font display)
    p_ui->font_disp.div_draw(p_system,&p_ui->font_disp);
    //div button S,M,L
    p_ui->S.div_draw(p_system,&p_ui->S);
    p_ui->M.div_draw(p_system,&p_ui->M);
    p_ui->L.div_draw(p_system,&p_ui->L);
    //div button pen
    p_ui->pen.div_draw(p_system,&p_ui->pen);
    //div message pen
    p_ui->message.div_draw(p_system,&p_ui->message);
    //div shape pen
    p_ui->shape.div_draw(p_system,&p_ui->shape);
    //div button save
    p_ui->save.div_draw(p_system,&p_ui->save);
    //div button test
    p_ui->test.div_draw(p_system,&p_ui->test);
}

void std_button_press(SYSTEM *p_system,UI *p_ui)
{
    //div mesh
    if(p_system->event.xmotion.x>p_ui->mesh.x&&
            p_system->event.xmotion.x<p_ui->mesh.x+p_ui->mesh.width&&
            p_system->event.xmotion.y>p_ui->mesh.y&&
            p_system->event.xmotion.y<p_ui->mesh.y+p_ui->mesh.height)
    {
        if(p_system->pen==1)
            p_system->font[(p_system->event.xmotion.x-p_ui->mesh.x)/(p_ui->mesh.width/p_system->fontsize)][(p_system->event.xmotion.y-p_ui->mesh.y)/(p_ui->mesh.height/p_system->fontsize)]=1;
        else
            p_system->font[(p_system->event.xmotion.x-p_ui->mesh.x)/(p_ui->mesh.width/p_system->fontsize)][(p_system->event.xmotion.y-p_ui->mesh.y)/(p_ui->mesh.height/p_system->fontsize)]=0;
        std_view(p_system,p_ui);
    }
    //div button S
    else if(p_ui->S.visible==1 &&
            p_system->event.xmotion.x>p_ui->S.x &&
            p_system->event.xmotion.x<p_ui->S.x+p_ui->S.width &&
            p_system->event.xmotion.y>p_ui->S.y &&
            p_system->event.xmotion.y<p_ui->S.y+p_ui->S.width)
    {
        p_ui->S.chosen=1;
        p_ui->M.chosen=0;
        p_ui->L.chosen=0;
        p_system->choice=1;
        p_system->fontsize=16;
        clear_font(p_system);
        p_system->pen=1;
        std_view(p_system,p_ui);
    }
    //div button M
    else if(p_ui->M.visible==1 &&
            p_system->event.xmotion.x>p_ui->M.x &&
            p_system->event.xmotion.x<p_ui->M.x+p_ui->M.width &&
            p_system->event.xmotion.y>p_ui->M.y &&
            p_system->event.xmotion.y<p_ui->M.y+p_ui->M.width)
    {
        p_ui->S.chosen=0;
        p_ui->M.chosen=1;
        p_ui->L.chosen=0;
        p_system->choice=2;
        p_system->fontsize=32;
        clear_font(p_system);
        p_system->pen=1;
        std_view(p_system,p_ui);
    }
    //div button L
    else if(p_ui->L.visible==1 &&
            p_system->event.xmotion.x>p_ui->L.x &&
            p_system->event.xmotion.x<p_ui->L.x+p_ui->L.width &&
            p_system->event.xmotion.y>p_ui->L.y &&
            p_system->event.xmotion.y<p_ui->L.y+p_ui->L.width)
    {
        p_ui->S.chosen=0;
        p_ui->M.chosen=0;
        p_ui->L.chosen=1;
        p_system->choice=3;
        p_system->fontsize=64;
        clear_font(p_system);
        p_system->pen=1;
        std_view(p_system,p_ui);
    }
    //div button pen
    else if(p_ui->pen.visible==1 &&
            p_system->event.xmotion.x>p_ui->pen.x &&
            p_system->event.xmotion.x<p_ui->pen.x+p_ui->pen.width &&
            p_system->event.xmotion.y>p_ui->pen.y &&
            p_system->event.xmotion.y<p_ui->pen.y+p_ui->pen.width)
    {
        p_ui->pen.chosen=1;
        if(p_system->pen==1)
            p_system->pen=2;
        else 
            p_system->pen=1;
        std_view(p_system,p_ui);
    }
   //save
    else if(p_ui->save.visible==1 &&
            p_system->event.xmotion.x>p_ui->save.x &&
            p_system->event.xmotion.x<p_ui->save.x+p_ui->save.width &&
            p_system->event.xmotion.y>p_ui->save.y &&
            p_system->event.xmotion.y<p_ui->save.y+p_ui->save.width)
    {
        p_ui->save.chosen=1;
        std_view(p_system,p_ui);
        save_font(p_system);
    }
    //test_module
    else if(p_ui->test.visible==1 &&
            p_system->event.xmotion.x>p_ui->test.x &&
            p_system->event.xmotion.x<p_ui->test.x+p_ui->test.width &&
            p_system->event.xmotion.y>p_ui->test.y &&
            p_system->event.xmotion.y<p_ui->test.y+p_ui->test.width)
    {
        p_ui->save.chosen=1;
        std_view(p_system,p_ui);
        test_module(p_system);
    }
}

void std_button_release(SYSTEM *p_system,UI *p_ui)
{
    //div button pen
    if(p_ui->pen.visible==1 && p_ui->pen.chosen==1)
    {
        p_ui->pen.chosen=0;
        std_view(p_system,p_ui);
    }
    //div button save
    else if(p_ui->save.visible==1 && p_ui->save.chosen==1)
    {
        p_ui->save.chosen=0;
        std_view(p_system,p_ui);
    }
    //div button test
    else if(p_ui->test.visible==1 && p_ui->test.chosen==1)
    {
        p_ui->test.chosen=0;
        std_view(p_system,p_ui);
    }

}

void std_motion_notify(SYSTEM *p_system,UI *p_ui)
{
    //div mesh
    if(p_system->event.xmotion.x>p_ui->mesh.x&&
            p_system->event.xmotion.x<p_ui->mesh.x+p_ui->mesh.width&&
            p_system->event.xmotion.y>p_ui->mesh.y&&
            p_system->event.xmotion.y<p_ui->mesh.y+p_ui->mesh.height)
    {
        if(p_system->pen==1)
            p_system->font[(p_system->event.xmotion.x-p_ui->mesh.x)/(p_ui->mesh.width/p_system->fontsize)][(p_system->event.xmotion.y-p_ui->mesh.y)/(p_ui->mesh.height/p_system->fontsize)]=1;
        else
            p_system->font[(p_system->event.xmotion.x-p_ui->mesh.x)/(p_ui->mesh.width/p_system->fontsize)][(p_system->event.xmotion.y-p_ui->mesh.y)/(p_ui->mesh.height/p_system->fontsize)]=0;
        std_view(p_system,p_ui);
    }

}

//div button 控件绘图函数
void div_button(SYSTEM *p_system,DIV_BUTTON *p_div)
{
    if(p_div->visible!=0)
    {
        XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),0xFFFF00);
        XFillRectangle(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x,p_div->y,p_div->width,p_div->height);
        XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),0x000000);
        XDrawString(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x+5,p_div->y+p_div->height-5,p_div->caption,strlen(p_div->caption));
    }
}

//div button 控件绘图函数 button S,M,L
void div_button2(SYSTEM *p_system,DIV_BUTTON *p_div)
{
    if(p_div->visible!=0)
    {
        XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),BLACK);
        XDrawRectangle(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x,p_div->y,p_div->width,p_div->height);
        if(p_div->chosen!=0)
        {
            XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),0xEEEEEE);
        }
        else
        {
            XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),WHITE);
        }
        XFillRectangle(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x+1,p_div->y+1,p_div->width-1,p_div->height-1);
        //caption
        XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),BLACK);
        XDrawString(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x+5,p_div->y+p_div->height-5,p_div->caption,strlen(p_div->caption));
    }
}

//div message 控件绘图函数
void div_message(SYSTEM *p_system,DIV_MESSAGE *p_div)
{
    if(p_div->visible!=0)
    {
        XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),0x0);
        XDrawString(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x,p_div->y,p_div->caption,strlen(p_div->caption));
    }
}

//div shape 控件绘图函数 (自定义类型) --画笔控制
void div_shape(SYSTEM *p_system,DIV_SHAPE *p_div)
{
    if(p_div->visible!=0)
    {
        if(p_system->pen==1)
        {
            XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),BLACK);
            XFillArc(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x,p_div->y,p_div->radius,p_div->radius,0,360*64);
        }
        else
        {
            XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),WHITE);
            XFillArc(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x,p_div->y,p_div->radius,p_div->radius,0,360*64);
            XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),BLACK);
            XDrawArc(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x,p_div->y,p_div->radius,p_div->radius,0,360*64);
        }
    }
}

//div shape2 控件绘图函数 --网格
void div_mesh(SYSTEM *p_system,DIV_SHAPE2 *p_div)
{
    int i,j,size=p_system->fontsize;
    if(p_div->visible!=0)
    {
        XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),WHITE);
        //Framework
        XFillRectangle(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x,p_div->y,p_div->width,p_div->height);
        XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),BLACK);
        XDrawRectangle(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x,p_div->y,p_div->width,p_div->height);
        //Mesh
        XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),0xEEEEEE);
        for(i=1;i<size;i++)
        {
            XDrawLine(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x+1,p_div->y+i*p_div->height/size,p_div->x+p_div->width-1,p_div->y+i*p_div->height/size);
            XDrawLine(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x+i*p_div->width/size,p_div->y+1,p_div->x+i*p_div->width/size,p_div->y+p_div->height-1);
        }
    }
}

//div shape2 控件绘图函数 --字体
void div_font(SYSTEM *p_system,DIV_SHAPE2 *p_div)
{
    int i,j,size=p_system->fontsize;
    if(p_div->visible!=0)
    {
        XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),0x333333);
        for(i=0;i<size;i++)
        {
            for(j=0;j<size;j++)
            {
                if(p_system->font[i][j]==1)
                {
                    XFillRectangle(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x+1+i*p_div->width/size,p_div->y+1+j*p_div->height/size,p_div->width/size-1,p_div->height/size-1);
                }
            }
        }
    }
}

//div shape2 控件绘图函数 --字体实时显示
void div_font_display(SYSTEM *p_system,DIV_SHAPE2 *p_div)
{
    int i,j,size=p_system->fontsize;
    if(p_div->visible!=0)
    {
        XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),WHITE);
        XFillRectangle(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x,p_div->y,p_div->width,p_div->height);
        XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),BLACK);
        XDrawRectangle(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),p_div->x,p_div->y,size+1,size+1);
        for(i=0;i<size;i++)
        {
            for(j=0;j<size;j++)
            {
                if(p_system->font[i][j]==1)
                    XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),0x333333);
                else
                    XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),WHITE);
                XDrawPoint(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),i+1+p_div->x,j+1+p_div->y);
            }
        }
    }
}

void clear_font(SYSTEM *p_system)
{
    int i,j;
    for(i=0;i<64;i++)
    {
        for(j=0;j<64;j++) {
            p_system->font[i][j]=0;
        }
    }
}

void save_font(SYSTEM *p_system)
{
    int i,j;
    FILE *p_file;
    p_file=fopen("font/02","wb");
    if(p_file!=NULL)
    {
        for(j=0;j<p_system->fontsize;j++)
        {
            for(i=0;i<p_system->fontsize;i++)
            {
                fwrite(&(p_system->font[i][j]),sizeof(int),1,p_file);
            }
        }
        fclose(p_file);
    }
}

void test_module(SYSTEM *p_system)
{
    SYSTEM system2;
    init_system(&system2);
    while(1)
    {
        XNextEvent(system2.display,&system2.event);
        if(system2.event.type==Expose)
            color_view(&system2);
        if(system2.event.type==KeyPress)
            break;
    }
    XCloseDisplay(system2.display);
}

void color_view(SYSTEM *p_system)
{
    int i,j,k,size=p_system->fontsize;
    char buffer[4096]="";
    XGetWindowAttributes(p_system->display,p_system->window,&(p_system->attributes));
    XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),0xFFFFFF);
    k=0;
    //color_table1
    for(i=0;i<8;i++)
    {
        for(j=0;j<256;j++)
        {
            XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),j<<16);
            XDrawPoint(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),i+280,j+20);
            XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),j<<8);
            XDrawPoint(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),i+290,j+20);
            XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),j<<0);
            XDrawPoint(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),i+300,j+20);
        }
}
    //color_table2
    for(i=0;i<256;i++)
    {
        for(j=0;j<256;j++)
        {
            XSetForeground(p_system->display,DefaultGC(p_system->display,p_system->screen),i+(j<<8)+(k<<16));
            XDrawPoint(p_system->display,p_system->window,DefaultGC(p_system->display,p_system->screen),i+20,j+20);
        }
    }
}
