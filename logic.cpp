#include "logic.h"
Game::Game(SDL_Window* window, SDL_Renderer* renderer, int boardSizePixels) :
    squareSizePixels(boardSizePixels / (12 + 2)), gameModeCurrent(GameMode::playing) {
    if (window != nullptr && renderer != nullptr) {
        Checker::loadTextures(renderer);
        textureCheckerBoard = TextureLoader::loadTexture("Checker Board.bmp", renderer);
        textureTeamRedWon = TextureLoader::loadTexture("Team Red Won Text.bmp", renderer);
        textureTeamGreenWon = TextureLoader::loadTexture("Team Green Won Text.bmp", renderer);
        textureTeamBlueWon = TextureLoader::loadTexture("Team Blue Won Text.bmp", renderer);
        textureTeamYellowWon = TextureLoader::loadTexture("Team Yellow Won Text.bmp", renderer);


        resetBoard();
        bool running = true;
        while (running) {
            processEvents(running);
            draw(renderer);
        }
        TextureLoader::deallocateTextures();
    }
}



void Game::processEvents(bool& running) {
    bool mouseDownThisFrame = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            mouseDownThisFrame = (mouseDownStatus == 0);
            if (event.button.button == SDL_BUTTON_LEFT)
                mouseDownStatus = SDL_BUTTON_LEFT;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                mouseDownStatus = SDL_BUTTON_RIGHT;
            break;
        case SDL_MOUSEBUTTONUP:
            mouseDownStatus = 0;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                running = false;
                break;

            case SDL_SCANCODE_R:
                resetBoard();
                break;
            }
        }
    }

    if (mouseDownThisFrame) {
        int mouseX = 0, mouseY = 0;
        SDL_GetMouseState(&mouseX, &mouseY);
        int squareX = mouseX / squareSizePixels - 1;
        int squareY = mouseY / squareSizePixels - 1;

        if (gameModeCurrent == GameMode::playing)
            checkCheckersWithMouseInput(squareX, squareY);
    }
}



void Game::checkCheckersWithMouseInput(int x, int y) {
    if (x > -1 && x < 12 && y > -1 && y < 12) {
        if ((indexCheckerInPlay > -1 && indexCheckerInPlay < listCheckers.size()) == false) {
            for (int count = 0; count < listCheckers.size(); count++) {
                Checker* checkerSelected = &listCheckers[count];
                if (checkerSelected->getPosX() == x && checkerSelected->getPosY() == y &&
                    checkerSelected->getTeam() == teamSelectedForGameplay) {
                    indexCheckerInPlay = count;
                }
            }
        }
        else {
            int indexCheckerErase = -1;
            int distanceMoved = listCheckers[indexCheckerInPlay].tryToMoveToPosition(x, y, listCheckers, indexCheckerErase, checkerInPlayCanOnlyMove2Squares);
            if (indexCheckerErase > -1 && indexCheckerErase < listCheckers.size()) {
                listCheckers.erase(listCheckers.begin() + indexCheckerErase);
                if (indexCheckerInPlay > indexCheckerErase)
                    indexCheckerInPlay--;
            }
            switch (distanceMoved) {
            case 0:
                indexCheckerInPlay = -1;
                if (checkerInPlayCanOnlyMove2Squares) {
                    checkerInPlayCanOnlyMove2Squares = false;
                    incrementTeamSelectedForGameplay();
                }
                break;

            case 1:
                if (checkerInPlayCanOnlyMove2Squares == false) {
                    indexCheckerInPlay = -1;
                    incrementTeamSelectedForGameplay();
                }
                break;

            case 2:
                if (indexCheckerInPlay > -1 && indexCheckerInPlay < listCheckers.size() &&
                    listCheckers[indexCheckerInPlay].checkHowFarCanMoveInAnyDirection(listCheckers) == 2) {
                    checkerInPlayCanOnlyMove2Squares = true;
                }
                else {
                    indexCheckerInPlay = -1;
                    checkerInPlayCanOnlyMove2Squares = false;
                    incrementTeamSelectedForGameplay();
                }
                break;
            }

            checkWin();
        }
    }
}



void Game::incrementTeamSelectedForGameplay() {
    for (int count = 0; count < 4; count++) {
        switch (teamSelectedForGameplay) {
        case Checker::Team::red:
            teamSelectedForGameplay = Checker::Team::green;
            break;
        case Checker::Team::green:
            teamSelectedForGameplay = Checker::Team::blue;
            break;
        case Checker::Team::blue:
            teamSelectedForGameplay = Checker::Team::yellow;
            break;
        case Checker::Team::yellow:
            teamSelectedForGameplay = Checker::Team::red;
            break;
        }
        if (teamStillHasAtLeastOneMoveLeft(teamSelectedForGameplay))
            return;
    }
}



void Game::draw(SDL_Renderer* renderer) {
    SDL_RenderClear(renderer);

    if (textureCheckerBoard != nullptr)
        SDL_RenderCopy(renderer, textureCheckerBoard, NULL, NULL);
    for (auto& checkerSelected : listCheckers)
        checkerSelected.draw(renderer, squareSizePixels);
    if (indexCheckerInPlay > -1 && indexCheckerInPlay < listCheckers.size())
        listCheckers[indexCheckerInPlay].drawPossibleMoves(renderer, squareSizePixels, listCheckers, checkerInPlayCanOnlyMove2Squares);
    SDL_Texture* textureDrawSelected = nullptr;

    switch (gameModeCurrent) {
    case GameMode::teamRedWon:      textureDrawSelected = textureTeamRedWon;    break;
    case GameMode::teamGreenWon:    textureDrawSelected = textureTeamGreenWon;  break;
    case GameMode::teamBlueWon:     textureDrawSelected = textureTeamBlueWon;   break;
    case GameMode::teamYellowWon:   textureDrawSelected = textureTeamYellowWon; break;
    }

    //Draw the texture overlay if needed.
    if (textureDrawSelected != nullptr)
        SDL_RenderCopy(renderer, textureDrawSelected, NULL, NULL);


    //Send the image to the window.
    SDL_RenderPresent(renderer);
}




void Game::resetBoard() {
    gameModeCurrent = GameMode::playing;
    listCheckers.clear();
    teamSelectedForGameplay = Checker::Team::red;
    for (int x = 0; x < 12; x++) {
        for (int y = 0; y < 12; y++) {
            if ((x + y) % 2 == 1) {
                if (x >= 3 && x < 9 && y < 3)
                    listCheckers.push_back(Checker(x, y, Checker::Team::red));
                else if (x >= 9 && y >= 3 && y < 9)
                    listCheckers.push_back(Checker(x, y, Checker::Team::green));
                else if (x >= 3 && x < 9 && y >= 9)
                    listCheckers.push_back(Checker(x, y, Checker::Team::blue));
                else if (x < 3 && y >= 3 && y < 9)
                    listCheckers.push_back(Checker(x, y, Checker::Team::yellow));
            }
        }
    }
}



void Game::checkWin() {
    int result =
        teamStillHasAtLeastOneMoveLeft(Checker::Team::red) << 3 |
        teamStillHasAtLeastOneMoveLeft(Checker::Team::green) << 2 |
        teamStillHasAtLeastOneMoveLeft(Checker::Team::blue) << 1 |
        teamStillHasAtLeastOneMoveLeft(Checker::Team::yellow) << 0;
    switch (result) {
    case (1 << 3):
        gameModeCurrent = GameMode::teamRedWon;
        break;
    case (1 << 2):
        gameModeCurrent = GameMode::teamGreenWon;
        break;
    case (1 << 1):
        gameModeCurrent = GameMode::teamBlueWon;
        break;
    case (1 << 0):
        gameModeCurrent = GameMode::teamYellowWon;
        break;
    }
}



bool Game::teamStillHasAtLeastOneMoveLeft(Checker::Team team) {
    for (auto& checkerSelected : listCheckers)
        if (checkerSelected.getTeam() == team &&
            checkerSelected.checkHowFarCanMoveInAnyDirection(listCheckers) > 0)
            return true;

    return false;
}
