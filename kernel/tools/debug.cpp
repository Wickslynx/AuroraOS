#include <log.h>
#include <system.h>
#include <fs.h>

static inline void term_print(const char* msg) {
  while (*msg) {
    if (*msg == '\n') { 
      serial_write('\r');
    }
    serial_write(*msg++);
  }
}

static inline void donun() {
  return; // do nothing.
}

void debug_terminal() {
  term_print("AuroraOS - Debugger, type <help>\r\n");
  

  while (1) {
    term_print("\033[31madmin@auroraos -> \033[0m");
    char cmd[256];
    serial_readline(cmd, 256);
  
    if (strcmp(cmd, "help") == 0) {
      term_print("List of available commands:\r\n");
      term_print("  ls   - list all files in your current directory\r\n");
      term_print("  cd   - change into a directory, use .. to go backwards\r\n");
      term_print("  proc - view all active processes\r\n");
      term_print("  run  - run a .elf file\r\n");
      term_print("  exit - exit terminal\r\n");
    } else if (strcmp(cmd, "ls") == 0) {
      // ehnmm.. need to do other stuff before, ill ignore this.
      term_print("ls: not implemented yet\r\n");
    } else if (strcmp(cmd, "cd") == 0) {
      // no directories either.
      term_print("cd: not implemented yet\r\n");
    } else if (strcmp(cmd, "proc") == 0) {
      // processes are not fully implemented. no task manager either.
      term_print("Tasks:\r\n");
      term_print("There are no current tasks running..\r\n");
    } else if (strcmp(cmd, "exit") == 0) {
      term_print("Exiting terminal...\r\n");
      break; 
    } else if (strcmp(cmd, "run") == 0) {
      // i need processes for this, rn this whole thing is a proof of concept.
      term_print("run: not implemented yet\r\n");
    } else if (strlen(cmd) > 0) { 
      term_print("Unknown command: ");
      term_print(cmd);
      term_print("\r\n");
    }
  }
}
