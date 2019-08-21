#pragma once

#ifndef MYGLCANVAS_H
#define MYGLCANVAS_H

#include <FL/gl.h>
#include <FL/glut.h>
#include <FL/glu.h>
#include <glm/glm.hpp>
#include <time.h>
#include <iostream>
#include <vector>

class MyGLCanvas : public Fl_Gl_Window {
public:
	glm::vec3 rotVec;
	glm::vec3 eyePosition;

	int wireframe;
	int smooth;
	int fill;
	int animate;
	float scale;

	int subDivision, prevSubDivision;

	glm::vec3 rgb_1, rgb_2, rgb_3, rgb_4;
	glm::vec3 prgb_1, prgb_2, prgb_3, prgb_4;

	MyGLCanvas(int x, int y, int w, int h, const char *l = 0);
	~MyGLCanvas();

private:
	void draw();
	void drawScene();

	void drawAxis();

	int handle(int);
	void resize(int x, int y, int w, int h);
	void updateCamera(int width, int height);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vertexNormals;
	std::vector<glm::vec4> vertexColors;

	glm::vec3 midPoint(glm::vec3 p1, glm::vec3 p2);
	glm::vec3 triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 c1, glm::vec3 c2, glm::vec3 c3);
	glm::vec3 averageColors(glm::vec4 c1, glm::vec4 c2, glm::vec4 c3);
	void weightedVertexNormal(glm::vec3 fn1, glm::vec3 fn2, glm::vec3 fn3);
	void pyramid(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4);
	void dividePyramid(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4, int count);

	void normalizeNormal(glm::vec3 v);
	void draw_vertex(int index);
};

#endif // !MYGLCANVAS_H