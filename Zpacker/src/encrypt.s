;void
;	encrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size)

section .text
	global encrypt

encrypt:
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

encipher:;---------------------; begin encipher subprocedure

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
	xor rax, rax               ; sum = 0

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
;------------------------------; end encipher
	add rsi, 8
	sub r13, 8
__while_size_ge_8:
	cmp r13, 8
	jge encipher

	pop r13                    ; restore preserved registers
	pop r12

	leave
	ret
