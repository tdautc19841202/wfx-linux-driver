/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Implementation of mac80211 API.
 *
 * Copyright (c) 2017-2019, Silicon Laboratories, Inc.
 * Copyright (c) 2010, ST-Ericsson
 */
#ifndef WFX_STA_H
#define WFX_STA_H

#include <linux/version.h>
#include <net/mac80211.h>

#include "wsm_cmd_api.h"

struct wfx_dev;
struct wfx_vif;

struct wfx_wsm_event {
	struct list_head link;
	WsmHiEventIndBody_t evt;
};

struct wsm_edca_params {
	/* NOTE: index is a linux queue id. */
	WsmHiEdcaQueueParamsReqBody_t	params[4];
	bool				uapsd_enable[4];
};

struct wfx_grp_addr_table {
	bool enable;
	int num_addresses;
	u8 address_list[8][ETH_ALEN];
};

// mac80211 interface
int wfx_start(struct ieee80211_hw *);
void wfx_stop(struct ieee80211_hw *);
int wfx_config(struct ieee80211_hw *, u32);
int wfx_set_rts_threshold(struct ieee80211_hw *, u32);
u64 wfx_prepare_multicast(struct ieee80211_hw *, struct netdev_hw_addr_list *);
void wfx_configure_filter(struct ieee80211_hw *, unsigned, unsigned *, u64);
int wfx_get_stats(struct ieee80211_hw *, struct ieee80211_low_level_stats *);

int wfx_add_interface(struct ieee80211_hw *, struct ieee80211_vif *);
void wfx_remove_interface(struct ieee80211_hw *, struct ieee80211_vif *);
int wfx_change_interface(struct ieee80211_hw *, struct ieee80211_vif *, enum nl80211_iftype, bool);
void wfx_flush(struct ieee80211_hw *, struct ieee80211_vif *, u32, bool);
int wfx_conf_tx(struct ieee80211_hw *, struct ieee80211_vif *,
		u16, const struct ieee80211_tx_queue_params *);
void wfx_bss_info_changed(struct ieee80211_hw *, struct ieee80211_vif *, struct ieee80211_bss_conf *, u32);
int wfx_hw_scan(struct ieee80211_hw *, struct ieee80211_vif *, struct ieee80211_scan_request *);
int wfx_sta_add(struct ieee80211_hw *, struct ieee80211_vif *, struct ieee80211_sta *);
int wfx_sta_remove(struct ieee80211_hw *, struct ieee80211_vif *, struct ieee80211_sta *);
void wfx_sta_notify(struct ieee80211_hw *, struct ieee80211_vif *,
		    enum sta_notify_cmd, struct ieee80211_sta *);
int wfx_set_key(struct ieee80211_hw *, enum set_key_cmd, struct ieee80211_vif *,
		struct ieee80211_sta *, struct ieee80211_key_conf *);
int wfx_set_tim(struct ieee80211_hw *, struct ieee80211_sta *, bool);

#if (KERNEL_VERSION(4, 4, 0) > LINUX_VERSION_CODE)
int wfx_ampdu_action(struct ieee80211_hw *, struct ieee80211_vif *,
		     enum ieee80211_ampdu_mlme_action, struct ieee80211_sta *,
		     u16, u16 *, u8);
#else
#if (KERNEL_VERSION(4, 4, 69) > LINUX_VERSION_CODE)
int wfx_ampdu_action(struct ieee80211_hw *, struct ieee80211_vif *,
		     enum ieee80211_ampdu_mlme_action, struct ieee80211_sta *,
		     u16, u16 *, u8, bool);
#else
int wfx_ampdu_action(struct ieee80211_hw *, struct ieee80211_vif *, struct ieee80211_ampdu_params *);
#endif
#endif
int wfx_add_chanctx(struct ieee80211_hw *hw, struct ieee80211_chanctx_conf *conf);
void wfx_remove_chanctx(struct ieee80211_hw *hw, struct ieee80211_chanctx_conf *conf);
void wfx_change_chanctx(struct ieee80211_hw *, struct ieee80211_chanctx_conf *, u32 changed);
int wfx_assign_vif_chanctx(struct ieee80211_hw *, struct ieee80211_vif *, struct ieee80211_chanctx_conf *);
void wfx_unassign_vif_chanctx(struct ieee80211_hw *, struct ieee80211_vif *, struct ieee80211_chanctx_conf *);

// WSM Callbacks
void wfx_suspend_resume(struct wfx_vif *wvif, WsmHiSuspendResumeTxIndBody_t *arg);

// Other Helpers
void __wfx_cqm_bssloss_sm(struct wfx_vif *wvif, int init, int good, int bad);
void wfx_cqm_bssloss_sm(struct wfx_vif *wvif, int init, int good, int bad);

void wfx_update_filtering(struct wfx_vif *wvif);
int wfx_set_pm(struct wfx_vif *wvif, const WsmHiSetPmModeReqBody_t *arg);

#endif /* WFX_STA_H */
