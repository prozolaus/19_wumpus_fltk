#include "std_lib_facilities.h"

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
    Room(int, Room *, Room *, Room *);
    int number;
    vector<Room *> adjacents;
    bool has_pit, has_bat;
};

Room::Room(int i, Room *a, Room *b, Room *c)
    : number{i}, adjacents(3), has_pit{false}, has_bat{false}
{
    adjacents[0] = a;
    adjacents[1] = b;
    adjacents[2] = c;
}

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

void skip_to_int()
{
    if (cin.fail())
    {
        cin.clear();
        char ch;
        while (cin >> ch)
        {
            if (isdigit(ch))
            {
                cin.unget();
                return;
            }
        }
    }
    error("No input");
}

int get_int()
{
    int n = 0;
    while (true)
    {
        if (cin >> n)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return n;
        }
        cout << "Not valid\n";
        skip_to_int();
    }
}

int get_int(int low, int high, const string &greeting, const string &sorry = "Wrong input! Try again!")
{
    while (true)
    {
        cout << greeting << " (" << low << '-' << high << ")? ";
        int n = get_int();
        if (low <= n && n <= high)
            return n;
        cout << sorry << endl;
    }
}

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
    bool player_moves(int r);
    After_shooting_state player_shoots();
    void wumpus_woke_up();
    void bats_flight();

private:
    vector<Room> rooms;
    Player player;
    Wumpus wumpus;
    bool is_moved_to_adjacents(Room *&current_loc, int new_room);
};

Cave::Cave()
    : rooms(20)
{
    rooms[0] = Room(1, &rooms[1], &rooms[4], &rooms[7]);
    rooms[1] = Room(2, &rooms[0], &rooms[2], &rooms[9]);
    rooms[2] = Room(3, &rooms[1], &rooms[3], &rooms[11]);
    rooms[3] = Room(4, &rooms[2], &rooms[4], &rooms[13]);
    rooms[4] = Room(5, &rooms[0], &rooms[3], &rooms[5]);
    rooms[5] = Room(6, &rooms[4], &rooms[6], &rooms[14]);
    rooms[6] = Room(7, &rooms[5], &rooms[7], &rooms[16]);
    rooms[7] = Room(8, &rooms[0], &rooms[6], &rooms[8]);
    rooms[8] = Room(9, &rooms[7], &rooms[9], &rooms[17]);
    rooms[9] = Room(10, &rooms[1], &rooms[8], &rooms[10]);
    rooms[10] = Room(11, &rooms[9], &rooms[11], &rooms[18]);
    rooms[11] = Room(12, &rooms[2], &rooms[10], &rooms[12]);
    rooms[12] = Room(13, &rooms[11], &rooms[13], &rooms[19]);
    rooms[13] = Room(14, &rooms[3], &rooms[12], &rooms[14]);
    rooms[14] = Room(15, &rooms[5], &rooms[13], &rooms[15]);
    rooms[15] = Room(16, &rooms[14], &rooms[16], &rooms[19]);
    rooms[16] = Room(17, &rooms[6], &rooms[15], &rooms[17]);
    rooms[17] = Room(18, &rooms[8], &rooms[16], &rooms[18]);
    rooms[18] = Room(19, &rooms[10], &rooms[17], &rooms[19]);
    rooms[19] = Room(20, &rooms[12], &rooms[15], &rooms[18]);
}

void Cave::shuffle()
{
    vector<int> v;
    for (int i = 0; i < 20; i++)
    {
        v.push_back(i);
        //for play again (reshuffle)
        rooms[i].has_bat = false;
        rooms[i].has_pit = false;
    }
    srand(time(0));
    random_shuffle(v.begin(), v.end());

    player.location = &rooms[v[0]];
    wumpus.location = &rooms[v[1]];
    rooms[v[2]].has_bat = true;
    rooms[v[3]].has_bat = true;
    rooms[v[4]].has_pit = true;
    rooms[v[5]].has_pit = true;
    /*
    cout << "Wumpus: " << v[1] + 1 << endl;
    cout << "Bat1: " << v[2] + 1 << endl;
    cout << "Bat2: " << v[3] + 1 << endl;
    cout << "Pit1: " << v[4] + 1 << endl;
    cout << "Pit2: " << v[5] + 1 << endl;
*/
}

void Cave::hazard_warnings()
{
    if (player.location->adjacents[0]->has_bat ||
        player.location->adjacents[1]->has_bat ||
        player.location->adjacents[2]->has_bat)
        cout << "\nBats nearby!" << endl;
    if (player.location->adjacents[0]->has_pit ||
        player.location->adjacents[1]->has_pit ||
        player.location->adjacents[2]->has_pit)
        cout << "\nI feel a draft!" << endl;
    if (player.location->adjacents[0] == wumpus.location ||
        player.location->adjacents[1] == wumpus.location ||
        player.location->adjacents[2] == wumpus.location)
        cout << "\nI smell the Wumpus!" << endl;
}

void Cave::room_description()
{
    cout << "\nYou are in room " << player.location->number << endl;
    cout << "Tunnels lead to "
         << player.location->adjacents[0]->number << " "
         << player.location->adjacents[1]->number << " "
         << player.location->adjacents[2]->number << endl;
}

bool Cave::is_moved_to_adjacents(Room *&current_loc, int new_room)
{
    for (int i = 0; i < 3; i++)
        if (current_loc->adjacents[i]->number == new_room)
        {
            current_loc = current_loc->adjacents[i];
            return true;
        }
    return false;
}

bool Cave::player_moves(int new_room)
{
    if (is_moved_to_adjacents(player.location, new_room))
        return true;
    cout << "Wrong number of room! Try again!" << endl;
    return false;
}

After_shooting_state Cave::player_shoots()
{
    int n = get_int(1, 3, "Number of rooms");
    vector<int> shoot_rooms(n);
    for (int i = 0; i < n; i++)
        shoot_rooms[i] = get_int(1, 20, "Room #" + to_string(i + 1));

    Room *arrow_loc = player.location;
    for (int i = 0; i < n; i++)
    {
        if (!is_moved_to_adjacents(arrow_loc, shoot_rooms[i]))
            arrow_loc = &rooms[randint(20)];
        if (arrow_loc == wumpus.location)
            return After_shooting_state::wumpus_shot;
        if (arrow_loc == player.location)
            return After_shooting_state::player_shot;
    }
    player.arrows--;
    if (player.arrows == 0)
        return After_shooting_state::no_arrows;
    return After_shooting_state::wumpus_awakened;
}

void Cave::wumpus_woke_up()
{
    int i = randint(4);
    if (i != 3)
        wumpus.location = wumpus.location->adjacents[i];
}

void Cave::bats_flight()
{
    int random_room = rand() % 20 + 1;
    player.location = &rooms[random_room];
}
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

void Game::start()
{
    cout << "Hunt the Wumpus\n";
    while (true)
    {
        cout << "\nType 1 to read the instructions, 2 to play the game, or 3 to quit. ";
        int i = 0;
        cin >> i;
        if (i == 1)
            print_instructions();
        else if (i == 2)
            play();
        else if (i == 3)
            return;
        else
        {
            cout << "\nWrong input! Try again!\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    cout << endl;
}

void Game::play()
{
    cave.shuffle();
    char c = 0;
    while (c != 'q')
    {
        if (cave.get_wumpus_loc() == cave.get_player_loc())
        {
            cout << "\nTsk tsk tsk - Wumpus got you in room " << cave.get_player_loc()->number << "!\n\nHa ha ha! You lose!\n\n";
            return;
        }
        if (cave.get_player_loc()->has_pit)
        {
            cout << "\nYYYIIIIEEEE . . . Fell in pit (room " << cave.get_player_loc()->number << ")\n\nHa ha ha! You lose!\n\n";
            return;
        }
        if (cave.get_player_loc()->has_bat)
        {
            cout << "\nZap - super bat snatch! Elsewhereville for you!\n\n";
            cave.bats_flight();
            continue;
        }
        cave.hazard_warnings();
        cave.room_description();
        cout << "Shoot, move or quit (s-m-q)? ";
        cin >> c;
        if (c == 'm')
        {
            int next_room = 0;
            bool moved = false;
            while (!moved)
            {
                cout << "Where to? ";
                next_room = get_int();
                moved = cave.player_moves(next_room);
            }
        }
        else if (c == 's')
        {
            After_shooting_state state = cave.player_shoots();
            switch (state)
            {
            case wumpus_shot:
                cout << "\nAha! You got the Wumpus!\n"
                     << "Hee hee hee - the Wumpus'll getcha next time!\n";
                break;
            case player_shot:
                cout << "\nOuch! Arrow got you!\n"
                     << "Ha ha ha - you lose!\n";
                break;
            case no_arrows:
                cout << "\nYou have run out of arrows!\n"
                     << "Ha ha ha - you lose!\n";
            default: // wumpus awakened
                cave.wumpus_woke_up();
                break;
            }
            if (state != wumpus_awakened)
                break;
        }
        else if (c == 'q')
            cout << "\nChicken!\n";
        else
        {
            cout << "\nWrong input! Try again!\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void Game::print_instructions()
{
    cout << "\nWelcome to Hunt the Wumpus'!\n\n"
         << "The Wumpus lives in a cave of 20 rooms. Each room\n"
         << "has 3 tunnels leading to other rooms. (Look at a\n"
         << "dodecahedron to see how this works - if you don't know\n"
         << "what a dodecahedron is, ask someone.)\n\n"
         << "Hazards:\n"
         << "Bottomless pits - two rooms have bottomless pits in them.\n"
         << "If you go there, you fall into the pit (and lose!)\n"
         << "Super bats - two other rooms have super bats. If you\n"
         << "go there, a bat grabs you and takes you to some other\n"
         << "room at random (which might be troublesome).\n\n"
         << "Wumpus:\n"
         << "The Wumpus is not bothered by the hazards (he has sucker\n"
         << "feet and is too big for a bat to lift). Usually\n"
         << "he is asleep. Two things wake him up: your entering\n"
         << "his room or your shooting an arrow.\n"
         << "If the Wumpus wakes, he moves (75% chance) one room\n"
         << "or stays still (25% chance). After that, if he is where you\n"
         << "are, he eats you up (and you lose)!\n\n"
         << "You:\n"
         << "Each turn you may move or shoot a crooked arrow.\n"
         << "Moving: you can go one room (through one tunnel).\n"
         << "Arrows: you have 5 arrows. You lose when you run out.\n"
         << "Each arrow can go from 1 to 5 rooms. You aim by telling\n"
         << "the computer the room numbers you want the arrow to go to.\n"
         << "If the arrow can't go that way (i.e., no tunnel) it moves\n"
         << "at random to the next room.\n"
         << "If the arrow hits the Wumpus, you win.\n"
         << "If the arrow hits you, you lose.\n\n"
         << "Warnings: when you are one room away from a hazard,\n"
         << "the computer says:\n"
         << "Wumpus - I smell a Wumpus!\n"
         << "Bat - 'Bats nearby!'\n"
         << "Pit - 'I feel a draft!'\n\n";
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int main()
try
{
    Game game;
    game.start();
}
catch (const std::exception &e)
{
    std::cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch (...)
{
    cerr << "Some kind of exception\n";
    return 2;
}
