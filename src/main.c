#include "raylib.h"
#include <stdio.h>

#define TEXTURE_SIZE 32.0F
#define MAX_BULLETS 64

typedef struct {
    int height, width;
    char* name;
} Window;

Window window = {
    .width = TEXTURE_SIZE * 20,
    .height = TEXTURE_SIZE * 15,
    .name = "MiniJam-205",
};

typedef struct {
    Vector2 position, center;
    unsigned int place;
    float speed;
    Texture2D texture;
} Player;

typedef struct {
    Rectangle rectangle;
    float speed;
    bool active;
} Bullet;

typedef struct {
    Bullet bullets[MAX_BULLETS];
    Texture2D texture;
} Bullets;

void InitPlayer(Player* player);
void UpdatePlayer(Player* player);
void UpdatePlayerPosition(Player* player);
void DrawPlayer(Player* player);

void InitBullets(Bullets* bullets);
void UpdateBullets(Bullets* bullets);
void DrawBullets(Bullets* bullets);
void ShootGun(Bullets* bullets, Player* player);

int main(void)
{
    InitWindow(window.width, window.height, window.name);
    SetTargetFPS(60);

    Player player;
    InitPlayer(&player);

    Bullets bullets;
    InitBullets(&bullets);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE))
            ShootGun(&bullets, &player);
        UpdatePlayer(&player);
        UpdateBullets(&bullets);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawBullets(&bullets);
        DrawPlayer(&player);
        EndDrawing();
    }

    CloseWindow();
}

void InitPlayer(Player* player)
{
    player->position = (Vector2) { (float)window.width / 2, (float)window.height / 2 };
    player->center = (Vector2) { player->position.x + (TEXTURE_SIZE / 2), player->position.y + (TEXTURE_SIZE / 2) };
    player->place = 0;
    player->speed = 5.0F;
    player->texture = LoadTexture("assets/Horse.png");
    if (!IsTextureValid(player->texture))
        printf("\033[31mERROR: Player (Horse.png) texture could not be loaded\033[0m\n");
}

void UpdatePlayer(Player* player)
{
    player->center = (Vector2) { player->position.x + (TEXTURE_SIZE / 2), player->position.y + (TEXTURE_SIZE / 2) };
    UpdatePlayerPosition(player);
}

void UpdatePlayerPosition(Player* player)
{
    if (IsKeyDown(KEY_W))
        player->position.y -= player->speed;
    if (IsKeyDown(KEY_S))
        player->position.y += player->speed;
    if (IsKeyDown(KEY_A))
        player->position.x -= player->speed;
    if (IsKeyDown(KEY_D))
        player->position.x += player->speed;
}

void DrawPlayer(Player* player)
{
    DrawTextureV(player->texture, player->position, WHITE);
}

void InitBullets(Bullets* bullets)
{
    for (int i = 0; i < MAX_BULLETS; i++)
        bullets->bullets[i].active = false;
    bullets->texture = LoadTexture("assets/Bullet.png");
    if (!IsTextureValid(bullets->texture))
        printf("\033[31mERROR: Bullet (Bullet.png) texture could not be loaded\033[0m\n");
}

void UpdateBullets(Bullets* bullets)
{
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets->bullets[i].active)
            continue;

        bullets->bullets[i].rectangle.x += bullets->bullets[i].speed;

        Rectangle r = bullets->bullets[i].rectangle;
        if (r.x < 0 || r.x > window.width || r.y < 0 || r.y > window.height)
            bullets->bullets[i].active = false;
    }
}

void DrawBullets(Bullets* bullets)
{
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets->bullets[i].active)
            continue;
        DrawTextureV(bullets->texture, (Vector2) { bullets->bullets[i].rectangle.x, bullets->bullets[i].rectangle.y }, WHITE);
    }
}

void ShootGun(Bullets* bullets, Player* player)
{
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets->bullets[i].active) {
            bullets->bullets[i].rectangle = (Rectangle) {
                .x = player->position.x + TEXTURE_SIZE,
                .y = player->center.y,
                .width = TEXTURE_SIZE / 4,
                .height = TEXTURE_SIZE / 4,
            };
            bullets->bullets[i].speed = 8.0F;
            bullets->bullets[i].active = true;
            break;
        }
    }
}
