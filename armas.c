#include "armas.h"
#include "raylib.h"

void inicializaProjeteis(Projetil *baixo, Projetil *frente){
    for (int p = 0; p < Quant_projeteis; p++) {
        baixo[p].ativo = false;// inicializa todos os projeteis como inativos e da tamanho a eles
        baixo[p].projetil_retan.height = 10;
        baixo[p].projetil_retan.width = 10;
        frente[p].ativo = false;
        frente[p].projetil_retan.height = 10;
        frente[p].projetil_retan.width = 10;
        
    }
}

void atiraFrente(Projetil *frente, bool *teclaR, Rectangle *jogador){
    int flagproje = 1;
    for (int proFrente = 0; proFrente < Quant_projeteis && flagproje; proFrente++) {
        if (!frente[proFrente].ativo) {
            frente[proFrente].ativo = true;
            frente[proFrente].projetil_retan.x = jogador->x + 25;
            frente[proFrente].projetil_retan.y = jogador->y + 20;
            *teclaR = true;
            flagproje = 0;
        }
    }
}

void atiraBaixo(Projetil *baixo, bool *teclaEspaco, Rectangle *jogador){
    int flagproje = 1;
    for (int pro = 0; pro < Quant_projeteis && flagproje; pro++) {
        if (!baixo[pro].ativo) {
            baixo[pro].ativo = true;
            baixo[pro].projetil_retan.x = jogador->x + 20;
            baixo[pro].projetil_retan.y = jogador->y + 25;
            *teclaEspaco = true;
            flagproje = 0;
        }
    }
}