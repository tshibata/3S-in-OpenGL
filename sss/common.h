namespace sss
{

extern const int screenWidth;
extern const int screenHeight;
extern const char * screenTitle;

bool initiate();
bool update(unsigned int dt);
void terminate();

void buttonPressed(int pos);
void buttonReleased(int pos);
void wheelMoved(float d);

GLuint initProgram(const GLchar * * vertSource, const GLchar * * fragSource);

unsigned char pixelLabel(float x, float y);

}

