-- PHYSICS

local function create_test_bodies(n)
    local pos = { x=0, y=0 }
    local vel = { x=0, y=0 }
    local acc = { x=0, y=0 }

    local bodies = {}

    math.randomseed(os.time())

    for i = 1, n do
        pos.x = math.random(80, 1200)
        pos.y = math.random(20, 700)

        vel.x = math.random(-100, 100)
        vel.y = math.random(-100, 100)

        acc.x = math.random(-50, 50)
        acc.y = math.random(50, 50)

        bodies[i] = {
            position        = { x=pos.x, y=pos.y },
            velocity        = { x=vel.x, y=vel.y },
            acceleration    = { x=acc.x, y=acc.y },
            size            = { x=48, y=32 },
        }

    end

    return bodies
end

local num_bodies    = 8
local bodies        = create_test_bodies(num_bodies)

function update(delta)
    if key_pressed(string.byte("R")) then
        bodies = create_test_bodies(num_bodies)
    end

    for _, body in ipairs(bodies) do
        if (body.position.x < 0) or ((body.position.x + body.size.x + 1) > 1280) then
            body.velocity.x = -body.velocity.x
        end

        if (body.position.y < 0) or ((body.position.y + body.size.y + 1) > 720) then
            body.velocity.y = -body.velocity.y
        end

        body.velocity.x = body.velocity.x + body.acceleration.x * delta
        body.velocity.y = body.velocity.y + body.acceleration.y * delta

        body.position.x = body.position.x + body.velocity.x * delta
        body.position.y = body.position.y + body.velocity.y * delta

        draw_rectangle(body.position, body.size, { r=1.0, g=1.0, b=1.0, a=1.0 })

        local cx = body.position.x + (body.size.x / 2)
        local cy = body.position.y + (body.size.y / 2)

        draw_line({ x=cx, y=cy }, { x=cx + body.velocity.x, y=cy + body.velocity.y }, { r=0.0, g=0.0, b=1.0, a=1.0 })
        draw_line({ x=cx, y=cy }, { x=cx + body.acceleration.x, y=cy + body.acceleration.y }, { r=1.0, g=0.0, b=0.0, a=1.0 })
    end
end
