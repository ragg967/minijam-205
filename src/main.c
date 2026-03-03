#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define TEXTURE_SIZE 32.0F
#define MAX_BULLETS 64
#define RACERS 24

typedef struct Window {
    int height, width;
    char* name;
} Window;

Window window = {
    .width = TEXTURE_SIZE * 20,
    .height = TEXTURE_SIZE * 15,
    .name = "MiniJam-205",
};

typedef struct Player {
    Vector2 position, center;
    unsigned int place, speed;
    Texture2D texture;
} Player;

typedef struct Bullet {
    Rectangle rectangle;
    float speed;
    bool active;
} Bullet;

typedef struct Bullets {
    Bullet bullets[MAX_BULLETS];
    Texture2D texture;
} Bullets;

typedef struct Racer {
    Vector2 position, center;
    unsigned int place, speed;
} Racer;

typedef struct Racers {
    Racer racers[RACERS];
    Texture2D texture;
} Racers;

void InitPlayer(Player* player);
void UpdatePlayer(Player* player);
void UpdatePlayerPosition(Player* player);
void DrawPlayer(Player* player);

void InitBullets(Bullets* bullets);
void UpdateBullets(Bullets* bullets);
void DrawBullets(Bullets* bullets);
void ShootGun(Bullets* bullets, Player* player);

void InitRacers(Racers* racers);
void DrawRacers(Racers* reacers);
void UpdateRacers(Racers* racers);

int main(void)
{
    InitWindow(window.width, window.height, window.name);
    SetTargetFPS(60);

    Player player;
    InitPlayer(&player);

    Bullets bullets;
    InitBullets(&bullets);

    Racers racers;
    InitRacers(&racers);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE))
            ShootGun(&bullets, &player);
        UpdateBullets(&bullets);
        UpdateRacers(&racers);
        UpdatePlayer(&player);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawBullets(&bullets);
        DrawRacers(&racers);
        DrawPlayer(&player);
        EndDrawing();
    }

    CloseWindow();
}

void UpdateRacers(Racers* racers)
{
    for (int i = 0; i < RACERS - 1; i++) {
        racers->racers[i].position.x += racers->racers[i].speed;
    }
}

void DrawRacers(Racers* racers)
{
    for (int i = 0; i < RACERS - 1; i++) {
        DrawTextureV(racers->texture, racers->racers[i].position, WHITE);
    }
}

void InitRacers(Racers* racers)
{
    for (int i = 0; i < RACERS - 1; i++) {
        racers->racers[i] = (Racer) {
            .center = 0,
            .place = i,
            .speed = rand() % (6 - 1 + 1) + 1,
            .position = (Vector2) { 0, 10 + i * ((window.height - 20) / (RACERS - 1)) },
        };
    }
    racers->texture = LoadTexture("assets/Horse.png");
    if (!IsTextureValid(racers->texture))
        printf("\033[31mERROR: Racers' (Horse.png) texture could not be loaded\033[0m\n");
}

void InitPlayer(Player* player)
{
    player->position = (Vector2) { (float)window.width / 2, (float)window.height / 2 };
    player->center = (Vector2) { player->position.x + (TEXTURE_SIZE / 2), player->position.y + (TEXTURE_SIZE / 2) };
    player->place = 24;
    player->speed = 5;
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
