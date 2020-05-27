%define SYSCALL_WRITE		0x1
%define SYSCALL_MPROTECT	0xa
%define STDOUT			0x1
%define PROT_RWX		0x7
%define CALL_INSTR_SIZE		0x5

section .text
	global load_begin
	global load_end

load_begin:
;------------------------------; Store variables
	call mark_below
	db "128 bit key here", "rel ptld", "ptldsize", "rel text"
	db "relentry", "textsize"
;------------------------------; Get variables address
; | 0    | *(16)       | *24         | *(32)       | *(40)        | *48        |
; | rdx  | r8          | r9          | r10         | r11          | r14        |
; | key  | rel ptld    | ptld size   | rel text    | rel entry    | text size  |
; | key  | (ptld addr) | (ptld size) | (text addr) | (entry addr) | (text size)|
mark_below:
	pop rax
	push rdx                   ; backup rdx
	push r14                   ; backup r14

	mov rdx, rax

	mov r8, rdx
	mov r9, rdx
	mov r10, rdx
	mov r11, rdx
	mov r14, rdx
	add r8, 16                 ; align vars to correct addresses
	add r9, 24
	add r10, 32
	add r11, 40
	add r14, 48
	mov r8, [r8]               ; dereference vars
	mov r9, [r9]
	mov r10, [r10]
	mov r11, [r11]
	mov r14, [r14]

	mov rax, rdx               ; get begin_payload addr
	sub rax, CALL_INSTR_SIZE

	push r15                   ; backup r15
	mov r15, rax
	xchg r15, r8
	sub r8, r15                ; r8 = r8-rax
	mov r15, rax
	xchg r15, r10
	sub r10, r15               ; r10 = rax - r10
	mov r15, rax
	xchg r15, r11
	sub r11, r15               ; r11 = rax - r11
	pop r15                    ; restore r15

	push rax                   ; save begin_payload [rsp + 40]
	push r8                    ; save ptld addr     [rsp + 32]
	push r9                    ; save ptld size     [rsp + 24]
	push r10                   ; save text addr     [rsp + 16]
	push r11                   ; save entry addr    [rsp + 8]
	push rdx                   ; save key           [rsp]
;------------------------------; Show-off
	mov rax, "zpacker:"
	push rax

	; write(1, "zpacker:", 8);
	mov rdi, STDOUT
	mov rsi, rsp
	mov rdx, 8
	mov rax, SYSCALL_WRITE
	syscall

	add rsp, 8
;------------------------------; make ptld writable
	mov r8, [rsp + 32]         ; get ptld addr
	mov r9, [rsp + 24]         ; get ptld len

	;mprotect(ptld_addr, ptld_size, PROT_READ | PROT_WRITE | PROT_EXEC);
	mov rdi, r8
	mov rsi, r9
	mov rdx, PROT_RWX
	mov rax, SYSCALL_MPROTECT
	syscall

;------------------------------; decrypt text
	mov rdx, [rsp]             ; get key
	mov r10, [rsp + 16]        ; get text_addr

	mov rax, r14               ; get text_size

	;decrypt(32, text_addr, key, text_size);
	mov rdi, 32
	mov rsi, r10
	mov rdx, rdx
	mov rcx, rax
	call decrypt
;------------------------------; return to text
	mov r11, [rsp + 8]         ; get entry addr
	add rsp, 48                ; restore stack as it was
	pop r14                    ; restore r14
	pop rdx                    ; restore rdx
	push r11
	ret

;void
;	decrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size)

decrypt:
	push rbp
	mov rbp, rsp
	push r12                   ; backup preserved registers
	push r13

	; | mut  | mut  | const | const | const      |
	; | rsi  | r13  | rdx   | rdi   | r12        |
	; | data | size | key   | delta | num_rounds |
	mov r12, rdi
	mov r13, rcx
	mov rdi, 0x9E3779B9        ; init constant delta

	jmp __while_size_ge_8

decipher:;---------------------; begin decipher subprocedure

	; (registers above are preserved and const)
	; | mut | const (* mut) | mut   | mut | mut | mut | mut |
	; | rax | rsi   ([rsi]) | rcx   | r8  | r9  | r10 | r11 |
	; | sum | v     ( v[] ) | count | ..  | ..  | v0  | v1  |
	mov r11, [rsi]
	mov r10, r11
	shr r11, 32                ; v0 = v[0]
	mov rax, 0xffffffff
	and r10, rax               ; v1 = v[1]
	mov rcx, r12               ; count = num_rounds
	mov rax, rdi
	imul rax, rcx              ; sum = delta * num_rounds

	mov rax, 0xffffffff

	xor r11, rax
	xor r10, rax

	xor rax,rax
 ; load result back in v

	mov rax, r11               ; [.... 0000] rax
	shl rax, 32                ; [0000 ....] rax
	mov rcx, 0xffffffff
	and r10, rcx               ; [.... 1111] r10
	or  rax, r10               ; [0000 1111] rax | r10
	mov [rsi], rax             ; store result in v
;------------------------------; end decipher
	add rsi, 8
	sub r13, 8
__while_size_ge_8:
	cmp r13, 8
	jge decipher

	pop r13                    ; restore preserved registers
	pop r12

	leave
	ret
load_end:
