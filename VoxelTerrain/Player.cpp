#include "stdafx.h"
#include "Player.h"


Player::Player(void)
{
	cameraPosition = glm::vec3(0.0f,0.0f,0.0f);
	cameraRotation = glm::vec3(0.0f,0.0f,0.0f);
	lastCameraPosition = glm::vec3(0.0f,0.0f,0.0f);
	speed = 1.0f;
}


Player::~Player(void)
{

}
void Player::SetCamera(int cameraType, glm::vec3 cameraVector)
{
	switch(cameraType)
	{
	case CAMERA_POSITION:
		{
			cameraPosition = cameraVector;
			break;
		}
	case CAMERA_ROTATION:
		{
			cameraRotation = cameraVector;
			break;
		}
	}
}
glm::vec3 Player::GetCamera(int cameraType)
{
	switch(cameraType)
	{
	case CAMERA_POSITION:
		{
			return cameraPosition;
		}
	case CAMERA_ROTATION:
		{
			return cameraRotation;
		}
	default:
		{
			return cameraPosition;
		}
	}
}
void Player::UpdateLook(CPoint point)
{
	deltaX = (int)(point.x - lastCameraPosition.x);
	deltaY = (int)(point.y - lastCameraPosition.y);
	lastCameraPosition.x = (float) point.x;
	lastCameraPosition.y = (float) point.y;

	cameraRotation.x += (float) deltaX * 1.0f;
	cameraRotation.y += (float) deltaY * 1.0f;

	if(cameraRotation.y >= 90.0f)
		cameraRotation.y = 90.0f;
	if(cameraRotation.y <= -90.0f)
		cameraRotation.y = -90.0f;
}
void Player::Move(int direction)
{
	switch(direction)
	{
		case FORWARD:
			{
				cameraPosition.x += (float)sin(cameraRotation.x * 3.14f/180.0f) * speed;
				cameraPosition.z -= (float)cos(cameraRotation.x * 3.14f/180.0f) * speed;
				break;
			}
		case BACKWARDS:
			{
				cameraPosition.x -= (float)sin(cameraRotation.x * 3.14f/180.0f) * speed;
				cameraPosition.z += (float)cos(cameraRotation.x * 3.14f/180.0f) * speed;
				break;
			}
		case LEFT:
			{
				cameraPosition.x += (float)sin((cameraRotation.x - 90.0f) * 3.14f/180.0f) * speed;
				cameraPosition.z -= (float)cos((cameraRotation.x - 90.0f) * 3.14f/180.0f) * speed;
				break;
			}
		case RIGHT:
			{
				cameraPosition.x += (float)sin((cameraRotation.x + 90.0f) * 3.14f/180.0f) * speed;
				cameraPosition.z -= (float)cos((cameraRotation.x + 90.0f) * 3.14f/180.0f) * speed;
				break;
			}
		case UP:
			{
				cameraPosition.y += speed;
				break;
			}
		case DOWN:
			{
				cameraPosition.y -= speed;
				break;
			}
	}
}