#pragma once
#include <cmath>
#include <glm.hpp>
#define FORWARD 1
#define BACKWARDS 2
#define LEFT 3
#define RIGHT 4
#define UP 5
#define DOWN 6
#define CAMERA_POSITION 10
#define CAMERA_ROTATION 11
class Player
{
private:
	int deltaX;
	int deltaY;
	float speed;
	glm::vec3 lastCameraPosition;
	glm::vec3 cameraPosition;
	glm::vec3 cameraRotation;
public:
	Player(void);
	~Player(void);
	void SetCamera(int cameraType, glm::vec3 cameraVector);
	glm::vec3 GetCamera(int cameraType);
	void UpdateLook(CPoint point);
	void Move(int direction);
};

