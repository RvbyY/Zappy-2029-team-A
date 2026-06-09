#include "state/Expulsion.hpp"

Expulsion::Expulsion(int playerId, int x, int y)
    : _playerId(playerId),
      _x(x),
      _y(y),
      _remainingTicks(DefaultLifetimeTicks)
{
}

int Expulsion::playerId() const
{
    return _playerId;
}

int Expulsion::x() const
{
    return _x;
}

int Expulsion::y() const
{
    return _y;
}

bool Expulsion::belongsToPlayer(int playerId) const
{
    return _playerId == playerId;
}

void Expulsion::tick()
{
    if (_remainingTicks > 0)
        --_remainingTicks;
}

bool Expulsion::isExpired() const
{
    return _remainingTicks <= 0;
}