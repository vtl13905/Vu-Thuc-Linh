#include "Checker.h"


SDL_Texture* Checker::textureRedKing = nullptr, * Checker::textureRedRegular = nullptr,
    * Checker::textureGreenKing = nullptr, * Checker::textureGreenRegular = nullptr,
    * Checker::textureBlueKing = nullptr, * Checker::textureBlueRegular = nullptr,
    * Checker::textureYellowKing = nullptr, * Checker::textureYellowRegular = nullptr;





Checker::Checker(int setPosX, int setPosY, Team setTeam) :
	posX(setPosX), posY(setPosY), team(setTeam) {
}



void Checker::loadTextures(SDL_Renderer* renderer) {
    textureRedKing = TextureLoader::loadTexture("Checker Red King.bmp", renderer);
    textureRedRegular = TextureLoader::loadTexture("Checker Red Regular.bmp", renderer);

    textureGreenKing = TextureLoader::loadTexture("Checker Green King.bmp", renderer);
    textureGreenRegular = TextureLoader::loadTexture("Checker Green Regular.bmp", renderer);

    textureBlueKing = TextureLoader::loadTexture("Checker Blue King.bmp", renderer);
    textureBlueRegular = TextureLoader::loadTexture("Checker Blue Regular.bmp", renderer);

    textureYellowKing = TextureLoader::loadTexture("Checker Yellow King.bmp", renderer);
    textureYellowRegular = TextureLoader::loadTexture("Checker Yellow Regular.bmp", renderer);
}



void Checker::draw(SDL_Renderer* renderer, int squareSizePixels) {
    draw(renderer, squareSizePixels, posX, posY);
}



void Checker::drawPossibleMoves(SDL_Renderer* renderer, int squareSizePixels, std::vector<Checker>& listCheckers, bool canOnlyMove2Squares) {
    int distance = checkHowFarCanMoveInDirection(1, 1, listCheckers);
    if (distance > 0 && (canOnlyMove2Squares == false || distance == 2))
        draw(renderer, squareSizePixels, posX + distance, posY + distance, true);

    distance = checkHowFarCanMoveInDirection(1, -1, listCheckers);
    if (distance > 0 && (canOnlyMove2Squares == false || distance == 2))
        draw(renderer, squareSizePixels, posX + distance, posY - distance, true);

    distance = checkHowFarCanMoveInDirection(-1, 1, listCheckers);
    if (distance > 0 && (canOnlyMove2Squares == false || distance == 2))
        draw(renderer, squareSizePixels, posX - distance, posY + distance, true);

    distance = checkHowFarCanMoveInDirection(-1, -1, listCheckers);
    if (distance > 0 && (canOnlyMove2Squares == false || distance == 2))
        draw(renderer, squareSizePixels, posX - distance, posY - distance, true);
}



int Checker::checkHowFarCanMoveInAnyDirection(std::vector<Checker>& listCheckers) {
    return (std::max(checkHowFarCanMoveInDirection(1, 1, listCheckers),
            std::max(checkHowFarCanMoveInDirection(-1, 1, listCheckers),
            std::max(checkHowFarCanMoveInDirection(1, -1, listCheckers),
                     checkHowFarCanMoveInDirection(-1, -1, listCheckers)))));
}



int Checker::tryToMoveToPosition(int x, int y, std::vector<Checker>& listCheckers, int& indexCheckerErase, bool canOnlyMove2Squares) {

    int xDirection = ((x - posX) > 0 ? 1 : -1);
    int yDirection = ((y - posY) > 0 ? 1 : -1);

    int distance = checkHowFarCanMoveInDirection(xDirection, yDirection, listCheckers);
    if (distance > 0 && (canOnlyMove2Squares == false || distance == 2)) {
        int xMovable = posX + xDirection * distance;
        int yMovable = posY + yDirection * distance;
        if (xMovable == x && yMovable == y) {
            if (distance == 2) {
                int xRemove = posX + xDirection;
                int yRemove = posY + yDirection;

                bool checkerFound = false;
                for (int count = 0; count < listCheckers.size() && checkerFound == false; count++) {
                    Checker& checkerSelected = listCheckers[count];
                    if (checkerSelected.posX == xRemove && checkerSelected.posY == yRemove) {
                        indexCheckerErase = count;
                        checkerFound = true;
                    }
                }
            }
            posX = xMovable;
            posY = yMovable;

            switch (team) {
            case Team::red:
                if (posY == 11)
                    isAKing = true;
                break;
            case Team::green:
                if (posX == 0)
                    isAKing = true;
                break;
            case Team::blue:
                if (posY == 0)
                    isAKing = true;
                break;
            case Team::yellow:
                if (posX == 11)
                    isAKing = true;
                break;
            }

            return distance;
        }
    }

    return 0;
}



int Checker::getPosX() {
    return posX;
}


int Checker::getPosY() {
    return posY;
}



Checker::Team Checker::getTeam() {
    return team;
}



void Checker::draw(SDL_Renderer* renderer, int squareSizePixels, int x, int y, bool drawTransparent) {
    SDL_Texture* textureDrawSelected = nullptr;

    switch (team) {
    case Team::red:
        textureDrawSelected = (isAKing ? textureRedKing : textureRedRegular);
        break;
    case Team::green:
        textureDrawSelected = (isAKing ? textureGreenKing : textureGreenRegular);
        break;
    case Team::blue:
        textureDrawSelected = (isAKing ? textureBlueKing : textureBlueRegular);
        break;
    case Team::yellow:
        textureDrawSelected = (isAKing ? textureYellowKing : textureYellowRegular);
        break;
    }


    if (textureDrawSelected != nullptr) {
        if (drawTransparent)
            SDL_SetTextureAlphaMod(textureDrawSelected, 128);
        else
            SDL_SetTextureAlphaMod(textureDrawSelected, 255);

        SDL_Rect rect = {
            (x + 1) * squareSizePixels,
            (y + 1) * squareSizePixels,
            squareSizePixels,
            squareSizePixels };

        SDL_RenderCopy(renderer, textureDrawSelected, NULL, &rect);
    }
}



int Checker::checkHowFarCanMoveInDirection(int xDirection, int yDirection, std::vector<Checker>& listCheckers) {
    if (abs(xDirection) == 1 && abs(yDirection) == 1) {
        if (isAKing || (team == Team::red && yDirection > 0) || (team == Team::green && xDirection < 0) ||
            (team == Team::blue && yDirection < 0) || (team == Team::yellow && xDirection > 0)) {
            int x = posX + xDirection;
            int y = posY + yDirection;
            if (x > -1 && x < 12 && y > -1 && y < 12 &&
                (x < 3 && y < 3) == false && (x >= 9 && y < 3) == false && (x < 3 && y >= 9) == false && (x >= 9 && y >= 9) == false) {
                Checker* checkerSelected = findCheckerAtPosition(x, y, listCheckers);
                if (checkerSelected == nullptr)
                    return 1;

                else if (checkerSelected->team != team) {
                    x = posX + xDirection * 2;
                    y = posY + yDirection * 2;

                    if (x > -1 && x < 12 && y > -1 && y < 12 &&
                        (x < 3 && y < 3) == false && (x >= 9 && y < 3) == false && (x < 3 && y >= 9) == false && (x >= 9 && y >= 9) == false) {
                        checkerSelected = findCheckerAtPosition(x, y, listCheckers);
                        if (checkerSelected == nullptr)
                            return 2;
                    }
                }
            }
        }
    }

    return 0;
}



Checker* Checker::findCheckerAtPosition(int x, int y, std::vector<Checker>& listCheckers) {
    for (auto& checkerSelected : listCheckers)
        if (checkerSelected.posX == x && checkerSelected.posY == y)
            return &checkerSelected;

    return nullptr;
}
