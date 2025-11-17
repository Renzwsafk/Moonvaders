#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>


#define ASSERT(_e, ...) if (!_e) { fprintf(stderr, __VA_ARGS__); exit(1); }

#define W 1024
#define H 764
#define NAME "Moonvaders"

#define KBD_MAX_KEYS 350
#define M_MAX_BUTTONS 5
#define PLAYER_MAX_SPEED 4


typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef size_t usize;
typedef ssize_t isize;

typedef struct _delegate
{
	void (*logic)(void);
	void (*draw)(void);
} delegate_t;

typedef struct _enitity
{
	i32 x, y;
	i32 dx, dy;
	i16 health;
	SDL_Texture *texture;
} entity_t;

static entity_t player;
static delegate_t delegate;
SDL_Window *window;
SDL_Renderer *renderer;
static int keyboard[KBD_MAX_KEYS];
static int mouse[M_MAX_BUTTONS];
static SDL_Rect rect = {0, 0, 100, 100};
// function declaration
int main(int,char**);
static void sdl_init(void);
static void sdl_input(void);
static void clear(void);
static void present(void);
static void cleanup(void);
static void key_down(SDL_KeyboardEvent*);
static void key_up(SDL_KeyboardEvent*);
static void mouse_button_down(SDL_MouseButtonEvent*);
static void mouse_button_up(SDL_MouseButtonEvent*);
static void logic(void);
static int move_player_position(i32,i32);
static void do_player(void);
static SDL_Texture *load_texture(char*); 
static void blit(SDL_Texture*,int,int);

int main(int argc, char **argv)
{
	memset(&player, 0, sizeof(entity_t));
	sdl_init();

	atexit(cleanup);
	player.texture = load_texture("moonvader-with-rifle.png");

	player.x = 100;
	player.y = 50;
	for (;;)
	{
		clear();
		sdl_input(); 
		blit(player.texture, player.x, player.y); 
		delegate.logic();
		present();
		SDL_Delay(16);
	}
	
	return 0;
}

static void sdl_init(void)
{
	i32 window_flags, renderer_flags;
	window_flags = SDL_WINDOW_SHOWN;
	renderer_flags = SDL_RENDERER_ACCELERATED;

	ASSERT(!SDL_Init(SDL_INIT_VIDEO), "SDL2 initialization error: %s\n", SDL_GetError());
	ASSERT(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), "SDL2 image initialization error %s\n", SDL_GetError());

	window = SDL_CreateWindow(NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W, H, window_flags);
	ASSERT(window, "Window creation error: %s\n", SDL_GetError());

	renderer = SDL_CreateRenderer(window, -1, renderer_flags); 	
	ASSERT(renderer, "Renderer creation error: %s\n", SDL_GetError());

	delegate.logic = logic;


}

static void sdl_input(void)
{
	SDL_Event ev;	

	while (SDL_PollEvent(&ev))
	{
		switch (ev.type)
		{
			case SDL_QUIT:
				printf("Quitting... \n");
				exit(0);
				break;
			case SDL_KEYDOWN:
				key_down(&ev.key);
				break;	
			case SDL_KEYUP:
				key_up(&ev.key);
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouse_button_down(&ev.button);
				break;
			case SDL_MOUSEBUTTONUP:
				mouse_button_up(&ev.button);
				break;
			default:
				break;
		}
	}
}

static void clear(void)
{
	SDL_SetRenderDrawColor(renderer, 200, 0, 0, 0); 
	SDL_RenderClear(renderer);
}

static void present(void)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &rect); 
	SDL_RenderPresent(renderer);
}

static void cleanup(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

static void key_down(SDL_KeyboardEvent *ev)
{
	if (ev->repeat == 0 && ev->keysym.scancode < KBD_MAX_KEYS) keyboard[ev->keysym.scancode] = 1; 
}

static void key_up(SDL_KeyboardEvent *ev)
{
	if (ev->repeat == 0 && ev->keysym.scancode < KBD_MAX_KEYS) keyboard[ev->keysym.scancode] = 0; 
}

static void mouse_button_down(SDL_MouseButtonEvent *ev)
{
	if (ev->button < M_MAX_BUTTONS)
	{
		mouse[ev->button] = 1;	
	}
}

static void mouse_button_up(SDL_MouseButtonEvent *ev)
{
	if (ev->button < M_MAX_BUTTONS)
	{
		mouse[ev->button] = 0;
	}
}

static void logic(void)
{
	do_player();
}


static void do_player(void)
{

	
}

static SDL_Texture *load_texture(char *filename)
{
	SDL_Texture *texture;
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
	texture = IMG_LoadTexture(renderer, filename);

	return texture;
}

static void blit(SDL_Texture *texture, int x, int y)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	SDL_RenderCopy(renderer, texture, NULL, &rect); 
}
