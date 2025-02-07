/*
 * Copyright 2016-2024, Intel Corporation
 * Contributor: Petar Andrić
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*
 * Wrapper function to use with the disassembler.
 * This should allow us to use a different disassembler,
 * without changing the intercept.c source file.
 *
 * The result of disassembling deliberately lacks a lot
 * of information about the instruction seen, to make it
 * easy to interface a new disassembler.
 */

#ifndef INTERCEPT_DISASM_WRAPPER_H
#define INTERCEPT_DISASM_WRAPPER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct intercept_disasm_result {
	const unsigned char *address;

	bool is_set;

	bool is_syscall;

	/* Length in bytes, zero if disasm was not successful. */
	unsigned length;

	/*
	 * Flag marking instructions that have a RIP relative address
	 * as an operand.
	 */
	bool has_ip_relative_opr;
	bool is_abs_jump;

	int32_t rip_disp;
	const unsigned char *rip_ref_addr;

	int16_t a7_set;
	bool is_a7_modified;
	bool is_ra_used;
	uint8_t reg_set;

#ifndef NDEBUG
	/*
	 * Switched to char array instead of pointer because capstone doesn't
	 * allocate (thankfully) space for each instruction's mnemonic, but
	 * it uses the same space, i.e., all surrounding instructions would share
	 * the same string. This is only in DEBUG mode, so we can be generous
	 * with 16 B.
	 */
	char mnemonic[16];
#endif
};

struct intercept_disasm_context;

struct intercept_disasm_context *
intercept_disasm_init(const unsigned char *begin, const unsigned char *end);

void intercept_disasm_destroy(struct intercept_disasm_context *context);

struct intercept_disasm_result
intercept_disasm_next_instruction(struct intercept_disasm_context *context,
					const unsigned char *code);

#endif
