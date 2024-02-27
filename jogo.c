#include "raylib.h"
#include "inimigos.h"
#include "armas.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "menu.h"

int main()
{

    SetConfigFlags(FLAG_VSYNC_HINT); // isso veio no codigo q os monitores deram, n mexi
    // InitMenu();                      // inicializa o menu
    InitWindow(1280, 720, "Jogo"); // inicializa a janela do jogo
    InitAudioDevice();

    //carrega musica
    Music mainMusic = LoadMusicStream("resources/audio/CyberpunkSonata.mp3");
    mainMusic.looping = true;
    SetMusicVolume(mainMusic, 3.0f);
    PlayMusicStream(mainMusic);
    
    //texturas/assets
    //carrega textura da nave espacial/jogador
    Image naveImage = LoadImage("resources/textures/nave.png");
    Image *naveImagePtr = &naveImage;
    ImageResize(naveImagePtr, 90, 90);
    Texture2D nave = LoadTextureFromImage(*naveImagePtr);

    //carrega textura do morcego
    Image morcegoImage = LoadImage("resources/textures/morcego.png");
    Image *morcegoImagePtr = &morcegoImage;
    ImageResize(morcegoImagePtr, 60, 60);
    Texture2D morcego = LoadTextureFromImage(*morcegoImagePtr);

    //carrega textura do capetinha
    Image capetinhaImage = LoadImage("resources/textures/capetinha.png");
    Image *capetinhaImagePtr = &capetinhaImage;
    ImageResize(capetinhaImagePtr, 60, 60);
    Texture2D capetinha = LoadTextureFromImage(*capetinhaImagePtr);

    //carrega textura dos projeteis para baixo
    Image projetilImage = LoadImage("resources/textures/arma.png");
    Image *projetilImagePtr = &projetilImage;
    ImageResize(projetilImagePtr, 50, 50);
    Texture2D projetil = LoadTextureFromImage(*projetilImagePtr);

    //carrega textura dos projeteis para frente
    Image projetilFImage = LoadImage("resources/textures/armaFrente.png");
    Image *projetilFImagePtr = &projetilFImage;
    ImageResize(projetilFImagePtr, 50, 50);
    Texture2D projetilF = LoadTextureFromImage(*projetilFImagePtr);

    //carrega fundo principal do jogo
    Image backgroundImage = LoadImage("resources/textures/mainbackground.png");
    Image *backgroundImagePtr = &backgroundImage;
    ImageResize(backgroundImagePtr, 1280, 720);
    Texture2D background = LoadTextureFromImage(*backgroundImagePtr);

    float scrolling = 0.0f;
   
    // criacao dos inimigos
    Inimigo_solo capetinhas[Quant_inimigos];
    Inimigo_voa morceguinhos[Quant_inimigos];
    float velocidadeCapetinhas = 200.0f;
    float velocidadeMorceguinhos = 200.0f;
    inicializaInimigos(capetinhas, morceguinhos); // funcao para inicializar os inimigos

    // Criação dos projeteis
    Projetil projeteis[Quant_projeteis];
    Projetil projeteisFrente[Quant_projeteis];
    const float velocidadeProjeteis = 800.0f;
    inicializaProjeteis(projeteis, projeteisFrente);

    //garante que a textura vai ser adicionada a cada inimigo
    for (int i = 0; i < Quant_inimigos; i++)
    { 
        morceguinhos[i].texturaMorcego = morcego;
        capetinhas[i].texturaCapetinha = capetinha;
    }

    //garante que a textura vai ser adicionada a cada projetil
    for (int i = 0; i < Quant_projeteis; i++)
    { 
        projeteis[i].texturaProjetil = projetil;
        projeteisFrente[i].texturaProjetil = projetilF;
    }

    const int screenWidth = GetScreenWidth(); // salvando duas variáveis para tamanho da tela
    const int screenHeight = GetScreenHeight();
    int alturaSolo = 150;

    Rectangle player = {50, 50, 40, 40};   // Posição do jogador(também uma struct rectangle)
    const float velocidadePlayer = 500.0f; // Velocidade do jogador

    // Define a taxa de quadros que o jogo vai rodar
    SetTargetFPS(60);

    bool apertaEspaco = false; // Variável que auxilia o jogador a só atirar uma vez a cada espaço pressionado
    bool apertaR = false;      // Mesma coisa para atirar para frente

    // variaveis relacionadas aos inimigos
    int tempoDificuldade = 10; // a cada 10 segundos a dificuldade aumenta, alguns limites foram impostos
    float tempoCapetinhas = 0; // tempo q nasce o primeiro capetinha e a cada 1 segundo spawna capetinha(aumentando a frequencia quanto mais o jogador jogar)
    float alterarTempoCapetinha = 2;
    float tempoMorceguinhos = 4; // tempo que nasce o primeiro morceguinho, aumenta a frequencia cada 7 segundos(mas menos)

    // variavel que indica o tempo da partida
    int tempoJogo = 30;
    int contadorTempo = 1; // variavel para fazer 1 segundo passar
    char tempoTexto[10] = {0};

    char coordenadas[500] = {0};

    // variável da pontuacao
    int pontuacao = 0;
    char pontuacaoTexto[20] = {0};

    double time = GetTime();

    while (!WindowShouldClose() && !IsKeyPressed(KEY_CAPS_LOCK))
    { // loop principal(passa por ele sl quantas vezes por frame)

        /*UpdateMenu(); */
        /*DrawMenu(); 
            GameState state = GetGameState();

          switch (state) 
          {
          case MENU:
              break;
          case GAME:

              break;
          case CREDITS:

              break;
          case EXIT:
              CloseMenu();
              return 0; 
          } */
        
        UpdateMusicStream(mainMusic);

        double delta = GetTime() - time;
        time = GetTime();

        if (time > tempoDificuldade)
        { // a cada 10 segundos aumenta a velocidade dos inimigos
            tempoDificuldade += 10;
            velocidadeCapetinhas += 20;
            velocidadeMorceguinhos += 20;
            if (alterarTempoCapetinha >= 0.3)
                alterarTempoCapetinha -= 0.1; // no começo os bixos do solo spawnam a cada 2 segundos, mas diminui gradualmente até 1 a cada 0.3 segundos dps de muito tempo
        }

        if (time > contadorTempo)
        { // a cada segundo tira um segundo do tempo limite do personagem
            tempoJogo--;
            contadorTempo++;
        }

        // spawnador de inimigos chao
        if (time > tempoCapetinhas)
        {
            tempoCapetinhas += alterarTempoCapetinha;
            spawnaCapetinha(capetinhas, alturaSolo);
            
        }

        // spawnador de inimigos voadores
        if (time > tempoMorceguinhos)
        { // voadores como são mais fortes aparecem a cada 5 segundos sempre
            tempoMorceguinhos += 5;
            spawnaMorceguinhos(morceguinhos, alturaSolo);
        }

        // movimentacao dos inimigos(os dois no mesmo for)
        for (int iniMove = 0; iniMove < Quant_inimigos; iniMove++)
        { // checa qual inimigo ta ativo e move ele, alem disso deleta ele se sair da tela
            if (capetinhas[iniMove].ativo)
            {
                if (capetinhas[iniMove].solo_retan.x <= screenWidth && capetinhas[iniMove].solo_retan.x >= 0)
                    capetinhas[iniMove].solo_retan.x += capetinhas[iniMove].direcao * delta * velocidadeCapetinhas;
                else
                    capetinhas[iniMove].ativo = false;
            }

            if (morceguinhos[iniMove].ativo)
            {
                if (morceguinhos[iniMove].voa_retan.x >= 0)
                    morceguinhos[iniMove].voa_retan.x -= delta * velocidadeMorceguinhos;
                else
                    morceguinhos[iniMove].ativo = false;
            }
        }

        // Movimentação do jogador
        if (IsKeyDown(KEY_W))
            player.y -= velocidadePlayer * delta;
        if (IsKeyDown(KEY_S))
            player.y += velocidadePlayer * delta;
        if (IsKeyDown(KEY_D))
            player.x += velocidadePlayer * delta;
        if (IsKeyDown(KEY_A))
            player.x -= velocidadePlayer * delta;

        // Restrições para manter o jogador dentro da tela
        if (player.x > screenWidth - 50)
            player.x = screenWidth - 50;
        if (player.x < 25)
            player.x = 25;
        if (player.y > (screenHeight - alturaSolo - 50))
            player.y = screenHeight - alturaSolo - 50;
        if (player.y < 0)
            player.y = 0;

        // Atirar projéteis para frente
        if (IsKeyDown(KEY_R) && !apertaR)
        {

            atiraFrente(projeteisFrente, &apertaR, &player);

        }
        if (IsKeyReleased(KEY_R))
        {

            apertaR = false; // isso aq é pra vc só consguir atirar uma vez a cada aperto da tecla
        }

        // Atirar projéteis para baixo
        if (IsKeyDown(KEY_SPACE) && !apertaEspaco)
        {
            atiraBaixo(projeteis, &apertaEspaco, &player);
        }
        if (IsKeyReleased(KEY_SPACE))
        {
            apertaEspaco = false;
        }

        // Movimentação dos projéteis e checagem de colisões
        for (int n = 0; n < Quant_projeteis; n++)
        {
            if (projeteis[n].ativo)
            {
                for (int o = 0; o < Quant_inimigos; o++)
                {
                    if (CheckCollisionRecs(projeteis[n].projetil_retan, capetinhas[o].solo_retan))
                    {
                        capetinhas[o].ativo = false;
                        capetinhas[o].solo_retan.height = 0;
                        capetinhas[o].solo_retan.width = 0;
                        projeteis[n].ativo = false;
                        tempoJogo++;
                        pontuacao += 10;
                    }

                    if (CheckCollisionRecs(projeteis[n].projetil_retan, morceguinhos[o].voa_retan))
                    {
                        projeteis[n].ativo = false;
                        if (morceguinhos[o].vida == 1)
                        {
                            morceguinhos[o].ativo = false;
                            morceguinhos[o].voa_retan.height = 0;
                            morceguinhos[o].voa_retan.width = 0;
                            tempoJogo += morceguinhos[o].TempoAdicional;
                            pontuacao += 10 * morceguinhos[o].TempoAdicional;
                        }
                        else
                            morceguinhos[o].vida--;
                    }
                }

                if ((projeteis[n].projetil_retan.y <= screenHeight - alturaSolo) && projeteis[n].ativo)
                    projeteis[n].projetil_retan.y += velocidadeProjeteis * delta;
                else
                    projeteis[n].ativo = false;
            }

            if (projeteisFrente[n].ativo)
            {
                for (int p = 0; p < Quant_inimigos; p++)
                {
                    if (CheckCollisionRecs(projeteisFrente[n].projetil_retan, capetinhas[p].solo_retan))
                    {
                        capetinhas[p].ativo = false;
                        capetinhas[p].solo_retan.height = 0;
                        capetinhas[p].solo_retan.width = 0;
                        projeteisFrente[n].ativo = false;
                        tempoJogo++;
                        pontuacao += 10;
                    }

                    if (CheckCollisionRecs(projeteisFrente[n].projetil_retan, morceguinhos[p].voa_retan))
                    {
                        projeteisFrente[n].ativo = false;
                        if (morceguinhos[p].vida == 1)
                        {
                            morceguinhos[p].ativo = false;
                            morceguinhos[p].voa_retan.height = 0;
                            morceguinhos[p].voa_retan.width = 0;
                            tempoJogo += morceguinhos[p].TempoAdicional;
                            pontuacao += 10 * morceguinhos[p].TempoAdicional;
                        }
                        else
                            morceguinhos[p].vida--;
                    }
                }

                if ((projeteisFrente[n].projetil_retan.x <= screenWidth) && projeteisFrente[n].ativo)
                    projeteisFrente[n].projetil_retan.x += velocidadeProjeteis * delta;
                else
                    projeteisFrente[n].ativo = false;
            }
        }

        // aqui é a colisão do personagem com os inimigos, onde vai ser implementada a morte o reinicio etc
        for (int ini = 0; ini < Quant_inimigos; ini++)
        {

            if (capetinhas[ini].ativo)
            {
                if (CheckCollisionRecs(player, capetinhas[ini].solo_retan))
                {
                    capetinhas[ini].ativo = false;
                }
            }

            if (morceguinhos[ini].ativo)
            {
                if (CheckCollisionRecs(player, morceguinhos[ini].voa_retan))
                {
                    morceguinhos[ini].ativo = false; // aqui vai ser a morte!!
                }
            }
        }

        DrawRectangle(0, screenHeight - alturaSolo, screenWidth, alturaSolo, BLANK); // chao
        BeginDrawing();                                                                 // aqui começa a desenhar na tela
        ClearBackground(RAYWHITE);

        //tentei fazer o fundo se mexer
        scrolling -= 0.1f;
        if (scrolling <= -background.width/15) scrolling = 0;
        DrawTextureEx(background, (Vector2){ scrolling, 0 }, 0.0f, 1.0f, WHITE);
        DrawTextureEx(background, (Vector2){ background.width*2 + scrolling, 0 }, 0.0f, 1.0f, WHITE);

        DrawTexture(nave, player.x, player.y, RAYWHITE);

        desenhaMorceguinhos(morceguinhos);
        desenhaCapetinha(capetinhas);
        desenhaProjeteis(projeteis, projeteisFrente);

        sprintf(coordenadas, "player (%02.02f, %02.02f)", player.x, player.y); // isso aqui mostra as coordenadas do jogador, tiraremos futuramente
        DrawText(coordenadas, 10, 10, 20, DARKGRAY);
        sprintf(tempoTexto, "%d", tempoJogo);
        DrawText(tempoTexto, screenWidth - 80, 10, 60, DARKGRAY);
        sprintf(pontuacaoTexto, "%d", pontuacao);
        DrawText(pontuacaoTexto, screenWidth - 280, 10, 60, DARKBLUE);

        // Desenho dos objetos na tela
        DrawRectangleRec(player, BLANK); // jogador

        for (int l = 0; l < Quant_projeteis; l++)
        { // projeteis do jogador
            if (projeteis[l].ativo)
                DrawRectangleRec(projeteis[l].projetil_retan, BLANK);
            if (projeteisFrente[l].ativo)
                DrawRectangleRec(projeteisFrente[l].projetil_retan, BLANK);
        }

        for (int k = 0; k < Quant_inimigos; k++)
        {
            if (capetinhas[k].ativo)
                DrawRectangleRec(capetinhas[k].solo_retan, BLANK);
            if (morceguinhos[k].ativo)
                DrawRectangleRec(morceguinhos[k].voa_retan, BLANK);
        }

        EndDrawing(); // finaliza o que tem q desenhar
    }

    UnloadMusicStream(mainMusic);         
    CloseAudioDevice();

    UnloadTexture(nave);
    UnloadTexture(background);
    UnloadTexture(morcego);
    UnloadTexture(capetinha);

    CloseWindow(); // fecha a janela

    return 0;
}
