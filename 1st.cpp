#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>

// ======================= COMPONENT SYSTEM =======================
class Component {
public:
    virtual void update() {}
    virtual ~Component() {}
};

// ======================= TRANSFORM =======================
class Transform : public Component {
public:
    float x = 0, y = 0;

    void update() override {}
};

// ======================= VELOCITY =======================
class Velocity : public Component {
public:
    float vx = 0, vy = 0;

    Velocity(float vx, float vy) : vx(vx), vy(vy) {}
};

// ======================= ENTITY =======================
class Entity {
private:
    std::vector<std::unique_ptr<Component>> components;

public:
    template<typename T, typename... Args>
    T* addComponent(Args&&... args) {
        T* comp = new T(std::forward<Args>(args)...);
        components.emplace_back(comp);
        return comp;
    }

    template<typename T>
    T* getComponent() {
        for (auto& c : components) {
            if (T* ptr = dynamic_cast<T*>(c.get())) {
                return ptr;
            }
        }
        return nullptr;
    }

    void update() {
        for (auto& c : components) {
            c->update();
        }
    }
};

// ======================= SYSTEMS =======================

// Physics System
class PhysicsSystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities) {
        for (auto& e : entities) {
            auto t = e->getComponent<Transform>();
            auto v = e->getComponent<Velocity>();

            if (t && v) {
                t->x += v->vx;
                t->y += v->vy;
            }
        }
    }
};

// Render System (Console)
class RenderSystem {
public:
    void render(std::vector<std::unique_ptr<Entity>>& entities) {
        system("cls"); // Windows (use "clear" for Linux)

        for (auto& e : entities) {
            auto t = e->getComponent<Transform>();
            if (t) {
                std::cout << "Entity at (" << t->x << ", " << t->y << ")\n";
            }
        }
    }
};

// Input System (Simulated)
class InputSystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities) {
        for (auto& e : entities) {
            auto v = e->getComponent<Velocity>();

            if (v) {
                // Simple AI movement
                v->vx = (rand() % 3 - 1); // -1, 0, 1
                v->vy = (rand() % 3 - 1);
            }
        }
    }
};

// ======================= GAME =======================
class Game {
private:
    std::vector<std::unique_ptr<Entity>> entities;

    PhysicsSystem physics;
    RenderSystem renderer;
    InputSystem input;

    bool running = true;

public:
    void init() {
        // Create player
        auto player = std::make_unique<Entity>();
        player->addComponent<Transform>();
        player->addComponent<Velocity>(1, 1);
        entities.push_back(std::move(player));

        // Create enemy
        auto enemy = std::make_unique<Entity>();
        enemy->addComponent<Transform>();
        enemy->addComponent<Velocity>(-1, 0);
        entities.push_back(std::move(enemy));
    }

    void run() {
        using namespace std::chrono;

        while (running) {
            auto start = high_resolution_clock::now();

            update();
            render();

            // ~60 FPS
            std::this_thread::sleep_for(milliseconds(100));

            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
        }
    }

    void update() {
        input.update(entities);
        physics.update(entities);

        for (auto& e : entities) {
            e->update();
        }
    }

    void render() {
        renderer.render(entities);
    }
};

// ======================= MAIN =======================
int main() {
    Game game;
    game.init();
    game.run();
    return 0;
}
