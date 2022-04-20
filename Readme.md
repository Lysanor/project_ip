## Como rodar?

## Instalação
- Linux:
    - Instalando o raylib
        - `sudo apt install build-essential git`
        - `sudo apt install cmake`
        - `sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev`
        - `git clone https://github.com/raysan5/raylib.git raylib`
        - `cd raylib/src/`
        - `make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED`
        - `sudo make install RAYLIB_LIBTYPE=SHARED`
    - Rodando o jogo
        - `git clone https://github.com/mallocaholic/exemplo-raylib.git`
        - `cd exemplo-raylib`
        - `make all` 

- Windows:
   - Prerequisitos
        - Git
    - Instalando o raylib
        - Fazer download do instalador da [biblioteca](https://raysan5.itch.io/raylib/purchase?popup=1) (baixe o do MINGW) e faça a instalação completa. 
    - Rodando o jogo
        - `git clone https://github.com/mallocaholic/exemplo-raylib.git`
        - Abra o Makefile com um editor de texto e atualize os endereços da RAYLIB_PATH e do COMPILER_PATH.
        - Digite no powershell (Shift + Botão Direito no diretório)  `mingw32-make PLATFORM=PLATFORM_DESKTOP`
        Salve, com o nome que desejar, aperte em ok e pronto! Seu jogo irá compilar!
        - Para abrir o jogo, execute o `.exe`.
