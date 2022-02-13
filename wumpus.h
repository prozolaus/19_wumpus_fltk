#include "std_lib_facilities.h"
#include "Point.h"

using namespace Graph_lib;

namespace Wumpus_lib
{

    const int scale = 30;
    const int x_indent = 100;
    const int y_indent = 100;

    enum After_shooting_state
    {
        wumpus_shot,
        player_shot,
        no_arrows,
        wumpus_awakened
    };

    struct Room
    {
        Room() : number{0}, has_pit{false}, has_bat{false} {}
        Room(int, Room *, Room *, Room *, Point xy = Point());
        int number;
        vector<Room *> adjacents;
        bool has_pit, has_bat;
        Point coordinates;
    };

    struct Player
    {
        Player() : location{nullptr}, arrows{5} {}
        Player(Room *r) : location{r}, arrows{5} {}
        Room *location;
        int arrows;
    };

    struct Wumpus
    {
        Wumpus() : location{nullptr} {}
        Wumpus(Room *wr) : location{wr} {}
        Room *location;
    };

    //------------------------------------------------------------------------
    //------------------------------------------------------------------------

    struct Cave
    {
        Cave();
        void shuffle();
        Room *get_player_loc() { return player.location; }
        Room *get_wumpus_loc() { return wumpus.location; }
        void hazard_warnings();
        void room_description();
        bool is_moved_to_adjacents(Room *&current_loc, int new_room);
        bool player_moves(int r);
        After_shooting_state player_shoots();
        void wumpus_woke_up();
        void bats_flight();
        Point get_coords(int room);
        vector<Room *> get_adjacents(int room);
        Room *get_room(int room);
        void decrease_arrows() { player.arrows--; }
        int get_arrows() { return player.arrows; }

    private:
        vector<Room> rooms;
        Player player;
        Wumpus wumpus;
    };

    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------

    struct Game
    {
        void start();
        void play();
        void print_instructions();

    private:
        Cave cave;
    };

};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
