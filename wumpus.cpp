#include "wumpus.h"

namespace Wumpus_lib
{

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

    Room::Room(int i, Room *a, Room *b, Room *c, Point xy)
        : number{i}, adjacents(3), has_pit{false}, has_bat{false}, coordinates{xy}
    {
        adjacents[0] = a;
        adjacents[1] = b;
        adjacents[2] = c;
    }

    //------------------------------------------------------------------------
    //------------------------------------------------------------------------

    Cave::Cave()
        : rooms(20)
    {
        rooms[0] = Room(1, &rooms[1], &rooms[4], &rooms[7], Point{6 * scale, int(8.5 * scale)});
        rooms[1] = Room(2, &rooms[0], &rooms[2], &rooms[9], Point{5 * scale, int(5.5 * scale)});
        rooms[2] = Room(3, &rooms[1], &rooms[3], &rooms[11], Point{3 * scale, int(5 * scale)});
        rooms[3] = Room(4, &rooms[2], &rooms[4], &rooms[13], Point{3 * scale, int(8 * scale)});
        rooms[4] = Room(5, &rooms[0], &rooms[3], &rooms[5], Point{5 * scale, int(10.5 * scale)});
        rooms[5] = Room(6, &rooms[4], &rooms[6], &rooms[14], Point{8 * scale, int(11.5 * scale)});
        rooms[6] = Room(7, &rooms[5], &rooms[7], &rooms[16], Point{11 * scale, int(10.5 * scale)});
        rooms[7] = Room(8, &rooms[0], &rooms[6], &rooms[8], Point{10 * scale, int(8.5 * scale)});
        rooms[8] = Room(9, &rooms[7], &rooms[9], &rooms[17], Point{11 * scale, int(5.5 * scale)});
        rooms[9] = Room(10, &rooms[1], &rooms[8], &rooms[10], Point{8 * scale, int(3.5 * scale)});
        rooms[10] = Room(11, &rooms[9], &rooms[11], &rooms[18], Point{8 * scale, int(1.5 * scale)});
        rooms[11] = Room(12, &rooms[2], &rooms[10], &rooms[12], Point{5 * scale, int(2.5 * scale)});
        rooms[12] = Room(13, &rooms[11], &rooms[13], &rooms[19], Point{3 * scale, 0});
        rooms[13] = Room(14, &rooms[3], &rooms[12], &rooms[14], Point{0, int(9 * scale)});
        rooms[14] = Room(15, &rooms[5], &rooms[13], &rooms[15], Point{8 * scale, int(14.5 * scale)});
        rooms[15] = Room(16, &rooms[14], &rooms[16], &rooms[19], Point{16 * scale, int(9 * scale)});
        rooms[16] = Room(17, &rooms[6], &rooms[15], &rooms[17], Point{13 * scale, int(8 * scale)});
        rooms[17] = Room(18, &rooms[8], &rooms[16], &rooms[18], Point{13 * scale, int(5 * scale)});
        rooms[18] = Room(19, &rooms[10], &rooms[17], &rooms[19], Point{11 * scale, int(2.5 * scale)});
        rooms[19] = Room(20, &rooms[12], &rooms[15], &rooms[18], Point{13 * scale, 0});

        for (int i = 0; i < rooms.size(); i++)
        {
            rooms[i].coordinates.x += x_indent;
            rooms[i].coordinates.y += y_indent;
        }
    }

    void Cave::shuffle()
    {
        vector<int> v;
        for (int i = 0; i < 20; i++)
        {
            v.push_back(i);
            // for play again (reshuffle)
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
        player.arrows = 5;
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
        int random_room = rand() % 20;
        player.location = &rooms[random_room];
    }

    Point Cave::get_coords(int room)
    {
        if (room < 1 && room > 20)
            error("get_coords(int): Wrong number of room!");
        return rooms[room - 1].coordinates;
    }

    vector<Room *> Cave::get_adjacents(int room)
    {
        if (room < 1 && room > 20)
            error("get_adjacents(int): Wrong number of room!");
        return rooms[room - 1].adjacents;
    }

    Room *Cave::get_room(int room)
    {
        if (room < 1 && room > 20)
            error("get_room(int): Wrong number of room!");
        return &rooms[room - 1];
    }
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------

    void Game::start()
    {
        cout << "Hunt the Wumpus\n";
        while (true)
        {
            cout << "\nType 1 to read the instructions, 2 to play the game, or 3 to quit. ";
            int i = 0;
            cin >> i;
            if (i == 1)
                print_instructions(cout);
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

    ostream &Game::print_instructions(ostream &os)
    {
        os << "\nЛаскаво просимо до гри \"Полювання на Вампуса!\"\n\n"
           << "Вампус живе в печері з 20 кімнат. Кожна кімната\n"
           << "має 3 тунелі, що ведуть до інших кімнат. Подивіться на\n"
           << "додекаедр, щоб побачити, як це працює - якщо ви не знаєте\n"
           << "що таке додекаедр, запитайте у когось.\n\n"
           << "Небезпеки:\n"
           << "-Бездонні ями - у двох кімнатах є бездонні ями.\n"
           << "Якщо Ви підете туди, Ви впадете в яму (і програєте!)\n"
           << "-Супер кажани - у двох інших кімнатах є супер кажани. Якщо Ви\n"
           << "підете туди, кажан схопить Вас і віднесе в іншу\n"
           << "кімнату навмання (що може бути неприємним).\n\n"
           << "Вампус:\n"
           << "Вампуса не турбують небезпеки (у нього є присоска і "
           << "ноги \nта він ще занадто великий, щоб кажан міг його підняти). Зазвичай\n"
           << "він спить. Його будять дві речі: Ваш вхід "
           << "до його кімнати чи Ваш пуск стріли.\n"
           << "Якщо Вампус прокидається, він переміщається (імовірність 75%) \nна одну кімнату "
           << "або залишається нерухомим (ймовірність 25%). \nПісля цього, якщо він там, де Ви, "
           << "він Вас з'їдає (і Ви програєте)!\n\n"
           << "Ви:\n"
           << "Кожен хід Ви можете рухатися або пускати криву стрілу.\n"
           << "За один хід Ви можете пройти тільки до однієї кімнати (через один тунель).\n"
           << "Стріли: у Вас є 5 стріл. Ви програєте, коли вони закінчаться.\n"
           << "Кожна стріла може проходити від 1 до 3 кімнат. Ви цілитеся, кажучи\n"
           << "комп'ютеру номери кімнат, до яких має бути спрямована стріла.\n"
           << "Якщо стріла не може рухатися в цьому напрямку (тобто немає тунелю), \nвона рухається "
           << "навмання до наступної кімнати.\n"
           << "Якщо стріла потрапить у Вампуса, Ви виграєте.\n"
           << "Якщо стріла влучить у Вас, Ви програєте.\n\n"
           << "Попередження: коли Ви знаходитесь на відстані \nоднієї кімнати від небезпеки, "
           << "комп'ютер каже:\n"
           << "Вампус - \"Відчуваю смердіння Вампуса!\"\n"
           << "Кажан - \"Кажани поруч!\"\n"
           << "Яма - \"Поруч яма! Я відчуваю протяг!\"\n\n";
        return os;
    }

    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------

};