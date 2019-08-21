/*  =================== File Information =================
	File Name: main.cpp
	Description:
	Author: Michael Shah

	Purpose: Driver for 3D program to load .ply models 
	Usage:	
	===================================================== */

#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/names.h>
#include <FL/gl.h>
#include <FL/glut.h>
#include <FL/glu.h>

#include "MyGLCanvas.h"

using namespace std;

class MyAppWindow;
MyAppWindow *win;

class MyAppWindow : public Fl_Window {
public:
	Fl_Button* wireButton;
	Fl_Button* fillButton;
	Fl_Button* smoothButton;

	Fl_Button* animateButton;

	Fl_Slider* subdivisionSlider;

	Fl_Slider* rotXSlider;
	Fl_Slider* rotYSlider;
	Fl_Slider* rotZSlider;
	Fl_Slider* scaleSlider;

	Fl_Slider* r1Slider;
	Fl_Slider* g1Slider;
	Fl_Slider* b1Slider;
	Fl_Slider* r2Slider;
	Fl_Slider* g2Slider;
	Fl_Slider* b2Slider;
	Fl_Slider* r3Slider;
	Fl_Slider* g3Slider;
	Fl_Slider* b3Slider;
	Fl_Slider* r4Slider;
	Fl_Slider* g4Slider;
	Fl_Slider* b4Slider;

	MyGLCanvas* canvas;

public:
	// APP WINDOW CONSTRUCTOR
	MyAppWindow(int W, int H, const char*L = 0);

	static void idleCB(void* userdata) {
		win->canvas->redraw();
		//win->rotXSlider->value(win->canvas->rotVec.x);
		//win->rotYSlider->value(win->canvas->rotVec.y);
		//win->rotZSlider->value(win->canvas->rotVec.z);
	}

private:
	// Someone changed one of the sliders
	static void toggleCB(Fl_Widget* w, void* userdata) {
		int value = ((Fl_Button*)w)->value();
		printf("value: %d\n", value);
		*((int*)userdata) = value;
	}

	static void subdivisionCB(Fl_Widget* w, void* userdata) {
		int value = ((Fl_Slider*)w)->value();
		printf("value: %d\n", value);
		*((int*)userdata) = value;
	}

	static void sliderFloatCB(Fl_Widget* w, void* userdata) {
		float value = ((Fl_Slider*)w)->value();
		printf("value: %f\n", value);
		*((float*)userdata) = value;
	}
};


MyAppWindow::MyAppWindow(int W, int H, const char*L) : Fl_Window(W, H, L) {
	begin();
	// OpenGL window

	canvas = new MyGLCanvas(10, 10, w() - 320, h() - 20);

	Fl_Pack* pack = new Fl_Pack(w()- 310, 30, 150, h(), "");
	pack->box(FL_DOWN_FRAME);
	pack->type(Fl_Pack::VERTICAL);
	pack->spacing(30);
	pack->begin();

	Fl_Pack* buttonsPack = new Fl_Pack(w() - 100, 30, 100, h(), "Render");
	buttonsPack->box(FL_DOWN_FRAME);
	buttonsPack->labelfont(1);
	buttonsPack->type(Fl_Pack::VERTICAL);
	buttonsPack->spacing(0);
	buttonsPack->begin();
		wireButton = new Fl_Check_Button(0, 0, pack->w() - 20, 20, "Wire");
		wireButton->value(canvas->wireframe);
		wireButton->callback(toggleCB, (void*)(&(canvas->wireframe)));
		smoothButton = new Fl_Check_Button(0, 0, pack->w() - 20, 20, "Smooth");
		smoothButton->value(canvas->smooth);
		smoothButton->callback(toggleCB, (void*)(&(canvas->smooth)));
		fillButton = new Fl_Check_Button(0, 0, pack->w() - 20, 20, "Fill");
		fillButton->value(canvas->fill);
		fillButton->callback(toggleCB, (void*)(&(canvas->fill)));
		animateButton = new Fl_Check_Button(0, 0, pack->w() - 20, 20, "Animate");
		animateButton->value(canvas->fill);
		animateButton->callback(toggleCB, (void*)(&(canvas->animate)));
	buttonsPack->end();

	Fl_Pack* radioPack = new Fl_Pack(w() - 100, 30, 100, h(), "Recurse Depth");
	radioPack->box(FL_DOWN_FRAME);
	radioPack->labelfont(1);
	radioPack->type(Fl_Pack::VERTICAL);
	radioPack->spacing(0);
	radioPack->begin();
		//slider for controlling number of segments in X
		Fl_Box *recurseBox = new Fl_Box(0, 0, pack->w() - 20, 20, "Subdivision Level");
		subdivisionSlider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		subdivisionSlider->align(FL_ALIGN_TOP);
		subdivisionSlider->type(FL_HOR_SLIDER);
		subdivisionSlider->bounds(0, 10);
		//subdivisionSlider->step(1);
		subdivisionSlider->value(canvas->subDivision);
		subdivisionSlider->callback(subdivisionCB, (void*)(&(canvas->subDivision)));
	radioPack->end();

	Fl_Pack* cameraPack = new Fl_Pack(w() - 100, 30, 100, h(), "Camera");
	cameraPack->box(FL_DOWN_FRAME);
	cameraPack->labelfont(1);
	cameraPack->type(Fl_Pack::VERTICAL);
	cameraPack->spacing(0);
	cameraPack->begin();
		//slider for controlling rotation
		Fl_Box *rotXTextbox = new Fl_Box(0, 0, pack->w() - 20, 20, "RotateX");
		rotXSlider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		rotXSlider->align(FL_ALIGN_TOP);
		rotXSlider->type(FL_HOR_SLIDER);
		rotXSlider->bounds(-359, 359);
		rotXSlider->step(1);
		rotXSlider->value(canvas->rotVec.x);
		rotXSlider->callback(sliderFloatCB, (void*)(&(canvas->rotVec.x)));
		Fl_Box *rotYTextbox = new Fl_Box(0, 0, pack->w() - 20, 20, "RotateY");
		rotYSlider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		rotYSlider->align(FL_ALIGN_TOP);
		rotYSlider->type(FL_HOR_SLIDER);
		rotYSlider->bounds(-359, 359);
		rotYSlider->step(1);
		rotYSlider->value(canvas->rotVec.y);
		rotYSlider->callback(sliderFloatCB, (void*)(&(canvas->rotVec.y)));
		Fl_Box *rotZTextbox = new Fl_Box(0, 0, pack->w() - 20, 20, "RotateZ");
		rotZSlider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		rotZSlider->align(FL_ALIGN_TOP);
		rotZSlider->type(FL_HOR_SLIDER);
		rotZSlider->bounds(-359, 359);
		rotZSlider->step(1);
		rotZSlider->value(canvas->rotVec.z);
		rotZSlider->callback(sliderFloatCB, (void*)(&(canvas->rotVec.z)));
		Fl_Box *scaleTextbox = new Fl_Box(0, 0, pack->w() - 20, 20, "Scale");
		scaleSlider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		scaleSlider->align(FL_ALIGN_TOP);
		scaleSlider->type(FL_HOR_SLIDER);
		scaleSlider->bounds(0.1, 2);
		scaleSlider->value(canvas->scale);
		scaleSlider->callback(sliderFloatCB, (void*)(&(canvas->scale)));
		cameraPack->end();
	pack->end();

	Fl_Pack* packCol2 = new Fl_Pack(w() - 155, 30, 150, h(), "");
	packCol2->box(FL_DOWN_FRAME);
	packCol2->type(Fl_Pack::VERTICAL);
	packCol2->spacing(30);
	packCol2->begin();

		Fl_Pack* colorPack = new Fl_Pack(w() - 100, 30, 100, h(), "RGB Values");
		colorPack->box(FL_DOWN_FRAME);
		colorPack->labelfont(1);
		colorPack->type(Fl_Pack::VERTICAL);
		colorPack->spacing(0);
		colorPack->begin();

		Fl_Box *r1Box = new Fl_Box(0, 0, pack->w() - 20, 20, "Vertex 1 R");
		r1Slider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		r1Slider->align(FL_ALIGN_TOP);
		r1Slider->type(FL_HOR_SLIDER);
		r1Slider->bounds(0.0f, 1.0f);
		subdivisionSlider->step(0.05f);
		r1Slider->value(canvas->rgb_1[0]);
		r1Slider->callback(sliderFloatCB, (void*)(&(canvas->rgb_1[0])));

		Fl_Box *g1Box = new Fl_Box(0, 0, pack->w() - 20, 20, "Vertex 1 G");
		g1Slider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		g1Slider->align(FL_ALIGN_TOP);
		g1Slider->type(FL_HOR_SLIDER);
		g1Slider->bounds(0.0f, 1.0f);
		subdivisionSlider->step(0.05f);
		g1Slider->value(canvas->rgb_1[1]);
		g1Slider->callback(sliderFloatCB, (void*)(&(canvas->rgb_1[1])));

		Fl_Box *b1Box = new Fl_Box(0, 0, pack->w() - 20, 20, "Vertex 1 B");
		b1Slider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		b1Slider->align(FL_ALIGN_TOP);
		b1Slider->type(FL_HOR_SLIDER);
		b1Slider->bounds(0.0f, 1.0f);
		subdivisionSlider->step(0.05f);
		b1Slider->value(canvas->rgb_1[2]);
		b1Slider->callback(sliderFloatCB, (void*)(&(canvas->rgb_1[2])));

		Fl_Box *r2Box = new Fl_Box(0, 0, pack->w() - 20, 20, "Vertex 2 R");
		r2Slider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		r2Slider->align(FL_ALIGN_TOP);
		r2Slider->type(FL_HOR_SLIDER);
		r2Slider->bounds(0.0f, 1.0f);
		subdivisionSlider->step(0.05f);
		r2Slider->value(canvas->rgb_2[0]);
		r2Slider->callback(sliderFloatCB, (void*)(&(canvas->rgb_2[0])));

		Fl_Box *g2Box = new Fl_Box(0, 0, pack->w() - 20, 20, "Vertex 2 G");
		g2Slider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		g2Slider->align(FL_ALIGN_TOP);
		g2Slider->type(FL_HOR_SLIDER);
		g2Slider->bounds(0.0f, 1.0f);
		subdivisionSlider->step(0.05f);
		g2Slider->value(canvas->rgb_2[1]);
		g2Slider->callback(sliderFloatCB, (void*)(&(canvas->rgb_2[1])));

		Fl_Box *b2Box = new Fl_Box(0, 0, pack->w() - 20, 20, "Vertex 2 B");
		b2Slider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		b2Slider->align(FL_ALIGN_TOP);
		b2Slider->type(FL_HOR_SLIDER);
		b2Slider->bounds(0.0f, 1.0f);
		subdivisionSlider->step(0.05f);
		b2Slider->value(canvas->rgb_2[2]);
		b2Slider->callback(sliderFloatCB, (void*)(&(canvas->rgb_2[2])));

		Fl_Box *r3Box = new Fl_Box(0, 0, pack->w() - 20, 20, "Vertex 3 R");
		r3Slider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		r3Slider->align(FL_ALIGN_TOP);
		r3Slider->type(FL_HOR_SLIDER);
		r3Slider->bounds(0.0f, 1.0f);
		subdivisionSlider->step(0.05f);
		r3Slider->value(canvas->rgb_3[0]);
		r3Slider->callback(sliderFloatCB, (void*)(&(canvas->rgb_3[0])));

		Fl_Box *g3Box = new Fl_Box(0, 0, pack->w() - 20, 20, "Vertex 3 G");
		g3Slider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		g3Slider->align(FL_ALIGN_TOP);
		g3Slider->type(FL_HOR_SLIDER);
		g3Slider->bounds(0.0f, 1.0f);
		subdivisionSlider->step(0.05f);
		g3Slider->value(canvas->rgb_3[1]);
		g3Slider->callback(sliderFloatCB, (void*)(&(canvas->rgb_3[1])));

		Fl_Box *b3Box = new Fl_Box(0, 0, pack->w() - 20, 20, "Vertex 3 B");
		b3Slider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		b3Slider->align(FL_ALIGN_TOP);
		b3Slider->type(FL_HOR_SLIDER);
		b3Slider->bounds(0.0f, 1.0f);
		subdivisionSlider->step(0.05f);
		b3Slider->value(canvas->rgb_3[2]);
		b3Slider->callback(sliderFloatCB, (void*)(&(canvas->rgb_3[2])));

		Fl_Box *r4Box = new Fl_Box(0, 0, pack->w() - 20, 20, "Vertex 4 R");
		r4Slider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		r4Slider->align(FL_ALIGN_TOP);
		r4Slider->type(FL_HOR_SLIDER);
		r4Slider->bounds(0.0f, 1.0f);
		subdivisionSlider->step(0.05f);
		r4Slider->value(canvas->rgb_4[0]);
		r4Slider->callback(sliderFloatCB, (void*)(&(canvas->rgb_4[0])));

		Fl_Box *g4Box = new Fl_Box(0, 0, pack->w() - 20, 20, "Vertex 4 G");
		g4Slider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		g4Slider->align(FL_ALIGN_TOP);
		g4Slider->type(FL_HOR_SLIDER);
		g4Slider->bounds(0.0f, 1.0f);
		subdivisionSlider->step(0.05f);
		g4Slider->value(canvas->rgb_4[1]);
		g4Slider->callback(sliderFloatCB, (void*)(&(canvas->rgb_4[1])));

		Fl_Box *b4Box = new Fl_Box(0, 0, pack->w() - 20, 20, "Vertex 4 B");
		b4Slider = new Fl_Value_Slider(0, 0, pack->w() - 20, 20, "");
		b4Slider->align(FL_ALIGN_TOP);
		b4Slider->type(FL_HOR_SLIDER);
		b4Slider->bounds(0.0f, 1.0f);
		subdivisionSlider->step(0.05f);
		b4Slider->value(canvas->rgb_4[2]);
		b4Slider->callback(sliderFloatCB, (void*)(&(canvas->rgb_4[2])));

		colorPack->end();
	packCol2->end();

	end();
}


/**************************************** main() ********************/
int main(int argc, char **argv) {
	win = new MyAppWindow(850, 600, "Pyramid Fractal");
	win->resizable(win);
	Fl::add_idle(MyAppWindow::idleCB);
	win->show();
	return(Fl::run());
}