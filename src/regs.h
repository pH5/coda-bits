/* CODA register layout
 * SPDX-License-Identifier: 0BSD
 */
#ifndef __REGS_H__
#define __REGS_H__

#include <stdint.h>

static const struct reg {
	uint16_t addr;
	char *name;
} regs[] = {
	{ 0x000,  "CODE_RUN"           },
	{ 0x004,  "CODE_DOWN"          },
	{ 0x008,  "HOST_IN_REQ"        },
	{ 0x00c,  "INT_CLEAR"          },
	{ 0x010,  "INT_STATUS"         },
	{ 0x014,  "CODE_RESET"         },
	{ 0x018,  "CUR_PC"             },
	{ 0x024,  "SW_RESET"           },
	{ 0x034,  "SW_RESET_STATUS"    },
	{ 0x100,  "CODE_BUF_ADDR"      },
	{ 0x104,  "WORK_BUF_ADDR"      },
	{ 0x108,  "PARA_BUF_ADDR"      },
	{ 0x10c,  "STREAM_CTRL"        },
	{ 0x110,  "FRAME_MEM_CTRL"     },
	{ 0x114,  "BIT_STREAM_PARAM"   },
	{ 0x118,  "TEMP_BUF_ADDR"      },
	{ 0x11c,  "BIT_RESET_CTRL"     },
	{ 0x120,  "RD_PTR"             },
	{ 0x124,  "WR_PTR"             },
	{ 0x140,  "AXI_SRAM_USE"       },
	{ 0x150,  "FRM_DIS_FLG"        },
	{ 0x160,  "BUSY"               },
	{ 0x164,  "RUN_COMMAND"        },
	{ 0x168,  "RUN_INDEX"          },
	{ 0x16c,  "RUN_COD_STD"        },
	{ 0x170,  "INT_ENABLE"         },
	{ 0x174,  "INT_REASON"         },
	{ 0x178,  "RUN_AUX_STD"        },

	{ 0x1000, "GDI_PRI_RD_PRIO_L"   },
	{ 0x1004, "GDI_PRI_RD_PRIO_H"   },
	{ 0x1008, "GDI_PRI_WR_PRIO_L"   },
	{ 0x100c, "GDI_PRI_WR_PRIO_H"   },
	{ 0x1010, "GDI_PRI_RD_LOCK_CNT" },
	{ 0x1014, "GDI_PRI_WR_LOCK_CNT" },
	{ 0x1018, "GDI_SEC_RD_PRIO_L"   },
	{ 0x101c, "GDI_SEC_RD_PRIO_H"   },
	{ 0x1020, "GDI_SEC_WR_PRIO_L"   },
	{ 0x1024, "GDI_SEC_WR_PRIO_H"   },
	{ 0x1028, "GDI_SEC_RD_LOCK_CNT" },
	{ 0x102c, "GDI_SEC_WR_LOCK_CNT" },
	{ 0x1030, "GDI_SEC_CLIENT_EN"   },
	{ 0x1034, "GDI_CONTROL"         },
	{ 0x1038, "GDI_PIC_INIT_HOST"   },
	{ 0x1060, "GDI_PINFO_REQ"       },
	{ 0x1064, "GDI_PINFO_ACK"       },
	{ 0x1068, "GDI_PINFO_ADDR"      },
	{ 0x106c, "GDI_PINFO_DATA"      },
	{ 0x1070, "GDI_BWB_ENABLE"      },
	{ 0x1074, "GDI_BWB_SIZE"        },
	{ 0x1078, "GDI_BWB_STD_STRUCT"  },
	{ 0x107c, "GDI_BWB_STATUS"      },
	{ 0x1080, "GDI_STATUS"          },
	{ 0x1084, "GDI_DEBUG_0"         },
	{ 0x1088, "GDI_DEBUG_1"         },
	{ 0x108c, "GDI_DEBUG_2"         },
	{ 0x1090, "GDI_DEBUG_3"         },
	{ 0x10a0, "GDI_WPROT_ERR_CLR"   },
	{ 0x10a4, "GDI_WPROT_ERR_RSN"   },
	{ 0x10a8, "GDI_WPROT_ERR_ADR"   },
	{ 0x10ac, "GDI_WPROT_RGN_EN"    },
	{ 0x10b0, "GDI_WPROT_RGN0_STA"  },
	{ 0x10b4, "GDI_WPROT_RGN0_END"  },
	{ 0x10b8, "GDI_WPROT_RGN1_STA"  },
	{ 0x10bc, "GDI_WPROT_RGN1_END"  },
	{ 0x10c0, "GDI_WPROT_RGN2_STA"  },
	{ 0x10c4, "GDI_WPROT_RGN2_END"  },
	{ 0x10c8, "GDI_WPROT_RGN3_STA"  },
	{ 0x10cc, "GDI_WPROT_RGN3_END"  },
	{ 0x10d0, "GDI_WPROT_RGN4_STA"  },
	{ 0x10d4, "GDI_WPROT_RGN4_END"  },
	{ 0x10d8, "GDI_WPROT_RGN5_STA"  },
	{ 0x10dc, "GDI_WPROT_RGN5_END"  },
	{ 0x10e0, "GDI_SIZE_ERR_FLAG"   },
	{ 0x10f0, "GDI_BUS_CTRL"        },
	{ 0x10f4, "GDI_BUS_STATUS"      },
	{ 0x1100, "GDI_ADR_RQ_SIZE_ERR_PRI0" },
	{ 0x1104, "GDI_ADR_RQ_SIZE_ERR_PRI1" },
	{ 0x1108, "GDI_ADR_RQ_SIZE_ERR_PRI2" },
	{ 0x110c, "GDI_ADR_WQ_SIZE_ERR_PRI0" },
	{ 0x1110, "GDI_ADR_WQ_SIZE_ERR_PRI1" },
	{ 0x1114, "GDI_ADR_WQ_SIZE_ERR_PRI2" },
	{ 0x1100, "GDI_ADR_RQ_SIZE_ERR_SEC0" },
	{ 0x1104, "GDI_ADR_RQ_SIZE_ERR_SEC1" },
	{ 0x1108, "GDI_ADR_RQ_SIZE_ERR_SEC2" },
	{ 0x110c, "GDI_ADR_WQ_SIZE_ERR_SEC0" },
	{ 0x1110, "GDI_ADR_WQ_SIZE_ERR_SEC1" },
	{ 0x1114, "GDI_ADR_WQ_SIZE_ERR_SEC2" },
	{ 0x1400, "GDI_INFO_CONTROL"    },
	{ 0x1404, "GDI_INFO_PIC_SIZE"   },
	{ 0x1408, "GDI_INFO_BASE_Y"     },
	{ 0x140c, "GDI_INFO_BASE_CB"    },
	{ 0x1410, "GDI_INFO_BASE_CR"    },
	{ /* sentinel */ }
};

#endif /* __REGS_H__ */
