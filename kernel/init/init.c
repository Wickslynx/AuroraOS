void init_kernel() {
    idt_init();
    isr_init();
    fpu_init();
    irq_init();
}