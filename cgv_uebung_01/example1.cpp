// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include "Angel.h"

const int NumPoints = 5000;

//----------------------------------------------------------------------------

void
init( void )
{
    //Hier wird ein Array geschaffen, in dem die Daten stehen, die später an die Grafikkarte geschickt werden.
    //Der Speicher im RAM des CPU ist damit allokiert, aber es steht noch nichts (sinnvolles) drin.
	 vec2 points[NumPoints];

    //initialisieren der Punkte mit Zufallsdaten
    for(int i = 0; i < NumPoints; i++){
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
   

    // 2. Berechne die anderen Punkte in einer Schleife und speichere sie im Array
    


    //AB HIER BITTE KEINEN EIGENEN CODE MEHR EINFÜGEN!!!
    /****************************************************************************************/    

    // Create a vertex array object
    GLuint vao;
    glGenVertexArraysAPPLE( 1, &vao );
    glBindVertexArrayAPPLE( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "shader/vshader_01.glsl", "shader/fshader_01.glsl" );
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
    glDrawArrays( GL_POINTS, 0, NumPoints );    // draw the points
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

//----------------------------------------------------------------------------

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

	


#ifndef __APPLE__
    glewInit();
#endif
    
    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}
