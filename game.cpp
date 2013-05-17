#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "game.h"
#include "functions.h"


Game::Game(){
	SDL_Surface *img;

    puts("Initializing SDL...");
    if (SDL_Init(SDL_INIT_EVERYTHING)){
        puts("Error!");
        exit(1);
    }
    puts("Complete!");
    puts("Initializing SDL_Mixer...");
    if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,4096)){
        puts("Error!");
        exit(1);
    }
    Mix_AllocateChannels(20);
    puts("Complete!");
    puts("Initializing SDL_ttf...");
    if (TTF_Init()==-1){
        puts("Error!");
        exit(1);
    }
    puts("Complete!");
    SDL_WM_SetCaption("Tetris",NULL);
    SDL_ShowCursor(SDL_DISABLE);
    vinfo=SDL_GetVideoInfo();
    puts("Setting up video mode...");
    screen=SDL_SetVideoMode(800,480,32,SDL_HWSURFACE);//vinfo->current_w,vinfo->current_h,32,SDL_SWSURFACE | SDL_FULLSCREEN);//
    if (screen==NULL){
        puts("Error!");
        exit(1);
    }
    puts("Complete!");

    img=IMG_Load("resources/menu_background.png");
    background=SDL_ConvertSurface(img,screen->format,SDL_SWSURFACE);
	SDL_FreeSurface(img);
	img=background;
    background=SDL_CreateRGBSurface(SDL_HWSURFACE,screen->w,screen->h,32,255<<16,255<<8,255,0);
    resize_to(img,background);
    SDL_FreeSurface(img);
    font=TTF_OpenFont("resources/font.ttf",screen->h>>4);
    boxh=screen->h*4/5/height;
    board_x=(screen->w-(boxh+1)*width+1)>>1;
    board_y=(screen->h-(boxh+1)*height+1)>>1;
    quit=0;
    state=MENU;
}

Game::~Game(){
    puts("Closing font");
    TTF_CloseFont(font);
    puts("Complete");
	puts("Deleting data");
	SDL_FreeSurface(background);
    puts("Complete!");
    puts("Closing SDL_ttf");
    TTF_Quit();
    puts("Complete!");
    puts("Closing SDL_mixer");
    Mix_CloseAudio();
    puts("Complete!");
    puts("Closing SDL");
    SDL_Quit();
    puts("Complete!");
}

void Game::run(){
    while (!quit){
        switch (state){
            case MENU:
                menu();
                break;
            case PLAY:
                play();
                break;
        }
    }
}

void Game::draw_menu(){
    SDL_Surface *aux=IMG_Load("resources/logo.bmp"),*img=SDL_ConvertSurface(aux,screen->format,SDL_SWSURFACE);
    SDL_Rect dst;

    SDL_FreeSurface(aux);
    aux=SDL_CreateRGBSurface(SDL_SWSURFACE,img->w*(screen->h-10)/2/img->h,(screen->h-10)/2,32,screen->format->Rmask,screen->format->Gmask,screen->format->Bmask,screen->format->Amask);
    resize_to(img,aux);
    SDL_FreeSurface(img);
    SDL_SetColorKey(aux,SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,255,255,255));
	set_rect(&dst,(screen->w-aux->w)/2,5,0,0);
    SDL_BlitSurface(background,NULL,screen,NULL);
    SDL_BlitSurface(aux,NULL,screen,&dst);
	SDL_FreeSurface(aux);
    SDL_Flip(screen);
}

void Game::draw_play(){
    SDL_Rect rect;

    SDL_BlitSurface(background,NULL,screen,NULL);
    set_rect(&rect,board_x-1,board_y-1,(boxh+1)*width+1,(boxh+1)*height+1);
    SDL_FillRect(screen,&rect,SDL_MapRGB(screen->format,22,22,22));
    for (int i=0;i<height;++i)
        for (int j=0;j<width;++j){
            set_rect(&rect,board_x+(boxh+1)*j,board_y+(boxh+1)*i,boxh,boxh);
            SDL_FillRect(screen,&rect,SDL_MapRGB(screen->format,0,0,0));
        }
    SDL_Flip(screen);
}

void Game::menu(){
    //Variables
    FILE *tin=fopen("resources/menu.txt","r");
    SDL_Surface **on_v,**off_v;
    SDL_Color on,off;
    bool quit_menu=0;
    int l=0,p=0;

    //Initialize
    read_color(tin,&off);
    read_color(tin,&on);
    fscanf(tin,"%d\r\n",&l);
    on_v=new SDL_Surface*[l];
    off_v=new SDL_Surface*[l];
    for (int i=0;i<l;++i){
        char s[21];

        fscanf(tin,"%s",s);
        off_v[i]=TTF_RenderText_Blended(font,s,off);
        on_v[i]=TTF_RenderText_Blended(font,s,on);
    }
    fclose(tin);
    draw_menu();
    replace_with(on_v[0],(screen->w-on_v[0]->w)>>1,(screen->h>>1)+0*TTF_FontHeight(font));
    for (int i=1;i<l;++i)
        replace_with(off_v[i],(screen->w-off_v[i]->w)>>1,(screen->h>>1)+i*TTF_FontHeight(font));

    //Run
    while (!quit_menu){
        int start=SDL_GetTicks(),end;
        SDL_Event event;

        while (SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_UP:
                            replace_with(off_v[p],(screen->w-off_v[p]->w)>>1,(screen->h>>1)+p*TTF_FontHeight(font));
                            ++p;
                            p%=l;
                            replace_with(on_v[p],(screen->w-on_v[p]->w)>>1,(screen->h>>1)+p*TTF_FontHeight(font));
                            break;
                        case SDLK_DOWN:
                            replace_with(off_v[p],(screen->w-off_v[p]->w)>>1,(screen->h>>1)+p*TTF_FontHeight(font));
                            --p;
                            p=(p+l)%l;
                            replace_with(on_v[p],(screen->w-on_v[p]->w)>>1,(screen->h>>1)+p*TTF_FontHeight(font));
                            break;
                        case SDLK_RETURN:
                            switch (p){
                                case 0:
                                    state=PLAY;
                                    quit_menu=1;
                                    break;
                                case 1:
                                    quit_menu=1;
                                    quit=1;
                                    break;
                            }
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }
        end=SDL_GetTicks();
        if (end-start<frame)
            SDL_Delay(frame-end+start);
    }

    //Clean up
    for (int i=0;i<l;++i){
        SDL_FreeSurface(on_v[i]);
        SDL_FreeSurface(off_v[i]);
    }
    delete[] on_v;
    delete[] off_v;
}

void Game::play(){
    //Variables
    bool quit_play=0,**grid=NULL;
    FILE *tin=fopen("resources/tetrominos.txt","r");
    tetromino_list **tetrominos=NULL;
    Tetromino *fig[3]={NULL,NULL,NULL};
    char *fill=new char[50];
    int l;
    key keyboard[4]={{0,0},{0,0},{0,0},{0,0}}; //up,right,down,left

    //Initialize
    level=1;
    to_next_lv=4;
    points=0;
    grid=new bool*[height+2];
    for (int i=0;i<=height+1;++i){
        grid[i]=new bool[width+2]();
        grid[i][0]=grid[i][width+1]=1;
    }
    memset(grid[height+1]+1,1,width);
    fscanf(tin,"%d",&l);
    fgets(fill,50,tin);
    fgets(fill,50,tin);
    tetrominos=new tetromino_list*[l]();
    for (int i=0;i<l;++i){
        int w,h,r,g,b,t;

        fscanf(tin,"%*d %*s %d %d %s %d %d %d %d",&w,&h,fill,&r,&g,&b,&t);
        tetrominos[i]=new tetromino_list(w,h,fill,SDL_MapRGB(screen->format,r,g,b),t,boxh);
    }
    fclose(tin);
    srand(time(NULL));
    fig[0]=new Tetromino(tetrominos[rand()%l]->first,level);
    fig[1]=new Tetromino(tetrominos[rand()%l]->first,level);
    draw_play();
    pop=Mix_LoadWAV("resources/pop.ogg");

    //Play
    fig[0]->update=1;
    while (!quit_play){
        int start=SDL_GetTicks(),end;
        SDL_Event event;

        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_UP:
                            keyboard[1].pressed=keyboard[2].pressed=keyboard[3].pressed=0;
                            keyboard[1].downtime=keyboard[2].downtime=keyboard[3].downtime=0;
                            keyboard[0].pressed=1;
                            keyboard[0].downtime=0;
                            break;
                        case SDLK_RIGHT:
                            keyboard[0].pressed=keyboard[2].pressed=keyboard[3].pressed=0;
                            keyboard[0].downtime=keyboard[2].downtime=keyboard[3].downtime=0;
                            keyboard[1].pressed=1;
                            keyboard[1].downtime=0;
                            break;
                        case SDLK_DOWN:
                            keyboard[0].pressed=keyboard[1].pressed=keyboard[3].pressed=0;
                            keyboard[0].downtime=keyboard[1].downtime=keyboard[3].downtime=0;
                            keyboard[2].pressed=1;
                            keyboard[2].downtime=0;
                            break;
                        case SDLK_LEFT:
                            keyboard[0].pressed=keyboard[1].pressed=keyboard[2].pressed=0;
                            keyboard[0].downtime=keyboard[1].downtime=keyboard[2].downtime=0;
                            keyboard[3].pressed=1;
                            keyboard[3].downtime=0;
                            break;
                        case SDLK_RETURN:
                            quit_play=1;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym){
                        case SDLK_UP:
                            keyboard[0].pressed=0;
                            keyboard[0].downtime=0;
                            break;
                        case SDLK_RIGHT:
                            keyboard[1].pressed=0;
                            keyboard[1].downtime=0;
                            break;
                        case SDLK_DOWN:
                            keyboard[2].pressed=0;
                            keyboard[2].downtime=0;
                            break;
                        case SDLK_LEFT:
                            keyboard[3].pressed=0;
                            keyboard[3].downtime=0;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        Tetromino_move(fig,grid,keyboard,tetrominos,l);
        if (fig[0]->update){
            if (fig[0]->rotated){
                fig[0]->ox=fig[0]->x;
                fig[0]->oy=fig[0]->y;
                fig[0]->node=fig[0]->node->next;
                if (fig[0]->y-fig[0]->node->h>=0){
                    SDL_Rect f_loc;  //final location

                    set_rect(&f_loc,(board_x+(fig[0]->x-1)*(boxh+1)),(board_y+(fig[0]->y-fig[0]->node->h)*(boxh+1)),0,0);
                    SDL_BlitSurface(fig[0]->node->look,NULL,screen,&f_loc);
                }
                else{
                    SDL_Rect copy_from,f_loc;

                    set_rect(&copy_from,0,(fig[0]->node->h-fig[0]->y)*(boxh+1),(fig[0]->node->w)*(boxh+1),fig[0]->y*(boxh+1));
                    set_rect(&f_loc,board_x+(fig[0]->x-1)*(boxh+1),board_y,0,0);
                    SDL_BlitSurface(fig[0]->node->look,&copy_from,screen,&f_loc);
                }
                fig[0]->update=0;
                fig[0]->rotated=0;
                SDL_Flip(screen);
            }
            else{
                if (fig[0]->y-fig[0]->node->h>=0){
                    SDL_Rect f_loc;

                    set_rect(&f_loc,board_x+(fig[0]->x-1)*(boxh+1),board_y+(fig[0]->y-fig[0]->node->h)*(boxh+1),0,0);
                    SDL_BlitSurface(fig[0]->node->look,NULL,screen,&f_loc);

                }
                else{
                    SDL_Rect copy_from,f_loc;

                    set_rect(&copy_from,0,(fig[0]->node->h-fig[0]->y)*(boxh+1),(fig[0]->node->w)*(boxh+1),fig[0]->y*(boxh+1));
                    set_rect(&f_loc,board_x+(fig[0]->x-1)*(boxh+1),board_y,0,0);
                    SDL_BlitSurface(fig[0]->node->look,&copy_from,screen,&f_loc);
                }
                fig[0]->ox=fig[0]->x;
                fig[0]->oy=fig[0]->y;
                fig[0]->update=0;
                SDL_Flip(screen);
            }
        }
        end=SDL_GetTicks();
        if (end-start<frame)
            SDL_Delay(frame-end+start);
    }

    //Clean up
    Mix_FreeChunk(pop);
    for (int i=0;i<3;++i)
        if (fig[i])
            delete fig[i];
    for (int i=0;i<l;++i)
        delete tetrominos[i];
    delete[] tetrominos;
    for (int i=0;i<=height+1;++i)
        delete[] grid[i];
    delete[] grid;
    delete[] fill;
    state=MENU;
}

void Game::update_score(){
    SDL_Color color={255,255,255};
    char score_str[10]="";
    SDL_Surface *score_to_blit;

    itoa(points,score_str,10);
    score_to_blit=TTF_RenderText_Blended(font,score_str,color);
    replace_with(score_to_blit,0,0);
    SDL_FreeSurface(score_to_blit);
}

void Game::Tetromino_move(Tetromino *fig[3], bool **grid, key keyboard[4], tetromino_list **tetrominos, int l){
    //Moving downwards
    if (keyboard[2].pressed){
        if (keyboard[2].downtime<=0){
            if (fig[0]->move_down(grid)){
                if (!fig[0]->update)
                    Tetromino_delete(fig[0]);
                ++fig[0]->y;
                fig[0]->time=1000/level;
                fig[0]->update=1;
                keyboard[2].downtime=60;
            }
            else
                Tetromino_down_end(fig,grid,tetrominos,l);
        }
        else
            keyboard[2].downtime-=frame;
    }
    else
        if (fig[0]->time<=0){
            if (fig[0]->move_down(grid)){
                if (!fig[0]->update)
                    Tetromino_delete(fig[0]);
                ++fig[0]->y;
                fig[0]->time=1000/level;
                fig[0]->update=1;
            }
            else
                Tetromino_down_end(fig,grid,tetrominos,l);
        }
        else
            fig[0]->time-=frame;
    //Rotate
    if (keyboard[0].pressed && !fig[0]->update){
        if (keyboard[0].downtime<=0){
            if (fig[0]->rotate(grid)){
                Tetromino_delete(fig[0]);
                fig[0]->update=1;
                fig[0]->rotated=1;
                keyboard[0].downtime=200;
                Mix_PlayChannel(-1,pop,0);
            }
        }
        else
            keyboard[0].downtime-=frame;
    }
    //Moving right
    if (keyboard[1].pressed && !fig[0]->update){
        if (keyboard[1].downtime<=0){
            if (fig[0]->move_right(grid)){
                if (!fig[0]->update)
                    Tetromino_delete(fig[0]);
                ++fig[0]->x;
                fig[0]->update=1;
                keyboard[1].downtime=100;
            }
        }
        else
            keyboard[1].downtime-=frame;
    }
    //Moving left
    if (keyboard[3].pressed && !fig[0]->update){
        if (keyboard[3].downtime<=0){
            if (fig[0]->move_left(grid)){
                if (!fig[0]->update)
                    Tetromino_delete(fig[0]);
                --fig[0]->x;
                fig[0]->update=1;
                keyboard[3].downtime=100;
            }
        }
        else
            keyboard[3].downtime-=frame;
    }
}

void Game::Tetromino_delete(Tetromino *fig){
    for (int i=0;i<fig->node->h;++i)
        for (int j=0;j<fig->node->w;++j)
            if (fig->node->shape[i][j]){
                SDL_Rect loc={(Sint16)(board_x+(fig->ox-1+j)*(boxh+1)),(Sint16)(board_y+(fig->oy-fig->node->h+i)*(boxh+1)),(Uint16)boxh,(Uint16)boxh};
                SDL_FillRect(screen,&loc,0);
            }
}

void Game::Tetromino_down_end(Tetromino *fig[3], bool **grid, tetromino_list **tetrominos, int l){
    int point_to_add=1;

    for (int i=0;i<fig[0]->node->h;++i)
        for (int j=0;j<fig[0]->node->w;++j)
            if (fig[0]->node->shape[i][j])
                grid[fig[0]->y+1-fig[0]->node->h+i][fig[0]->x+j]=1;
    for (int i=0;i<fig[0]->node->h;++i){
        bool full_line=1;
        for (int j=1;j<=width;++j)
            full_line&=grid[fig[0]->y+1-fig[0]->node->h+i][j];
        if (full_line){
            SDL_Rect from={(Sint16)board_x,(Sint16)board_y,(Uint16)((boxh+1)*10),(Uint16)((boxh+1)*(fig[0]->y-fig[0]->node->h+i))},to={(Sint16)board_x,(Sint16)(board_y+boxh+1),(Uint16)0,(Uint16)0};

            point_to_add=(point_to_add<<1)+1;
            delete[] grid[fig[0]->y+1-fig[0]->node->h+i];
            for (int j=fig[0]->y+1-fig[0]->node->h+i;j>1;--j)
                grid[j]=grid[j-1];
            grid[1]=new bool[width+2]();
            grid[1][0]=grid[1][width+1]=1;
            SDL_BlitSurface(screen,&from,screen,&to);
            for (int j=0;j<width;++j){
                set_rect(&to,board_x+(boxh+1)*j,board_y,boxh,boxh);
                SDL_FillRect(screen,&to,SDL_MapRGB(screen->format,0,0,0));
            }
            --to_next_lv;
            if (to_next_lv<=0){
                to_next_lv=level+3;
                ++level;
            }
        }
    }
    delete fig[0];
    fig[0]=fig[1];
    fig[1]=new Tetromino(tetrominos[rand()%l]->first,level);
    ///MUST CONSIDER IF TETROMINO CAN ENTER
    fig[0]->update=fig[0]->can_enter(grid);
    points+=point_to_add;
    update_score();
}

void Game::replace_with(SDL_Surface *from, short x, short y){
    SDL_Rect src={x,y,(Uint16)from->w,(Uint16)from->h};
    SDL_BlitSurface(background,&src,screen,&src);
    SDL_BlitSurface(from,NULL,screen,&src);
    SDL_UpdateRect(screen,src.x,src.y,src.w,src.h);
}


