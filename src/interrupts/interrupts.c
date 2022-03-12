#include <include/interrupts.h>
#include <include/serial.h>
#include <include/port.h>
#include <stdint.h>

#include <lib/string.h>
#include <lib/printf.h>

void load_idt();
void load_isr();
void remap_pic();
void idt_set_gate(uint32_t vector, uint64_t offset);

void interrupts_init(){
    // register ISR gates in the IDT - 0 to 47 for now
    load_isr(); 

    /** The PIC sends IRQ interrupts starting from 0x0,
      * but the CPU uses 0-15 for internal software interrupts. 
      * So we remap the IRQs starting from 0x20 (32). */
    remap_pic(); 

    // load the IDT from idt_ptr
    load_idt();
}

idt_gate_t idt[IDT_ENTRIES];
idt_t idt_ptr;
handler_t isr_handlers[IDT_ENTRIES];

void idt_set_gate(uint32_t vector, uint64_t offset){
    idt[vector].offset_lo = offset & 0xFFFF;
    idt[vector].offset_mid = (offset >> 16) & 0xFFFF;
    idt[vector].offset_hi = (offset >> 32) & 0xFFFFFFFF;

    idt[vector].segment_selector = LIMINE_CS;
    idt[vector].ist_zero = 0x0;
    idt[vector].flags = IDT_FLAGS;
    idt[vector].zero = 0x0;
}

uint64_t isr_handler(uint64_t rsp){
    registers_t* regs = (registers_t*) rsp;
    if(regs->isr_number < 32){
        if(isr_handlers[regs->isr_number] != 0){
            handler_t handler = isr_handlers[regs->isr_number];
            handler(regs);

        }
        else {
            sprint("\nUnhandled interrupt: 0x");
            sprint(itoa(regs->isr_number, 16));
            sprint("\n");
            dprintf("err code: %x", regs->error_code);
            for(;;); // hang because the exception is unhandled
        }

    } else {
        if(isr_handlers[regs->isr_number] != 0){
            handler_t handler = isr_handlers[regs->isr_number];
            handler(regs);
        }
        outb(0x20, 0x20);
        if (regs->isr_number >= 40){
            outb(0xA0, 0x20);
        }
    }
    return rsp;
}

void load_idt(){
    idt_ptr.base = (uint64_t)&idt;
    idt_ptr.limit = sizeof(idt_gate_t) * IDT_ENTRIES - 1;
    asm volatile("lidt %0" : : "m" (idt_ptr));
}

void isr_install_handler(uint32_t num, handler_t handler){
    sprint("\nhandler installed\n");
    isr_handlers[num] = handler;
}
void remap_pic(){
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void load_isr(){
    idt_set_gate(0, (uint64_t) isr0);
    idt_set_gate(1, (uint64_t) isr1);
    idt_set_gate(2, (uint64_t) isr2);
    idt_set_gate(3, (uint64_t) isr3);
    idt_set_gate(4, (uint64_t) isr4);
    idt_set_gate(5, (uint64_t) isr5);
    idt_set_gate(6, (uint64_t) isr6);
    idt_set_gate(7, (uint64_t) isr7);
    idt_set_gate(8, (uint64_t) isr8);
    idt_set_gate(9, (uint64_t) isr9);
    idt_set_gate(10, (uint64_t) isr10);
    idt_set_gate(11, (uint64_t) isr11);
    idt_set_gate(12, (uint64_t) isr12);
    idt_set_gate(13, (uint64_t) isr13);
    idt_set_gate(14, (uint64_t) isr14);
    idt_set_gate(15, (uint64_t) isr15);
    idt_set_gate(16, (uint64_t) isr16);
    idt_set_gate(17, (uint64_t) isr17);
    idt_set_gate(18, (uint64_t) isr18);
    idt_set_gate(19, (uint64_t) isr19);
    idt_set_gate(20, (uint64_t) isr20);
    idt_set_gate(21, (uint64_t) isr21);
    idt_set_gate(22, (uint64_t) isr22);
    idt_set_gate(23, (uint64_t) isr23);
    idt_set_gate(24, (uint64_t) isr24);
    idt_set_gate(25, (uint64_t) isr25);
    idt_set_gate(26, (uint64_t) isr26);
    idt_set_gate(27, (uint64_t) isr27);
    idt_set_gate(28, (uint64_t) isr28);
    idt_set_gate(29, (uint64_t) isr29);
    idt_set_gate(30, (uint64_t) isr30);
    idt_set_gate(31, (uint64_t) isr31);
    idt_set_gate(32, (uint64_t) isr32);
    idt_set_gate(33, (uint64_t) isr33);
    idt_set_gate(34, (uint64_t) isr34);
    idt_set_gate(35, (uint64_t) isr35);
    idt_set_gate(36, (uint64_t) isr36);
    idt_set_gate(37, (uint64_t) isr37);
    idt_set_gate(38, (uint64_t) isr38);
    idt_set_gate(39, (uint64_t) isr39);
    idt_set_gate(40, (uint64_t) isr40);
    idt_set_gate(41, (uint64_t) isr41);
    idt_set_gate(42, (uint64_t) isr42);
    idt_set_gate(43, (uint64_t) isr43);
    idt_set_gate(44, (uint64_t) isr44);
    idt_set_gate(45, (uint64_t) isr45);
    idt_set_gate(46, (uint64_t) isr46);
    idt_set_gate(47, (uint64_t) isr47);
    // idt_set_gate(48, (uint64_t) isr48);
    // idt_set_gate(49, (uint64_t) isr49);
    // idt_set_gate(50, (uint64_t) isr50);
    // idt_set_gate(51, (uint64_t) isr51);
    // idt_set_gate(52, (uint64_t) isr52);
    // idt_set_gate(53, (uint64_t) isr53);
    // idt_set_gate(54, (uint64_t) isr54);
    // idt_set_gate(55, (uint64_t) isr55);
    // idt_set_gate(56, (uint64_t) isr56);
    // idt_set_gate(57, (uint64_t) isr57);
    // idt_set_gate(58, (uint64_t) isr58);
    // idt_set_gate(59, (uint64_t) isr59);
    // idt_set_gate(60, (uint64_t) isr60);
    // idt_set_gate(61, (uint64_t) isr61);
    // idt_set_gate(62, (uint64_t) isr62);
    // idt_set_gate(63, (uint64_t) isr63);
    // idt_set_gate(64, (uint64_t) isr64);
    // idt_set_gate(65, (uint64_t) isr65);
    // idt_set_gate(66, (uint64_t) isr66);
    // idt_set_gate(67, (uint64_t) isr67);
    // idt_set_gate(68, (uint64_t) isr68);
    // idt_set_gate(69, (uint64_t) isr69);
    // idt_set_gate(70, (uint64_t) isr70);
    // idt_set_gate(71, (uint64_t) isr71);
    // idt_set_gate(72, (uint64_t) isr72);
    // idt_set_gate(73, (uint64_t) isr73);
    // idt_set_gate(74, (uint64_t) isr74);
    // idt_set_gate(75, (uint64_t) isr75);
    // idt_set_gate(76, (uint64_t) isr76);
    // idt_set_gate(77, (uint64_t) isr77);
    // idt_set_gate(78, (uint64_t) isr78);
    // idt_set_gate(79, (uint64_t) isr79);
    // idt_set_gate(80, (uint64_t) isr80);
    // idt_set_gate(81, (uint64_t) isr81);
    // idt_set_gate(82, (uint64_t) isr82);
    // idt_set_gate(83, (uint64_t) isr83);
    // idt_set_gate(84, (uint64_t) isr84);
    // idt_set_gate(85, (uint64_t) isr85);
    // idt_set_gate(86, (uint64_t) isr86);
    // idt_set_gate(87, (uint64_t) isr87);
    // idt_set_gate(88, (uint64_t) isr88);
    // idt_set_gate(89, (uint64_t) isr89);
    // idt_set_gate(90, (uint64_t) isr90);
    // idt_set_gate(91, (uint64_t) isr91);
    // idt_set_gate(92, (uint64_t) isr92);
    // idt_set_gate(93, (uint64_t) isr93);
    // idt_set_gate(94, (uint64_t) isr94);
    // idt_set_gate(95, (uint64_t) isr95);
    // idt_set_gate(96, (uint64_t) isr96);
    // idt_set_gate(97, (uint64_t) isr97);
    // idt_set_gate(98, (uint64_t) isr98);
    // idt_set_gate(99, (uint64_t) isr99);
    // idt_set_gate(100, (uint64_t) isr100);
    // idt_set_gate(101, (uint64_t) isr101);
    // idt_set_gate(102, (uint64_t) isr102);
    // idt_set_gate(103, (uint64_t) isr103);
    // idt_set_gate(104, (uint64_t) isr104);
    // idt_set_gate(105, (uint64_t) isr105);
    // idt_set_gate(106, (uint64_t) isr106);
    // idt_set_gate(107, (uint64_t) isr107);
    // idt_set_gate(108, (uint64_t) isr108);
    // idt_set_gate(109, (uint64_t) isr109);
    // idt_set_gate(110, (uint64_t) isr110);
    // idt_set_gate(111, (uint64_t) isr111);
    // idt_set_gate(112, (uint64_t) isr112);
    // idt_set_gate(113, (uint64_t) isr113);
    // idt_set_gate(114, (uint64_t) isr114);
    // idt_set_gate(115, (uint64_t) isr115);
    // idt_set_gate(116, (uint64_t) isr116);
    // idt_set_gate(117, (uint64_t) isr117);
    // idt_set_gate(118, (uint64_t) isr118);
    // idt_set_gate(119, (uint64_t) isr119);
    // idt_set_gate(120, (uint64_t) isr120);
    // idt_set_gate(121, (uint64_t) isr121);
    // idt_set_gate(122, (uint64_t) isr122);
    // idt_set_gate(123, (uint64_t) isr123);
    // idt_set_gate(124, (uint64_t) isr124);
    // idt_set_gate(125, (uint64_t) isr125);
    // idt_set_gate(126, (uint64_t) isr126);
    // idt_set_gate(127, (uint64_t) isr127);
    // idt_set_gate(128, (uint64_t) isr128);
    // idt_set_gate(129, (uint64_t) isr129);
    // idt_set_gate(130, (uint64_t) isr130);
    // idt_set_gate(131, (uint64_t) isr131);
    // idt_set_gate(132, (uint64_t) isr132);
    // idt_set_gate(133, (uint64_t) isr133);
    // idt_set_gate(134, (uint64_t) isr134);
    // idt_set_gate(135, (uint64_t) isr135);
    // idt_set_gate(136, (uint64_t) isr136);
    // idt_set_gate(137, (uint64_t) isr137);
    // idt_set_gate(138, (uint64_t) isr138);
    // idt_set_gate(139, (uint64_t) isr139);
    // idt_set_gate(140, (uint64_t) isr140);
    // idt_set_gate(141, (uint64_t) isr141);
    // idt_set_gate(142, (uint64_t) isr142);
    // idt_set_gate(143, (uint64_t) isr143);
    // idt_set_gate(144, (uint64_t) isr144);
    // idt_set_gate(145, (uint64_t) isr145);
    // idt_set_gate(146, (uint64_t) isr146);
    // idt_set_gate(147, (uint64_t) isr147);
    // idt_set_gate(148, (uint64_t) isr148);
    // idt_set_gate(149, (uint64_t) isr149);
    // idt_set_gate(150, (uint64_t) isr150);
    // idt_set_gate(151, (uint64_t) isr151);
    // idt_set_gate(152, (uint64_t) isr152);
    // idt_set_gate(153, (uint64_t) isr153);
    // idt_set_gate(154, (uint64_t) isr154);
    // idt_set_gate(155, (uint64_t) isr155);
    // idt_set_gate(156, (uint64_t) isr156);
    // idt_set_gate(157, (uint64_t) isr157);
    // idt_set_gate(158, (uint64_t) isr158);
    // idt_set_gate(159, (uint64_t) isr159);
    // idt_set_gate(160, (uint64_t) isr160);
    // idt_set_gate(161, (uint64_t) isr161);
    // idt_set_gate(162, (uint64_t) isr162);
    // idt_set_gate(163, (uint64_t) isr163);
    // idt_set_gate(164, (uint64_t) isr164);
    // idt_set_gate(165, (uint64_t) isr165);
    // idt_set_gate(166, (uint64_t) isr166);
    // idt_set_gate(167, (uint64_t) isr167);
    // idt_set_gate(168, (uint64_t) isr168);
    // idt_set_gate(169, (uint64_t) isr169);
    // idt_set_gate(170, (uint64_t) isr170);
    // idt_set_gate(171, (uint64_t) isr171);
    // idt_set_gate(172, (uint64_t) isr172);
    // idt_set_gate(173, (uint64_t) isr173);
    // idt_set_gate(174, (uint64_t) isr174);
    // idt_set_gate(175, (uint64_t) isr175);
    // idt_set_gate(176, (uint64_t) isr176);
    // idt_set_gate(177, (uint64_t) isr177);
    // idt_set_gate(178, (uint64_t) isr178);
    // idt_set_gate(179, (uint64_t) isr179);
    // idt_set_gate(180, (uint64_t) isr180);
    // idt_set_gate(181, (uint64_t) isr181);
    // idt_set_gate(182, (uint64_t) isr182);
    // idt_set_gate(183, (uint64_t) isr183);
    // idt_set_gate(184, (uint64_t) isr184);
    // idt_set_gate(185, (uint64_t) isr185);
    // idt_set_gate(186, (uint64_t) isr186);
    // idt_set_gate(187, (uint64_t) isr187);
    // idt_set_gate(188, (uint64_t) isr188);
    // idt_set_gate(189, (uint64_t) isr189);
    // idt_set_gate(190, (uint64_t) isr190);
    // idt_set_gate(191, (uint64_t) isr191);
    // idt_set_gate(192, (uint64_t) isr192);
    // idt_set_gate(193, (uint64_t) isr193);
    // idt_set_gate(194, (uint64_t) isr194);
    // idt_set_gate(195, (uint64_t) isr195);
    // idt_set_gate(196, (uint64_t) isr196);
    // idt_set_gate(197, (uint64_t) isr197);
    // idt_set_gate(198, (uint64_t) isr198);
    // idt_set_gate(199, (uint64_t) isr199);
    // idt_set_gate(200, (uint64_t) isr200);
    // idt_set_gate(201, (uint64_t) isr201);
    // idt_set_gate(202, (uint64_t) isr202);
    // idt_set_gate(203, (uint64_t) isr203);
    // idt_set_gate(204, (uint64_t) isr204);
    // idt_set_gate(205, (uint64_t) isr205);
    // idt_set_gate(206, (uint64_t) isr206);
    // idt_set_gate(207, (uint64_t) isr207);
    // idt_set_gate(208, (uint64_t) isr208);
    // idt_set_gate(209, (uint64_t) isr209);
    // idt_set_gate(210, (uint64_t) isr210);
    // idt_set_gate(211, (uint64_t) isr211);
    // idt_set_gate(212, (uint64_t) isr212);
    // idt_set_gate(213, (uint64_t) isr213);
    // idt_set_gate(214, (uint64_t) isr214);
    // idt_set_gate(215, (uint64_t) isr215);
    // idt_set_gate(216, (uint64_t) isr216);
    // idt_set_gate(217, (uint64_t) isr217);
    // idt_set_gate(218, (uint64_t) isr218);
    // idt_set_gate(219, (uint64_t) isr219);
    // idt_set_gate(220, (uint64_t) isr220);
    // idt_set_gate(221, (uint64_t) isr221);
    // idt_set_gate(222, (uint64_t) isr222);
    // idt_set_gate(223, (uint64_t) isr223);
    // idt_set_gate(224, (uint64_t) isr224);
    // idt_set_gate(225, (uint64_t) isr225);
    // idt_set_gate(226, (uint64_t) isr226);
    // idt_set_gate(227, (uint64_t) isr227);
    // idt_set_gate(228, (uint64_t) isr228);
    // idt_set_gate(229, (uint64_t) isr229);
    // idt_set_gate(230, (uint64_t) isr230);
    // idt_set_gate(231, (uint64_t) isr231);
    // idt_set_gate(232, (uint64_t) isr232);
    // idt_set_gate(233, (uint64_t) isr233);
    // idt_set_gate(234, (uint64_t) isr234);
    // idt_set_gate(235, (uint64_t) isr235);
    // idt_set_gate(236, (uint64_t) isr236);
    // idt_set_gate(237, (uint64_t) isr237);
    // idt_set_gate(238, (uint64_t) isr238);
    // idt_set_gate(239, (uint64_t) isr239);
    // idt_set_gate(240, (uint64_t) isr240);
    // idt_set_gate(241, (uint64_t) isr241);
    // idt_set_gate(242, (uint64_t) isr242);
    // idt_set_gate(243, (uint64_t) isr243);
    // idt_set_gate(244, (uint64_t) isr244);
    // idt_set_gate(245, (uint64_t) isr245);
    // idt_set_gate(246, (uint64_t) isr246);
    // idt_set_gate(247, (uint64_t) isr247);
    // idt_set_gate(248, (uint64_t) isr248);
    // idt_set_gate(249, (uint64_t) isr249);
    // idt_set_gate(250, (uint64_t) isr250);
    // idt_set_gate(251, (uint64_t) isr251);
    // idt_set_gate(252, (uint64_t) isr252);
    // idt_set_gate(253, (uint64_t) isr253);
    // idt_set_gate(254, (uint64_t) isr254);
    // idt_set_gate(255, (uint64_t) isr255);
}

