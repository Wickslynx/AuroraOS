void* operator new(unsigned int size) {
  return (void*) 0x0000; // Placeholder, no fs nor memory system is implemented.
}

void* operator new[](unsigned int size) {
  return (void*) 0x0000;
}

void operator delete(void* ptr) {
  (void)ptr;
  return;
}

void operator delete[](void* ptr) {
  (void)ptr;
  return;
}
