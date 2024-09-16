#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

int topx = 40, topy = 12, e_topx, e_topy, ox = 79, oy = 12, blgx = 2, blgy = 12, ran, a, b, i, top_random, blg_puan, o_puan, top_basla;
char tus, top = 184, cizgi = 219;
int lastUpdateTime = 0; // Last time the ball was updated
int updateInterval = 50; // Interval in milliseconds

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
    topx = 40;
    topy = 12;
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
    gotoxy(26, 8); textcolor(4); printf("PPPPP  OOOO  NN    N  GGGGG");
    gotoxy(26, 9); textcolor(4); printf("P   P  O  O  N N   N  G    ");
    gotoxy(26, 10); textcolor(4); printf("PPPPP  O  O  N  N  N  G GGG");
    gotoxy(26, 11); textcolor(4); printf("P      O  O  N   N N  G   G");
    gotoxy(26, 12); textcolor(4); printf("P      OOOO  N    NN  GGGGG");
    gotoxy(26, 14); textcolor(3); printf("---------------------------");
    gotoxy(27, 15); textcolor(13); printf("Game by KARTOPU AND G™KSU");
    gotoxy(26, 16); textcolor(3); printf("---------------------------");
    gotoxy(27, 18); textcolor(14); printf("PRESS ANY KEY TO CONTINUE");
    gotoxy(80, 25); getchar(); clrscr();
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

int bilgisayar_oyna() {
    // Check if the ball is moving towards the computer paddle
    if (topx <= blgx + 1) {
        // Basic AI: Follow the ball's vertical position with a reaction time
        if (topy < blgy) {
            blgy--;  // Move up
        } else if (topy > blgy + 1) {
            blgy++;  // Move down
        }

        // Add reaction delay to avoid perfect accuracy
        if (rand() % 100 < 5) {
            // Randomly introduce slight delays in movement
            if (topy < blgy) {
                blgy--;  // Move up
            } else if (topy > blgy + 1) {
                blgy++;  // Move down
            }
        }

        // Ensure the paddle stays within bounds
        if (blgy < 2) blgy = 2;
        if (blgy > 22) blgy = 22;
    }
    return 0;
}


int oyuncu_oyna() {
    // Adjust the paddle movement speed
    int speed = 2; // Increase this value to make the paddle move faster

    if (tus == 'w') { oy -= speed; }  // Move up
    if (tus == 's') { oy += speed; }  // Move down

    // Ensure the paddle stays within bounds
    if (oy < 2) { oy = 2; }
    if (oy > 24) { oy = 24; }

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
        if (topy >= 24) { 
            if (direction == 1) direction = 3; // Change direction if moving down-left
            if (direction == 0) direction = 2; // Change direction if moving down-right
        }

        // Check for collisions with paddles (left and right)
        if (topx <= 2 && (topy == blgy || topy == blgy - 1 || topy == blgy + 1)) {
            // Ball hits the left paddle
            if (direction == 1) direction = 0; // Change direction from down-left to down-right
            if (direction == 3) direction = 2; // Change direction from up-left to up-right
        }
        if (topx >= 78 && (topy == oy || topy == oy - 1 || topy == oy + 1)) {
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
    if (topx < 2 && topy != blgy && topy != blgy - 1 && topy != blgy + 1) {
        o_puan++; basla();
    }
    if (topx > 79 && topy != oy && topy != oy - 1 && topy != oy + 1) {
        blg_puan++; basla();
    }

    gotoxy(1, 25); textcolor(14); printf("Bilgisayar: %d  Oyuncu: %d", blg_puan, o_puan);
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
            tus = record.Event.KeyEvent.uChar.AsciiChar; // Get the character
        } else {
            tus = 0;  // No key pressed
        }
    } else {
        tus = 0;  // No input available
    }
    return 0;
}

int debugger() {
    gotoxy(1, 1); printf("topx: %d topy: %d e_topx: %d e_topy: %d", topx, topy, e_topx, e_topy);
    return 0;
}

int main() {
    setConsoleSize(80, 25);
    srand((unsigned int)time(NULL));
    acilis();
    basla();

    lastUpdateTime = GetTickCount(); // Initialize the last update time

    do {
        tus_okuma();
        bilgisayar_oyna();
        oyuncu_oyna();
        top_hareket();

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
