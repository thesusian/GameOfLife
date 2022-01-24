#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <unistd.h>

const int WindowWidth = 1000;
const int WindowHeight = 1000;

const int CellSize = 10; //only tested on 10
const int CellMargin = 0;

const int CellCountX = WindowWidth/CellSize;
const int CellCountY = WindowHeight/CellSize;

const int Speed = 1000; //max 1000000

const std::string Filename = "Setup.txt";

int GetNeighborsCount(bool ActiveCells[CellCountX][CellCountY], int x, int y);

int main() {
    
    //define window
    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight),"Game of Life",sf::Style::Titlebar);
    
    //cells arrays
    sf::RectangleShape Cell[CellCountX][CellCountY];
    bool CellActiveState[CellCountX][CellCountY];

    for (int x = 0; x < CellCountX; x++) {
        for (int y = 0; y < CellCountY; y++) {
            Cell[x][y].setSize(sf::Vector2f(CellSize - CellMargin, CellSize - CellMargin));
            Cell[x][y].setPosition(CellSize/2+x*CellSize, CellSize/2+y*CellSize);
        }
    }
    
    //read from file, example(https://www.conwaylife.com/patterns/period45glidergun.cells)
    std::ifstream readfile(Filename);
    if (readfile.is_open()) {
        int y = 0;
        std::string line;
        while (std::getline(readfile, line)) {
            for (int x = 0; x < (int)line.length(); x++) { 
                if (line[x] == 'O') { //active
                    CellActiveState[x][y] = true;
                } else if (line[x] == '.') { //deactive
                    CellActiveState[x][y] = false;
                } else if (line[x] == '!') { //comment
                    break;
                } else {
                    std::cout << "Something went wrong: \n line: " << y << std::endl;
                }
            }
            y++;
        }

    } else {
        std::cerr << "Cant open file " << Filename << std::endl;
        return 1;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        
        //start timer
        auto StartTime = std::chrono::high_resolution_clock::now();
        
        //sync the tmp and the original count
        bool CellActiveStateTmp[CellCountX][CellCountY];
        for (int x = 0; x < CellCountX; x++) {
            for (int y = 0; y < CellCountY; y++) {
                CellActiveStateTmp[x][y] = CellActiveState[x][y];
            }
        }

        //the rules (https://en.wikipedia.org/wiki/Conway's_Game_of_Life#Rules)
        for (int x = 0; x < CellCountX; x++) {
            for (int y = 0; y < CellCountY; y++) {
                int NeighborsCount = GetNeighborsCount(CellActiveStateTmp, x, y);
                
                if (NeighborsCount < 2) {
                    CellActiveState[x][y] = false;
                } else if (NeighborsCount == 3) {
                    CellActiveState[x][y] = true;
                } else if (NeighborsCount > 3) {
                    CellActiveState[x][y] = false;
                }
            }
        }

        //chnage color based on state
        for (int x = 0; x < CellCountX; x++) {
            for (int y = 0; y < CellCountY; y++) {
                if (CellActiveState[x][y]) {
                    Cell[x][y].setFillColor(sf::Color::White);
                } else {
                    Cell[x][y].setFillColor(sf::Color::Black);
                }
                window.draw(Cell[x][y]);
            }
        }    

        //print finish time
        auto FinishTime = std::chrono::high_resolution_clock::now();
        //std::cout << "Processing Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(FinishTime - StartTime).count() << " ms" << std::endl;
        (void)FinishTime;
        (void)StartTime;
        
        //display the frame
        window.display();

        usleep(1000000/Speed);
    }

    return 0;
}

int GetNeighborsCount(bool ActiveCells[CellCountX][CellCountY], int x, int y) {
    int NeighborsCount = 0;
    
    //cheking from top and going clockwise
    // 8 1 2
    // 7 * 3
    // 6 5 4
    
    if (x != 0 && y != 0 && x != CellCountX-1 && y != CellCountY-1) {
        if (ActiveCells[ x ][y+1]) {NeighborsCount++;} //1
        if (ActiveCells[x+1][y+1]) {NeighborsCount++;} //2
        if (ActiveCells[x+1][ y ]) {NeighborsCount++;} //3
        if (ActiveCells[x+1][y-1]) {NeighborsCount++;} //4
        if (ActiveCells[ x ][y-1]) {NeighborsCount++;} //5
        if (ActiveCells[x-1][y-1]) {NeighborsCount++;} //6
        if (ActiveCells[x-1][ y ]) {NeighborsCount++;} //7
        if (ActiveCells[x-1][y+1]) {NeighborsCount++;} //8
    } else if (x == 0 && y != 0 && y != CellCountY-1) {             //left
        if (ActiveCells[ x ][y+1]) {NeighborsCount++;} //1
        if (ActiveCells[x+1][y+1]) {NeighborsCount++;} //2
        if (ActiveCells[x+1][ y ]) {NeighborsCount++;} //3
        if (ActiveCells[x+1][y-1]) {NeighborsCount++;} //4
        if (ActiveCells[ x ][y-1]) {NeighborsCount++;} //5
    } else if (x != 0 && y == 0 && x != CellCountX-1) {             //top
        if (ActiveCells[x+1][ y ]) {NeighborsCount++;} //3
        if (ActiveCells[x+1][y-1]) {NeighborsCount++;} //4
        if (ActiveCells[ x ][y-1]) {NeighborsCount++;} //5
        if (ActiveCells[x-1][y-1]) {NeighborsCount++;} //6
        if (ActiveCells[x-1][ y ]) {NeighborsCount++;} //7
    } else if (y != 0 && x == CellCountX-1 && y != CellCountY-1) {  //right
        if (ActiveCells[ x ][y+1]) {NeighborsCount++;} //1
        if (ActiveCells[ x ][y-1]) {NeighborsCount++;} //5
        if (ActiveCells[x-1][y-1]) {NeighborsCount++;} //6
        if (ActiveCells[x-1][ y ]) {NeighborsCount++;} //7
        if (ActiveCells[x-1][y+1]) {NeighborsCount++;} //8
    } else if (x != 0 && x != CellCountX-1 && y == CellCountY-1) {  //bottom
        if (ActiveCells[x-1][ y ]) {NeighborsCount++;} //7
        if (ActiveCells[x-1][y+1]) {NeighborsCount++;} //8
        if (ActiveCells[ x ][y+1]) {NeighborsCount++;} //1
        if (ActiveCells[x+1][y+1]) {NeighborsCount++;} //2
        if (ActiveCells[x+1][ y ]) {NeighborsCount++;} //3
    } else if (x == 0 && y == 0) {                                  //top left
        if (ActiveCells[x+1][ y ]) {NeighborsCount++;} //3
        if (ActiveCells[x+1][y-1]) {NeighborsCount++;} //4
        if (ActiveCells[ x ][y-1]) {NeighborsCount++;} //5
    } else if (y == 0 && x == CellCountX-1) {                       //top right
        if (ActiveCells[ x ][y-1]) {NeighborsCount++;} //5
        if (ActiveCells[x-1][y-1]) {NeighborsCount++;} //6
        if (ActiveCells[x-1][ y ]) {NeighborsCount++;} //7
    } else if (x == CellCountX-1 && y == CellCountY-1) {            //bottom right
        if (ActiveCells[ x ][y+1]) {NeighborsCount++;} //1
        if (ActiveCells[x-1][ y ]) {NeighborsCount++;} //7
        if (ActiveCells[x-1][y+1]) {NeighborsCount++;} //8
    } else if (x == 0 && y == CellCountY-1) {                       //bottom left
        if (ActiveCells[ x ][y+1]) {NeighborsCount++;} //1
        if (ActiveCells[x+1][y+1]) {NeighborsCount++;} //2
        if (ActiveCells[x+1][ y ]) {NeighborsCount++;} //3
    } else { 
        std::cerr << "ERROR: Cell did't match any state: " << x << " " << y << std::endl;
    }

    return NeighborsCount;
}
