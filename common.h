extern const int screenWidth;
extern const int screenHeight;

bool initiate();
bool update(float x, float y);
void terminate();

void buttonPressed(int pos, float x, float y);
void buttonReleased(int pos, float x, float y);
void wheelMoved(float d, float x, float y);

