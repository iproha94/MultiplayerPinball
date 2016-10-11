#pragma once

enum  PhysicsCategory {
    None = 0,
    Ball = (1 << 0),
    Bumper = (1 << 1),
    Kicker = (1 << 2),
    Flipper = (1 << 3),
    All = PhysicsCategory::Ball | PhysicsCategory::Bumper | PhysicsCategory::Kicker | PhysicsCategory::Flipper
};
