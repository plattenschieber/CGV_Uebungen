// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include "Angel.h"


const int NumTimesToSubdivide = 5;

/*******************************/
/* Berechne die beiden Werte */ 
const int NumTetraeder = 0;
const int NumVertices = 5000;
/*******************************/

vec2 points[5000];

int fps = 20;
float angle_param = 0.1;

float etime;
GLint time_loc;

//----------------------------------------------------------------------------


void
init( void )
{
    //Hier wird ein Array geschaffen, in dem die Daten stehen, die später an die Grafikkarte geschickt werden.
    //Der Speicher im RAM des CPU ist damit allokiert, aber es steht noch nichts (sinnvolles) drin.

    //initialisieren der Punkte mit Zufallsdaten
    for(int i = 0; i < NumVertices; i++){
       float x = (float)rand() / RAND_MAX;
       float y = (float)rand() / RAND_MAX;
       points[i] = vec2(x*2-1, y*2-1);
    }

    // Diese drei Punkte werden später die Eckpunkte des Serpienski-Dreiecks
    vec2 vertices[3] = {
        vec2( -1.0, -1.0 ), vec2( 0.0, 1.0 ), vec2( 1.0, -1.0 )
    };

    /****************************************************************************************/
    //AB HIER BITTE EIGENEN CODE EINFÜGEN!!!

    // 1. Suche dir irgendeinen Punkt als Startpunkt aus, der innerhalb Serpinski-Dreieck liegt.
    points[0] = vec2(0.0, 0.0);

    // 2. Berechne die anderen Punkte in einer Schleife und speichere sie im Array
    for(int i = 1; i < NumVertices; i++){
       int zufall = rand() % 3;
       points[i] = (points[i-1] + vertices[zufall]) / 2;
    }

    //AB HIER BITTE KEINEN EIGENEN CODE MEHR EINFÜGEN!!!
    /****************************************************************************************/    

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
    GLuint program = InitShader( "../shader/vshader_01.glsl", "../shader/fshader_01.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

//----------------------------------------------------------------------------

void
display( void )
{
  
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    glDrawArrays( GL_POINTS, 0, NumVertices );    // draw the points
    glFlush();

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
      if(angle_param > 0.01) angle_param -= 0.01;
      std::cout << "angle_param: " << angle_param << std::endl;
      break;

   case GLUT_KEY_RIGHT:
       angle_param += 0.01;
       std::cout << "angle_param: " << angle_param << std::endl;
       break;
   default:
      break;
   }
}

//----------------------------------------------------------------------------


void timer(int value){

	float angle = angle_param*DegreesToRadians; // small angle in radians

	//**************************************************************
	//Füge hier deinen code ein, der die Punkte um die z-Achse dreht und neu zur Grafikkarte hochlädt





    // Create and initialize a buffer object
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
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 512, 512 );

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out    
    // glutInitContextVersion( 3, 1 );
    // glutInitContextProfile( GLUT_CORE_PROFILE );

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
