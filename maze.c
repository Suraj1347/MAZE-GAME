#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 5
#define STACK_SIZE 100
#define TIME_LIMIT 30

typedef struct {
    int x, y;
} Position;

typedef struct {
    Position stack[STACK_SIZE];
    int top;
} Stack;

void initializeMaze(char maze[SIZE][SIZE]);
void printMaze(char maze[SIZE][SIZE]);
void push(Stack* s, Position p);
Position pop(Stack* s);
int isEmpty(Stack* s);
int isValidMove(char maze[SIZE][SIZE], int x, int y);
void playGame(char maze[SIZE][SIZE]);

int main() {
    char maze[SIZE][SIZE];
    initializeMaze(maze);
    playGame(maze);
    return 0;
}

void initializeMaze(char maze[SIZE][SIZE]) {
    char tempMaze[SIZE][SIZE] = {
        {'#', '#', '#', '#', '#'},
        {'#', ' ', ' ', '#', '#'},
        {'#', '#', ' ', ' ', '#'},
        {'#', ' ', '#', 'E', '#'},
        {'#', '#', '#', '#', '#'}
    };
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            maze[i][j] = tempMaze[i][j];
}

void printMaze(char maze[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            printf("%c ", maze[i][j]);
        printf("\n");
    }
}

int isEmpty(Stack* s) {
    return s->top == -1;
}

void push(Stack* s, Position p) {
    if (s->top < STACK_SIZE - 1)
        s->stack[++s->top] = p;
}

Position pop(Stack* s) {
    return isEmpty(s) ? (Position){-1, -1} : s->stack[s->top--];
}

int isValidMove(char maze[SIZE][SIZE], int x, int y) {
    return (x >= 0 && x < SIZE && y >= 0 && y < SIZE && maze[x][y] != '#');
}

void playGame(char maze[SIZE][SIZE]) {
    Stack pathStack = {.top = -1};
    Position currentPos = {1, 1};
    maze[currentPos.x][currentPos.y] = 'P';

    time_t startTime = time(NULL);
    while (1) {
        int elapsedTime = (int)(time(NULL) - startTime);
        if (elapsedTime >= TIME_LIMIT) {
            printf("Time's up! You failed to escape the maze.\n");
            break;
        }

        printf("Time remaining: %d seconds\n", TIME_LIMIT - elapsedTime);
        printMaze(maze);
        printf("Current position: (%d, %d)\n", currentPos.x, currentPos.y);

        char move;
        printf("Enter your move (W/A/S/D for up/left/down/right, U for undo): ");
        scanf(" %c", &move);

        Position lastPos = currentPos;

        switch (move) {
            case 'W': case 'w': currentPos.x--; break;
            case 'A': case 'a': currentPos.y--; break;
            case 'S': case 's': currentPos.x++; break;
            case 'D': case 'd': currentPos.y++; break;
            case 'U': case 'u':
                if (!isEmpty(&pathStack))
                    currentPos = pop(&pathStack);
                continue;
            default:
                printf("Invalid move. Try again.\n");
                continue;
        }

        if (isValidMove(maze, currentPos.x, currentPos.y)) {
            push(&pathStack, lastPos);
            maze[lastPos.x][lastPos.y] = ' ';
            maze[currentPos.x][currentPos.y] = 'P';
        } else {
            currentPos = lastPos;
            printf("Invalid move. You hit a wall!\n");
        }

        if (currentPos.x == 3 && currentPos.y == 3) {
            maze[currentPos.x][currentPos.y] = 'E';
            printMaze(maze);
            printf("Congratulations! You've reached the exit!\n");
            break;
        }
    }
}

