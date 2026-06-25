#include "boss.h"
#include <raylib.h>

extern infoEntidade chefao;
extern bool bossAtivo;
extern bool bossPodeReceberDano;
extern float bossVelY;
extern float bossCountdown;
extern bool bossMovendo;
extern bool bossPreparando;
extern float bossJumpCooldown;
extern float bossDamageCooldown;
extern float bossDamageInterval;
extern int bossDamageValor;

void loadBoss() {
    bossVelY = 0.0f;
    bossCountdown = 3.0f;
    bossMovendo = false;
    bossPreparando = false;
    bossPodeReceberDano = false;
    bossJumpCooldown = 0.0f;
    bossDamageCooldown = 0.0f;
}

void unloadBoss() {
    // Caso use texturas no futuro, descarregue aqui
}
