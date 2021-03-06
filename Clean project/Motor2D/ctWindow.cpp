#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctWindow.h"

#include "SDL/include/SDL.h"


ctWindow::ctWindow() : ctModule()
{
	window = NULL;
	screen_surface = NULL;
	name = "window";
}

// Destructor
ctWindow::~ctWindow()
{
}

// Called before render is available
bool ctWindow::Awake(pugi::xml_node& config)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		iconExe = SDL_LoadBMP("textures/icon.bmp");

		//Create window
		Uint32 flags = SDL_WINDOW_SHOWN;
		bool fullscreen = config.child("fullscreen").attribute("value").as_bool(false);
		bool borderless = config.child("borderless").attribute("value").as_bool(false);
		bool resizable = config.child("resizable").attribute("value").as_bool(false);
		bool fullscreen_window = config.child("fullscreen_window").attribute("value").as_bool(false);

		width = config.child("resolution").attribute("width").as_int(640);
		height = config.child("resolution").attribute("height").as_int(480);
		scale = config.child("resolution").attribute("scale").as_int(1);

		if (fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if (resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if (fullscreen_window == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(App->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		SDL_SetWindowIcon(window, iconExe);

		if (window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}
	w_scalade = (float)screen_surface->w / width;
	h_scalade = (float)screen_surface->h / height;
	return ret;
}

// Called before quitting
bool ctWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (window != NULL)
		SDL_DestroyWindow(window);

	if (iconExe != NULL)
		SDL_FreeSurface(iconExe);

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Set new window title
void ctWindow::SetTitle(const char* new_title)
{
	//title.create(new_title);
	SDL_SetWindowTitle(window, new_title);
}

void ctWindow::GetWindowSize(uint& width, uint& height) const
{
	width = this->width;
	height = this->height;
}

uint ctWindow::GetScale() const
{
	return scale;
}

float ctWindow::GetWScalade() const
{
	return w_scalade;
}
float ctWindow::GetHScalade() const
{
	return h_scalade;
}