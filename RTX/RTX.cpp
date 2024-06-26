#include "VKConfig.h"
#include <iostream>

VulkanClass* vk;

namespace win {
	int width = 3840;
	int height = 2160;
}

namespace camera {
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, -5.0f);
	glm::vec3 fwd = glm::vec3(0.0f, 0.0f, 1.0f);
	float angle = 0;
	float Xangle = 0;
}

Transform transform;

namespace hostSwapChain {
	uint32_t currentFrame = 0;
}

const float pi = 3.141592653589793238462643;

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_A) {
		camera::pos -= 0.1f * glm::cross(camera::fwd, glm::vec3(0.0, 1.0, 0.0));
	}
	if (key == GLFW_KEY_D) {
		camera::pos += 0.1f * glm::cross(camera::fwd, glm::vec3(0.0, 1.0, 0.0));
	}
	if (key == GLFW_KEY_Q) {
		camera::pos += glm::vec3(0.0f, 0.1f, 0.0f);
	}
	if (key == GLFW_KEY_E) {
		camera::pos += glm::vec3(0.0f, -0.1f, 0.0f);
	}
	if (key == GLFW_KEY_W) {
		//camera::pos += glm::vec3(0.0f, 0.0f, 0.1f);
		camera::pos += 0.1f * camera::fwd;
	}
	if (key == GLFW_KEY_S) {
		//camera::pos += glm::vec3(0.0f, 0.0f, -0.1f);
		camera::pos -= 0.1f * camera::fwd;
	}
	if (key == GLFW_KEY_LEFT) {
		camera::angle += 0.1f;
		camera::fwd = glm::vec3(sin(camera::angle), camera::fwd.y, cos(camera::angle));
	}
	if (key == GLFW_KEY_RIGHT) {
		camera::angle -= 0.1f;
		camera::fwd = glm::vec3(sin(camera::angle), camera::fwd.y, cos(camera::angle));
	}
	if (key == GLFW_KEY_UP) {
		camera::Xangle -= 0.1f;
		camera::fwd = glm::vec3(camera::fwd.x, sin(camera::Xangle), cos(camera::Xangle));
	}
	if (key == GLFW_KEY_DOWN) {
		camera::Xangle += 0.1f;
		camera::fwd = glm::vec3(camera::fwd.x, sin(camera::Xangle), cos(camera::Xangle));
	}
	if (key == GLFW_KEY_1) {
		transform.lightingModel = 1;
	}
	if (key == GLFW_KEY_2) {
		transform.lightingModel = 2;
	}
	if (key == GLFW_KEY_3) {
		transform.lightingModel = 3;
	}
	if (key == GLFW_KEY_4) {
		transform.lightingModel = 4;
	}
	if (key == GLFW_KEY_0) {
		transform.lightingModel = 0;
	}
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {


	vk->framebufferResized = true;

}

void display() {

	vkWaitForFences(vk->getLogicalDevice(), 1, &vk->inFlightFence[hostSwapChain::currentFrame], VK_TRUE, UINT32_MAX);

	vk->draw(hostSwapChain::currentFrame);

	hostSwapChain::currentFrame = (hostSwapChain::currentFrame + 1) % vk->getMaxFramesInFlight();

	//vk->drawGui();

}

void idle() {

	transform.M = glm::rotate(glm::mat4(1.0f), pi/2.0f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)) * glm::mat4(1.0f);
	transform.V = glm::lookAt(camera::pos, camera::pos + camera::fwd, glm::vec3(0.0f, 1.0f, 0.0f));
	transform.P = glm::perspective(glm::radians(45.0f), win::width / (float)win::height, 0.1f, 100.0f);

	transform.cameraPos = camera::pos;

	vk->transform = transform;

	vk->updateTransform();

}

int main() {

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	GLFWwindow* window = glfwCreateWindow(win::width, win::height, "RTX", 0, nullptr);

	vk = new VulkanClass(window);
	vk->createTransformBuffer(sizeof(transform));
	vk->createTransformDescriptorSet();
	vk->createSamplerDescriptorSet();

	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetWindowSizeCallback(window, windowResizeCallback);

	while (!glfwWindowShouldClose(window)) {

		idle();
		display();

		glfwPollEvents();

	}

	vkDeviceWaitIdle(vk->getLogicalDevice());

	delete vk;

	return 0;

}