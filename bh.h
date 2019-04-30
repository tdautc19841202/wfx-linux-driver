// SPDX-License-Identifier: GPL-2.0-only
/*
 * Interrupt bottom half.
 *
 * Copyright (c) 2017-2019, Silicon Laboratories, Inc.
 * Copyright (c) 2010, ST-Ericsson
 */
#ifndef WFX_BH_H
#define WFX_BH_H

/* extern */ struct wfx_dev;

int wfx_bh_register(struct wfx_dev *wdev);
void wfx_bh_unregister(struct wfx_dev *wdev);
void wfx_bh_request_rx(struct wfx_dev *wdev);
void wfx_bh_request_tx(struct wfx_dev *wdev);

#endif /* WFX_BH_H */
