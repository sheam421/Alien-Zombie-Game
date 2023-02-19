// *********************************************************
// Course: TCP1101 PROGRAMMING FUNDAMENTALS
// Year: Trimester 1, 2022/23 (T2215)
// Lab: TT1L
// Names: MD RAKIBUL HASSAN SHEAM | VISHNU KARMEGAM
// IDs: 1211306276 | 1211103388 
// Emails: 1211306276@student.mmu.edu.my | 1211103388@student.mmu.edu.my
// Phones: +601160738949 | +601123041303
// *********************************************************




#include <iostream>
#include <cstdlib>
#include <fstream>
#include <math.h>
using namespace std;


namespace pf
{
    const int rowMX = 19;
    const int coloumnMx = 19;
    const int zCountMx = 10;

    char gameBoard[rowMX][coloumnMx];
    int aAxisX = 0, aAxisY = 0;
    int gameRow = 3;
    int gameColumn = 5;

    int zCount = 2;
    int zLife[zCountMx];
    int zAttack[zCountMx];
    int zRange[zCountMx];
    int zAxisX[zCountMx], zAxisY[zCountMx];
    int aLife = 100;
    int aAttack = 0;

    int ClearScreen()
    {
        #if defined(_WIN32)
            return std::system("cls");
        #elif defined(__linux__) || defined(__APPLE__)
            return std::system("clear");
        #endif
    }

    int Pause()
    {
        #if defined(_WIN32)
            return std::system("pause");
        #elif defined(__linux__) || defined(__APPLE__)
            return std::system(R"(read -p "Press any key to continue . . . " dummy)");
        #endif
    }


    //this function use for getting randomly board character
    char getRandomCharacter() {
        int random_number = std::rand() % 8;
        if (random_number == 0)
            return ' ';

        else if (random_number == 1)
            return '^';
        else if (random_number == 2)
            return 'v';
        else if (random_number == 3)
            return '<';
        else if (random_number == 4)
            return '>';

        else if (random_number == 5)
            return 'h';
        else if (random_number == 6)
            return 'p';
        else if (random_number == 7)
            return 'r';
        
        return ' ';

    }


    //this function use for making the board also fixed the alien position.

    void BuildGameBoard()
    {
        for (int r = 0; r < gameRow; ++r)
            for (int c = 0; c < gameColumn; ++c)
                gameBoard[r][c] = pf::getRandomCharacter();

        aAxisY = gameRow / 2;
        aAxisX = gameColumn / 2;

        gameBoard[aAxisY][aAxisX] = 'A';


        for (int i = 1; i <= zCount; i++) {
            zAxisX[i - 1] = std::rand() % gameColumn;
            zAxisY[i - 1] = std::rand() % gameRow;

            while (gameBoard[zAxisY[i - 1]][zAxisX[i - 1]] <= '0' && gameBoard[zAxisY[i - 1]][zAxisX[i - 1]] >= '9') {
                zAxisX[i - 1] = std::rand() % gameColumn;
                zAxisY[i - 1] = std::rand() % gameRow;
            }
            gameBoard[zAxisY[i - 1]][zAxisX[i - 1]] = i + '0';
        }
    }





    //this funtion use for making the tamplate of the board.....
    void displayBoard()
    {
        cout<<endl;
    
        std::cout << "\t";
        for (int c = 0; c < (gameColumn -5)/2; ++c)
        {
            cout << "    ";
        }
        
        cout << " :: ALIEN VS ZOMBIE :: " << std::endl;

        for (int r = 0; r < gameRow; ++r)
        {
            std::cout << "\t";
            for (int c = 0; c < gameColumn; ++c)
            {
                std::cout << "-+-+";
            }
            std::cout << "-+" << std::endl;


            std::cout << r + 1 << "\t";
            for (int c = 0; c < gameColumn; ++c)
            {
                std::cout << " | " << gameBoard[r][c];
            }
            std::cout << " | " << std::endl;
        }

        std::cout << "\t";
        for (int c = 0; c < gameColumn; ++c)
        {
            std::cout << "-+-+";
        }
        std::cout << "-+" << std::endl << "\t  ";


        for (int c = 0; c < gameColumn; ++c)
        {
            std::cout << c + 1;
            if (c + 1 <= 9)   std::cout << "   ";
            else if (c + 1 <= 99)   std::cout << "  ";
        }

        std::cout << std::endl << std::endl;
    }




//taking the user input and makes the board....
    void configureGameSettings() {
        cout<<endl;
        cout << "Default game settings" << std::endl
            << "---------------------" << std::endl
            << "Board Rows: " << gameRow << std::endl
            << "Board Columns: " << gameColumn << std::endl
            << "Zombie Count: " << zCount << std::endl << std::endl;

        char ch;
        std::cout << "Do you want to change settings? (y/n) -> ";
        std::cin >> ch;
        if (ch == 'y' || ch == 'Y') {
            ClearScreen();
            cout<<endl;
            std::cout << "Board settings" << std::endl
                << "--------------" << std::endl;

            gameRow = gameColumn = zCount = 0;

            while (gameRow % 2 == 0 || gameRow <= 0 || gameRow > rowMX) {
                std::cout << "Enter rows => ";
                std::cin >> gameRow;
                if (gameRow % 2 == 0)
                    std::cout << "Please enter an odd positive value" << std::endl;
            }

            while (gameColumn % 2 == 0 || gameColumn <= 0 || gameColumn > coloumnMx) {
                std::cout << "Enter column => ";
                std::cin >> gameColumn;
                if (gameColumn % 2 == 0)
                    std::cout << "Please enter an odd positive value" << std::endl;
            }

            std::cout << std::endl << "Zombie settings" << std::endl
                << "--------------" << std::endl;
            while (zCount <= 0 || zCount > zCountMx) {
                std::cout << "Enter number of Zombies => ";
                std::cin >> zCount;
                if (zCount <= 0)
                    std::cout << "Please enter a value greater than 0" << std::endl;
            }
        }

    }



//zombie life, range and attack..
    void init() {
        for (int i = 0; i < zCount; i++) {
            zLife[i] = (5 + (rand() % 20)) * 10;
            zRange[i] = 1 + (rand() % 4);
            zAttack[i] = (1 + (rand() % 2)) * 10;
        }
    }

   
//from here
//turn = 0 for alien, turn = 1 .. 9: zombies
    void showValues( int turn) {
        if (turn == 0)
            std::cout << "-> ";
        else
            std::cout << "   ";
        std::cout << "Alien :\tLife\t" << aLife << ", Attack\t" << aAttack << std::endl;

        for (int i = 0; i < zCount; i++) {
            if (turn == i + 1)
                std::cout << "-> ";
            else
                std::cout << "   ";
            std::cout << "Zombie " << i + 1 << " :\tLife\t" << zLife[i] << ", Attack\t" << zAttack[i] << ", Range\t" << zRange[i] << std::endl;
        }
    }



    int finishGame() {
        if (aLife <= 0) {
            aLife = 0;
            gameBoard[aAxisY][aAxisX] = ' ';
            pf::ClearScreen();
            pf::displayBoard();
            pf::showValues(0);
            return -1;
        }

        for (int i = 0; i < zCount; i++) {
            if (zLife[i] > 0)
                return 0;
        }
        gameBoard[aAxisY][aAxisX] = ' ';
        pf::ClearScreen();
        pf::displayBoard();
        pf::showValues(0);
        return 1;
    }


//alien behavoir......
    int configureBehaviorforAlien(char character, char& replace, int offsetX, int offsetY) {
        if (character == 'r') {
            std::cout << "Alien stumbled upn a rock" << std::endl;
            while (character == 'r')
                character = gameBoard[aAxisY + offsetY][aAxisX + offsetX] = getRandomCharacter();
            return -1;
        }
        
        else if (character >= '1' && character <= '9') {
            int zombieNumber = character - '1';
            zLife[zombieNumber] -= aAttack;
            if (zLife[zombieNumber] > 0) {
                std::cout << "Alien attacks upon zombie " << zombieNumber + 1 << std::endl
                    << "Zombie " << zombieNumber + 1 << " receives the damage of " << aAttack << std::endl
                    << "Zombie " << zombieNumber + 1 << " is still alive" << std::endl;
                return -1;
            }
            
            else {
                gameBoard[zAxisY[zombieNumber]][zAxisX[zombieNumber]] = ' ';
                zLife[zombieNumber] = 0;
                std::cout << "Alien attacks upon zombie " << zombieNumber + 1 << std::endl
                    << "Zombie " << zombieNumber + 1 << " receives the damage of " << aAttack << std::endl
                    << "Zombie " << zombieNumber + 1 << " is dead" << std:: endl;
                return 0;
            }
        }
        
        
        
        
        
        else if (character == 'h') {
            std::cout << "Alien finds a health pack" << std::endl << "Alien's life is increased by 20" << std::endl;
            aLife += 20;
            if (aLife > 100) aLife = 100;
        }
        
        
        else if (character == 'p') {
            int nearestZombie = 0;
            float minDistance = 3.40282e+038;
            for (int i = 0; i < zCount; i++) {
                float zombieDistance = sqrt(pow(zAxisX[i] - aAxisX, 2) + pow(zAxisY[i] - aAxisY, 2));
                
                //(math calculation of detarmined the zombie distance)
                
                if (zombieDistance < minDistance && zLife[i] > 0) {
                    minDistance = zombieDistance;
                    nearestZombie = i;
                }
            }

            zLife[nearestZombie] -= 10;
            if (zLife[nearestZombie] <= 0) {
                std::cout << "Alien finds a pod " << std::endl
                    << "Zombie " << nearestZombie + 1 << " receives the damage of 10" << std::endl
                    << "Zombie " << nearestZombie + 1 << " is dead" << std::endl;
                gameBoard[zAxisY[nearestZombie]][zAxisX[nearestZombie]] = ' ';
                zLife[nearestZombie] = zAxisX[nearestZombie] = zAxisY[nearestZombie] = 0;
                
            }
            else
                std::cout << "Alien finds a pod " << std::endl
                << "Zombie " << nearestZombie + 1 << " receives the damage of 10" << std::endl
                << "Zombie " << nearestZombie + 1 << " is still alive" << std::endl;
        }

        else if (character == '>') {
            std::cout << "Alien finds an arrow" << std::endl << "Alien's attack is increased by 20" << std::endl;
            aAttack += 20;
            replace = 'r';
        }
        else if (character == '<') {
            std::cout << "Alien finds an arrow" << std::endl << "Alien's attack is increased by 20" << std::endl;
            aAttack += 20;
            replace = 'l';
        }
        else if (character == '^') {
            std::cout << "Alien finds an arrow" << std::endl << "Alien's attack is increased by 20" << std::endl;
            aAttack += 20;
            replace = 'u';
        }
        else if (character == 'v') {
            std::cout << "Alien finds an arrow" << std::endl << "Alien's attack is increased by 20" << std::endl;
            aAttack += 20;
            replace = 'd';
        }
        else if (character == ' ' || character == '.') {
            std::cout << "Alien finds an empty space" << std::endl;
        }
        return 0;
    }





// here showing how the alien find the boarder....
    int moveA(char dir, char& nDir) {
        
        nDir = dir;
        int retVal = 0;
        
        if (dir == 'l') {
            if (aAxisX <= 0) {
                std::cout << "Alien hits the border" << std::endl;
                return -1;
            }
            retVal = configureBehaviorforAlien(gameBoard[aAxisY][aAxisX - 1], nDir, -1, 0);
            if (retVal != -1) {
                gameBoard[aAxisY][aAxisX] = '.';
                gameBoard[aAxisY][aAxisX - 1] = 'A';
                aAxisX--;
            }
        }
        
        
        
        else if (dir == 'r') {
            if (aAxisX >= gameColumn - 1) {
                std::cout << "Alien hits the border" << std::endl;
                return -1;
            }
            retVal = configureBehaviorforAlien(gameBoard[aAxisY][aAxisX + 1], nDir, 1, 0);
            if (retVal != -1) {
                gameBoard[aAxisY][aAxisX] = '.';
                gameBoard[aAxisY][aAxisX + 1] = 'A';
                aAxisX++;
            }

        }
        
        
        else if (dir == 'u') {
            if (aAxisY <= 0) {
                std::cout << "Alien hits the border" << std::endl;
                return -1;
            }
            retVal = configureBehaviorforAlien(gameBoard[aAxisY - 1][aAxisX], nDir, 0, -1);
            if (retVal != -1) {
                gameBoard[aAxisY][aAxisX] = '.';
                gameBoard[aAxisY - 1][aAxisX] = 'A';
                aAxisY--;
            }
        }
        else if (dir == 'd') {
            if (aAxisY >= gameRow - 1) {
                std::cout << "Alien hits the border" << std::endl;
                return -1;
            }
            retVal = configureBehaviorforAlien(gameBoard[aAxisY + 1][aAxisX], nDir, 0, 1);
            if (retVal != -1) {
                gameBoard[aAxisY][aAxisX] = '.';
                gameBoard[aAxisY + 1][aAxisX] = 'A';
                aAxisY++;
            }
        }
        return retVal;
    }



    void clearEnd(){
        aAttack = 0;
        for (int row = 0; row < gameRow; ++row)
            for (int col = 0; col < gameColumn; ++col)
                if (gameBoard[row][col] == '.')
                    gameBoard[row][col] = getRandomCharacter();
    }

    //this function using for changing arrow to movie alien in random direction..
    int AlterArrowDirection(int bRow, int bCol, char dir){
        
        if ((gameBoard[bRow - 1][bCol - 1] == '<' || gameBoard[bRow - 1][bCol - 1] == '>' || gameBoard[bRow - 1][bCol - 1] == '^' || gameBoard[bRow - 1][bCol - 1] == 'v') &&
           (dir == 'l' || dir == 'r' || dir == 'u' || dir == 'd')) {
            std::cout << "Arrow " << gameBoard[bRow - 1][bCol - 1] << " is switched to ";
            if (dir == 'u')
                gameBoard[bRow - 1][bCol - 1] = '^';
            else if (dir == 'd')
                gameBoard[bRow - 1][bCol - 1] = 'v';
            else if (dir == 'r')
                gameBoard[bRow - 1][bCol - 1] = '>';
            else if (dir == 'l')
                gameBoard[bRow - 1][bCol - 1] = '<';
            else return -1;
            std::cout << gameBoard[bRow - 1][bCol - 1] << std::endl;
            return 0;
        }
        else return -1;
    }


//zombie
    int moveZ(int zombieIndex) {
        int rDir = rand() % 4;
        if (rDir == 0 && zAxisX[zombieIndex] < gameColumn - 1 &&
            (gameBoard[zAxisY[zombieIndex]][zAxisX[zombieIndex] + 1] <= '0' || gameBoard[zAxisY[zombieIndex]][zAxisX[zombieIndex] + 1] >= '9') &&
            gameBoard[zAxisY[zombieIndex]][zAxisX[zombieIndex] + 1] != 'A')
        {
            std::cout << "Zombie " << zombieIndex + 1 << " moved right" << std::endl;
            gameBoard[zAxisY[zombieIndex]][zAxisX[zombieIndex]] = getRandomCharacter();
            zAxisX[zombieIndex]++;
            gameBoard[zAxisY[zombieIndex]][zAxisX[zombieIndex]] = zombieIndex + '1';
            return 0;
        }


        else if (rDir == 1 && zAxisX[zombieIndex] > 0 &&
            (gameBoard[zAxisY[zombieIndex]][zAxisX[zombieIndex] - 1] <= '0' || gameBoard[zAxisY[zombieIndex]][zAxisX[zombieIndex] - 1] >= '9')
            && gameBoard[zAxisY[zombieIndex]][zAxisX[zombieIndex] - 1] != 'A')
        {
            std::cout << "Zombie " << zombieIndex + 1 << " moved left" << std::endl;
            gameBoard[zAxisY[zombieIndex]][zAxisX[zombieIndex]] = getRandomCharacter();
            zAxisX[zombieIndex]--;
            gameBoard[zAxisY[zombieIndex]][zAxisX[zombieIndex]] = zombieIndex + '1';
            return 0;
        }


        else if (rDir == 2 && zAxisY[zombieIndex] > 0 &&
            (gameBoard[zAxisY[zombieIndex] - 1][zAxisX[zombieIndex]] <= '0' || gameBoard[zAxisY[zombieIndex] - 1][zAxisX[zombieIndex] + 1] >= '9') &&
            gameBoard[zAxisY[zombieIndex] - 1][zAxisX[zombieIndex]] != 'A')
        {
            std::cout << "Zombie " << zombieIndex + 1 << " moved up" << std::endl;
            gameBoard[zAxisY[zombieIndex]][zAxisX[zombieIndex]] = getRandomCharacter();
            zAxisY[zombieIndex]--;
            gameBoard[zAxisY[zombieIndex]][zAxisX[zombieIndex]] = zombieIndex + '1';
            return 0;
        }


        else if (rDir == 3 && zAxisY[zombieIndex] < gameRow - 1 &&
            (gameBoard[zAxisY[zombieIndex] + 1][zAxisX[zombieIndex]] <= '0' || gameBoard[zAxisY[zombieIndex] + 1][zAxisX[zombieIndex] + 1] >= '9') &&
            gameBoard[zAxisY[zombieIndex] + 1][zAxisX[zombieIndex]] != 'A')
        {
            std::cout << "Zombie " << zombieIndex + 1 << " moved down" << std::endl;
            gameBoard[zAxisY[zombieIndex]][zAxisX[zombieIndex]] = getRandomCharacter();
            zAxisY[zombieIndex]++;
            gameBoard[zAxisY[zombieIndex]][zAxisX[zombieIndex]] = zombieIndex + '1';
            return 0;
        }
        return -1;
    }

    bool decreaseAlienLife(int zombieNumber) {
        int startingX = zAxisX[zombieNumber]-zRange[zombieNumber]+1;
        int startingY = zAxisY[zombieNumber] - zRange[zombieNumber]+1;
        int endingX = zAxisX[zombieNumber] + zRange[zombieNumber]-1;
        int endingY = zAxisY[zombieNumber] + zRange[zombieNumber]-1;

        for (int i = startingY; i <= endingY; i++) {
            if (i >= 0 && i < gameRow)
                for (int j = startingX; j <= endingX; j++) {
                    if (i >= 0 && i < gameColumn && gameBoard[i][j] == 'A')
                        return true;
                }
        }
        return false;
    }

    void zturn(){
        for (int i = 0; i < zCount; i++) {
            if (zLife[i] > 0) {
                pf::ClearScreen();
                pf::displayBoard();
                pf::showValues(i + 1);
                while (moveZ(i) == -1);
                Pause();

                pf::ClearScreen();
                pf::displayBoard();
                pf::showValues(i + 1);
                if (decreaseAlienLife(i)) {
                    aLife -= zAttack[i];
                    std::cout << "Zombie " << i + 1 << " attacks alien" << std::endl << "Alien receives the damage of " << zAttack[i] << std:: endl;
                    if (aLife > 0)
                        std::cout << "Alien is still alive" << std::endl;
                    else {
                        std::cout << "Alien is dead" << std::endl;
                        Pause();
                        return;
                    }
                }
                else
                    std::cout << "Zombie " << i + 1 << " is unable to attack alien" << std::endl << "Alien is too far"<< std::endl;
                Pause();

                pf::ClearScreen();
                pf::displayBoard();
                pf::showValues(i + 1);
                std::cout << "Zombie " << i+1 << "'s turn ends" << std::endl;
                Pause();
            }
            
        }
    }

    void saveGame(std::string filename) {
        std::ofstream out;
        out.open(filename);
        out << gameRow << " " << gameColumn << std::endl;
        for (int row = 0; row < gameRow; ++row) {
            for (int col = 0; col < gameColumn; ++col)
                if(gameBoard[row][col]!=' ')
                    out << gameBoard[row][col] << " ";
                else
                    out << 'E' << " ";
            out << std::endl;
        }

        out << aAxisX << " " << aAxisY << " " << aLife << " " << aAttack << std::endl;
        out << zCount << std::endl;
        for (int i = 0; i < zCount; i++)
            out << zAxisX[i] << " " << zAxisY[i] << " " << zLife[i] << " " << zAttack[i] << " " << zRange[i] << std::endl;

        out.close();
    }

    void loadGame(std::string filename) {
        std::ifstream in;
        in.open(filename);
        in >> gameRow >> gameColumn ;
        for (int row = 0; row < gameRow; ++row)
            for (int col = 0; col < gameColumn; ++col) {
                in >> gameBoard[row][col];
                if (gameBoard[row][col] == 'E')
                    gameBoard[row][col] = ' ';
            }

        in >> aAxisX >> aAxisY >> aLife >> aAttack;
        in >> zCount ;
        for (int i = 0; i < zCount; i++)
            in >> zAxisX[i] >> zAxisY[i] >> zLife[i] >> zAttack[i] >> zRange[i] ;

        in.close();
    }
    
}



void gameInterface(int turn) {
    pf::ClearScreen();
    pf::displayBoard();
    pf::showValues(0);
}

int alienTurn() {
    string command;
    cout << "Command> ";
    cin >> command;
    

    if (command == "up") {
        char nextMove = 'u';
        while (pf::moveA(nextMove, nextMove) != -1) {
            pf::Pause();
            gameInterface(0);
        }
        pf::Pause();
        gameInterface(0);
        cout << "Alien's turn end. The trail is reset" << endl;
        pf::clearEnd();
        return 0;
    }


    else if (command == "down") {
        char nextMove = 'd';
        while (pf::moveA(nextMove, nextMove) != -1) {
            pf::Pause();
            gameInterface(0);
        }

        pf::Pause();
        gameInterface(0);
        cout << "Alien's turn end. The trail is reset" << endl;
        pf::clearEnd();
        return 0;
    }

    else if (command == "right") {
        char nextMove = 'r';
        while (pf::moveA(nextMove, nextMove) != -1) {
            pf::Pause();
            gameInterface(0);
        }
        pf::Pause();
        gameInterface(0);
        cout << "Alien's turn end. The trail is reset" << endl;
        pf::clearEnd();
        return 0;
    }

    else if (command == "left") {
        char nextMove = 'l';
        while (pf::moveA(nextMove, nextMove) != -1) {
            pf::Pause();
            gameInterface(0);
        }

        pf::Pause();
        gameInterface(0);
        cout << "Alien's turn end. The trail is reset" << endl;
        pf::clearEnd();
        return 0;
    }

    else if (command == "help") {
        cout << "1. up\t\t- Move up\n2. down\t\t- Move down\n3. left\t\t- Move left\n4. right\t- Move right\n5. arrow\t- Change the direction of an arrow\n6. help\t\t- Display these user commands\n7. save\t\t- Save the game\n8. load\t\t- Load a game\n9. quit\t\t- Quit the game\n";
    }

    else if (command == "arrow") {
        int row, col;
        string dir;
        cout << "Enter row => ";
        cin >> row;
        cout << "Enter Column => ";
        cin >> col;
        cout << "Enter direction => ";
        cin >> dir;
        if (pf::AlterArrowDirection(row, col, dir[0]) == -1)
            cout << "Invalid command" << endl;
    }

    else if (command == "save") {
        string filename;
        cout << "Enter filename you want to save your game to=> ";
        cin >> filename;
        pf::saveGame(filename);
    }

    else if (command == "load") {
        string filename;
        char choice;
        cout << "Do you want to save your game before loading(y/n)=>";
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            cout << "Enter filename you want to save your game to=> ";
            cin >> filename;
            pf::saveGame(filename);
        }
        cout << "Enter filename you want to load your game from=> ";
        cin >> filename;
        pf::loadGame(filename);
    }

    else if (command == "quit") {
        pf::Pause();
        pf::ClearScreen();
        pf::configureGameSettings();
        pf::init();
        pf::BuildGameBoard();
    }
    else {
        cout << "Invalid command" << endl;
    }
    pf::Pause();
    return -1;
}

void mainGame() {
    pf::configureGameSettings();
    pf::init();
    pf::BuildGameBoard();

    

    while (pf::finishGame()==0) {
        
        pf::ClearScreen();
        pf::displayBoard();
        pf::showValues(0);

        while (pf::finishGame() == 0 && alienTurn() == -1) {
            pf::ClearScreen();
            pf::displayBoard();
            pf::showValues(0);
        }
        pf::Pause();

        if(pf::finishGame() == 0)
            pf::zturn();
    }

    if (pf::finishGame() == -1)
        cout << "You Lose" << endl;
    else if(pf::finishGame() == 1)
        cout << "You Won" << endl;


}




int main()
{
    char choice = 'y';
    while (choice == 'y') {
        mainGame();
        cout << "Play again(y/n) => ";
        cin >> choice;
        if(choice=='y')
            pf::Pause();
        pf::ClearScreen();
    }
    return 0;
}
