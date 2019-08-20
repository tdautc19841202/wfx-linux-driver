/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * O(1) TX queue with built-in allocator.
 *
 * Copyright (c) 2017-2018, Silicon Laboratories, Inc.
 * Copyright (c) 2010, ST-Ericsson
 */
#ifndef WFX_QUEUE_H
#define WFX_QUEUE_H

#include "wsm_cmd_api.h"

/* extern */ struct sk_buff;
/* extern */ struct wfx_dev;
/* extern */ struct wfx_vif;
/* extern */ struct ieee80211_tx_queue_stats;
/* extern */ struct wfx_txpriv;

/* forward */ struct wfx_queue_stats;

typedef void (*wfx_queue_skb_dtor_t)(struct wfx_dev *wdev,
					struct sk_buff *skb,
					const struct wfx_txpriv *txpriv);

struct wfx_txpriv {
	u8 link_id;
	u8 raw_link_id;
	u8 tid;
	u8 rate_id;
	u8 offset;
	u8 vif_id;
};

struct wfx_queue_item {
	struct list_head	head;
	struct sk_buff		*skb;
	u32			packet_id;
	unsigned long		queue_timestamp;
	ktime_t			xmit_timestamp;
	struct wfx_txpriv	txpriv;
	u8			generation;
};

struct wfx_queue {
	struct wfx_queue_stats *stats;
	size_t			capacity;
	size_t			num_queued;
	size_t			num_pending;
	size_t			num_sent;
	struct wfx_queue_item *pool;
	struct list_head	queue;
	struct list_head	free_pool;
	struct list_head	pending;
	int			tx_locked_cnt;
	int			*link_map_cache;
	bool			overfull;
	spinlock_t		lock; /* Protect queue entry */
	u8			queue_id;
	u8			generation;
	struct timer_list	gc;
	unsigned long		ttl;
};

struct wfx_queue_stats {
	spinlock_t		lock; /* Protect stats entry */
	int			*link_map_cache;
	int			num_queued;
	size_t			map_capacity;
	wait_queue_head_t	wait_link_id_empty;
	wfx_queue_skb_dtor_t	skb_dtor;
	struct wfx_dev		*wdev;
};

int wfx_queue_stats_init(struct wfx_queue_stats *stats,
			    size_t map_capacity,
			    wfx_queue_skb_dtor_t skb_dtor,
			    struct wfx_dev *wdev);
int wfx_queue_init(struct wfx_queue *queue,
		      struct wfx_queue_stats *stats,
		      u8 queue_id,
		      size_t capacity,
		      unsigned long ttl);

void wfx_queue_wait_empty_vif(struct wfx_vif *wvif);

int wfx_queue_clear(struct wfx_queue *queue);
void wfx_queue_stats_deinit(struct wfx_queue_stats *stats);
void wfx_queue_deinit(struct wfx_queue *queue);

size_t wfx_queue_get_num_queued(struct wfx_queue *queue,
				   u32 link_id_map);
int wfx_queue_put(struct wfx_queue *queue,
		     struct sk_buff *skb,
		     struct wfx_txpriv *txpriv);
int wfx_queue_get(struct wfx_queue *queue,
		     u32 link_id_map,
		     struct wmsg **tx,
		     struct ieee80211_tx_info **tx_info,
		     const struct wfx_txpriv **txpriv);
int wfx_queue_requeue(struct wfx_queue *queue, u32 packet_id);
int wfx_queue_remove(struct wfx_queue *queue,
			u32 packet_id);
int wfx_queue_get_skb(struct wfx_queue *queue, u32 packet_id,
			 struct sk_buff **skb,
			 const struct wfx_txpriv **txpriv);
void wfx_queue_lock(struct wfx_queue *queue);
void wfx_queue_unlock(struct wfx_queue *queue);
unsigned wfx_queue_get_pkt_us_delay(struct wfx_queue *queue, u32 pkt_id);

bool wfx_queue_stats_is_empty(struct wfx_queue_stats *stats,
				 u32 link_id_map);

void wfx_queue_dump_old_frames(struct wfx_dev *wdev, unsigned limit_ms);

static inline u8 wfx_queue_get_queue_id(u32 packet_id)
{
	return (packet_id >> 16) & 0xFF;
}

static inline u8 wfx_queue_get_generation(u32 packet_id)
{
	return (packet_id >>  8) & 0xFF;
}

#endif /* WFX_QUEUE_H */
