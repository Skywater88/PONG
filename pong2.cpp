#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <math.h>

// Define AI movement speed
#define AI_SPEED 3
#define AI_REACTION_DELAY 1  // Adjust to control AI reaction speed

int isBotMode = 0;  // 0 for Player vs AI, 1 for Bot vs Bot

int topx = 48, topy = 15, e_topx, e_topy, ox = 90, oy = 15, blgx = 8, blgy = 15, ran, a, b, i, top_random, blg_puan, o_puan, top_basla;
char tus, top = 184, cizgi = 219;
int lastUpdateTime = 0; // Last time the ball was updated
int updateInterval = 50; // Interval in milliseconds
clock_t botModeTextTime = 0;

// Clear the screen
void clrscr() {
    system("cls");
}

void setConsoleSize(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = {0, 0, width - 1, height - 1};
    COORD bufferSize = {width, height};

    // Set the buffer size
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // Set the window size
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

int direction; // Global variable to store the ball's movement direction

int basla() {
    topx = 48;
    topy = 15;
    top_basla = rand() % 4; // Randomize the starting direction

    // Set the initial direction (0: sag_asagi, 1: sol_asagi, 2: sag_yukari, 3: sol_yukari)
    direction = top_basla;
    return 0;
}

// Move the cursor
void gotoxy(int x, int y) {
    COORD coord = {x - 1, y - 1};  // Windows uses 0-based, gotoxy() in old C is 1-based.
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Set text color
void textcolor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int acilis() {
    clrscr();
    gotoxy(36, 12); textcolor(4); printf("PPPPP  OOOO  NN    N  GGGGG");
    gotoxy(36, 13); textcolor(4); printf("P   P  O  O  N N   N  G    ");
    gotoxy(36, 14); textcolor(4); printf("PPPPP  O  O  N  N  N  G GGG");
    gotoxy(36, 15); textcolor(4); printf("P      O  O  N   N N  G   G");
    gotoxy(36, 16); textcolor(4); printf("P      OOOO  N    NN  GGGGG");
    gotoxy(36, 18); textcolor(3); printf("---------------------------");
    gotoxy(37, 19); textcolor(13); printf("Game by KARTOPU AND G™KSU");
    gotoxy(36, 20); textcolor(3); printf("---------------------------");
    gotoxy(37, 22); textcolor(14); printf("PRESS ANY KEY TO CONTINUE");
    gotoxy(95, 29); getchar(); clrscr();
    return 0;
}

int top_ciz() {
    gotoxy(topx, topy); textcolor(3); printf("%c", top);
    return 0;
}

int top_sil() {
    gotoxy(topx, topy); textcolor(3); printf(" ");
    return 0;
}

int oyuncu_ciz() {
    gotoxy(ox, oy - 1); textcolor(5); printf("%c", cizgi);
    gotoxy(ox, oy); textcolor(5); printf("%c", cizgi);
    gotoxy(ox, oy + 1); textcolor(5); printf("%c", cizgi);
    return 0;
}

int oyuncu_sil() {
    gotoxy(ox, oy - 1); textcolor(5); printf(" ");
    gotoxy(ox, oy); textcolor(5); printf(" ");
    gotoxy(ox, oy + 1); textcolor(5); printf(" ");
    return 0;
}

int oyuncu_ai() {
    // Predict the ball's future position
    float predictedY;
    int paddleCenter = oy;
    int ballX = topx;
    int ballY = topy;
    int ballDirection = direction;
    float ballSpeedX = (ballDirection == 0 || ballDirection == 1) ? -1.0f : 1.0f; // Reverse direction
    float ballSpeedY = (ballDirection == 0 || ballDirection == 2) ? 1.0f : -1.0f;

    // Predict future position of the ball
    while (ballX > ox && ballX <= 80) {
        ballX += (int)ballSpeedX;
        ballY += (int)ballSpeedY;

        // Bounce off the top and bottom edges
        if (ballY <= 1 || ballY >= 24) {
            ballSpeedY = -ballSpeedY;
            ballY += (int)ballSpeedY;
        }
    }

    predictedY = (float)ballY;

    // Smoothly adjust the paddle position
    if (paddleCenter < predictedY - AI_REACTION_DELAY) {
        oy += AI_SPEED;  // Move down
    } else if (paddleCenter > predictedY + AI_REACTION_DELAY) {
        oy -= AI_SPEED;  // Move up
    }

    // Ensure the paddle stays within bounds
    if (oy < 2) oy = 2;
    if (oy > 24) oy = 24;

    return 0;
}

int bilgisayar_oyna() {
    // Predict the ball's future position based on its current direction and speed
    float predictedY;
    int paddleCenter = blgy;
    int ballX = topx;
    int ballY = topy;
    int ballDirection = direction;
    float ballSpeedX = (ballDirection == 0 || ballDirection == 1) ? 1.0f : -1.0f;
    float ballSpeedY = (ballDirection == 0 || ballDirection == 2) ? 1.0f : -1.0f;

    // Predict future position of the ball
    while (ballX < blgx && ballX >= 0) {
        ballX += (int)ballSpeedX;
        ballY += (int)ballSpeedY;

        // Bounce off the top and bottom edges
        if (ballY <= 1 || ballY >= 24) {
            ballSpeedY = -ballSpeedY;
            ballY += (int)ballSpeedY;
        }
    }

    predictedY = (float)ballY;

    // Smoothly adjust the paddle position
    if (paddleCenter < predictedY - AI_REACTION_DELAY) {
        blgy += AI_SPEED;  // Move down
    } else if (paddleCenter > predictedY + AI_REACTION_DELAY) {
        blgy -= AI_SPEED;  // Move up
    }

    // Ensure the paddle stays within bounds
    if (blgy < 2) blgy = 2;
    if (blgy > 24) blgy = 24;

    return 0;
}

int oyuncu_oyna() {
    // Adjust the paddle movement speed
    int speed = 2; // Increase this value to make the paddle move faster

    if (tus == 'w') { oy -= speed; }  // Move up
    if (tus == 's') { oy += speed; }  // Move down

    // Ensure the paddle stays within bounds
    if (oy < 2) { oy = 2; }
    if (oy > 28) { oy = 28; } // Updated for new height

    return 0;
}

int bilgisayar_ciz() {
    gotoxy(blgx, blgy - 1); textcolor(12); printf("%c", cizgi);
    gotoxy(blgx, blgy); textcolor(12); printf("%c", cizgi);
    gotoxy(blgx, blgy + 1); textcolor(12); printf("%c", cizgi);
    return 0;
}

int bilgisayar_sil() {
    gotoxy(blgx, blgy - 1); textcolor(12); printf(" ");
    gotoxy(blgx, blgy); textcolor(12); printf(" ");
    gotoxy(blgx, blgy + 1); textcolor(12); printf(" ");
    return 0;
}

// Function to move the ball down and right
void sag_asagi() {
    topx++; // Move right
    topy++; // Move down
}

// Function to move the ball down and left
void sol_asagi() {
    topx--; // Move left
    topy++; // Move down
}

// Function to move the ball up and right
void sag_yukari() {
    topx++; // Move right
    topy--; // Move up
}

// Function to move the ball up and left
void sol_yukari() {
    topx--; // Move left
    topy--; // Move up
}

// Define your ball movement logic here (functions like sag_asagi(), sol_asagi(), etc.)

int top_hareket() {
    int currentTime = GetTickCount(); // Get the current time
    int elapsedTime = currentTime - lastUpdateTime; // Calculate elapsed time

    if (elapsedTime >= updateInterval) {
        // Move the ball according to the current direction
        switch (direction) {
            case 0: sag_asagi(); break;    // Moving down and right
            case 1: sol_asagi(); break;    // Moving down and left
            case 2: sag_yukari(); break;   // Moving up and right
            case 3: sol_yukari(); break;   // Moving up and left
        }

        // Update the last update time
        lastUpdateTime = currentTime;

        // Check for collisions with top/bottom walls
        if (topy <= 1) { 
            if (direction == 3) direction = 1; // Change direction if moving up-left
            if (direction == 2) direction = 0; // Change direction if moving up-right
        }
        if (topy >= 29) { // Updated for new height
            if (direction == 1) direction = 3; // Change direction if moving down-left
            if (direction == 0) direction = 2; // Change direction if moving down-right
        }

        // Check for collisions with paddles (left and right)
        if (topx <= 9 && (topy == blgy || topy == blgy - 1 || topy == blgy + 1)) {
            // Ball hits the left paddle
            if (direction == 1) direction = 0; // Change direction from down-left to down-right
            if (direction == 3) direction = 2; // Change direction from up-left to up-right
        }
        if (topx >= 88 && (topy == oy || topy == oy - 1 || topy == oy + 1)) {
            // Ball hits the right paddle
            if (direction == 0) direction = 1; // Change direction from down-right to down-left
            if (direction == 2) direction = 3; // Change direction from up-right to up-left
        }

        e_topx = topx;  // Store the previous x position
        e_topy = topy;  // Store the previous y position
    }

    return 0;
}

int puan() {
    if (topx < 8 && topy != blgy && topy != blgy - 1 && topy != blgy + 1) {
        o_puan++; basla();
    }
    if (topx > 90 && topy != oy && topy != oy - 1 && topy != oy + 1) {
        blg_puan++; basla();
    }

    gotoxy(1, 30); textcolor(14); printf("Bilgisayar: %d  Oyuncu: %d", blg_puan, o_puan);
    return 0;
}

int tus_okuma() {
    DWORD events;
    INPUT_RECORD record;
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    
    // Check if there's input available
    GetNumberOfConsoleInputEvents(hInput, &events);
    if (events > 0) {
        ReadConsoleInput(hInput, &record, 1, &events);
        if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown) {
            if (record.Event.KeyEvent.wVirtualKeyCode == VK_UP) {
                tus = 'w'; // Up arrow key pressed
            } else if (record.Event.KeyEvent.wVirtualKeyCode == VK_DOWN) {
                tus = 's'; // Down arrow key pressed
            } else {
                tus = record.Event.KeyEvent.uChar.AsciiChar; // Other keys
            }
        } else {
            tus = 0;  // No key pressed
        }
    } else {
        tus = 0;  // No input available
    }
    return 0;
}

void isBotOrPlayerText(){
    // Toggle between Player vs AI and Bot vs Bot modes with the 'B' key
    if (tus == 'B' || tus == 'b') {
        isBotMode = !isBotMode;  // Toggle bot mode on/off
        botModeTextTime = clock();  // Record the current time

        if (isBotMode) {
            gotoxy(36, 1);printf("\n                    \n");
            gotoxy(37, 1);printf("\nBot Mode Enabled!\n");
        } else {
            gotoxy(36, 1);printf("\n                    \n");
            gotoxy(36, 1);printf("\nPlayer Mode Enabled!\n");
        }
    }

    // Check if the text has been displayed for more than 2 seconds
    if (botModeTextTime != 0 && (clock() - botModeTextTime) / CLOCKS_PER_SEC > 2) {
        gotoxy(36, 1);printf("\n                    \n");  // Clear the text
        botModeTextTime = 0;  // Reset the timer
    }
}

int debugger() {
    gotoxy(1, 1); printf("topx: %d topy: %d e_topx: %d e_topy: %d", topx, topy, e_topx, e_topy);
    return 0;
}

int main() {
    setConsoleSize(96, 30);
    srand((unsigned int)time(NULL));
    acilis();
    basla();

    lastUpdateTime = GetTickCount(); // Initialize the last update time

    do {
        tus_okuma();

        isBotOrPlayerText();
        // Check if Bot vs Bot mode is enabled
        if (isBotMode) {
            oyuncu_ai();  // Player is controlled by AI
        } else {
            oyuncu_oyna();  // Player is controlled by keyboard
        }

        bilgisayar_oyna();  // Opponent AI
        top_hareket();      // Move the ball

        top_ciz();
        oyuncu_ciz();
        bilgisayar_ciz();
        puan();
        debugger();

        // Sleep to reduce CPU usage, adjust this if needed
        Sleep(10);

        top_sil();
        oyuncu_sil();
        bilgisayar_sil();

    } while (tus != 27);  // ESC to exit

    return 0;
}

