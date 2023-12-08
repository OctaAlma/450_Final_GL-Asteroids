#include "Explosion.h"

enum EXHAUST{
    LEFT = 0,
    RIGHT = 1
};

class ExhaustFire: public Explosion
{
public:
    ExhaustFire(const std::string RESOURCE_DIR, int e);
    void setRoll(float angle) { roll = angle; }
    void step(MatrixStack M, bool wPressed);
    void draw(std::shared_ptr<MatrixStack> &P, 
    std::shared_ptr<MatrixStack> &MV, int width, int height, 
    std::shared_ptr<Texture> &alphaTex, std::shared_ptr<Program> &prog);

private:
    void sendColorBuf();
    int exhaust;
    float roll;
};