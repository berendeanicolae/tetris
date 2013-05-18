#include "tetromino.h"

///Must verify what this function is
void rotate(char *fill, int w, int h);
void set_rect(SDL_Rect *rect, Sint16 x, Sint16 y, Uint16 w, Uint16 h);

tetromino_node::tetromino_node(int w_, int h_, char *fill, int color, int boxh): w(w_), h(h_), next(NULL){
    char *s=fill;
    SDL_Surface *box_3d=SDL_CreateRGBSurface(SDL_SWSURFACE,boxh,boxh,32,255<<16,255<<8,255,0);
    SDL_Rect rect_to_fill;
    int new_color;

    shape=new bool*[h]();
    for (int i=0;i<h;++i){
        shape[i]=new bool[w];
        for (int j=0;j<w;++j)
            shape[i][j]=*s++=='X';
    }
    SDL_FillRect(box_3d,NULL,color);
    //whiter
    new_color=0;
    for (int i=8;i<=24;i+=8)
        new_color|=(1<<i)-1;
        /*if ((color&((1<<i)-1))+50>(1<<i)-1)
            new_color|=(1<<i)-1;
        else
            ;//new_color|=(color&((1<<i)-1))+50;*/
    set_rect(&rect_to_fill,1,1,boxh-2,1);
    SDL_FillRect(box_3d,&rect_to_fill,new_color);
    set_rect(&rect_to_fill,1,1,1,boxh-2);
    SDL_FillRect(box_3d,&rect_to_fill,new_color);
    //darker
    new_color=0;
    set_rect(&rect_to_fill,1,boxh-2,boxh-2,1);
    SDL_FillRect(box_3d,&rect_to_fill,new_color);
    set_rect(&rect_to_fill,boxh-2,1,1,boxh-2);
    SDL_FillRect(box_3d,&rect_to_fill,new_color);
    ///
    look=SDL_CreateRGBSurface(SDL_HWSURFACE,w*(boxh+1),h*(boxh+1),32,255<<16,255<<8,255,0);
    SDL_SetColorKey(look,SDL_SRCCOLORKEY,0);
    for (int i=0;i<h;++i)
        for (int j=0;j<w;++j){
            set_rect(&rect_to_fill,(Sint16)(j*(boxh+1)),(Sint16)(i*(boxh+1)),(Uint16)boxh,(Uint16)boxh);
            if (shape[i][j])
                SDL_BlitSurface(box_3d,NULL,look,&rect_to_fill);
        }
    SDL_FreeSurface(box_3d);
}

tetromino_node::~tetromino_node(){
    if (shape){
        for (int i=0;i<h;++i)
            delete[] shape[i];
        delete[] shape;
    }
    if (look)
        SDL_FreeSurface(look);
}

tetromino_list::tetromino_list(int w, int h, char *fill, int color, int turns, int boxh){
    tetromino_node *last,*temp;

    last=first=new tetromino_node(w,h,fill,color,boxh);
    switch (turns){
        case 4:
            rotate(fill,w,h);
            temp=new tetromino_node(h,w,fill,color,boxh);
            last->next=temp;
            last=temp;
            rotate(fill,h,w);
            temp=new tetromino_node(w,h,fill,color,boxh);
            last->next=temp;
            last=temp;
        case 2:
            rotate(fill,w,h);
            temp=new tetromino_node(h,w,fill,color,boxh);
            last->next=temp;
            last=temp;
        default:
            break;
    }
    last->next=first;
}


tetromino_list::~tetromino_list(){
    if (first){
        tetromino_node *temp1=first->next;

        first->next=NULL;
        while (temp1){
            tetromino_node *temp2=temp1;

            temp1=temp1->next;
            delete temp2;
        }
    }
}

void tetromino_list::rotate(char *fill, int w, int h){
    char **v=new char*[h],*s=fill;

    for (int i=0;i<h;++i){
        v[i]=new char[w];
        for (int j=0;j<w;++j)
            v[i][j]=*s++;
    }
    s=fill;
    for (int j=0;j<w;++j)
        for (int i=h-1;i>=0;--i)
            *s++=v[i][j];
    for (int i=0;i<h;++i)
        delete[] v[i];
    delete[] v;
}

bool Tetromino::can_enter(bool **grid){
    for (int i=oy-node->h+1>0 ? 0 : node->h-oy;i<node->h;++i)
        for (int j=0;j<node->w && ox+j<=width+1;++j)
            if (node->shape[i][j] && grid[oy-node->h+i+1][ox+j])
                return 0;
    return 1;
}

bool Tetromino::move_left(bool **grid){
    for (int i=oy-node->h+1>0 ? 0 : node->h-oy;i<node->h;++i)
        for (int j=0;j<node->w && ox+j<=width+1;++j)
            if (node->shape[i][j] && grid[oy+1-node->h+i][ox+j-1])
                return 0;
    return 1;
}

bool Tetromino::move_down(bool **grid){
    for (int i=oy-node->h+1>0 ? 0 : node->h-oy;i<node->h;++i)
        for (int j=0;j<node->w && ox+j<=width+1;++j)
            if (node->shape[i][j] && grid[oy+2-node->h+i][ox+j])
                return 0;
    return 1;
}

bool Tetromino::move_right(bool **grid){
    for (int i=oy-node->h+1>0 ? 0 : node->h-oy;i<node->h;++i)
        for (int j=0;j<node->w && ox+j<=width+1;++j)
            if (node->shape[i][j] && grid[oy+1-node->h+i][ox+j+1])
                return 0;
    return 1;
}

bool Tetromino::rotate(bool **grid){
    tetromino_node *next_node=node;

    next_node=next_node->next;
    for (int i=oy-next_node->h+1>0 ? 0 : next_node->h-oy;i<next_node->h;++i)
        for (int j=0;j<next_node->w && ox+j<=width+1;++j)
            if (next_node->shape[i][j] && grid[oy+1-next_node->h+i][ox+j])
                return 0;
    return 1;
}

void set_rect(SDL_Rect *rect, Sint16 x, Sint16 y, Uint16 w, Uint16 h){
    rect->x=x;
    rect->y=y;
    rect->w=w;
    rect->h=h;
}
