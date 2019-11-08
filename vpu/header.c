#include <stddef.h>
#include <asm/remoteproc.h>

#include <trace.h>

struct resource_table_hdr {
	struct resource_table		header;

	uint32_t			offset[2];

	struct {
		struct fw_rsc_hdr	header;
		struct fw_rsc_carveout	carveout;
	} carveout;

	struct {
		struct fw_rsc_hdr	header;
		struct fw_rsc_trace	trace;
	} trace;
};

const struct resource_table_hdr resource_table
__attribute__((used, section (".resource_table"))) = {
	.header = {
		.ver = 1,
		.num = 2, /* Number of resources */
	},

	.offset[0] = offsetof(struct resource_table_hdr, carveout),
	.offset[1] = offsetof(struct resource_table_hdr, trace),

	.carveout = {
		.header = {
			.type = RSC_CARVEOUT,
		},
		.carveout = {
			.da = 0xf4000000,
			.pa = 0xf4000000,
			.len = 0x2000,
			.name = "firmware",
		},
	},

	/* Trace resource to printf() into */
	.trace = {
		.header = {
			.type = RSC_TRACE,
		},
		.trace = {
			.da = (uint32_t)trace_buf,
			.len = TRACE_BUFFER_SIZE,
			.name = "trace",
		},
	},
};
