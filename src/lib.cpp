#include <emscripten/bind.h>
#include <stdio.h>
#include <thread>

void print_from_thread() {
  std::thread([] { printf("Printed from a thread.\n"); }).join();
}

EMSCRIPTEN_BINDINGS(module) {
  emscripten::function("print_from_thread", &print_from_thread);
}