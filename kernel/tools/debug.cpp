#include <log.h>
#include <system.h>
#include <fs.h> // isn't really finished.. But for now this will be enough.

static inline void term_print(const char* msg) {
  while (*msg) {
    if (msg == '\n') {
      serial_write('\r');
    }

    serial_write(*msg++);
  }
}

static inline void donun() {
  return; // do nothing.
}

void debug_terminal() {
  term_print("AuroraOS - Debugger, type <help>? \r");
  term_print("\033[31m admin@auroraos -> \033[0m");
  char cmd[256];
  serial_readline(cmd, 256);

  // for now i'll hardcode commands.
  if (cmd == "help") {
    term_print("List of available commands: \n ls - list all files in your current directory. \n cd - change into a directory, use .. to go backwards. \n proc - view all active processes. \n run - run a .elf file");
  } else if (strcmp(cmd, "ls") == 0) {
    // ehnmm.. need to do other stuff before, ill ignore this.
    donun();
  } else if (strcmp(cmd, "cd") == 0) {
    // no directories either.
    donun();
  } else if (strcmp(cmd, "proc") == 0) {
    // processes are not fully implemented. no task manager either.
    donun();
    term_print("Tasks:\n There are no current tasks running..");
  } else if (strcmp(cmd, "exit") == 0) {
    for (;;) {}; // run forever.. Do nothing.
  } else if (strcmp(cmd, "run") == 0) {
    // i need processes for this, rn this whole thing is a proof of conecpt.
    donun();
  }

  
}

