#include <log.h>
#include <system.h>
#include <fs.h> // isn't really finished.. But for now this will be enough.

static inline void term_print(const char* msg) {
  while (*msg) {
    if (msg == "\n") {
      serial_write("\r");
    }

    serial_write(*msg++);
  }
}

static inline void donun() {
  return; // do nothing.
}

void debug_terminal() {
  term_print("AuroraOS - Debugger, type <help>? ");
  term_print("\033[31m admin@auroraos -> \033[0m");
  const char* cmd = serial_read();

  // for now i'll hardcode commands.
  if (cmd == 'help') {
    term_print("List of available commands: \n ls - list all files in your current directory. \n cd - change into a directory, use .. to go backwards. \n proc - view all active processes.");
  } else if (cmd == 'ls') {
    // ehnmm.. need to do other stuff before, ill ignore this.
    donun();
  } else if (cmd == 'cd') {
    // no directories either.
    donun();
  } else if (cmd == 'proc') {
    // processes are not fully implemented. no task manager either.
    donun();
  } else if (cmd == 'exit') {
    for (;;) {}; // run forever.. Do nothing.
  } else if (cmd == 'run') {
    // i need processes for this, rn this whole thing is a proof of conecpt.
  }

  
}

