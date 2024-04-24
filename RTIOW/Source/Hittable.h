#pragma once
#include "RTIOWHelper.h"
#include "Interval.h"
#include "Ray.h"

struct HitRecord {
    glm::vec3 HitPosition = { 0.f, 0.f, 0.f };
    glm::vec3 Normal = { 0.f, 0.f, 0.f };
    float T = std::numeric_limits<float>::infinity();
    bool IsExteriorFace = true;
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool Hit(const Ray& ray, HitRecord& record, Interval interval = Interval::Front()) const = 0;
};

class HittableList : public Hittable {
public:
    std::vector<Ref<Hittable>> objects;

    HittableList() {}
    HittableList(Ref<Hittable> object) { Add(object); }

    void Clear() { objects.clear(); }

    void Add(Ref<Hittable> object) { objects.push_back(object); }

    bool Hit(const Ray& r, HitRecord& rec, Interval interval = Interval::Front()) const override{
        // [POTENTIAL FUTURE BUG]: Moved HitRecord inside the loop, 
        bool hasHit = false;

        for (const auto& object : objects) {
            HitRecord tempRec;
            if (object->Hit(r, tempRec, interval)) {
                hasHit = true;
                interval.Max = tempRec.T;
                rec = tempRec;
            }
        }
        return hasHit;
    }
};