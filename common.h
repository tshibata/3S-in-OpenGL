extern const int screenWidth;
extern const int screenHeight;

bool initiate();
bool update();
void terminate();

void buttonPressed(int pos);
void buttonReleased(int pos);
void wheelMoved(float d);

GLuint initProgram(const GLchar * * vertSource, const GLchar * * fragSource);

unsigned char pixelLabel(float x, float y);

