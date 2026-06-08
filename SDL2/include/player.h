#ifndef PLAYER_H
#define PLAYER_H

/*
 * player.h  –  ADT för en spelare 
 */

#include <SDL2/SDL.h>
#include <stdbool.h>

/* ══════════════════════════════════════════════════════════════════════════
 * KONSTANTER
 * ══════════════════════════════════════════════════════════════════════════ */

#define MAX_PLAYERS 4

/* Standardstorlek per roll */
#define REAPER_W    17
#define REAPER_H    30
#define SURVIVOR_W  15
#define SURVIVOR_H  25

/* Standardhastighet per roll */
#define REAPER_SPEED   140.0f
#define SURVIVOR_SPEED 150.0f

/* Startpositioner (index 0 = Reaper, 1–3 = Survivors) */
#define PLAYER_START_X_0  100.0f
#define PLAYER_START_X_1  920.0f
#define PLAYER_START_X_2  950.0f
#define PLAYER_START_X_3  980.0f
#define PLAYER_START_Y_0  780.0f
#define PLAYER_START_Y_1  200.0f
#define PLAYER_START_Y_2  200.0f
#define PLAYER_START_Y_3  200.0f

/* Tid i sekunder för att öppna en kista */
#define CHEST_OPEN_TIME  10.0f

/* Position dit en escapad survivor förflyttas (utanför kartan) */
#define ESCAPED_PLAYER_X  -10000.0f
#define ESCAPED_PLAYER_Y  -10000.0f

/* ══════════════════════════════════════════════════════════════════════════
 * TYPDEFINITION
 * ══════════════════════════════════════════════════════════════════════════ */

typedef struct {
    /* Position och storlek */
    float x, y;
    float speed;
    int   w, h;

    /* Roll och status */
    bool  is_killer;          /* true = Reaper, false = Survivor    */
    bool  is_alive;
    bool  has_escaped;        /* Survivor tog sig ut genom dörren   */
    int   player_id;          /* 0 = Reaper, 1–3 = Survivors        */

    /* Rörelse och animation */
    bool  flip;               /* Spegla sprite horisontellt         */
    bool  is_moving;
    bool  is_attacking;
    int   current_frame;
    bool  is_dead_anim_done;

    /* Kistinteraktion */
    bool  is_opening_chest;
    float chest_timer;        /* Räknar upp mot CHEST_OPEN_TIME     */
    int   target_chest_id;    /* Index i chests[]-arrayen, -1 = ingen */
} Player;

/* ══════════════════════════════════════════════════════════════════════════
 * FUNKTIONSDEKLARATIONER
 * ══════════════════════════════════════════════════════════════════════════ */

/*
 * player_init  –  Initiera en spelare med standardvärden för given roll.

 */
void player_init(Player* p, int id);

/*
 * player_reset  –  Återställ en spelare till startläge (t.ex. vid omstart).
 */
void player_reset(Player* p, int id);


float player_center_x(const Player* p);
float player_center_y(const Player* p);

/*
 * player_vision_radius  –  Synfältsradie i pixlar för given spelare.
 */
float player_vision_radius(const Player* p);

/*
 * player_can_see  –  Kontrollera om 'viewer' har 'target' inom synfältet.
 */
bool player_can_see(const Player* viewer, const Player* target);


bool player_overlaps_rect(const Player* p, SDL_Rect r);

/*
 * player_kill  –  Markera spelaren som död och återställ animationsstate.
 */
void player_kill(Player* p);

/*
 * player_escape  –  Markera spelaren som escapad och flytta utanför kartan.
 */
void player_escape(Player* p);


void player_start_chest(Player* p, int chest_id);


void player_cancel_chest(Player* p);


bool player_tick_chest(Player* p, float deltaTime);

#endif /* PLAYER_H */
