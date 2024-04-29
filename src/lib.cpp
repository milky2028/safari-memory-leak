#include <stdio.h>
#include <cstddef>
#include <sstream>
#include <string>

size_t files_written = 0;

void write_file() {
  std::stringstream ss;
  ss << "written_from_wasm_" << files_written << ".txt\n";
  auto filename = ss.str();

  FILE* handle = fopen(filename.c_str(), "w");
  if (handle == NULL) {
    printf("Error opening file handle.\n");
    return;
  }

  fprintf(handle, "Some text written to file.");
  fclose(handle);

  printf("File written successfully.\n");
  files_written++;
}

int main() {
  write_file();
}