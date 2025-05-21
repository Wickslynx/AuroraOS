void* operator new(unsigned int size) {
  return (void*) 0x0;
}

void* operator new[](unsigned int size) {
  return;
}

void operator delete(void* ptr) {
  return;
}

void operator delete[](void* ptr) {
  return;
}
