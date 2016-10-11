#include <cstdlib>
#include <iostream>
#include "serverPinball.h"

using namespace std;

int main(int argc, char** argv) {
  ServerPinball *serverPinball = ServerPinball::instance();
  serverPinball->launch();
  delete serverPinball;
  
  return 0;
}

