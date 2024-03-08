#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : position_(position), world_up_(up), yaw_(yaw), pitch_(pitch), front_(kFront), movement_speed_(kMovementSpeed), mouse_sensitivity_(kSensitivity), zoom_(kZoom), near_(kNear), far_(kFar), m_bLockOnFloor(false)
{
	this->UpdateCameraVectors();
}
glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(
		this->position_,
		this->position_ + this->front_,
		this->up_);
}
void Camera::GetRotation()
{
	glm::mat4 view = GetViewMatrix();
	auto test = view * glm::inverse(view);
	for (int i = 0; i < 4; ++i) {
		printf("%f %f %f %f\n", test[i][0], test[i][1], test[i][2], test[i][3]);
	}
	std::cout << std::endl;
}
glm::vec3 Camera::GetPosition() const
{
	return this->position_;
}
void Camera::SetPosition(glm::vec3 new_position)
{
	this->position_ = new_position;
}
void Camera::SetPositionY(float new_positionY)
{
	this->position_.y = new_positionY;
}
GLfloat Camera::GetZoom() const
{
	return this->zoom_;
}
void Camera::SetZoom(GLfloat new_zoom)
{
	this->zoom_ = new_zoom;
}
GLfloat Camera::GetNear() const
{
	return this->near_;
}
void Camera::SetNear(GLfloat new_near)
{
	this->near_ = new_near;
}

GLfloat Camera::GetFar() const
{
	return this->far_;
}

void Camera::SetFar(GLfloat new_far)
{
	this->far_ = new_far;
}

void Camera::ProcessInput(GLFWwindow* window, float dt)
{
	// WASD
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		this->PosMovement(FORWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		this->PosMovement(BACKWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		this->PosMovement(LEFT, dt);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		this->PosMovement(RIGHT, dt);

	// UP and DOWN
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		this->PosMovement(UP, dt);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		this->PosMovement(DOWN, dt);
}
void Camera::PosMovement(Camera_Movement direction, float deltaTime)
{
	glm::vec3 front = (this->m_bLockOnFloor) ? this->front_ : glm::vec3(this->front_.x, 0, this->front_.z);
	glm::vec3 right = (this->m_bLockOnFloor) ? this->right_ : glm::vec3(this->right_.x, 0, this->right_.z);
		
	float velocity = this->movement_speed_ * deltaTime;
	if (direction == FORWARD)
		this->position_ += glm::vec3(front.x, 0, front.z) * velocity;
	if (direction == BACKWARD)
		this->position_ -= glm::vec3(front.x, 0, front.z) * velocity;
	if (direction == LEFT)
		this->position_ -= glm::vec3(right.x, 0, right.z) * velocity;
	if (direction == RIGHT)
		this->position_ += glm::vec3(right.x, 0, right.z) * velocity;
	if (!this->m_bLockOnFloor)
	{
		if (direction == UP)
			this->position_ += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
		if (direction == DOWN)
			this->position_ -= glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
	}
}

void Camera::ViewMovement(GLfloat xoffset, GLfloat yoffset)
{
	xoffset *= this->mouse_sensitivity_;
	yoffset *= this->mouse_sensitivity_;

	this->yaw_ += xoffset;
	this->pitch_ += yoffset;

	if (this->pitch_ > 89.0f)
		this->pitch_ = 89.0f;
	if (this->pitch_ < -89.0f)
		this->pitch_ = -89.0f;

	this->UpdateCameraVectors();
}

glm::vec3 Camera::GetFront() const
{
	return this->front_;
}

void Camera::SetFront(glm::vec3 new_front)
{
	this->front_ = new_front;
}

GLboolean Camera::GetbLockOnFloor() const
{
	return this->m_bLockOnFloor;
}

void Camera::SetbLockOnFloor(GLboolean bLockOn)
{
	this->m_bLockOnFloor = bLockOn;
}

void Camera::UpdateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 front = glm::vec3(1.0);

	front.x = cos(glm::radians(this->yaw_)) * cos(glm::radians(this->pitch_));
	front.y = sin(glm::radians(this->pitch_));
	front.z = sin(glm::radians(this->yaw_)) * cos(glm::radians(this->pitch_));
	// this->front_ = glm::normalize(front);
	this->front_ = LinearLerp(this->front_, glm::normalize(front), 0.5);

	// also re-calculate the Right and Up vector
	this->right_ = glm::normalize(glm::cross(this->front_, this->world_up_));
	this->up_ = glm::normalize(glm::cross(this->right_, this->front_));
}

glm::vec3 LinearLerp(glm::vec3 start, glm::vec3 end, float t)
{
	return start + (end - start) * t;
}
