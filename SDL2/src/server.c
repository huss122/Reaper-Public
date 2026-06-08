
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../include/network.h"

int main(int argc, char** argv)
{

    SDL_Init(0);

    if (SDLNet_Init() == -1) {
        printf("SDLNet_Init fel: %s\n", SDLNet_GetError());
        return 1;
    }

    /* ──────────────────────────────────────────────────────────────────────
     * 2. ÖPPNA UDP-SOCKET PÅ EN KÄND PORT
     * ────────────────────────────────────────────────────────────────────── */
    UDPsocket serverSocket = SDLNet_UDP_Open(SERVER_PORT);
    if (!serverSocket) {
        printf("SDLNet_UDP_Open fel: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        SDL_Quit();
        return 1;
    }
    printf("Server lyssnar på port %d...\n", SERVER_PORT);

    /* ──────────────────────────────────────────────────────────────────────
     * 3. ALLOKERA PAKET
     * ────────────────────────────────────────────────────────────────────── */
    UDPpacket* recvPacket = SDLNet_AllocPacket(sizeof(PlayerPacket));
    UDPpacket* sendPacket = SDLNet_AllocPacket(sizeof(GameState));

    if (!recvPacket || !sendPacket) {
        printf("SDLNet_AllocPacket fel: %s\n", SDLNet_GetError());
        SDLNet_UDP_Close(serverSocket);
        SDLNet_Quit();
        SDL_Quit();
        return 1;
    }

    /* ──────────────────────────────────────────────────────────────────────
     * 4. KLIENT-ADRESSTABELL
     * ────────────────────────────────────────────────────────────────────── */
    IPaddress clientAddrs[MAX_PLAYERS];
    int       numClients = 0;
    memset(clientAddrs, 0, sizeof(clientAddrs));

    /* ──────────────────────────────────────────────────────────────────────
     * 5. SPELSTATE PÅ SERVERN
     * ────────────────────────────────────────────────────────────────────── */
    GameState gs;
    memset(&gs, 0, sizeof(gs));
    for (int i = 0; i < MAX_PLAYERS; i++) {
        gs.players[i].is_alive  = true;
        gs.players[i].player_id = i;
    }

    /* ──────────────────────────────────────────────────────────────────────
     * 6. HUVUDLOOP
     * ────────────────────────────────────────────────────────────────────── */
    printf("Väntar på anslutningar...\n");

    while (1)
    {
        if (SDLNet_UDP_Recv(serverSocket, recvPacket) == 1)
        {
            /* ────────────────────────────────────────────────────────────
             * 6a. DESERIALISERA INKOMMANDE PAKET
             * ──────────────────────────────────────────────────────────── */
            PlayerPacket pp;
            memcpy(&pp, recvPacket->data, sizeof(PlayerPacket));

            int id = pp.player_id;
            if (id < 0 || id >= MAX_PLAYERS) {
                printf("Ogiltigt player_id: %d – ignoreras\n", id);
                continue;
            }

            /* ────────────────────────────────────────────────────────────
             * 6b. REGISTRERA NY KLIENT
             * ──────────────────────────────────────────────────────────── */
            bool known = false;
            for (int i = 0; i < numClients; i++) {
                if (clientAddrs[i].host == recvPacket->address.host &&
                    clientAddrs[i].port == recvPacket->address.port) {
                    known = true;
                    break;
                }
            }
            if (!known && numClients < MAX_PLAYERS) {
                clientAddrs[numClients] = recvPacket->address;
                numClients++;

                Uint8* ip = (Uint8*)&recvPacket->address.host;
                printf("Ny klient ID=%d  IP=%d.%d.%d.%d  Port=%d  "
                       "Totalt anslutna: %d\n",
                       id, ip[0], ip[1], ip[2], ip[3],
                       SDL_SwapBE16(recvPacket->address.port),
                       numClients);
            }

            /* ────────────────────────────────────────────────────────────
             * 6c. UPPDATERA SERVERNS GAMESTATE*/
            bool was_alive = gs.players[id].is_alive;

            gs.players[id] = pp;  

            if (!was_alive) {
            
                gs.players[id].is_alive = false;
            }

            /* ────────────────────────────────────────────────────────────
             * 6d. KISTSYNKRONISERING
             * ──────────────────────────────────────────────────────────── */
            for (int c = 0; c < 8; c++) {
                if (pp.chest_states[c])
                    gs.chest_states[c] = true;
            }

            /* ────────────────────────────────────────────────────────────
             * 6e. BROADCAST GAMESTATE TILL ALLA KLIENTER
             * ──────────────────────────────────────────────────────────── */
            memcpy(sendPacket->data, &gs, sizeof(GameState));
            sendPacket->len = sizeof(GameState);

            for (int i = 0; i < numClients; i++) {
                sendPacket->address = clientAddrs[i];
                int result = SDLNet_UDP_Send(serverSocket, -1, sendPacket);
                if (result == 0) {
                    Uint8* ip = (Uint8*)&clientAddrs[i].host;
                    printf("VARNING: Kunde inte skicka till %d.%d.%d.%d\n",
                           ip[0], ip[1], ip[2], ip[3]);
                }
            }
        }

        SDL_Delay(1);
    }

    /* ──────────────────────────────────────────────────────────────────────
     * 7. STÄDA UPP 
     * ────────────────────────────────────────────────────────────────────── */
    SDLNet_FreePacket(recvPacket);
    SDLNet_FreePacket(sendPacket);
    SDLNet_UDP_Close(serverSocket);
    SDLNet_Quit();
    SDL_Quit();
    return 0;
}