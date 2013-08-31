#include "functions.h"

struct color{
    Sint16 r,g,b;
};

void resize_to(SDL_Surface *src, SDL_Surface *dst){
    int srci,srcj,ldsti,ldstj,dsti,dstj,i,j;
    Uint8 *p1,*p2;
    color first_j,var_j,first_i,var_i1,last_i,var_i2;

    for (srci=0,ldsti=-1;srci<src->h;++srci){
        dsti=srci*(dst->h-1)/(src->h-1);
        switch (dsti-ldsti){
            case 1:
                for (srcj=0,ldstj=-1;srcj<src->w;++srcj){
                    dstj=srcj*(dst->w-1)/(src->w-1);
                    switch (dstj-ldstj){
                        case 1:
                            p1=(Uint8 *)src->pixels+src->pitch*srci+src->format->BytesPerPixel*srcj;
                            p2=(Uint8 *)dst->pixels+dst->pitch*dsti+dst->format->BytesPerPixel*dstj;
                            *p2=*p1;
                            *(p2+1)=*(p1+1);
                            *(p2+2)=*(p1+2);
                        case 0:
                            break;
                        default:
                            p1=(Uint8 *)dst->pixels+dst->pitch*dsti+dst->format->BytesPerPixel*ldstj;
                            first_j.r=*p1;
                            first_j.g=*(p1+1);
                            first_j.b=*(p1+2);
                            p1=(Uint8 *)src->pixels+src->pitch*srci+src->format->BytesPerPixel*srcj;
                            var_j.r=*p1-first_j.r;
                            var_j.g=*(p1+1)-first_j.g;
                            var_j.b=*(p1+2)-first_j.b;
                            for (j=ldstj+1;j<=dstj;++j){
                                p1=(Uint8 *)dst->pixels+dst->pitch*dsti+dst->format->BytesPerPixel*j;
                                *p1=(Uint8)first_j.r+var_j.r*(j-ldstj)/(dstj-ldstj);
                                *(p1+1)=(Uint8)first_j.g+var_j.g*(j-ldstj)/(dstj-ldstj);
                                *(p1+2)=(Uint8)first_j.b+var_j.b*(j-ldstj)/(dstj-ldstj);
                            }
                            break;
                    }
                    ldstj=dstj;
                }
            case 0:
                break;
            default:
                for (srcj=0,ldstj=-1;srcj<src->w;++srcj){
                    dstj=srcj*(dst->w-1)/(src->w-1);
                    p1=(Uint8 *)dst->pixels+dst->pitch*ldsti+dst->format->BytesPerPixel*ldstj;
                    first_i.r=*p1;
                    first_i.g=*(p1+1);
                    first_i.b=*(p1+2);
                    p1=(Uint8 *)src->pixels+src->pitch*srci+src->format->BytesPerPixel*srcj;
                    var_i1.r=*p1-first_i.r;
                    var_i1.g=*(p1+1)-first_i.g;
                    var_i1.b=*(p1+2)-first_i.b;
                    switch (dstj-ldstj){
                        case 1:
                            for (i=ldsti+1;i<=dsti;++i){
                                p1=(Uint8 *)dst->pixels+dst->pitch*i+dst->format->BytesPerPixel*dstj;
                                *p1=(Uint8)first_i.r+var_i1.r*(i-ldsti)/(dsti-ldsti);
                                *(p1+1)=(Uint8)first_i.g+var_i1.g*(i-ldsti)/(dsti-ldsti);
                                *(p1+2)=(Uint8)first_i.b+var_i1.b*(i-ldsti)/(dsti-ldsti);
                            }
                        case 0:
                            break;
                        default:
                            p1=(Uint8 *)dst->pixels+dst->pitch*ldsti+dst->format->BytesPerPixel*dstj;
                            last_i.r=*p1;
                            last_i.g=*(p1+1);
                            last_i.b=*(p1+2);
                            p1=(Uint8 *)src->pixels+src->pitch*srci+src->format->BytesPerPixel*srcj;
                            var_i2.r=*p1-last_i.r;
                            var_i2.g=*(p1+1)-last_i.g;
                            var_i2.b=*(p1+2)-last_i.b;
                            for (i=ldsti+1;i<=dsti;++i){
                                first_j.r=first_i.r+var_i1.r*(i-ldsti)/(dsti-ldsti);
                                first_j.g=first_i.g+var_i1.g*(i-ldsti)/(dsti-ldsti);
                                first_j.b=first_i.b+var_i1.b*(i-ldsti)/(dsti-ldsti);
                                var_j.r=last_i.r+var_i2.r*(i-ldsti)/(dsti-ldsti)-first_j.r;
                                var_j.g=last_i.g+var_i2.g*(i-ldsti)/(dsti-ldsti)-first_j.g;
                                var_j.b=last_i.b+var_i2.b*(i-ldsti)/(dsti-ldsti)-first_j.b;
                                for (j=ldstj+1;j<=dstj;++j){
                                    p1=(Uint8 *)dst->pixels+dst->pitch*i+dst->format->BytesPerPixel*j;
                                    *p1=(Uint8)first_j.r+var_j.r*(j-ldstj)/(dstj-ldstj);
                                    *(p1+1)=(Uint8)first_j.g+var_j.g*(j-ldstj)/(dstj-ldstj);
                                    *(p1+2)=(Uint8)first_j.b+var_j.b*(j-ldstj)/(dstj-ldstj);
                                }
                            }
                            break;
                    }
                    ldstj=dstj;
                }
                break;
        }
        ldsti=dsti;
    }
}

void read_color(FILE *tin, SDL_Color *color){
    int r,g,b;

    fscanf(tin,"%d %d %d",&r,&g,&b);
    color->r=(Uint8) r;
    color->g=(Uint8) g;
    color->b=(Uint8) b;
}

void set_rect(SDL_Rect *rect, Sint16 x, Sint16 y, Uint16 w, Uint16 h){
    rect->x=x;
    rect->y=y;
    rect->w=w;
    rect->h=h;
}
