/*
=================
cRocket.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cRocket.h"

void cRocket::render()
{
	spritePos2D.x = 50;

	glPushMatrix();

	glTranslatef(spritePos2D.x, spritePos2D.y, 0.0f);
	glRotatef(spriteRotation, 0.0f, 0.0f, 1.0f);
	glScalef(spriteScaling.x, spriteScaling.y, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, GLTextureID); // Binding of GLtexture name 

	glBegin(GL_QUADS);
	glColor3f(255.0f, 255.0f, 255.0f);
	glTexCoord2f(spriteTexCoordData[0].x, spriteTexCoordData[0].y);
	glVertex2f(-(textureWidth / 2), -(textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[1].x, spriteTexCoordData[1].y);
	glVertex2f((textureWidth / 2), -(textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[2].x, spriteTexCoordData[2].y);
	glVertex2f((textureWidth / 2), (textureHeight / 2));
	glTexCoord2f(spriteTexCoordData[3].x, spriteTexCoordData[3].y);
	glVertex2f(-(textureWidth / 2), (textureHeight / 2));

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cRocket::update(float deltaTime)
{
	//prevents the rocket from being rotating more than 45 degrees
	//upwards or downwards
	if (m_InputMgr->isKeyDown(VK_RIGHT) && spriteRotation < 135)
	{
		spriteRotation += 5.0f;
	}
	if (m_InputMgr->isKeyDown(VK_LEFT) && spriteRotation > 45)
	{
		spriteRotation -= 5.0f;
	}

	//if up or down is pressed it moves the rocket correspondingly
	if (m_InputMgr->isKeyDown(VK_UP))
	{
		spritePos2D.y -= 10;
	}
	if (m_InputMgr->isKeyDown(VK_DOWN))
	{
		spritePos2D.y += 10;
	}

	//changes the scale of the ship
	if (m_InputMgr->isKeyDown(int('A')))
	{
		spriteScaling += 0.2f;
	}
	if (m_InputMgr->isKeyDown(int('S')))
	{
		spriteScaling -= 0.2f;
	}

	if (m_InputMgr->isKeyDown(VK_SPACE))
	{
		// Add new bullet sprite to the vector array
		theRocketBullets.push_back(new cBullet);
		int numBullets = theRocketBullets.size() - 1;
		theRocketBullets[numBullets]->setSpritePos(glm::vec2(spritePos2D.x , spritePos2D.y));
		theRocketBullets[numBullets]->setSpriteTranslation(glm::vec2(10.0f, 10.0f));
		theRocketBullets[numBullets]->setTexture(theGameTextures[4]->getTexture());
		theRocketBullets[numBullets]->setTextureDimensions(theGameTextures[4]->getTWidth(), theGameTextures[4]->getTHeight());
		theRocketBullets[numBullets]->setSpriteCentre();
		theRocketBullets[numBullets]->setBulletVelocity(glm::vec2(0.0f, 0.0f));
		theRocketBullets[numBullets]->setSpriteRotation(getSpriteRotation());
		theRocketBullets[numBullets]->setActive(true);
		theRocketBullets[numBullets]->setMdlRadius();
		// play the firing sound
		m_SoundMgr->getSnd("Shot")->playAudio(AL_TRUE);
	}


	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	for (vector<cBullet*>::iterator bulletIterartor = theRocketBullets.begin(); bulletIterartor != theRocketBullets.end(); ++bulletIterartor)
	{
		(*bulletIterartor)->update(deltaTime);
		for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
		{
			if ((*asteroidIterator)->collidedWith((*asteroidIterator)->getBoundingRect(), (*bulletIterartor)->getBoundingRect()))
			{
				// if a collision set the bullet to false
				//tells the asteroid to respawn
				//and increases score
				(*asteroidIterator)->respawn();
				(*bulletIterartor)->setActive(false);
				score++;
			}
		}
	}


	//for each bullet
	vector<cBullet*>::iterator bulletIterartor = theRocketBullets.begin();
	while (bulletIterartor != theRocketBullets.end())
	{
		if ((*bulletIterartor)->isActive() == false)
		{
			//once the bullet nolong is no longer active
			//it deletes the bullet
			bulletIterartor = theRocketBullets.erase(bulletIterartor);
			// play the explosion sound.
			m_SoundMgr->getSnd("Explosion")->playAudio(AL_TRUE);
		}
		else
		{
			//as long as each bullet exists
			//it updates and renders it
			(*bulletIterartor)->render();
			++bulletIterartor;
		}
	}

	//creates a rectangle at the sprite postion the same size as the sprite
	RECT crash = { spritePos2D.x - textureHeight / 2, spritePos2D.y - textureHeight / 2, spritePos2D.x + textureWidth / 2, spritePos2D.y + textureHeight / 2 };
	//checks each of the asteroids
	for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
	{
		//if any have collided with the rectangle of the ship
		if ((*asteroidIterator)->collidedWith((*asteroidIterator)->getBoundingRect(), crash))
		{
			//it changes a boolean
			dead = true;
		}
	}
}
/*
=================================================================
  Sets the velocity for the rocket
=================================================================
*/
void cRocket::setRocketVelocity(glm::vec2 rocketVel)
{
	rocketVelocity = rocketVel;
}
/*
=================================================================
  Gets the rocket velocity
=================================================================
*/
glm::vec2 cRocket::getRocketVelocity()
{
	return rocketVelocity;
}
bool cRocket::checkDead()
{
	//checks if the player has died or not
	if (dead)
		return true;
	else
		return false;
}
void cRocket::reset()
{
	//when the game is over it will reset the death variable
	dead = false;
	score = 0;
	for (vector<cBullet*>::iterator bulletIterartor = theRocketBullets.begin(); bulletIterartor != theRocketBullets.end(); ++bulletIterartor)
	{
		(*bulletIterartor)->setActive(false);
	}
}


