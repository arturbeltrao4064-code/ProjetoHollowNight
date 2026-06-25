#include "boss.h"
#include "mapa.h"
#include "personagem.h"
#include <raylib.h>

infoEntidade chefao = { 0 };
bool bossAtivo = false;
bool bossPodeReceberDano = false;
float bossVelY = 0.0f;
float bossCountdown = 3.0f;
bool bossMovendo = false;
bool bossPreparando = false;
float bossJumpCooldown = 0.0f;
float bossDamageCooldown = 0.0f;
float bossDamageInterval = 1.2f;
int bossDamageValor = 25;
