/*
 * minimal-keys diag6: which work item is each work queue running right now.
 *
 * Written by work_queue_main() (kernel/work.c) around every handler call,
 * read by a timer ISR outside the kernel. Diagnostic builds only.
 */

#ifndef ZEPHYR_INCLUDE_KERNEL_MK2_SYSWQ6_H_
#define ZEPHYR_INCLUDE_KERNEL_MK2_SYSWQ6_H_

#include <stdint.h>

#ifdef CONFIG_MK2_SYSWQ6

struct k_work_q;
struct k_work;

#define Z_MK2_WQ_SLOTS 8

/*
 * One slot per work queue thread, claimed once when the thread starts.
 *
 * Writer order (thread): start_ms -> work -> handler before the call,
 * handler = NULL after it. Reader order (ISR): handler -> start_ms -> work.
 * A reader that sees a non-NULL handler therefore sees a start_ms that is
 * at least as new as that handler. The ISR preempts the writer, never the
 * other way round, so no lock is needed on either side.
 */
struct z_mk2_wq_slot {
	struct k_work_q *volatile q;
	struct k_work *volatile work;
	void *volatile handler;
	volatile uint32_t start_ms;
};

extern struct z_mk2_wq_slot z_mk2_wq_slots[Z_MK2_WQ_SLOTS];

/* Queues that started after all slots were taken. Should stay 0. */
extern volatile uint32_t z_mk2_wq_slot_overflow;

#endif /* CONFIG_MK2_SYSWQ6 */

#endif /* ZEPHYR_INCLUDE_KERNEL_MK2_SYSWQ6_H_ */
