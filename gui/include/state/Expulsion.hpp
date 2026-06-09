#pragma once

class Expulsion {
public:
    static constexpr int DefaultLifetimeTicks = 120;

    Expulsion(int playerId, int x, int y);

    int playerId() const;
    int x() const;
    int y() const;

    bool belongsToPlayer(int playerId) const;
    void tick();
    bool isExpired() const;

private:
    int _playerId;
    int _x;
    int _y;
    int _remainingTicks;
};