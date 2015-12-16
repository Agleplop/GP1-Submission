#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cFontMgr.h"
#include "cSprite.h"
#include "asteroidsGame.h"
#include "cButton.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int cmdShow)
{

	//Set our window settings
	const int windowWidth = 1080;
	const int windowHeight = 720;
	const int windowBPP = 16;



	//This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	//The example OpenGL code
	windowOGL theOGLWnd;

	//Attach our the OpenGL window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

	//Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
	{
		//If it fails

		MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
	{
		MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	/* initialize random seed: */
	srand((unsigned int)time(NULL));

	// Create vector array of textures
	LPCSTR texturesToUse[] = { "Images\\Asteroid.png", "Images\\Alien1.png", "Images\\Bee.png", "Images\\Squid.png", "Images\\Laser.png" };
	for (int tCount = 0; tCount < 5; tCount++)
	{
		theGameTextures.push_back(new cTexture());
		theGameTextures[tCount]->createTexture(texturesToUse[tCount]);
	}

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[4] = { "Audio\\Dieseldotogg.wav", "Audio\\WhoLikestoParty.mp3","Audio\\Shot.mp3", "Audio\\Explosion.mp3" };

	theSoundMgr->add("Dieseldotogg", gameSounds[0]);
	theSoundMgr->add("Party", gameSounds[1]);
	theSoundMgr->add("Shot", gameSounds[2]);
	theSoundMgr->add("Explosion", gameSounds[3]);

	// load game fontss
	// Load Fonts
	LPCSTR gameFonts[2] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 24);

	// Create vector array of textures

	for (int astro = 0; astro < 10; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos(glm::vec2(1080, rand() % windowHeight));
		int randAsteroid = rand() % 4;
		theAsteroids[astro]->setTexture(theGameTextures[randAsteroid]->getTexture());
		theAsteroids[astro]->setTextureDimensions(theGameTextures[randAsteroid]->getTWidth(), theGameTextures[randAsteroid]->getTHeight());
		theAsteroids[astro]->setSpriteCentre();
		theAsteroids[astro]->setActive(true);
		theAsteroids[astro]->setMdlRadius();
		theAsteroids[astro]->speed();
	}

	//Creates all of the backgrounds
	//one for each of the playstates.
	cTexture textureBkgdMenu;
	textureBkgdMenu.createTexture("Images\\Background0.png");
	cBkGround spriteBkgdMenu;
	spriteBkgdMenu.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgdMenu.setTexture(textureBkgdMenu.getTexture());
	spriteBkgdMenu.setTextureDimensions(textureBkgdMenu.getTWidth(), textureBkgdMenu.getTHeight());

	cTexture textureBkgdPlay;
	textureBkgdPlay.createTexture("Images\\Background1.png");
	cBkGround spriteBkgdPlay;
	spriteBkgdPlay.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgdPlay.setTexture(textureBkgdPlay.getTexture());
	spriteBkgdPlay.setTextureDimensions(textureBkgdPlay.getTWidth(), textureBkgdPlay.getTHeight());

	cTexture textureBkgdExit;
	textureBkgdExit.createTexture("Images\\Background2.png");
	cBkGround spriteBkgdExit;
	spriteBkgdExit.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgdExit.setTexture(textureBkgdExit.getTexture());
	spriteBkgdExit.setTextureDimensions(textureBkgdExit.getTWidth(), textureBkgdExit.getTHeight());

	//creates the rocket ship that the player controls
	//and assigns all the attributes necessary 
	cTexture rocketTxt;
	rocketTxt.createTexture("Images\\RocketShip.png");
	cRocket rocketSprite;
	rocketSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	rocketSprite.setSpritePos(glm::vec2(50.0f, 380.0f));
	rocketSprite.setTexture(rocketTxt.getTexture());
	rocketSprite.setTextureDimensions(rocketTxt.getTWidth(), rocketTxt.getTHeight());
	rocketSprite.setSpriteCentre();
	rocketSprite.setRocketVelocity(glm::vec2(0.0f, 0.0f));
	rocketSprite.setSpriteRotation(90.0f);

	// Attach sound manager to rocket sprite
	rocketSprite.attachSoundMgr(theSoundMgr);

	// Create vector array of button textures
	vector<cTexture*> btnTextureList;
	LPCSTR btnTexturesToUse[] = { "Images/ExitButton.png", "Images/PlayButton.png" };
	for (int tCount = 0; tCount < 2; tCount++)
	{
		btnTextureList.push_back(new cTexture());
		btnTextureList[tCount]->createTexture(btnTexturesToUse[tCount]);
	}

	//creates the to buttons
	cButton exitButton;
	exitButton.attachInputMgr(theInputMgr);
	exitButton.setTexture(btnTextureList[0]->getTexture());
	exitButton.setTextureDimensions(btnTextureList[0]->getTWidth(), btnTextureList[0]->getTHeight());

	cButton playButton;
	playButton.attachInputMgr(theInputMgr);
	playButton.setTexture(btnTextureList[1]->getTexture());
	playButton.setTextureDimensions(btnTextureList[1]->getTWidth(), btnTextureList[1]->getTHeight());

	//initilises the variable theGameState
	//and makes the game start at the main menu
	gameState theGameState = MENU;

	//This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
	{
		pgmWNDMgr->processWNDEvents(); //Process any window events


		//We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		std::string score;

		switch (theGameState)
		{
		case MENU:

			//renders the main menu background
			spriteBkgdMenu.render();

			//draws and position the two buttons
			playButton.setSpritePos(glm::vec2(windowWidth / 2 - 32, windowHeight / 2));
			exitButton.setSpritePos(glm::vec2(windowWidth / 2 - 32, windowHeight / 2 + 48));
			playButton.render();
			exitButton.render();

			//calls update for the buttons
			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update(0.0f);

			//displays some text on the screen
			theFontMgr->getFont("Space")->printText("press A to play", FTPoint(windowWidth / 2 - 125 , windowHeight / 2 - 64, 0.0f));
			theFontMgr->getFont("Space")->printText("press X to quit", FTPoint(windowWidth / 2 - 125, windowHeight / 2 - 32, 0.0f));

			//checks if it has been told to start the game yet
			if (playButton.startGame())
				//if it have started it changes the game state to playing
				theGameState = PLAYING;

			//checks to see if the game has been quit
			if (exitButton.exitGame())
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);

			theSoundMgr->getSnd("Party")->playAudio(AL_TRUE);

			break; 
		case PLAYING:
		{

			//once the gamestate has been changed to playing
			//the game starts playing the actual game

			//renders background
			spriteBkgdPlay.render();

			//tells the rocket update
			rocketSprite.update(elapsedTime);

			//checks through all the of asteroids
			vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
			while (asteroidIterator != theAsteroids.end())
			{
				//if the player has died
				if (rocketSprite.checkDead() == true)
				{
					//it will end the game by changing the game state
					theGameState = END;
					for (int i = 0; i < 10; i++)
					{
						//it will then go through all of the asteroids and reset them to their starting position
						theAsteroids[i]->setSpritePos(glm::vec2(1080, rand() % windowHeight));
						//it will also reset the losing condition
						rocketSprite.reset();
					}
				}

				if ((*asteroidIterator)->isActive() == false)
				{
					asteroidIterator = theAsteroids.erase(asteroidIterator);
				}
				else
				{
					//as long as there are asteroids left
					//this will occur causing the asteroids
					//to update and render
					(*asteroidIterator)->update(elapsedTime);
					(*asteroidIterator)->render();
					++asteroidIterator;
				}
			}

			//this gets the score variable from the rocket
			//and converts it to a string

			score = std::to_string(rocketSprite.score);
			//this renders the rocket
			rocketSprite.render();
			//this displays text
			theFontMgr->getFont("Space")->printText("Your score is", FTPoint(15.0f, 15.0f, 0.0f));
			//this prints the string of the score
			theFontMgr->getFont("Space")->printText(score.c_str(), FTPoint(15.0f, 45.0f, 0.0f));

			//this plays the audio
			theSoundMgr->getSnd("Dieseldotogg")->playAudio(AL_TRUE);

			break; }
		case END:
			//renders background
			spriteBkgdExit.render();

			//positions and renders buttons
			playButton.setSpritePos(glm::vec2(windowWidth / 2 - 32, windowHeight / 2));
			exitButton.setSpritePos(glm::vec2(windowWidth / 2 - 32, windowHeight / 2 + 48));
			playButton.render();
			exitButton.render();

			//calls update for the buttons
			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update(0.0f);

			//displays text and the players score
			score = std::to_string(rocketSprite.score);
			theFontMgr->getFont("Space")->printText(score.c_str(), FTPoint(windowWidth / 2 - 10, 300.0f, 0.0f));
			theFontMgr->getFont("Space")->printText("Game over. you scored:", FTPoint(windowWidth / 2 - 180, 250.0f, 0.0f));
			theFontMgr->getFont("Space")->printText("press A to replay", FTPoint(windowWidth / 2 - 125, windowHeight / 2 + 150, 0.0f));
			theFontMgr->getFont("Space")->printText("press X to quit", FTPoint(windowWidth / 2 - 125, windowHeight / 2 + 182, 0.0f));

			//checks is the player has started playing the game again
			if (playButton.startGame())
				theGameState = PLAYING;

			//checks if the player has chosen to quit the game
			if (exitButton.exitGame())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}


			break;
		}


		pgmWNDMgr->swapBuffers();
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);



	}
	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window
	return 0; //Return success
}
