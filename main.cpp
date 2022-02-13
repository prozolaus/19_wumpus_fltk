#include "Simple_window.h"
#include "Graph.h"


int main()
try
{
  Point top_left{0, 0}; //will be top left corner of window
  Wumpus_window win{top_left, 1000, 650, "Hunt the Wumpus"};
  return gui_main();
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