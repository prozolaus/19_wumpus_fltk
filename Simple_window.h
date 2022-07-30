
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#ifndef SIMPLE_WINDOW_GUARD
#define SIMPLE_WINDOW_GUARD 1

#include "GUI.h" // for Simple_window only (doesn't really belong in Window.h)
#include "Graph.h"

using namespace Graph_lib;
using namespace Wumpus_lib;

//------------------------------------------------------------------------------

struct Simple_window : Graph_lib::Window
{
  Simple_window(Point xy, int w, int h, const string &title);

  void wait_for_button(); // simple event loop

private:
  Button next_button; // the "next" button
  bool button_pushed; // implementation detail

  static void cb_next(Address, Address); // callback for next_button
  void next();                           // action to be done when next_button is pressed
};

//------------------------------------------------------------------------------

struct Wumpus_window : Graph_lib::Window
{
  Wumpus_window(Point xy, int w, int h, const string &title)
      : Window(xy, w, h, title),
        cave_ptr{&wumpus_map.cave},
        arrows{Point{x_max() - 360, 69}, 20, 20, "Arrows:"},
        instructions_box{Point{Wumpus_lib::x_indent / 2, Wumpus_lib::y_indent / 2}, x_max() / 2, y_max(), ""},
        shoot_box1{Point{x_max() - 250, 30}, 50, 20, "Room #1"},
        shoot_box2{Point{x_max() - 250, 50}, 50, 20, "Room #2"},
        shoot_box3{Point{x_max() - 250, 70}, 50, 20, "Room #3"},
        notes{Point{x_max() - 350, 120}, 50, 20, ""},
        bats_note{Point{x_max() - 380, 150}, 130, 20, "Bats:"},
        pits_note{Point{x_max() - 380, 180}, 130, 20, "Pits:"},
        wumpus_note{Point{x_max() - 380, 210}, 130, 20, "Wumpus:"},
        zap_text{Point{50, 30}, ""},
        hazard_state_text{Point{50, 50}, ""},
        warn_text_bats{Point{x_max() - 230, 300}, ""},
        warn_text_pits{Point{x_max() - 230, 320}, ""},
        warn_text_wumpus{Point{x_max() - 230, 340}, ""},
        move_text{Point{x_max() - 210, 120}, 120, 20, ""},
        room1{Point{x_max() - 230, 150}, 30, 20, "1",
              [](Address, Address pw)
              {
                reference_to<Wumpus_window>(pw).move_to_room(1);
              }},
        room2{Point{x_max() - 200, 150}, 30, 20, "2",
              [](Address, Address pw)
              {
                reference_to<Wumpus_window>(pw).move_to_room(2);
              }},
        room3{Point{x_max() - 170, 150}, 30, 20, "3",
              [](Address, Address pw)
              {
                reference_to<Wumpus_window>(pw).move_to_room(3);
              }},
        room4{Point{x_max() - 140, 150}, 30, 20, "4",
              [](Address, Address pw)
              {
                reference_to<Wumpus_window>(pw).move_to_room(4);
              }},
        room5{Point{x_max() - 110, 150}, 30, 20, "5",
              [](Address, Address pw)
              {
                reference_to<Wumpus_window>(pw).move_to_room(5);
              }},
        room6{Point{x_max() - 230, 170}, 30, 20, "6",
              [](Address, Address pw)
              {
                reference_to<Wumpus_window>(pw).move_to_room(6);
              }},
        room7{Point{x_max() - 200, 170}, 30, 20, "7",
              [](Address, Address pw)
              {
                reference_to<Wumpus_window>(pw).move_to_room(7);
              }},
        room8{Point{x_max() - 170, 170}, 30, 20, "8",
              [](Address, Address pw)
              {
                reference_to<Wumpus_window>(pw).move_to_room(8);
              }},
        room9{Point{x_max() - 140, 170}, 30, 20, "9",
              [](Address, Address pw)
              {
                reference_to<Wumpus_window>(pw).move_to_room(9);
              }},
        room10{Point{x_max() - 110, 170}, 30, 20, "10",
               [](Address, Address pw)
               {
                 reference_to<Wumpus_window>(pw).move_to_room(10);
               }},
        room11{Point{x_max() - 230, 190}, 30, 20, "11",
               [](Address, Address pw)
               {
                 reference_to<Wumpus_window>(pw).move_to_room(11);
               }},
        room12{Point{x_max() - 200, 190}, 30, 20, "12",
               [](Address, Address pw)
               {
                 reference_to<Wumpus_window>(pw).move_to_room(12);
               }},
        room13{Point{x_max() - 170, 190}, 30, 20, "13",
               [](Address, Address pw)
               {
                 reference_to<Wumpus_window>(pw).move_to_room(13);
               }},
        room14{Point{x_max() - 140, 190}, 30, 20, "14",
               [](Address, Address pw)
               {
                 reference_to<Wumpus_window>(pw).move_to_room(14);
               }},
        room15{Point{x_max() - 110, 190}, 30, 20, "15",
               [](Address, Address pw)
               {
                 reference_to<Wumpus_window>(pw).move_to_room(15);
               }},
        room16{Point{x_max() - 230, 210}, 30, 20, "16",
               [](Address, Address pw)
               {
                 reference_to<Wumpus_window>(pw).move_to_room(16);
               }},
        room17{Point{x_max() - 200, 210}, 30, 20, "17",
               [](Address, Address pw)
               {
                 reference_to<Wumpus_window>(pw).move_to_room(17);
               }},
        room18{Point{x_max() - 170, 210}, 30, 20, "18",
               [](Address, Address pw)
               {
                 reference_to<Wumpus_window>(pw).move_to_room(18);
               }},
        room19{Point{x_max() - 140, 210}, 30, 20, "19",
               [](Address, Address pw)
               {
                 reference_to<Wumpus_window>(pw).move_to_room(19);
               }},
        room20{Point{x_max() - 110, 210}, 30, 20, "20",
               [](Address, Address pw)
               {
                 reference_to<Wumpus_window>(pw).move_to_room(20);
               }},
        shoot_button{Point{x_max() - 200, 50}, 70, 20, "Shoot",
                     [](Address, Address pw)
                     {
                       reference_to<Wumpus_window>(pw).shoot_player();
                     }},
        again_button{Point{x_max() - 100, 50}, 100, 20, "Play again",
                     [](Address, Address pw)
                     {
                       reference_to<Wumpus_window>(pw).play_again();
                     }},
        instr_button{Point{x_max() - 100, 80}, 100, 20, "Show rules",
                     [](Address, Address pw)
                     {
                       reference_to<Wumpus_window>(pw).instructions();
                     }},
        quit_button{Point{x_max() - 70, 0}, 70, 20, "Quit",
                    [](Address, Address pw)
                    {
                      reference_to<Wumpus_window>(pw).quit();
                    }},
        game_over{false}
  {
    attach(shoot_box1);
    attach(shoot_box2);
    attach(shoot_box3);
    attach(notes);
    attach(bats_note);
    attach(pits_note);
    attach(wumpus_note);
    attach(arrows);
    arrows.put(to_string(cave_ptr->get_arrows()));
    attach(zap_text);
    attach(hazard_state_text);
    attach(warn_text_bats);
    attach(warn_text_pits);
    attach(warn_text_wumpus);
    attach(move_text);
    attach(room1);
    attach(room2);
    attach(room3);
    attach(room4);
    attach(room5);
    attach(room6);
    attach(room7);
    attach(room8);
    attach(room9);
    attach(room10);
    attach(room11);
    attach(room12);
    attach(room13);
    attach(room14);
    attach(room15);
    attach(room16);
    attach(room17);
    attach(room18);
    attach(room19);
    attach(room20);
    attach(shoot_button);
    attach(again_button);
    attach(instr_button);
    attach(quit_button);
    attach(wumpus_map);
    attach(instructions_box);
    warn_text_bats.set_color(Color::dark_blue);
    warn_text_pits.set_color(Color::dark_red);
    warn_text_wumpus.set_color(Color::red);
    hazard_warnings();
    instructions_box.set_textsize(12);
    instructions_box.hide();
    move_text.put("Move to room #");
    move_text.hide();
    notes.put("Notes");
    notes.hide();
  }

private:
  Wumpus_map wumpus_map;
  Wumpus_lib::Cave *cave_ptr;
  In_box shoot_box1, shoot_box2, shoot_box3;
  In_box bats_note, pits_note, wumpus_note;
  Out_box arrows, move_text, notes;
  Multiline_out_box instructions_box;
  Text hazard_state_text, zap_text;
  Text warn_text_bats, warn_text_pits, warn_text_wumpus;
  Button room1, room2, room3, room4, room5;
  Button room6, room7, room8, room9, room10;
  Button room11, room12, room13, room14, room15;
  Button room16, room17, room18, room19, room20;
  Button shoot_button;
  Button again_button;
  Button instr_button;
  Button quit_button;
  bool game_over;
  void instructions();
  void move_to_room(int);
  void shoot_player();
  void clear_fields();
  void play_again();
  After_shooting_state shoot();
  void hazard_caught();
  void hazard_warnings();
  void quit() { Window::hide(); }
};

//------------------------------------------------------------------------------

#endif // SIMPLE_WINDOW_GUARD
