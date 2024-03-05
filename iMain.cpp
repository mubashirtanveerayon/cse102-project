#include "iGraphics.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>

#define WIDTH 450
#define HEIGHT 633
#define MAIN_MENU_STATE 0
#define GAME_STATE 6
#define TUTORIAL_STATE 1
#define GAME_STATE 2
#define TOTAL_NUMBER_OF_WORDS 37
#define NUMBER_OF_DEMOS 4
#define WORD_LENGTH 5

int page = MAIN_MENU_STATE;

int score = 0;
int currentWordIndex = -1;

char typedWord[6], misspelledWord[6];
int remainingLives = 5;
int remainingHints = 5;
int typedWordIndex = -1;
int pointerIndex = 0;

int pointerX[5] = {114, 167, 215, 265, 315};
int characterX[5] = {121, 174, 223, 271, 319};

int typedWordColor[3];
int revealedIndices[5];

int tutorialPageIndex = 0;

int bestScore = 0;

char tutorialImages[NUMBER_OF_DEMOS][20] = {"t11.bmp", "t222.bmp", "t33.bmp", "t4.bmp"};
char words[TOTAL_NUMBER_OF_WORDS][6] = {
    "APPLE", "TABLE", "MUSIC", "DANCE", "TIGER", "CHAIR", "WATCH", "LIGHT", "PAPER", "RIVER", "SNAKE", "EAGLE", "OCEAN", "HAPPY", "SMILE", "LAUGH", "FROST", "BREAD", "PIZZA", "QUEEN", "PLANT", "FRESH", "GREEN", "PHONE", "GRAPE", "HONEY", "STONE", "SPACE", "STORM", "SWEET", "OTHER", "CRUEL", "HEAVY", "LIMIT", "CREST", "GROUP", "QUEUE"};

char interfaceImages[6][20] = {"interface5.bmp", "interface4.bmp", "interface3.bmp", "interface2.bmp", "interface1.bmp", "interface0.bmp"};

// char meaning[TOTAL_NUMBER_OF_WORDS][46] = {
//     "fruit","furniture","soundful tapestry from instruments/vocals","a particular series of graceful movements","a large wild animal from the cat family","piece of furniture for one person to sit on","look at someone/thing for a period of time","brightness/illumination allowing us to see","a thin, flat material to write or draw on","natural wide flow of fresh water to the sea","a reptile with long, thin body without legs","a large bird living on other small animals","water body covering most of the surface","feelings of pleasure, glad or satisfaction","facial expression of being glad or pleased","a cause for derision or merriment","the temperature that causes freezing","food made from flour, water, and yeast","flat and round  baked pie of Italian origin","woman eminent in rank and politics","to put or set in the ground for growth","having its original qualities unimpaired","something that is environment-friendly","device used to transfer audio, video signal","small & round green or red-colored fruit","sweet & sticky substance made by bees","concretion of earthy or mineral matter","a boundless three-dimensional extent"
// };

char similarWords[TOTAL_NUMBER_OF_WORDS][20] = {"fruit", "surface", "sound", "move", "cat", "seat", "timepiece", "glow", "sheet", "stream", "serpent", "raptor", "sea", "glad", "grin", "chuckle", "ice", "loaf", "pie", "monarch", "flora", "new", "verdant", "device", "berry", "nectar", "rock", "void", "tempest", "sugary", "different", "brutal", "weighty", "edge", "peak", "team", "line"};

const int nextBtnX = 140;
const int nextBtnY = 20;

int keyStrokeCounter = 0;
int resetTimer, colorChangerTimer, gameOverTimer, exitTimer, tutorialStartTimer;
int correct, initializedTutorial;

void copyArray(char *dest, char *src, size_t length);
void swap(char *a, char *b);
int getRandomIndex(int max);
void shuffleArray(char *str);
void showText(void);
void renderHUD(void);
void evaluate(void);
void reset(void);
void gameOver(void);
void callReset(void);

void showMainMenu(void);
void showGameInterface(void);

void iDraw()
{
    // place your drawing codes here
    iClear();
    if (page == MAIN_MENU_STATE)
    {
        showMainMenu();
    }
    else
    {
        showGameInterface();
    }
}

/*
    function iMouseMove() is called when the user presses and drags the mouse.
    (mx, my) is the position where the mouse pointer is.
    */
void iMouseMove(int mx, int my)
{
}

/*
    function iMouse() is called when the user presses/releases the mouse.
    (mx, my) is the position where the mouse pointer is.
    */
void iMouse(int button, int state, int mx, int my)
{

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {

        printf("x: %d,y: %d\n", mx, my);

        if (page == MAIN_MENU_STATE)
        {
            if (mx >= 107 && mx <= 347 && my <= 276 && my >= 223)
            { // start button
                page = GAME_STATE;
                PlaySound("click.wav", NULL, SND_ASYNC);
            }
            else if (mx >= 104 && mx <= 345 && my <= 191 && my >= 141)
            { // tutorial button
                page = GAME_STATE;
                initializedTutorial = 1;
                PlaySound("click.wav", NULL, SND_ASYNC);
            }
            else if (mx >= 108 && mx <= 345 && my <= 110 && my >= 61)
            { // exit button
                PlaySound("click.wav", NULL, SND_ASYNC);
                exit(0);
            }
        }
    }
}

/*
    function iKeyboard() is called whenever the user hits a key in keyboard.
    key- holds the ASCII value of the key pressed.
    */
void iKeyboard(unsigned char key)
{
    if (page == MAIN_MENU_STATE || remainingLives == 0 || keyStrokeCounter == 5)
        return;

    if (key >= '1' && key <= '5')
    {

        if (remainingHints == 0)
        {
            PlaySound("blipSelect(1).wav", NULL, SND_ASYNC);
            // return;
        }
        else
        {

            int indexToReveal = key - 49;

            if (typedWord[indexToReveal] != '\0')
                return;

            remainingHints -= 1;
            keyStrokeCounter += 1;

            typedWord[indexToReveal] = words[currentWordIndex][indexToReveal];

            revealedIndices[indexToReveal] = 1;

            for (int i = 0; i < 5; i++)
            {
                if (typedWord[i] == '\0')
                {
                    pointerIndex = i;
                    break;
                }
            }

            if (keyStrokeCounter == 5)
                evaluate();
        }
    }

    else if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z') && typedWordIndex < 4)
    {

        // to upper case
        if (key >= 'a' && key <= 'z')
            key -= 32;

        while (typedWord[++typedWordIndex] != '\0')
        {
            continue;
        }

        typedWord[typedWordIndex] = key;

        keyStrokeCounter++;

        for (int i = 0; i < 5; i++)
        {
            if (typedWord[i] == '\0')
            {
                pointerIndex = i;
                break;
            }
        }
    }

    if (keyStrokeCounter == 5)
        evaluate();

    // place your codes for other keys here
}

/*
    function iSpecialKeyboard() is called whenver user hits special keys like-
    function keys, home, end, pg up, pg down, arraows etc. you have to use
    appropriate constants to detect them. A list is:
    GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
    GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
    GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
    GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
    */
void iSpecialKeyboard(unsigned char key)
{

    if (key == GLUT_KEY_END)
    {
        exit(0);
    }
    // place your codes for other keys here
}

void copyArray(char *dest, char *src, size_t length)
{
    for (int i = 0; i < length; i++)
    {
        dest[i] = src[i];
    }
}

void swap(char *a, char *b)
{
    char temp = *a;
    *a = *b;
    *b = temp;
}

int getRandomIndex(int max)
{
    return rand() % max;
}

void shuffleArray(char *str)
{

    int size = WORD_LENGTH;
    int shuffled = 0;
    do
    {
        for (int i = 0; i < size / 2; i++)
        {
            int randomIndex = getRandomIndex(size);
            swap(&str[i], &str[randomIndex]);
        }

        for(int i=0;i<size && !shuffled;i++){
            shuffled = str[i] != words[currentWordIndex][i];
        }
    } while (!shuffled);
}

void evaluate()
{
    correct = true;
    for (int i = 0; i < 5 && correct; i++)
    {
        correct = correct && typedWord[i] == words[currentWordIndex][i];
    }

    score += correct * 10;

    remainingLives -= (1 - correct);
    if (correct)
    {
        typedWordColor[0] = 0;
        typedWordColor[1] = 255;
        typedWordColor[2] = 0;
        PlaySound("pickupCoin.wav", NULL, SND_ASYNC);
    }
    else
    {
        typedWordColor[0] = 255;
        typedWordColor[1] = 0;
        typedWordColor[2] = 0;
        // printf("here");
        PlaySound("blipSelect(1).wav", NULL, SND_ASYNC);
    }

    if (remainingLives == 0)
    {
        iResumeTimer(gameOverTimer);
    }
    else
    {
        iResumeTimer(resetTimer);
    }
    if (initializedTutorial)
    {
        if (tutorialPageIndex + 1 == NUMBER_OF_DEMOS)
        {
            initializedTutorial = 0;
        }
        else
        {
            tutorialPageIndex++;
        }
    }
}

void reset()
{

    // currentWordIndex = getRandomIndex(TOTAL_NUMBER_OF_WORDS);
    int newWord;
    do
    {
        newWord = getRandomIndex(TOTAL_NUMBER_OF_WORDS);
    } while (newWord == currentWordIndex);
    currentWordIndex = newWord;

    for (int i = 0; i < 5; i++)
    {
        typedWord[i] = '\0';
        revealedIndices[i] = 0;
    }

    typedWordColor[0] = 255;
    typedWordColor[1] = 255;
    typedWordColor[2] = 255;

    typedWordIndex = -1;

    copyArray(misspelledWord, words[currentWordIndex], 6);

    shuffleArray(misspelledWord);

    keyStrokeCounter = 0;
    pointerIndex = 0;

    iPauseTimer(resetTimer);
}

void callReset()
{
    reset();
}

void gameOver()
{

    remainingLives = 5;
    remainingHints = 5;
    reset();
    iPauseTimer(gameOverTimer);
    page = MAIN_MENU_STATE;

    if (score > bestScore)
    {
        bestScore = score;
        // saving best score
        FILE *file = fopen("bs.txt", "w");
        fprintf(file, "%d", bestScore);
        fclose(file);
    }
    score = 0;
}

void showText()
{

    iSetColor(255, 255, 255);

    char scoreStr[10];
    sprintf(scoreStr, "%d", score);
    iText(110, 575, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);

    // char hintsStr[2];

    // sprintf(hintsStr,"%d",remainingHints);

    // iText(365,500,hintsStr,GLUT_BITMAP_TIMES_ROMAN_24);

    // best score
    char bestScoreStr[8];
    sprintf(bestScoreStr, "%d", bestScore);
    iSetColor(100, 255, 89);
    iText(110, 500, bestScoreStr, GLUT_BITMAP_TIMES_ROMAN_24);

    if (currentWordIndex < 0)
        return;
    iSetColor(255, 255, 255);
    iText(90, 420, "Misspelled Word: ", GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(255, 149, 28);
    iText(280, 420, misspelledWord, GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(255, 255, 255);
    iText(170, 370, "Hint :", GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(255, 231, 27);
    iText(245, 370, similarWords[currentWordIndex], GLUT_BITMAP_TIMES_ROMAN_24);

    iSetColor(typedWordColor[0], typedWordColor[1], typedWordColor[2]);

    for (int i = 0; i < 5; i++)
    {
        if (typedWord[i] == '\0')
            continue;
        char alphabet[2] = {typedWord[i]};
        if (revealedIndices[i])
        {
            iSetColor(255, 231, 27);
        }
        else
        {
            iSetColor(typedWordColor[0], typedWordColor[1], typedWordColor[2]);
        }
        iText(characterX[i], 220, alphabet, GLUT_BITMAP_TIMES_ROMAN_24);
    }

    if (remainingLives == 0)
    {
        iShowBMP(0, 0, "gameOver2.bmp");
    }
}

void renderHUD()
{

    for (int i = 0; i < remainingLives; i++)
    {
        iShowBMP2(390 - i * 35, 570, "heart1.bmp", 0);
    }

    iShowBMP2(pointerX[pointerIndex], 150, "arrow.bmp", 0);

    if (typedWordIndex == 4)
    {
        iSetColor(typedWordColor[0], typedWordColor[1], typedWordColor[2]);
        if (correct)
        {

            iText(155, 576, "+10", GLUT_BITMAP_TIMES_ROMAN_24);
        }
        else
        {

            iShowBMP2(390 - remainingLives * 35, 573, "cross.bmp", 0);
        }
    }
    if (initializedTutorial)
    {
        iShowBMP(0, 0, tutorialImages[tutorialPageIndex]);
    }
}

void showMainMenu(void)
{
    iShowBMP(0, 0, "mainmenu2.bmp");
}

void showGameInterface(void)
{
    iShowBMP(0, 0, interfaceImages[5 - remainingHints]);
    showText();
    renderHUD();
}

int main()
{

    // getting best score
    FILE *file = fopen("bs.txt", "r");
    if (file == NULL)
    {
        file = fopen("bs.txt", "w");
        fprintf(file, "0");
        fclose(file);
        file = fopen("bs.txt", "r");
    }
    fscanf(file, "%d", &bestScore);
    fclose(file);

    long long int seed = 1705415488; // time(NULL); // 1705415488
    printf("Seed: %lld", seed);

    srand(seed);
    reset();

    gameOverTimer = iSetTimer(3500, gameOver);

    resetTimer = iSetTimer(1500, callReset);

    iPauseTimer(gameOverTimer);
    iPauseTimer(resetTimer);
    iPauseTimer(tutorialStartTimer);
    iPauseTimer(exitTimer);

    iInitialize(WIDTH, HEIGHT, "Spelling Master");
    return 0;
}
