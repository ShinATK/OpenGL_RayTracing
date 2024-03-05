#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

glm::vec3 LinearLerp(glm::vec3 start, glm::vec3 end, float t);

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default Settings
const glm::vec3 kPosition = glm::vec3(0., 0., 0.);
const glm::vec3 kUp = glm::vec3(0., 1., 0.);
const glm::vec3 kFront = glm::vec3(0., 0., -1.);
const GLfloat kYaw = -90.0f;
const GLfloat kPicth = 0.0f;
const GLfloat kMovementSpeed = 5.0f;
const GLfloat kSensitivity = 0.1f;
const GLfloat kZoom = 45.0f;
const GLfloat kNear = 0.1f;
const GLfloat kFar = 100.0f;

class Camera
{
private:
	glm::vec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;
	glm::vec3 world_up_;

	GLfloat yaw_;
	GLfloat pitch_;

	GLfloat movement_speed_;
	GLfloat mouse_sensitivity_;
	GLfloat zoom_;

	GLfloat near_;
	GLfloat far_;

	GLboolean m_bLockOnFloor;

public:
	Camera(glm::vec3 position = kPosition, glm::vec3 up = kUp, float yaw = kYaw, float pitch = kPicth);


	void ProcessInput(GLFWwindow* window, float dt);
	void PosMovement(Camera_Movement direction, float dt);
	void ViewMovement(GLfloat xoffset, GLfloat yoffset);
	void UpdateCameraVectors();

	glm::vec3 GetFront() const;
	void SetFront(glm::vec3 new_front);

	glm::mat4 GetViewMatrix() const;
	void GetRotation();

	glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 new_position);
	void SetPositionY(float new_positionY);

	GLfloat GetZoom() const;
	void SetZoom(GLfloat new_zoom);
	GLfloat GetNear() const;
	void SetNear(GLfloat new_near);
	GLfloat GetFar() const;
	void SetFar(GLfloat new_far);

	GLboolean GetbLockOnFloor() const;
	void SetbLockOnFloor(GLboolean bLockOn);
};



