#!/usr/bin/env python3

def render_constant(duration):
    """Return the same frame render duration for every time stamp."""

    return lambda now: duration

# An application model should yield (buf_count, render_start) pairs.
# buf_count is a sequence number identifying the render, or -1 for
# frames already in the framebuffer before simulation started.
# render_start is the timestamp at which the render started. For
# computing latency, we assume that input was processed immediately
# before the rendering started.
# The simulator sends the timestamp of the next vblank back the
# application model.

def swap_chain(num_buffers, renderer):
    """Model of an application that renders via a swap chain."""

    now = 0
    render_start = None
    render_done = None
    buf_count = 0
    # Buffers ready to be displayed.
    buffers = [(-1, 0)]
    while True:
        vblank = yield buffers[0]

        while True:
            if render_done is None:
                if len(buffers) >= num_buffers:
                    # Block on acquiring buffer.
                    now = vblank
                render_start = now
                render_done = render_start + renderer(now)
            if render_done > vblank:
                break
            else:
                buffers.append((buf_count, render_start))
                buf_count += 1
                now = render_done
                render_done = None

        if len(buffers) > 1:
            del buffers[0]

def swap_discard(renderer):
    """Model of an application that renders as many frames as possible
    via triple buffering, potentially discarding some frames.
    """

    now = 0
    render_start = None
    render_done = None
    buf_count = 0
    buffers = [(-1, 0)]
    while True:
        vblank = yield buffers[0]

        while True:
            if render_done is None:
                if len(buffers) >= 3:
                    # Discard oldest waiting buffer.
                    del buffers[1]
                render_start = now
                render_done = render_start + renderer(now)
            if render_done > vblank:
                break
            else:
                buffers.append((buf_count, render_start))
                buf_count += 1
                now = render_done
                render_done = None

        if len(buffers) > 1:
            del buffers[0]

def simulate(num_frames, frame_rate, **apps):
    labels = f" {frame_rate:3} Hz refresh"
    column_headers = "frame:  vblank:"
    for label, app in apps.items():
        app.send(None)
        labels += f" | {label.replace('_', ' '):17}"
        column_headers += ' | buffer:  latency:'
    print(labels)
    print(column_headers)
    
    for frame in range(num_frames):
        vblank = int((frame / frame_rate) * 1000)
        line = f"{frame:4} {vblank:8}"
        for app in apps.values():
            buf_count, input_time = app.send(vblank)
            latency = vblank - input_time
            line += f"   | {buf_count:5} {latency:8} "
        print(line)

renderer = render_constant(25)
simulate(50, 60,
         double_chain=swap_chain(2, renderer),
         triple_chain=swap_chain(3, renderer),
         triple_discard=swap_discard(renderer)
         )
