// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#pragma once

#include "PipeEvent.hxx"
#include "IdleEvent.hxx"
#include "io/uring/Queue.hxx"

namespace Uring {

class Manager final : public Queue {
	PipeEvent event;
	IdleEvent idle_event;

public:
	explicit Manager(EventLoop &event_loop,
			 unsigned entries=1024, unsigned flags=0)
		:Queue(entries, flags),
		 event(event_loop, BIND_THIS_METHOD(OnSocketReady),
		       GetFileDescriptor()),
		 idle_event(event_loop, BIND_THIS_METHOD(OnIdle))
	{
		event.ScheduleRead();
	}

	void Submit() override {
		idle_event.Schedule();
	}

private:
	void OnSocketReady(unsigned flags) noexcept;
	void OnIdle() noexcept;
};

} // namespace Uring
