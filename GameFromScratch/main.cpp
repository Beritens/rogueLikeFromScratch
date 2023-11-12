// SDL2 particle system example
//
// Controls:
//   * Press space to toggle gravity
//   * Move mouse to move emitter

#include <chrono>
#include <cstdlib>

#include "SDL.h"

#define TWO_ENTITY_MAX (32<<10)
#include "entity.h"

// Config
constexpr int   WindowX = 800;
constexpr int   WindowY = 600;
constexpr float EmitterX = WindowX / 2.f;
constexpr float EmitterY = WindowY / 2.f;
constexpr float MinLifetime = 1.f;
constexpr float MaxLifetime = 5.f;
constexpr int   MaxParticles = 300;
constexpr float MaxSpeed = 15.f;
constexpr float MinSize = 0.2f;
constexpr float MaxSize = 0.5f;
constexpr float Gravity = 100.f;

static SDL_Renderer* gfx = nullptr;

// -- Utility -------------------------

struct float2 {
    float x, y;

    float2() = default;
    float2(float x, float y) : x{ x }, y{ y } {}
    explicit float2(float s) : x{ s }, y{ s } {}
};

float2 operator+(const float2& a, const float2& b) {
    return { a.x + b.x, a.y + b.y };
}
float2 operator-(const float2& a, const float2& b) {
    return { a.x - b.x, a.y - b.y };
}

float2 operator*(const float2& a, float s) {
    return { a.x * s, a.y * s };
}
float2 operator/(const float2& a, float s) {
    return { a.x / s, a.y / s };
}

float dot(const float2& a, const float2& b) {
    return a.x * b.x + a.y * b.y;
}
float length(const float2& a) {
    return sqrt(a.x * a.x + a.y * a.y);
}
float2 norm(float2 a) {
    return a / length(a);
}



struct Color {
    uint8_t r, g, b, a;

    Color() = default;
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : r{ r }, g{ g }, b{ b }, a{ a } {}
};

static float randf() {
    return float(rand() / double(RAND_MAX));
}

static float randf(float a, float b) {
    return a + (b - a) * randf();
}

static float2 rand_dir(float scale = 1.f) {
    for (;;) {
        float2 v(randf(-1.f, 1.f), randf(-1.f, 1.f));
        if (dot(v, v) <= 1.f)
            return v * scale;
    }
}

static float remap(float a, float b, float c, float d, float x) {
    return c + ((d - c) / (b - a)) * (x - a);
}

// -- Events --------------------------

struct QuitEvent {};

struct KeyDown {
    SDL_Keycode key;
    SDL_Scancode scancode;
};
struct KeyUp {
    SDL_Keycode key;
    SDL_Scancode scancode;
};
//extra stuff

class InputManager {
public:
    float2 GetMovement() {
        return movement;
    }
    void SetMovement(float2 m) {
        movement = m;
    }
    float2 GetShootDir() {
        return shootDir;
    }
    void SetShootDir(float2 m) {
        shootDir = m;
    }
private:
    float2 movement;
    float2 shootDir;
};

InputManager* mInputs;

struct Collision {
    two::Entity a;
    two::Entity b;
    float2 pointA;
    float2 pointB;
    float2 velA;
    float2 velB;
    float2 normal;
};

// -- Components ----------------------

struct Transform {
    float2 position;
    float2 scale;
};
struct Controller {
};

struct Sprite {
    Color color;
};

struct Particle {
    float lifetime;
};
struct Collider {
    float radius;

};
struct Follower {
    two::Entity target;
    float speed;
};
struct PhyObj {
    float2 velocity;
    bool rigid;
    float mass;
};

struct Emitter {
};

static void spawn_particle(two::World* world, two::Entity entity,
    const float2& origin) {
    world->pack(entity,
        Transform{ origin, float2(randf(MinSize, MaxSize)) },
        Sprite{ {0xbb, 0xaa, 0xee, 0xff} },
        Particle{ randf(MinLifetime, MaxLifetime)},
        PhyObj{ rand_dir(MaxSpeed), true, 1 });
}

static void spawn_particle_dir(two::World* world, two::Entity entity,
    const float2& origin, const float2& dir) {
    world->pack(entity,
        Transform{ origin, float2(randf(MinSize, MaxSize)) },
        Sprite{ {0xbb, 0xaa, 0xee, 0xff} },
        Particle{ randf(MinLifetime, MaxLifetime) },
        PhyObj{dir, true, 0.2f},
        Collider{0.2f});
}

static void spawn_Map(two::World* world, two::Entity player) {
    for (int i = 0; i < 200; i++) {
        world->pack(world->make_entity(),
            Transform{ {(1.0f*i),5}, {1,1} },
            Follower{ player, 10.0f },
            PhyObj{ {0,0}, true, 1 },
            Sprite{ {0xbb, 0xaa, 0xee, 0x9f} },
            Collider{0.5});
    }/*
    world->pack(world->make_entity(),
        Transform{ {(20.0f),5}, {2,2} },
        Follower{ player, 10.0f },
        PhyObj{ {0,0}, true },
        Sprite{ {0xbb, 0xaa, 0xee, 0x9f} },
        Collider{ 1 });
    world->pack(world->make_entity(),
        Transform{ {(0.0f),5}, {1,1} },
        Follower{ player, 10.0f },
        PhyObj{ {0,0}, true },
        Sprite{ {0xbb, 0xaa, 0xee, 0x9f} },
        Collider{ 0.5 });*/


    /*for (int i = 0; i < 20; i++) {
        world->pack(world->make_entity(),
            Transform{ {(128.0f + 32.0f * i),300}, {32,32} },
            Sprite{ {0xbb, 0xaa, 0xee, 0xff} },
            Follower{ player, 10000.0f },
            PhyObj{ {0,0} },
            Sprite{ {0xbb, 0xaa, 0xee, 0xff} },
            Collider{ float2(16) });
    }*/
}


// -- Systems -------------------------

class ParticleSystem : public two::System {
public:
    void update(two::World* world, float dt) override {
        two::Optional<two::Entity> attractor= world->view_one<Controller>();
        Transform at;
        if (attractor.has_value) {
            at = world->unpack<Transform>(attractor.value());
        }
        world->each<Transform, Particle,PhyObj, Sprite>(
            [&](two::Entity entity, Transform& tf, Particle& p,PhyObj phy , Sprite& sp) {
                
                p.lifetime -= dt;


                sp.color.a = remap(MinLifetime - 1.f, MaxLifetime + 1.f,
                    0, 255.f, p.lifetime);

                if (p.lifetime <= 0.f) {
                    world->destroy_entity(entity);
                }
            });
    }
};

class SpriteRenderer : public two::System {
public:
    void load(two::World*) override {
        SDL_SetRenderDrawBlendMode(gfx, SDL_BLENDMODE_BLEND);
    }

    void draw(two::World* world) override {
        SDL_SetRenderDrawColor(gfx, 0, 0, 0, 255);
        SDL_RenderClear(gfx);

        int w, h;
        SDL_GetRendererOutputSize(gfx, &w, &h);

        world->each<Transform, Sprite>([w, h](Transform& tf, Sprite& sprite) {
            SDL_Rect dst{ int(tf.position.x*32 - (tf.scale.x*16)), int(tf.position.y*32 - (tf.scale.y *16)),
                         int(tf.scale.x*32), int(tf.scale.y*32) };

        if (dst.x >= 0 && dst.y >= 0
            && dst.x + dst.w <= w && dst.y + dst.h <= h) {
            SDL_SetRenderDrawColor(gfx, sprite.color.r, sprite.color.g,
                sprite.color.b, sprite.color.a);
            SDL_RenderFillRect(gfx, &dst);
        }
            });
        SDL_RenderPresent(gfx);
    }
};

//class MoveSystem : public two::System {
//public:
//    void update(two::World* world, float) override {
//        int x, y;
//        SDL_GetMouseState(&x, &y);
//        world->each<Emitter>([x, y](Emitter& emitter) {
//            emitter.origin = float2(x, y);
//            });
//    }
//};

class ControlSystem : public two::System {
public:
    void update(two::World* world, float dt) override {
        float2 input = mInputs->GetMovement();
        world->each<Controller, PhyObj>([input, dt](Controller& controller, PhyObj& phyObj) {
            
            phyObj.velocity = input * (20);
            });
    }
};

class ShootSystem : public two::System {
public:
    void update(two::World* world, float dt) override {
        float2 input = mInputs->GetShootDir();
        float2 normed = norm(input);
        if (length(input) > 0.25f) {
            world->each<Controller, Transform>([input, dt, world, normed](Controller& controller, Transform& transform) {
                spawn_particle_dir(world, world->make_entity(), transform.position + normed, normed*20 + rand_dir(1));
            });
        }

        
    }
};

class FollowSystem : public two::System {
public:
    void update(two::World* world, float dt) override {
        world->each<Follower,PhyObj, Transform>([dt, world](Follower& f,PhyObj& phy, Transform& tf) {
            Transform target = world->unpack<Transform>(f.target);
            float2 diff = target.position - tf.position;
            diff = (norm(diff) * (f.speed));
            diff = diff - phy.velocity;
            phy.velocity = phy.velocity + diff * dt;
        });
        


    }
};
class PhysicsSystem : public two::System {
public:



    //}
    void update(two::World* world, float dt) override {
        //important lessons. unpacking is like the most expensive shit. Doing this n^2 times will cause huge laggs.
        //instead the first time arounf put those components in a vector. And then you can go over that... way faster
        std::vector<Collision> collisions;
        std::vector<Transform> transforms;
        std::vector<Collider> colliders;
        std::vector<two::Entity> colEntities;
        std::vector<float2> velocities;
        for (auto ent : world->view<Transform, PhyObj>()) {
            auto& tf = world->unpack<Transform>(ent);
            auto& phy = world->unpack<PhyObj>(ent);
            
            tf.position = tf.position + (phy.velocity * dt);
            if (phy.rigid && world->contains<Collider>(ent)) {

                auto& col = world->unpack<Collider>(ent);
                transforms.emplace_back(tf);
                colliders.emplace_back(col);
                colEntities.emplace_back(ent);
                velocities.emplace_back(phy.velocity);
            }

        }
        for (int i = 0; i < transforms.size(); i++) {
            for (int j = 0; j < transforms.size(); j++) {
                if (i >= j) {
                    continue;
                }
                float2 dirr = transforms[j].position - transforms[i].position;
                float dist = length(dirr);
                dirr = dirr / dist;
                if (dist < colliders[i].radius + colliders[j].radius) {
                    collisions.emplace_back(colEntities[i], colEntities[j], transforms[i].position + dirr * colliders[i].radius, transforms[j].position - dirr * colliders[j].radius, velocities[i], velocities[j], dirr);
                }
            }
        }
        //solve
        for (Collision col : collisions) {
            float2 diff = col.pointB - col.pointA;
            float2 normal = col.normal;
            
            auto& ta = world->unpack<Transform>(col.a);
            auto& tb = world->unpack<Transform>(col.b);
            auto& phyA = world->unpack<PhyObj>(col.a);
            auto& phyB = world->unpack<PhyObj>(col.b);
            float2 aVel = phyA.velocity;
            float2 bVel = phyB.velocity;
            float aMass = phyA.mass;
            float bMass = phyB.mass;
            float aInvMass = 1.f / aMass;
            float bInvMass = 1.f / bMass;

            //collision solver

            // Calculate total mass
            float totalMass = aMass + bMass;

            // Calculate the ratio of each object's mass to the total mass
            float ratioA = aMass / totalMass;
            float ratioB = bMass / totalMass;

            
            ta.position = ta.position + (diff * ratioB);
            tb.position = tb.position - (diff * ratioA);
            //impulse solver
            //assuming everything is a physics object for now lul
            float2 rVel = bVel - aVel;
            float nSpd = dot(rVel, normal);
            if (nSpd >= 0) {
                continue;
            }
            float j = ( -1.2f *  nSpd ) / (aInvMass + bInvMass);

            
            

            float2 impulse = normal * j;
            aVel = aVel - impulse * aInvMass;
            bVel = bVel + impulse * bInvMass;


            phyA.velocity = aVel;
            phyB.velocity = bVel;
        }



    }
};



// -- World ---------------------------

class MainWorld : public two::World {
public:
    void load() override {
        mInputs = new InputManager();

        make_system<PhysicsSystem>();
        make_system<FollowSystem>();
        make_system<ShootSystem>();
        make_system<ParticleSystem>();
        make_system<ControlSystem>();
        make_system<SpriteRenderer>();


        bind<KeyDown>(&MainWorld::keydown, this);
        bind<KeyUp>(&MainWorld::keyup, this);
        two::Entity player = make_entity();
        pack(player, Controller{},
            Transform{ {0,0}, {1,1} },
            PhyObj{ {0,0}, true, 10 },
            Collider{0.5f},
            Sprite{ {0xbb, 0xaa, 0xee, 0xff} },
            Emitter{});
        spawn_Map(this, player);
        /*for (int i = 0; i < MaxParticles; ++i) {
            spawn_particle(this, make_entity(), float2(EmitterX, EmitterY));
        }*/
    }

    void update(float dt) override {
        for (auto* system : systems()) {
            system->update(this, dt);
        }
    }

    bool keydown(const KeyDown& event) {
        if (event.key == SDLK_SPACE) {
            return true;
        }
        if (event.key == SDLK_w) {
            float2 curI = mInputs->GetMovement();
            curI.y -= 1;
            mInputs->SetMovement(curI);
        }
        if (event.key == SDLK_a) {
            float2 curI = mInputs->GetMovement();
            curI.x -= 1;
            mInputs->SetMovement(curI);
        }
        if (event.key == SDLK_s) {
            float2 curI = mInputs->GetMovement();
            curI.y += 1;
            mInputs->SetMovement(curI);
        }
        if (event.key == SDLK_d) {
            float2 curI = mInputs->GetMovement();
            curI.x += 1;
            mInputs->SetMovement(curI);
        }
        if (event.key == SDLK_i) {
            float2 curI = mInputs->GetShootDir();
            curI.y -= 1;
            mInputs->SetShootDir(curI);
        }
        if (event.key == SDLK_j) {
            float2 curI = mInputs->GetShootDir();
            curI.x -= 1;
            mInputs->SetShootDir(curI);
        }
        if (event.key == SDLK_k) {
            float2 curI = mInputs->GetShootDir();
            curI.y += 1;
            mInputs->SetShootDir(curI);
        }
        if (event.key == SDLK_l) {
            float2 curI = mInputs->GetShootDir();
            curI.x += 1;
            mInputs->SetShootDir(curI);
        }
        return false;
    }
    bool keyup(const KeyUp& event) {
        if (event.key == SDLK_SPACE) {
            auto& emitter = unpack_one<Emitter>();
            return true;
        }
        if (event.key == SDLK_w) {
            float2 curI = mInputs->GetMovement();
            curI.y += 1;
            mInputs->SetMovement(curI);
        }
        if (event.key == SDLK_a) {
            float2 curI = mInputs->GetMovement();
            curI.x += 1;
            mInputs->SetMovement(curI);
        }
        if (event.key == SDLK_s) {
            float2 curI = mInputs->GetMovement();
            curI.y -= 1;
            mInputs->SetMovement(curI);
        }
        if (event.key == SDLK_d) {
            float2 curI = mInputs->GetMovement();
            curI.x -= 1;
            mInputs->SetMovement(curI);
        }
        if (event.key == SDLK_i) {
            float2 curI = mInputs->GetShootDir();
            curI.y += 1;
            mInputs->SetShootDir(curI);
        }
        if (event.key == SDLK_j) {
            float2 curI = mInputs->GetShootDir();
            curI.x += 1;
            mInputs->SetShootDir(curI);
        }
        if (event.key == SDLK_k) {
            float2 curI = mInputs->GetShootDir();
            curI.y -= 1;
            mInputs->SetShootDir(curI);
        }
        if (event.key == SDLK_l) {
            float2 curI = mInputs->GetShootDir();
            curI.x -= 1;
            mInputs->SetShootDir(curI);
        }
        return false;
    }
};

int main(int, char* []) {
    auto* window = SDL_CreateWindow("Two ECS - Particles",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WindowX, WindowY, 0);
    SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
    gfx = SDL_CreateRenderer(
        window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    MainWorld world;
    world.load();

    auto frame_begin = std::chrono::high_resolution_clock::now();
    auto frame_end = frame_begin;
    bool running = true;
    SDL_Event e;

    world.bind<QuitEvent>([&running](const QuitEvent&) {
        running = false;
        return false;
        });

    while (running) {
        frame_begin = frame_end;
        frame_end = std::chrono::high_resolution_clock::now();
        auto dt_micro = std::chrono::duration_cast<std::chrono::microseconds>(
            (frame_end - frame_begin)).count();
        float dt = float(double(dt_micro) * 1e-6);
        //printf("%f \n", 1/dt);

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                world.emit(QuitEvent{});
                break;
            case SDL_KEYDOWN:
                if (e.key.repeat == 0) {
                    world.emit(KeyDown{ e.key.keysym.sym, e.key.keysym.scancode });
                }
                
                break;
            case SDL_KEYUP:
                world.emit(KeyUp{ e.key.keysym.sym, e.key.keysym.scancode });
                break;
            }
        
        }

        world.update(dt);
        for (auto* system : world.systems()) {
            system->draw(&world);
        }
        world.collect_unused_entities();
    }
    SDL_DestroyRenderer(gfx);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}