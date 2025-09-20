[BITS 16]
ORG 0x7C00
start:
 cli
 xor ax, ax
 mov ss, ax
 mov sp, 0x7C00
 ; ENABLE A20
 in al, 0x64
.wait_status:
 test al, 2
 jnz .wait_status
 mov al, 0xD1
 out 0x64, al
.wait2:
 in al, 0x64
 test al, 2
 jnz .wait2
 mov al, 0xDF
 out 0x60, al
 call delay
 ; Mask PIC 
 mov al, 0xFF
 out 0x21, al
 out 0xA1, al
 ; Загрузка GDT
 lgdt [gdt_descriptor]
 ; Включение Protected Mode
 mov eax, cr0
 or eax, 1
 mov cr0, eax
 ; Far jump в Protected Mode
 jmp 0x08:protected_mode_entry
; -------------------------
gdt_start:
 dq 0x0000000000000000
 dq 0x00CF9A000000FFFF
 dq 0x00CF92000000FFFF
gdt_end:
gdt_descriptor:
 dw gdt_end - gdt_start - 1
 dd gdt_start
; -------------------------
[BITS 32]
protected_mode_entry:
 mov ax, 0x10
 mov ds, ax
 mov es, ax
 mov fs, ax
 mov gs, ax
 mov ss, ax
 mov esp, 0x90000
 ; Сообщение в видеопамять
 mov edi, 0xB8000
 mov esi, msg_pm
.print_pm:
 lodsb
 test al, al
 jz .done_pm
 mov ah, 0x07
 stosw
 jmp .print_pm
.done_pm:
 call delay
 ; Выключение Protected Mode
 cli
 mov eax, cr0
 and eax, 0xFFFFFFFE
 mov cr0, eax
 ; Настройка сегментов для Real Mode
 mov ax, 0
 mov ds, ax
 mov es, ax
 mov ss, ax 
 mov sp, 0x7C00
 ; Far jump обратно в Real Mode
 jmp 0x0000:rm_entry
; -------------------------
[BITS 16]
rm_entry:
 sti
 mov ah, 0x00
 mov al, 0x03 ; Текстовый режим 80x25
 int 0x10
 mov si, msg_rm
.print_rm:
 lodsb
 or al, al
 jz .done_rm
 mov ah, 0x0E
 int 0x10
 jmp .print_rm
.done_rm:
.halt_loop:
 hlt
 jmp .halt_loop
; -------------------------
msg_pm db "Hewwo from pwotected mode! "
msg_rm db "Back to Real Mode!",0
; -------------------------
delay:
 pusha
 mov cx, 0xFFFF
.loop:
 loop .loop
 popa
 ret
; -------------------------
times 510-($-$$) db 0
dw 0xAA55
