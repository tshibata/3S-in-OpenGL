void initMatrix(float * matrix);
void matProd(float * a, float * b, float * c);
void rotX(float * mat, float a);
void rotY(float * mat, float a);
void rotZ(float * mat, float a);
void move(float * mat, float x, float y, float z);
void proj(float * mat, float width, float height, float depth);
void xyz2abc(float * xyz, float * abc, int offset); // TBD better rename?

