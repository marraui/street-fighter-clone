#include <Game.h>
#include <Hitbox.h>
#include <Character.h>
#include <InputKeyboard.h>

Game::Game(){
    //Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		mWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( mWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( mRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawBlendMode( mRenderer, SDL_BLENDMODE_BLEND );
				//Initialize renderer color
				SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

			}
		}
	}

	if( !success ){
        throw "Couldn't initialize game\n";
    }
}

Game::~Game(){
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    mRenderer = NULL;
    mWindow = NULL;

    //Quit SDL subsystems
    IMG_Quit();
	SDL_Quit();
}

void Game::gameLoop(){
    bool quit = false;

    SDL_Event e;

    Character* character = new Character( (SCREEN_WIDTH - 20) / 2, (SCREEN_HEIGHT - 40 ) / 2, 40, 80 );
	Stage* stage = new Stage();

	InputKeyboard* inputKeyboard = new InputKeyboard();
	bool firstFrame = true;

    while( !quit ){
        while( SDL_PollEvent( &e ) ){
            
            if( e.type == SDL_QUIT ){
                quit = true;
            }

        }
		
		if( firstFrame ){
			inputKeyboard->Initialize();
			firstFrame = false;
		}
		else{
			inputKeyboard->Update();
		}
        
		SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( mRenderer );

		character->handleInput( inputKeyboard , stage);

		character->move( stage );

		character->render( mRenderer );

		SDL_RenderPresent( mRenderer );
    }
    
}