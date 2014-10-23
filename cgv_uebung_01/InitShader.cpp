
#include "Angel.h"

/*
char vSource[] = "#version 130\n in vec4 vPosition;\nvoid main() {\n	gl_Position = vPosition;\n}\n";
char fsource[] = "#version 130\n out vec4 color;\n void main() {\n   color = vec4(1.0, 0.0, 0.0, 1.0);\n}\n";
*/

namespace Angel {

// Create a NULL-terminated string by reading the provided file
static char*
readShaderSource(const char* shaderFile)
{
    //öffne die Datei
    FILE* fp = fopen(shaderFile, "r");

    //wenn das nicht klappt, hör sofort auf
    if ( fp == NULL ) { return NULL; }

    //finde heraus wie groß in bytes die datei ist
    //1. gehe zum ende der datei
    fseek(fp, 0L, SEEK_END);
    //2. frage nach der Größe
    long size = ftell(fp);

    //mache einen speicherbereich für den shadertext von ausreichender Größe
    char* buf = new char[size + 1];

    //gehe zum Anfang der datei
    fseek(fp, 0L, SEEK_SET);

    //lies das erste Zeichen
    char c;
    c = fgetc(fp);

    //initialisiere den index für den buffer
    int i = 0;

    //Solange das Ende der Datei nicht erreicht ist
    while(c != EOF)
    {
       //Zeichen abspeichern
       buf[i] = c;
       //Nächstes Zeichen lesen
       c = fgetc(fp);
       //index für den Puffer inkrementieren
       i++;
    }    

    //Am Ende den Puffer mit 0 terminieren 
    buf[i] = '\0';

    //Datei schließen
    fclose(fp);

    //printf("buf is: %s",buf);
    return buf;
}


// Create a GLSL program object from vertex and fragment shader files
GLuint
InitShader(const char* vShaderFile, const char* fShaderFile)
{
    struct Shader {
	const char*  filename;
	GLenum       type;
	GLchar*      source;
    }  shaders[2] = {
	{ vShaderFile, GL_VERTEX_SHADER, NULL },
	{ fShaderFile, GL_FRAGMENT_SHADER, NULL }
    };

    GLuint program = glCreateProgram();
    
	
    for ( int i = 0; i < 2; ++i ) {
		Shader& s = shaders[i];
		
		s.source = readShaderSource( s.filename );
		if ( shaders[i].source == NULL ) {
			std::cerr << "Failed to read " << s.filename << std::endl;
			exit( EXIT_FAILURE );
		}
		
      /*
		if(i==0) s.source = vSource;
		else s.source = fsource;
      */

		GLuint shader = glCreateShader( s.type );

		glShaderSource( shader, 1, (const GLchar**) &s.source, NULL );
		glCompileShader( shader );

		GLint  compiled;
		glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled ) {
			std::cerr << s.filename << " failed to compile:" << std::endl;
			GLint  logSize;
			glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
			char* logMsg = new char[logSize];
			glGetShaderInfoLog( shader, logSize, NULL, logMsg );
			std::cerr << logMsg << std::endl;
			delete [] logMsg;

			exit( EXIT_FAILURE );
		}

		//delete [] s.source;

		glAttachShader( program, shader );
    }

    /* link  and error check */
    glLinkProgram(program);

    GLint  linked;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if ( !linked ) {
	std::cerr << "Shader program failed to link" << std::endl;
	GLint  logSize;
	glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
	char* logMsg = new char[logSize];
	glGetProgramInfoLog( program, logSize, NULL, logMsg );
	std::cerr << logMsg << std::endl;
	delete [] logMsg;

	exit( EXIT_FAILURE );
    }

    /* use program object */
    glUseProgram(program);

    return program;
}

}  // Close namespace Angel block
