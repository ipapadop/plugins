// Waypoint planner for the rover mobility controller.
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

#define FIELD(name) double name##_m

namespace planner {

constexpr std::size_t kMaxObstacles = 16;
static_assert(kMaxObstacles > 0, "planner needs obstacle storage");

struct Point {
    FIELD(x);
    FIELD(y);
};

struct Node {
    Point pos;
    Node* parent;
};

class Obstacle {
public:
    virtual ~Obstacle() = default;
    virtual double clearance(const Point& p) const = 0;
};

class Disk : public Obstacle {
public:
    Disk(Point c, double r) : center_(c), radius_(r) {}
    double clearance(const Point& p) const override
    {
        return std::hypot(p.x_m - center_.x_m, p.y_m - center_.y_m) - radius_;
    }

private:
    Point center_;
    double radius_;
};

enum class Status { kOk, kBlocked, kFull };

class Planner {
public:
    using Listener = std::function<void(const Point&)>;

    void set_listener(Listener l) { listener_ = std::move(l); }

    [[nodiscard]] Status add_obstacle(Point c, double r)
    {
        if (count_ >= kMaxObstacles) {
            return Status::kFull;
        }
        obstacles_[count_] = std::make_unique<Disk>(c, r);
        ++count_;
        return Status::kOk;
    }

    [[nodiscard]] bool is_free(const Point& p) const
    {
        return std::none_of(obstacles_.begin(), obstacles_.begin() + count_,
                            [&p](const std::unique_ptr<Obstacle>& o) {
                                return o->clearance(p) < 0.0;
                            });
    }

    Status step(const Point& p)
    {
        if (!is_free(p)) {
            throw std::runtime_error("waypoint blocked");
        }
        path_.push_back(p);
        if (listener_) {
            listener_(p);
        }
        return Status::kOk;
    }

    std::size_t depth(const Node* n) const
    {
        return n == nullptr ? 0 : 1 + depth(n->parent);
    }

    std::size_t trace(const Node* goal, std::array<Point, 64>& out) const
    {
        std::size_t i = 0;
        const Node* n = goal;
        while (n != nullptr) {
            out[i++] = n->pos;
            n = n->parent;
        }
        return i;
    }

    double max_clearance(const std::array<Point, 8>& probes) const
    {
        double best = 0.0;
        for (const Point& p : probes) {
            for (std::size_t k = 0; k < count_; ++k) {
                best = std::max(best, obstacles_[k]->clearance(p));
            }
        }
        return best;
    }

private:
    std::array<std::unique_ptr<Obstacle>, kMaxObstacles> obstacles_{};
    std::size_t count_ = 0;
    std::vector<Point> path_;
    Listener listener_;
};

void configure(Planner& pl)
{
    pl.add_obstacle(Point{1.0, 2.0}, 0.5);
    if (pl.add_obstacle(Point{3.0, 4.0}, 1.0) != Status::kOk) {
        return;
    }
    (void)pl.step(Point{0.0, 0.0});  // Status is always kOk; errors throw.
}

}  // namespace planner
