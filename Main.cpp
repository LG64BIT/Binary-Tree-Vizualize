#include <glew.h>
#include <glfw3.h>
#include "freeglut.h"
#include <iostream>
#include <string>

static int width = 1200;
static int height = 800;
struct Node {
	std::string name = "_";
	Node* parent = nullptr;
	Node* left = nullptr;
	Node* right = nullptr;
};
void drawText(float x, float y, void* font, const unsigned char* string) {

	glRasterPos2f(x, y);
	glutBitmapString(font, string);
}
void drawTree(Node* root, float x, float y, int level) {

	if (root->left != nullptr) {
		float nx = x - (width / 2.0 - 10) / pow(2, level);
		float ny = y + 100;
		glLineWidth(3);
		glColor3ub(255, 255, 255);
		glBegin(GL_LINES);
		glVertex2f(x, y);
		glVertex2f(nx, ny);
		glEnd();
		drawTree(root->left, nx, ny, level + 1);
	}
	if (root->right != nullptr) {
		float nx = x + (width / 2.0 - 10) / pow(2, level);
		float ny = y + 100;
		glLineWidth(3);
		glColor3ub(255, 255, 255);
		glBegin(GL_LINES);
		glVertex2f(x, y);
		glVertex2f(nx, ny);
		glEnd();
		drawTree(root->right, nx, ny, level + 1);
	}
	glColor3ub(100, 150, 200);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 36; i++) {
		glVertex2f(x + 20 * cos(i * 0.17453292519943295), y + 20 * sin(i * 0.17453292519943295));
	}
	glEnd();
	glColor3ub(0, 0, 0);
	drawText(x - 4.5, y + 5, GLUT_BITMAP_9_BY_15, (const unsigned char*)root->name.data());
}
int main() {

	int argc = 1;
	int nodes;
	std::cin >> nodes;
	Node* root = new Node;
	std::cin >> root->name;
	int previousDashes = 0;
	for (int i = 0; i < nodes - 1; i++) {
		std::string temp;
		std::cin >> temp;
		// left = 0, right = 1
		bool direction = temp[0] == '\\';
		int dashes = temp.size() - 2;
		char name = temp[temp.size() - 1];
		Node* newNode = new Node;
		while (previousDashes >= dashes) {
			previousDashes--;
			root = root->parent;
		}
		previousDashes = dashes;
		newNode->name = name;
		newNode->parent = root;
		if (direction) {
			root->right = newNode;
		}
		else {
			root->left = newNode;
		}
		root = newNode;
	}
	while (root->parent != nullptr) {
		root = root->parent;
	}
	char* argv[1] = { (char*)"Something" };
	glutInit(&argc, argv);
	glewInit();
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, __FILE__, NULL, NULL);
	glfwMakeContextCurrent(window);
	glOrtho(0, width, height, 0, -1, 1);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			break;
		}
		glClear(GL_COLOR_BUFFER_BIT);
		drawTree(root, width / 2, 100, 1);
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}