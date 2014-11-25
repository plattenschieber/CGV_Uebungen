// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include "Angel.h"


const int NumTimesToSubdivide = 5;
const int NumTetraeder = 1024;
const int NumVertices = 9 * NumTetraeder;

vec4 points[NumVertices];
int Index = 0;

int fps = 20;
float angle_param = 0.5;

float etime;
GLint time_loc;

//----------------------------------------------------------------------------


void triangle(const vec4& a, const vec4& b, const vec4& c)
{
   points[Index++] = a;
   points[Index++] = b;
   points[Index++] = c;
}

void divide_tetraeder(const vec4& a, const vec4& b, const vec4& c, const vec4& d, int count)
{
   if(count > 0)
   {
      //compute midpoints
      vec4 ab = (a + b) / 2.0;
      vec4 ac = (a + c) / 2.0;
      vec4 ad = (a + d) / 2.0;

      vec4 bc = (b + c) / 2.0;
      vec4 bd = (b + d) / 2.0;

      vec4 cd = (c + d) / 2.0;

      divide_tetraeder( a, ab, ac, ad, count -1);
      divide_tetraeder(ab,  b, bc, bd, count -1);
      divide_tetraeder(ad, bd, cd,  d, count -1);
      divide_tetraeder(ac, bc,  c, cd, count -1);        

   }
   else
   {
      triangle(a,b,c);
      triangle(b,d,c);
      triangle(d,a,c);
   }
}

void
init( void )
{
    // Specifiy the vertices for the initial tetraeder

	vec4 vertices[4] = {
        vec4(  0.0,   -1.0, -1.0,  1.0 ),
        vec4(  0.866, -1.0,  0.5,  1.0 ),
        vec4(  0.0,    1.0,  0.0,  1.0 ),
        vec4( -0.866, -1.0,  0.5,  1.0 )
    };


    divide_tetraeder(vertices[0], vertices[1], vertices[2], vertices[3], NumTimesToSubdivide);

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "../shader/vshader_03.glsl", "../shader/fshader_03.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	//Initialize the time parameter
	time_loc = glGetUniformLocation(program, "time");

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

//----------------------------------------------------------------------------

void
display( void )
{
  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // clear the window    
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );    // draw the points
	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

void specialKeys(int key, int x, int y)
{
   switch (key ) {
   case GLUT_KEY_UP:
      fps += 1;
      std::cout << "fps: " << fps << std::endl;
      break;

   case GLUT_KEY_DOWN:
      if(fps > 1) fps -= 1;
      std::cout << "fps: " << fps << std::endl;
      break;

   case GLUT_KEY_LEFT:
      angle_param -= 0.05;
      std::cout << "angle_param: " << angle_param << std::endl;
      break;

   case GLUT_KEY_RIGHT:
       angle_param += 0.05;
       std::cout << "angle_param: " << angle_param << std::endl;
       break;
   default:
      break;
   }
}

//----------------------------------------------------------------------------


void timer(int value){

	static float currentangle = 0.0f;
	currentangle += angle_param;

	float angle = currentangle*DegreesToRadians; // small angle in radians

	//**************************************************************
	//Füge hier deinen code ein, der die Punkte um die z-Achse dreht und neu zur Grafikkarte hochlädt

    //Benutze die Funktion RotateY um eine 4x4 Rotationsmatrix zu erzeugen
    mat4 rotMat = RotateY(angle);

    //Berechne die rotierten Vertices auf der CPU durch Multiplikation mit der Matrix
    for(int i = 0; i < NumVertices; i++) {
        points[i] = rotMat * points[i];
    }	
	
    //Anzahl der Sekunden seit dem letzten Aufruf
    //http://www.opengl.org/documentation/specs/glut/spec3/node70.html
    etime = 0.001 * glutGet(GLUT_ELAPSED_TIME);

    //Schicke diesen Wert zum Vertex Shader
    glUniform1f(time_loc, etime);

    //Sende alle Vertices erneut an die GPU
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

	//*******************************************************************/
	
	glutPostRedisplay();
	int delay = ceil(1000.0f / fps);
	glutTimerFunc( delay,timer,0);   
}

int
main( int argc, char **argv )
{
    
    glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	//glEnable(GL_DEPTH_TEST);
    glutInitWindowSize( 512, 512 );

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    //glutInitContextVersion( 3, 1 );
    //glutInitContextProfile( GLUT_CORE_PROFILE );
		
    glutCreateWindow( "Sierpinski Gasket" );

    glewInit();
    
    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutSpecialFunc( specialKeys );

    int delay = ceil(1000.0f / fps);
    glutTimerFunc( delay,timer,0);
    

    glutMainLoop();
    return 0;
}
