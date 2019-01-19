#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define MAX_NAME_LENGTH 16

int graines[2][6];

typedef enum gameState {
    MENU, JVJ, JVP, EXIT
} gameState;

typedef struct Images{
    SDL_Texture* menuTex;
    SDL_Texture* input1Tex;
    SDL_Texture* input2Tex;

} Images;

typedef struct Button{
    int min_x, max_x, min_y, max_y;
} Button;

typedef struct Buttons{
    Button jvjBtn;
    Button jvpBtn;
    Button quitterBtn;
    Button validerBtn;
    Button retourBtn;
} Buttons;

typedef enum BOOL {
    TRUE = 1,
    FALSE = 0,
    true = 1,
    false = 0
} BOOL;

void FreeImages(Images* images);
void chargerImages(Images* images, SDL_Renderer* renderer);
void initButtons(Buttons *btns);
int afficherImage(SDL_Renderer* renderer, SDL_Texture* texture);
int afficherTexte(SDL_Renderer* renderer, SDL_Texture* texture, int posX, int posY);
short int mouseABouton(int x, int y, Buttons btns);

int main(int argc, char *argv[]) {
    Images images;
    Buttons btns;
    gameState gs = MENU;
    unsigned int quitter = 0, 
        jvpTexteLength = 0, 
        jvj1TexteLength = 0, 
        jvj2TexteLength = 0;
    char* jvpTexteInput = (char*) malloc(16), 
        *jvj1TexteInput = (char*) malloc(16), 
        *jvj2TexteInput = (char*) malloc(16);
    BOOL isJVPTexteInput = FALSE, 
        isJVJ1TexteInput = FALSE, 
        isJVJ2TexteInput = FALSE;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    initButtons(&btns);

    SDL_Window* ecran = SDL_CreateWindow("Awélé",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            800, 600, SDL_WINDOW_RESIZABLE);
    SDL_SetWindowResizable(ecran, SDL_FALSE);
    if (ecran == NULL) {
        printf("Error : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(ecran, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    // Activation de l'alpha (transparence)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // Chargement des images
    chargerImages(&images, renderer);
    
    // Définir la rectangle de la saisie
    SDL_Rect jvpTexteRect, jvj1TexteRect, jvj2TexteRect;
    jvpTexteRect.x = 248;
    jvj1TexteRect.x = 59;
    jvj2TexteRect.x = 435;
    jvpTexteRect.y = jvj1TexteRect.y = jvj2TexteRect.y = 226;
    jvpTexteRect.w = jvj1TexteRect.w = jvj2TexteRect.w = 305;
    jvpTexteRect.h = jvj1TexteRect.h = jvj2TexteRect.h = 98;
    
    TTF_Font* SamanthaFBFont = TTF_OpenFont("The Foreign.otf", 64);
    if (SamanthaFBFont == NULL){
        printf("Error : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_Color black = {0, 0, 0};
    SDL_Color white = {255, 255, 255};
    SDL_Surface *jvpSurfaceTexte = NULL, 
        *jvj1SurfaceTexte = NULL, 
        *jvj2SurfaceTexte = NULL,
        *player1SurfaceTexte = NULL,
        *player2SurfaceTexte = NULL;
    SDL_Texture* jvpTexte = NULL, 
        *jvj1Texte = NULL, 
        *jvj2Texte = NULL,
        *player1Texte = NULL,
        *player2Texte = NULL;

    while (gs != EXIT)
    {
        switch (gs){
            case MENU:
                if (afficherImage(renderer, images.menuTex) == EXIT_FAILURE)
                    return EXIT_FAILURE;
            break;
            case JVJ:
                if (afficherImage(renderer, images.input2Tex) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                SDL_RenderCopy(renderer, jvj1Texte, NULL, &jvj1TexteRect);
                SDL_RenderCopy(renderer, jvj2Texte, NULL, &jvj2TexteRect);
            break;
            case JVP:
                if (afficherImage(renderer, images.input1Tex) == EXIT_FAILURE)
                    return EXIT_FAILURE;
                SDL_RenderCopy(renderer, jvpTexte, NULL, &jvpTexteRect);
            break;
            default: 
            break;
        }

        SDL_Event event;
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    if (gs == JVP && jvpTexteLength > 0){
                        jvpTexteLength--;
                        jvpTexteInput[strlen(jvpTexteInput)-1] = 0;
                        jvpSurfaceTexte = TTF_RenderText_Blended(SamanthaFBFont, jvpTexteInput, black);
                        jvpTexte = SDL_CreateTextureFromSurface(renderer, jvpSurfaceTexte);
                    } else if (gs == JVJ){
                        // Joueur 1
                        if (isJVJ1TexteInput && jvj1TexteLength > 0) {
                            jvj1TexteLength--;
                            jvj1TexteInput[strlen(jvj1TexteInput)-1] = 0;
                            jvj1SurfaceTexte = TTF_RenderText_Blended(SamanthaFBFont, jvj1TexteInput, black);
                            jvj1Texte = SDL_CreateTextureFromSurface(renderer, jvj1SurfaceTexte);
                        } // Joueur 2
                        else if (isJVJ2TexteInput && jvj2TexteLength > 0) {
                            jvj2TexteLength--;
                            jvj2TexteInput[strlen(jvj2TexteInput)-1] = 0;
                            jvj2SurfaceTexte = TTF_RenderText_Blended(SamanthaFBFont, jvj2TexteInput, black);
                            jvj2Texte = SDL_CreateTextureFromSurface(renderer, jvj2SurfaceTexte);
                        }
                    }
                }
            break;
            case SDL_TEXTINPUT:
                if (gs == JVP && jvpTexteLength < MAX_NAME_LENGTH)
                {
                    jvpTexteLength++;
                    strcat(jvpTexteInput, event.text.text);
                    jvpSurfaceTexte = TTF_RenderText_Blended(SamanthaFBFont, jvpTexteInput, black);
                    jvpTexte = SDL_CreateTextureFromSurface(renderer, jvpSurfaceTexte);
                } else if (gs == JVJ)
                {
                    // Joueur 1
                    if (isJVJ1TexteInput && jvj1TexteLength < MAX_NAME_LENGTH) {
                        jvj1TexteLength++;
                        strcat(jvj1TexteInput, event.text.text);
                        jvj1SurfaceTexte = TTF_RenderText_Blended(SamanthaFBFont, jvj1TexteInput, black);
                        jvj1Texte = SDL_CreateTextureFromSurface(renderer, jvj1SurfaceTexte);
                    } // Joueur 2
                    else if (isJVJ2TexteInput && jvj2TexteLength < MAX_NAME_LENGTH) {
                        jvj2TexteLength++;
                        strcat(jvj2TexteInput, event.text.text);
                        jvj2SurfaceTexte = TTF_RenderText_Blended(SamanthaFBFont, jvj2TexteInput, black);
                        jvj2Texte = SDL_CreateTextureFromSurface(renderer, jvj2SurfaceTexte);
                    }
                }
            break;
            case SDL_MOUSEBUTTONDOWN:
                if (gs == JVJ)
                {
                    int x = event.button.x, y=event.button.y;
                    // Cliquer l'un des cadres du texte
                    if (x >= jvj1TexteRect.x && x <= jvj1TexteRect.x+jvj1TexteRect.w &&
                        y >= jvj1TexteRect.y && y <= jvj1TexteRect.y+jvj1TexteRect.h)
                    {
                        SDL_StartTextInput();
                        isJVJ1TexteInput = TRUE;
                        isJVJ2TexteInput = FALSE;
                    } else if (x >= jvj2TexteRect.x && x <= jvj2TexteRect.x+jvj2TexteRect.w &&
                        y >= jvj2TexteRect.y && y <= jvj2TexteRect.y+jvj2TexteRect.h)
                    { 
                        SDL_StartTextInput();
                        isJVJ1TexteInput = FALSE;
                        isJVJ2TexteInput = TRUE;
                    } else {
                        isJVJ1TexteInput = FALSE;
                        isJVJ2TexteInput = FALSE;
                        SDL_StopTextInput();
                    }
                    // Cliquer sur la bouton 'Valider'
                    if (x >= btns.validerBtn.min_x && x <= btns.validerBtn.max_x &&
                        y >= btns.validerBtn.min_y && y <= btns.validerBtn.max_y)
                    {
                        printf("Vous avez cliqué sur la bouton Valider\n");
                    }

                    // Cliquer sur la bouton 'Retour'
                    if (x >= btns.retourBtn.min_x && x <= btns.retourBtn.max_x &&
                        y >= btns.retourBtn.min_y && y <= btns.retourBtn.max_y)
                    {
                        gs = MENU;
                    }
                } else if (gs == JVP)
                {
                    int x = event.button.x, y=event.button.y;
                    // Cliquer sur le cadre du texte
                    if (x >= jvpTexteRect.x && x <= jvpTexteRect.x+jvpTexteRect.w &&
                        y >= jvpTexteRect.y && y <= jvpTexteRect.y+jvpTexteRect.h)
                    {
                        isJVPTexteInput = TRUE;
                        
                        SDL_StartTextInput();
                    } else {
                        isJVPTexteInput = FALSE;
                        SDL_StopTextInput();
                    }

                    // Cliquer sur la bouton 'Valider'
                    if (x >= btns.validerBtn.min_x && x <= btns.validerBtn.max_x &&
                        y >= btns.validerBtn.min_y && y <= btns.validerBtn.max_y)
                    {
                        printf("Vous avez cliqué sur la bouton Valider\n");
                    }
                    // Cliquer sur la bouton 'Retour'
                    if (x >= btns.retourBtn.min_x && x <= btns.retourBtn.max_x &&
                        y >= btns.retourBtn.min_y && y <= btns.retourBtn.max_y)
                    {
                        gs = MENU;
                    }
                } else if (gs == MENU) {
                    int bouton = mouseABouton(event.button.x, event.button.y, btns);
                    if (bouton == 0) {
                        gs = JVJ;
                    } else if (bouton == 1) {
                        gs = JVP;
                    } else if (bouton == 2) {
                        gs = EXIT;
                    }
                }
                break;
            case SDL_QUIT:
                gs = EXIT;
                break;
        }
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
        if (isJVPTexteInput)
            SDL_RenderFillRect(renderer, &jvpTexteRect);
        if (isJVJ1TexteInput)
            SDL_RenderFillRect(renderer, &jvj1TexteRect);
        if (isJVJ2TexteInput)
            SDL_RenderFillRect(renderer, &jvj2TexteRect);
        
        /* À la place de SDL_Flip */
        SDL_RenderPresent(renderer);
    }

    FreeImages(&images);
    SDL_FreeSurface(jvpSurfaceTexte);
    SDL_FreeSurface(player1SurfaceTexte);
    SDL_FreeSurface(player2SurfaceTexte);
    SDL_DestroyTexture(jvpTexte);
    SDL_DestroyTexture(player1Texte);
    SDL_DestroyTexture(player2Texte);
    SDL_DestroyWindow(ecran);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(SamanthaFBFont);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}

void initButtons(Buttons *btns)
{
    short int w = 305, h = 98;

    // (x coords)
    btns->jvjBtn.min_x = btns->jvpBtn.min_x = 
    btns->retourBtn.min_x = btns->quitterBtn.min_x = 
    btns->validerBtn.min_x = 248;
    btns->jvjBtn.max_x = btns->jvpBtn.max_x = 
    btns->retourBtn.max_x = btns->quitterBtn.max_x = 
    btns->validerBtn.max_x =248 + w;
    // (y coords)
    btns->jvpBtn.min_y = 206;
    btns->jvpBtn.max_y = 206 + h;
    btns->jvjBtn.min_y = btns->validerBtn.min_y = 334;
    btns->jvjBtn.max_y = btns->validerBtn.max_y = 334 + h;
    btns->retourBtn.min_y = btns->quitterBtn.min_y = 461;
    btns->retourBtn.max_y = btns->quitterBtn.max_y = 461 + h;
}
void FreeImages(Images* images)
{
    SDL_DestroyTexture(images->menuTex);
    SDL_DestroyTexture(images->input1Tex);
    SDL_DestroyTexture(images->input2Tex);
}
void chargerImages(Images* images, SDL_Renderer* renderer)
{
    SDL_Surface* s = IMG_Load("images/menu.png");
    images->menuTex = SDL_CreateTextureFromSurface(renderer, s);
    s = IMG_Load("images/input1.png");
    images->input1Tex = SDL_CreateTextureFromSurface(renderer, s);
    s = IMG_Load("images/input2.png");
    images->input2Tex = SDL_CreateTextureFromSurface(renderer, s);
}

int afficherImage(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_Rect dest = {0, 0, 800, 600};

    /* SDL_RenderCopy est équivalent à SDL_BlitSurface */
    if (texture == NULL || SDL_RenderCopy(renderer, texture, NULL, &dest) != 0) {
        printf("Error : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int afficherTexte(SDL_Renderer* renderer, SDL_Texture* texture, int posX, int posY)
{
    SDL_Rect dest = {posX, posY, 200, 57};
    if (SDL_RenderCopy(renderer, texture, NULL, &dest) != 0) {
        printf("Error : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

short int mouseABouton(int x, int y, Buttons btns)
{
    short int bouton = -1;
    if (x >= btns.jvpBtn.min_x && x <= btns.jvpBtn.max_x)
    {
        // Cliquer sur la bouton 'JCJ'
        if (y >= btns.jvjBtn.min_y && y <= btns.jvjBtn.max_y)
            bouton = 0;
        // Cliquer sur la bouton 'JCP'
        else if (y >= btns.jvpBtn.min_y && y <= btns.jvpBtn.max_y)
            bouton = 1;
        // Cliquer sur la bouton 'QUITTER'
        else if (y >= btns.quitterBtn.min_y && y <= btns.quitterBtn.max_y)
            bouton = 2;
    }    
    
    return bouton;
}
