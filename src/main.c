#include "raylib.h"

#define TEXTURE_SIZE 16

typedef struct {
    int height, width;
    char* name;
} Window;

Window window = {
    .width = 800,
    .height = 450,
    .name = "MiniJam-205",
};

typedef struct {
    Vector2 position;
    unsigned int place, speed;
} Player;

void InitPlayer(Player* player);
void UpdatePlayerPosition(Player* player);

int main(void)
{
    InitWindow(window.width, window.height, window.name);
    SetTargetFPS(60);
    Player player;
    InitPlayer(&player);

    while (!WindowShouldClose()) {
        UpdatePlayerPosition(&player);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(player.position.x, player.position.y, TEXTURE_SIZE, TEXTURE_SIZE, GREEN);
        EndDrawing();
    }

    CloseWindow();
}

void InitPlayer(Player* player)
{
    player->place = 0;
    player->position = (Vector2) { (float)window.width / 2, (float)window.height / 2 };
    player->speed = 0;
}

void UpdatePlayerPosition(Player* player)
{
    if (IsKeyDown(KEY_W)) {
        player->position.y -= 5;
    }
    if (IsKeyDown(KEY_S)) {
        player->position.y += 5;
    }
    if (IsKeyDown(KEY_A)) {
        player->position.x -= 5;
    }
    if (IsKeyDown(KEY_D)) {
        player->position.x += 5;
    }
}
