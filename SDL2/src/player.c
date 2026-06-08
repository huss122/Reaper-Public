/*
 * player.c  –  Implementation av Player-ADT för ShadowBound

 */

#include "player.h"
#include <math.h>


#define SURVIVOR_VISION_RADIUS  220.0f
#define REAPER_VISION_RADIUS    150.0f

/* ══════════════════════════════════════════════════════════════════════════
 * INTERNA HJÄLPFUNKTIONER
 * ══════════════════════════════════════════════════════════════════════════ */


static float start_x(int id)
{
    float table[MAX_PLAYERS] = {
        PLAYER_START_X_0,
        PLAYER_START_X_1,
        PLAYER_START_X_2,
        PLAYER_START_X_3
    };
    return (id >= 0 && id < MAX_PLAYERS) ? table[id] : 0.0f;
}

static float start_y(int id)
{
    float table[MAX_PLAYERS] = {
        PLAYER_START_Y_0,
        PLAYER_START_Y_1,
        PLAYER_START_Y_2,
        PLAYER_START_Y_3
    };
    return (id >= 0 && id < MAX_PLAYERS) ? table[id] : 0.0f;
}

/* ══════════════════════════════════════════════════════════════════════════
 * INITIERING
 * ══════════════════════════════════════════════════════════════════════════ */


void player_init(Player* p, int id)
{
    if (!p) return;

    p->player_id = id;
    p->x         = start_x(id);
    p->y         = start_y(id);

    p->is_killer = (id == 0);
    p->speed     = p->is_killer ? REAPER_SPEED : SURVIVOR_SPEED;
    p->w         = p->is_killer ? REAPER_W     : SURVIVOR_W;
    p->h         = p->is_killer ? REAPER_H     : SURVIVOR_H;

    /* Status */
    p->is_alive          = true;
    p->has_escaped       = false;

    /* Rörelse och animation */
    p->flip              = false;
    p->is_moving         = false;
    p->is_attacking      = false;
    p->current_frame     = 0;
    p->is_dead_anim_done = false;

    /* Kistinteraktion */
    p->is_opening_chest  = false;
    p->chest_timer       = 0.0f;
    p->target_chest_id   = -1;
}


void player_reset(Player* p, int id)
{
    player_init(p, id);
}

/* ══════════════════════════════════════════════════════════════════════════
 * GEOMETRI OCH SYNFÄLT
 * ══════════════════════════════════════════════════════════════════════════ */

float player_center_x(const Player* p)
{
    return p->x + p->w / 2.0f;
}

float player_center_y(const Player* p)
{
    return p->y + p->h / 2.0f;
}

float player_vision_radius(const Player* p)
{
    return p->is_killer ? REAPER_VISION_RADIUS : SURVIVOR_VISION_RADIUS;
}


bool player_can_see(const Player* viewer, const Player* target)
{
    float dx = player_center_x(target) - player_center_x(viewer);
    float dy = player_center_y(target) - player_center_y(viewer);
    float r  = player_vision_radius(viewer);
    return (dx * dx + dy * dy) <= (r * r);
}


bool player_overlaps_rect(const Player* p, SDL_Rect r)
{
    SDL_Rect pr = {(int)p->x, (int)p->y, p->w, p->h};
    return pr.x < r.x + r.w && pr.x + pr.w > r.x &&
           pr.y < r.y + r.h && pr.y + pr.h > r.y;
}

/* ══════════════════════════════════════════════════════════════════════════
 * SPELSTATE-ÄNDRINGAR
 * ══════════════════════════════════════════════════════════════════════════ */

/*
 * player_kill  –  Döda spelaren och återställ animationsstate.
 */
void player_kill(Player* p)
{
    if (!p) return;
    p->is_alive          = false;
    p->current_frame     = 0;
    p->is_dead_anim_done = false;
}

/*
 * player_escape  –  Survivor tar sig ut genom exit-dörren.
 */
void player_escape(Player* p)
{
    if (!p) return;
    p->has_escaped      = true;
    p->is_moving        = false;
    p->is_opening_chest = false;
    p->x                = ESCAPED_PLAYER_X;
    p->y                = ESCAPED_PLAYER_Y;
}

/* ══════════════════════════════════════════════════════════════════════════
 * KISTINTERAKTION
 * ══════════════════════════════════════════════════════════════════════════ */


void player_start_chest(Player* p, int chest_id)
{
    if (!p) return;
    p->is_opening_chest = true;
    p->chest_timer      = 0.0f;
    p->target_chest_id  = chest_id;
}


void player_cancel_chest(Player* p)
{
    if (!p) return;
    p->is_opening_chest = false;
    p->chest_timer      = 0.0f;
}


bool player_tick_chest(Player* p, float deltaTime)
{
    if (!p || !p->is_opening_chest) return false;

    p->chest_timer += deltaTime;

    if (p->chest_timer >= CHEST_OPEN_TIME) {
        return true;  
    }
    return false;
}
