
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "Simple_window.h"

//------------------------------------------------------------------------------

Simple_window::Simple_window(Point xy, int w, int h, const string &title) : Window(xy, w, h, title),
                                                                            next_button(Point{x_max() - 70, 0}, 70, 20, "Next", cb_next),
                                                                            button_pushed(false)
{
    attach(next_button);
}

//------------------------------------------------------------------------------
void Simple_window::wait_for_button()
// modified event loop:
// handle all events (as per default), quit when button_pushed becomes true
// this allows graphics without control inversion
{
    // Simpler handler
    while (!button_pushed)
        Fl::wait();
    button_pushed = false;
    Fl::redraw();
}

//------------------------------------------------------------------------------

void Simple_window::cb_next(Address, Address pw)
// call Simple_window::next() for the window located at pw
{
    reference_to<Simple_window>(pw).next();
}

//------------------------------------------------------------------------------

void Simple_window::next()
{
    button_pushed = true;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void Wumpus_window::instructions()
{
    if (instr_button.label == "Сховати")
    {
        instructions_box.hide();
        instr_button.label = "Правила";
        move_text.hide();
        notes.hide();
        attach(wumpus_map);
    }
    else
    {
        Game game;
        ostringstream os;
        game.print_instructions(os);
        instructions_box.put(os.str());
        instructions_box.show();
        instr_button.label = "Сховати";
        move_text.show();
        notes.show();
        detach(wumpus_map);
    }
    redraw();
}

void Wumpus_window::hazard_warnings()
{
    warn_text_bats.set_label("");
    warn_text_pits.set_label("");
    warn_text_wumpus.set_label("");
    ostringstream os_bats, os_pits, os_wumpus;
    if (cave_ptr->get_player_loc()->adjacents[0]->has_bat ||
        cave_ptr->get_player_loc()->adjacents[1]->has_bat ||
        cave_ptr->get_player_loc()->adjacents[2]->has_bat)
        os_bats << "Кажани поруч! ";
    if (cave_ptr->get_player_loc()->adjacents[0]->has_pit ||
        cave_ptr->get_player_loc()->adjacents[1]->has_pit ||
        cave_ptr->get_player_loc()->adjacents[2]->has_pit)
        os_pits << "Поруч яма! ";
    if (cave_ptr->get_player_loc()->adjacents[0] == cave_ptr->get_wumpus_loc() ||
        cave_ptr->get_player_loc()->adjacents[1] == cave_ptr->get_wumpus_loc() ||
        cave_ptr->get_player_loc()->adjacents[2] == cave_ptr->get_wumpus_loc())
        os_wumpus << "Смердить Вампусом!";
    warn_text_bats.set_label(os_bats.str());
    warn_text_pits.set_label(os_pits.str());
    warn_text_wumpus.set_label(os_wumpus.str());
    redraw();
}

void Wumpus_window::hazard_caught()
{
    hazard_state_text.set_label("");
    zap_text.set_label("");
    ostringstream os, os_zap;
    while (!game_over)
    {
        if (cave_ptr->get_wumpus_loc() == cave_ptr->get_player_loc())
        {
            os << "Аааааа - Вампус натрапив на Вас і з'їв!";
            game_over = true;
        }
        else if (cave_ptr->get_player_loc()->has_pit)
        {
            os << "Аааааа . . . Ви впали в яму і програли!";
            game_over = true;
        }
        else if (cave_ptr->get_player_loc()->has_bat)
        {
            if (os_zap.str().empty())
                os_zap << "Цап - супер кажан схопив Вас і кудись закинув!";
            cave_ptr->bats_flight();
        }
        else
            break;
    }
    hazard_state_text.set_label(os.str());
    zap_text.set_label(os_zap.str());
    hazard_warnings();
    redraw();
}

void Wumpus_window::move_to_room(int n)
{
    if (game_over)
        return;
    Point first_room = cave_ptr->get_player_loc()->coordinates;
    if (!cave_ptr->player_moves(n))
        return;
    hazard_caught();
    Point second_room = cave_ptr->get_player_loc()->coordinates;
    wumpus_map.current_room.move(second_room.x - first_room.x, second_room.y - first_room.y);
    redraw();
}

After_shooting_state Wumpus_window::shoot()
{
    cave_ptr->decrease_arrows();
    arrows.put(to_string(cave_ptr->get_arrows()));

    vector<In_box *> shoot_boxes{&shoot_box1, &shoot_box2, &shoot_box3};
    const int count = 3;
    vector<int> shoot_rooms;
    for (int i = 0, r = 0; i < count; i++)
    {
        r = shoot_boxes[i]->get_int();
        if (r >= 1 && r <= 20)
            shoot_rooms.push_back(r);
    }

    Room *arrow_loc = cave_ptr->get_player_loc();
    for (int i = 0; i < shoot_rooms.size(); i++)
    {
        if (!cave_ptr->is_moved_to_adjacents(arrow_loc, shoot_rooms[i]))
            arrow_loc = cave_ptr->get_adjacents(arrow_loc->number)[rand() % count];
        if (arrow_loc == cave_ptr->get_wumpus_loc())
            return After_shooting_state::wumpus_shot;
        if (arrow_loc == cave_ptr->get_player_loc())
            return After_shooting_state::player_shot;
    }
    if (cave_ptr->get_arrows() == 0)
        return After_shooting_state::no_arrows;
    return After_shooting_state::wumpus_awakened;
}

void Wumpus_window::shoot_player()
{
    if (game_over)
        return;
    After_shooting_state state = shoot();
    ostringstream os;
    switch (state)
    {
    case wumpus_shot:
        os << "Круто! Ви підстрелили Вампуса! Вітаю з перемогою!";
        break;
    case player_shot:
        os << "Ай! Стріла влучила у Вас! Ви програли!";
        break;
    case no_arrows:
        os << "У Вас більше немає стріл! Ви програли!";
    default: // wumpus awakened
        cave_ptr->wumpus_woke_up();
        break;
    }
    if (state != wumpus_awakened)
    {
        hazard_state_text.set_label(os.str());
        if (state == wumpus_shot)
            hazard_state_text.set_color(Color::dark_green);
        game_over = true;
    }
    else
        hazard_caught();
    redraw();
}

void Wumpus_window::clear_rooms()
{
    shoot_box1.put("");
    shoot_box2.put("");
    shoot_box3.put("");
}

void Wumpus_window::clear_all_fields()
{
    clear_rooms();
    bats_note.put("");
    pits_note.put("");
    wumpus_note.put("");
}

void Wumpus_window::play_again()
{
    game_over = false;
    Point first_room = cave_ptr->get_player_loc()->coordinates;
    cave_ptr->shuffle();
    arrows.put(to_string(cave_ptr->get_arrows()));
    Point second_room = cave_ptr->get_player_loc()->coordinates;
    wumpus_map.current_room.move(second_room.x - first_room.x, second_room.y - first_room.y);
    hazard_state_text.set_label("");
    zap_text.set_label("");
    hazard_state_text.set_color(Color::black);
    hazard_warnings();
    clear_all_fields();
    redraw();
}