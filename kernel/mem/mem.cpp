void* operator new(unsigned int size) {
  return (void*) 0x0000; // Placeholder, no fs nor memory system is implemented.
}

void* operator new[](unsigned int size) {
  return new(size);
}

void operator delete(void* ptr) {
  return;
}

void operator delete[](void* ptr) {
  return;
}
