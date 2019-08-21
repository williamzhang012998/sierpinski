#include "MyGLCanvas.h"

const float EPSILON = 1e-5;
#define PI glm::pi<float>()  //PI is now a constant for 3.14159....
#define IN_RANGE(a,b) (((a>(b-EPSILON))&&(a<(b+EPSILON)))?1:0)

MyGLCanvas::MyGLCanvas(int x, int y, int w, int h, const char *l) : Fl_Gl_Window(x, y, w, h, l) {
	mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);
	
	rotVec = glm::vec3(0.0f, 0.0f, 0.0f);
	eyePosition = glm::vec3(0.0f, 0.0f, 3.0f);

	wireframe = 0;
	fill = 1;
	smooth = 0;
    animate = 1;
	scale = 0.45f;

	subDivision = 0;
    prevSubDivision = 3;

    rgb_1 = glm::vec3(1.0, 1.0, 1.0);
    rgb_2 = glm::vec3(1.0, 1.0, 1.0);
    rgb_3 = glm::vec3(1.0, 1.0, 1.0);
    rgb_4 = glm::vec3(1.0, 1.0, 1.0);

    prgb_1 = glm::vec3(1.0, 0.0, 1.0);
    prgb_2 = glm::vec3(1.0, 1.0, 0.0);
    prgb_3 = glm::vec3(0.0, 1.0, 1.0);
    prgb_4 = glm::vec3(0.0, 1.0, 0.0);
}

MyGLCanvas::~MyGLCanvas()
{
	vertices.clear();
    vertexNormals.clear();
    vertexColors.clear();
}

void MyGLCanvas::draw() {
	if (!valid()) {  //this is called when the GL canvas is set up for the first time or when it is resized...
		printf("establishing GL context\n");

		glViewport(0, 0, w(), h());
		updateCamera(w(), h());

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		GLfloat light_pos0[] = {eyePosition.x, eyePosition.y, eyePosition.z, 0.0f};
		GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };

		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		/****************************************/
		/*          Enable z-buferring          */
		/****************************************/

		glEnable(GL_DEPTH_TEST);
		glPolygonOffset(1, 1);
		glFrontFace(GL_CCW); //make sure that the ordering is counter-clock wise
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (smooth) {
		glShadeModel(GL_SMOOTH);
	}
	else {
		glShadeModel(GL_FLAT);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//rotate object
	glRotatef(rotVec.x, 1.0, 0.0, 0.0);
	glRotatef(rotVec.y, 0.0, 1.0, 0.0);
	glRotatef(rotVec.z, 0.0, 0.0, 1.0);

	glScalef(scale, scale, scale);

	//drawAxis();
	drawScene();
}

void MyGLCanvas::drawScene()
{
	glPushMatrix();

	// Dimension
	float r = 3.0f;

	// Points in tetrahedron
	float p1 = 0.0f;
	float p2 = r;
	float p3 = p2 * glm::sqrt(2.0f) * (2.0f / 3.0f);
	float p4 = -1.0f * (p2 / 3.0f);
	float p5 = -1.0f * p2 * (glm::sqrt(2.0f) / 3.0f);
	float p6 = p2 * (glm::sqrt(2.0f) / glm::sqrt(3.0f));
  	float p7 = -1.0f * p6;

  	glm::vec3 pointOne = glm::vec3(p1, p2, p1);
  	glm::vec3 pointTwo = glm::vec3(p3, p4, p1);
  	glm::vec3 pointThree = glm::vec3(p5, p4, p6);
  	glm::vec3 pointFour = glm::vec3(p5, p4, p7);

  	// Vertex colors of the four main points of the Sierpinski tetrahedron/pyramid.

    if (subDivision != prevSubDivision
        || rgb_1 != prgb_1
        || rgb_2 != prgb_2
        || rgb_3 != prgb_3
        || rgb_4 != prgb_4)
    {
        //std::cout << "here" << std::endl;
        vertices.clear();
        vertexNormals.clear();
        vertexColors.clear();
        dividePyramid(pointOne, pointTwo, pointThree, pointFour, rgb_1, rgb_2, rgb_3, rgb_4, subDivision);
        prevSubDivision = subDivision;
        prgb_1 = rgb_1;
        prgb_2 = rgb_2;
        prgb_3 = rgb_3;
    }
  	
    if (animate == 1)
    {
        if (IN_RANGE(rotVec[0], 359.0f))
        {
            rotVec[0] = -359.0f;
        }
        else
        {
            rotVec[0] += 0.2f;
        }
        
        if (IN_RANGE(rotVec[1], 359.0f))
        {
            rotVec[1] = -359.0f;
        }
        else
        {
            rotVec[1] += 0.2f;
        }

        if (IN_RANGE(rotVec[2], 359.0f))
        {
            rotVec[2] = -359.0f;
        }
        else
        {
            rotVec[2] += 0.2f;
        }
    }

    //std::cout << subDivision << std::endl;

    int numVertices = vertices.size();
    int numNormals = vertexNormals.size();

    //std::cout << numVertices << std::endl;
    //std::cout << numNormals << std::endl;
    //std::cout << vertexColors.size() << std::endl;

	if (wireframe) {
		glDisable(GL_LIGHTING);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glColor3f(1.0f, 1.0f, 0.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (int i = 0; i < numVertices; i += 3)
        {
            glBegin(GL_TRIANGLES);
            draw_vertex(i);
            draw_vertex(i + 1);
            draw_vertex(i + 2);
            glEnd();
        }
		glEnable(GL_LIGHTING);
	}

	if (fill) {
		glEnable(GL_POLYGON_OFFSET_FILL);
		glColor3f(0.5, 0.5, 0.5);
		glPolygonMode(GL_FRONT, GL_FILL);
		//shape->draw();
        // drawing roof and bottom triangles

        for (int i = 0; i < numVertices; i += 3)
        {
            glm::vec3 c = averageColors(vertexColors[i], vertexColors[i + 1], vertexColors[i + 2]);
            glColor3f(c[0], c[1], c[2]);
            glPolygonMode(GL_FRONT, GL_FILL);

            glBegin(GL_TRIANGLES);
            draw_vertex(i);
            draw_vertex(i + 1);
            draw_vertex(i + 2);
            glEnd();
        }
	}

	glPopMatrix();
}

int MyGLCanvas::handle(int e) {
	//printf("Event was %s (%d)\n", fl_eventnames[e], e);
	switch (e) {
	case FL_KEYUP:
		printf("keyboard event: key pressed: %c\n", Fl::event_key());
		break;
	case FL_MOUSEWHEEL:
		break;
	}

	return Fl_Gl_Window::handle(e);
}

void MyGLCanvas::resize(int x, int y, int w, int h) {
	Fl_Gl_Window::resize(x, y, w, h);
	puts("resize called");
}

void MyGLCanvas::drawAxis() {
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0, 0, 0); glVertex3f(0.0, 1.0, 0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);
	glEnd();
	glEnable(GL_LIGHTING);
}

void MyGLCanvas::updateCamera(int width, int height) {
	float xy_aspect;
	xy_aspect = (float)width / (float)height;
	// Determine if we are modifying the camera(GL_PROJECITON) matrix(which is our viewing volume)
		// Otherwise we could modify the object transormations in our world with GL_MODELVIEW
	glMatrixMode(GL_PROJECTION);
	// Reset the Projection matrix to an identity matrix
	glLoadIdentity();
	gluPerspective(45.0f, xy_aspect, 0.1f, 10.0f);
	gluLookAt(eyePosition.x, eyePosition.y, eyePosition.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

glm::vec3 MyGLCanvas::midPoint(glm::vec3 p1, glm::vec3 p2)
{
	return glm::vec3((p1[0] + p2[0]) / 2.0f, (p1[1] + p2[1]) / 2.0f, (p1[2] + p2[2]) / 2.0f);
}

glm::vec3 MyGLCanvas::triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 c1, glm::vec3 c2, glm::vec3 c3)
{
	vertices.push_back(p3);
    vertices.push_back(p2);
    vertices.push_back(p1);

    // push_back the vertex colors for this triangle face.
    vertexColors.push_back(glm::vec4(c3, 1.0f));
    vertexColors.push_back(glm::vec4(c2, 1.0f));
    vertexColors.push_back(glm::vec4(c1, 1.0f));

    // Compute this triangle face's face normal for use in the lighting calculations.

    glm::vec3 triangle_side_u = glm::vec3(p2[ 0 ] - p1[ 0 ], p2[ 1 ] - p1[ 1 ], p2[ 2 ] - p1[ 2 ]);
    glm::vec3 triangle_side_v = glm::vec3(p3[ 0 ] - p1[ 0 ], p3[ 1 ] - p1[ 1 ], p3[ 2 ] - p1[ 2 ]);

    // Cross product N = U x V where U = <x1,y1,z1> and V = <x2,y2,z2>.

    // Nx = ( z1 * y2 ) - ( y1 * z2 )
    // Ny = ( x1 * z2 ) - ( z1 * x2 )
    // Nz = ( y1 * x2 ) - ( x1 * y2 )

    float face_normal_x = ( triangle_side_u[ 2 ] * triangle_side_v[ 1 ] ) - ( triangle_side_u[ 1 ] * triangle_side_v[ 2 ] );
    float face_normal_y = ( triangle_side_u[ 0 ] * triangle_side_v[ 2 ] ) - ( triangle_side_u[ 2 ] * triangle_side_v[ 0 ] );
    float face_normal_z = ( triangle_side_u[1] * triangle_side_v[ 0 ] ) - ( triangle_side_u[ 0 ] * triangle_side_v[ 1 ] );

    float length = glm::sqrt( ( face_normal_x * face_normal_x ) + ( face_normal_y * face_normal_y ) + ( face_normal_z * face_normal_z ) );

    // Normalize this face normal.

    if (!IN_RANGE(length, 0.0f))
    {
        face_normal_x = face_normal_x / length;
        face_normal_y = face_normal_y / length;
        face_normal_z = face_normal_z / length;
    }

    glm::vec3 faceNormal = glm::vec3(face_normal_x, face_normal_y, face_normal_z);

    // Use the face normal of this triangle face as the vertex normal for all of the vertex normals
    // that make up this triangle face. These vertex normals will be used in the lighting calculations.
    // Instead, to compute the vertex normals, you could average all of the face normals that are adjacent
    // to a particular vertex as the vertex normal. This would provide a smooth surface appearance.

    if (!smooth)
    {
        vertexNormals.push_back(faceNormal);
        vertexNormals.push_back(faceNormal);
        vertexNormals.push_back(faceNormal);
    }

    // Return the face normal to later compute the average of all the face normals that are adjacent to a particular vertex.
    return faceNormal;
}

void MyGLCanvas::weightedVertexNormal(glm::vec3 fn1, glm::vec3 fn2, glm::vec3 fn3)
{
    // Sum all of the face normals adjacent to this vertex component wise.
    glm::vec3 face_normal_sum = glm::vec3(fn1[ 0 ] + fn2[ 0 ] + fn3[ 0 ], fn1[ 1 ] + fn2[ 1 ] + fn3[ 1 ], fn1[ 2 ] + fn2[ 2 ] + fn3[ 2 ]);

    // Compute the average.
    glm::vec3 face_normal_average = glm::vec3(face_normal_sum[ 0 ] / 3.0, face_normal_sum[ 1 ] / 3.0, face_normal_sum[ 2 ] / 3.0 );

    // Normalize the average.
    float length = glm::sqrt( ( face_normal_average[ 0 ] * face_normal_average[ 0 ] ) + ( face_normal_average[ 1 ] * face_normal_average[ 1 ] ) + ( face_normal_average[ 2 ] * face_normal_average[ 2 ] ) );

    if (!IN_RANGE(0.0f, length))
    {
        face_normal_average[ 0 ] =  face_normal_average[ 0 ] / length;
        face_normal_average[ 1 ] =  face_normal_average[ 1 ] / length;
        face_normal_average[ 2 ] =  face_normal_average[ 2 ] / length;
    }

    // This vertex normal is the normalized average of all the face normals that are adjacent to this vertex.
    vertexNormals.push_back(face_normal_average);
}

void MyGLCanvas::pyramid(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4)
{
    glm::vec3 fN1 = triangle( p1, p2, p3, c1, c2, c3); // Front face.
    glm::vec3 fN2 = triangle( p1, p4, p2, c1, c2, c4); // Right face.
    glm::vec3 fN3 = triangle( p1, p3, p4, c1, c3, c4); // Left face.
    glm::vec3 fN4 = triangle( p2, p4, p3, c2, c3, c4); // Bottom face.

    // Compute and add the vertex normals using the face normals returned.
    // These vertex normals will be used for the lighting calculations
    // making for a smooth appearance.
    if (smooth)
    {

        // Compute in counter-clockwise order since the vertices
        // were added in counter-clockwise order.
        weightedVertexNormal( fN1, fN3, fN4 );
        weightedVertexNormal( fN1, fN4, fN2 );
        weightedVertexNormal( fN1, fN2, fN3 );

        weightedVertexNormal( fN1, fN4, fN2 );
        weightedVertexNormal( fN2, fN4, fN3 );
        weightedVertexNormal( fN1, fN2, fN3 );

        weightedVertexNormal( fN2, fN4, fN3 );
        weightedVertexNormal( fN1, fN3, fN4 );
        weightedVertexNormal( fN1, fN2, fN3 );

        weightedVertexNormal( fN1, fN3, fN4 );
        weightedVertexNormal( fN2, fN4, fN3 );
        weightedVertexNormal( fN1, fN4, fN2 );
    }
}

void MyGLCanvas::dividePyramid(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4, int count)
{
    //std::cout << "Count: " << count << std::endl;
    // If the subdivision count is greater than zero.
    if (count > 0)
    {
        // Find the midpoints to all of the edges of this pyramid/tetrahedron.
        //std::cout << "here" << std::endl;
        glm::vec3 p1p2 = midPoint( p1, p2 );
        glm::vec3 p1p3 = midPoint( p1, p3 );
        glm::vec3 p1p4 = midPoint( p1, p4 );
        glm::vec3 p2p3 = midPoint( p2, p3 );
        glm::vec3 p2p4 = midPoint( p2, p4 );
        glm::vec3 p3p4 = midPoint( p3, p4 );

        // Subdivide the vertex colors as well--similar to subdividing the edges.

        glm::vec3 c1c2 = midPoint( c1, c2 );
        glm::vec3 c1c3 = midPoint( c1, c3 );
        glm::vec3 c1c4 = midPoint( c1, c4 );
        glm::vec3 c2c3 = midPoint( c2, c3 );
        glm::vec3 c2c4 = midPoint( c2, c4 );
        glm::vec3 c3c4 = midPoint( c3, c4 );

        // Each subdivision of a tetrahedron/pyramid produces four new pyramids from the subdivided pyramid.
        // One on top and three on the bottom.

        // Four recursive calls.

        dividePyramid( p1,    p1p2, p1p3, p1p4, c1,    c1c2, c1c3, c1c4, count - 1);
        dividePyramid( p1p2, p2,    p2p3, p2p4, c1c2, c2,    c2c3, c2c4, count - 1);
        dividePyramid( p1p3, p2p3, p3,    p3p4, c1c3, c2c3, c3,    c3c4, count - 1);
        dividePyramid( p1p4, p2p4, p3p4, p4,    c1c4, c2c4, c3c4, c4,    count - 1);
    }
    else
    {
        // No more subdivision, so assemble this tetrahedron/pyramid.
        // The recursive base case.
        pyramid( p1, p2, p3, p4, c1, c2, c3, c4);
    }
}

void MyGLCanvas::normalizeNormal(glm::vec3 v)
{
    glm::vec3 tmpV = glm::normalize(v);
    glNormal3f(tmpV.x, tmpV.y, tmpV.z);
}

void MyGLCanvas::draw_vertex(int index)
{
    normalizeNormal(vertexNormals[index]);
    glVertex3f(vertices[index][0], vertices[index][1], vertices[index][2]);
}

glm::vec3 MyGLCanvas::averageColors(glm::vec4 c1, glm::vec4 c2, glm::vec4 c3)
{
    return glm::vec3((c1[0]+c2[0]+c3[0])/3.0f, (c1[1]+c2[1]+c3[1])/3.0f, (c1[2]+c2[2]+c3[2])/3.0f);
}
