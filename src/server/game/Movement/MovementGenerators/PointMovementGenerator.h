/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRINITY_POINTMOVEMENTGENERATOR_H
#define TRINITY_POINTMOVEMENTGENERATOR_H

#include "MovementGenerator.h"
#include "FollowerReference.h"
#include "PathFinderMovementGenerator.h"
#include "Spell.h"

template<class T>
class PointMovementGenerator : public MovementGeneratorMedium< T, PointMovementGenerator<T> >
{
    public:
        PointMovementGenerator(uint32 _id, float _x, float _y, float _z, bool _generatePath, float _speed = 0.0f) : id(_id),
            i_x(_x), i_y(_y), i_z(_z), m_generatePath(_generatePath), speed(_speed) {}

        void Initialize(T &);
        void Finalize(T &);
        void Reset(T &);
        bool Update(T &, const uint32 &);

        void MovementInform(T &);

        void unitSpeedChanged() { i_recalculateSpeed = true; }

        MovementGeneratorType GetMovementGeneratorType() { return POINT_MOTION_TYPE; }

        bool GetDestination(float& x, float& y, float& z) const { x=i_x; y=i_y; z=i_z; return true; }
    private:
        uint32 id;
        float i_x, i_y, i_z;
        float speed;
        bool i_recalculateSpeed;
        bool m_generatePath;
};

class AssistanceMovementGenerator : public PointMovementGenerator<Creature>
{
    public:
        AssistanceMovementGenerator(float _x, float _y, float _z) :
            PointMovementGenerator<Creature>(0, _x, _y, _z, true) {}

        MovementGeneratorType GetMovementGeneratorType() { return ASSISTANCE_MOTION_TYPE; }
        void Finalize(Unit &);
};

// Does almost nothing - just doesn't allows previous movegen interrupt current effect.
class EffectMovementGenerator : public MovementGenerator
{
    public:
        explicit EffectMovementGenerator(uint32 Id, float _x, float _y, float _z) : m_Id(Id),
        i_x(_x), i_y(_y), i_z(_z)        {}
        void Initialize(Unit &) {}
        void Finalize(Unit &unit);
        void Reset(Unit &) {}
        bool Update(Unit &u, const uint32&);

        MovementGeneratorType GetMovementGeneratorType() { return EFFECT_MOTION_TYPE; }
    private:
        uint32 m_Id;
        float i_x, i_y, i_z;
};

// Charge Movement Generator
class ChargeMovementGenerator : public MovementGenerator
{
    public:
        explicit ChargeMovementGenerator(uint32 Id, float _x, float _y, float _z, float _speed = 0.0f, PathFinderMovementGenerator* _path = NULL) : m_Id(Id),
        i_x(_x), i_y(_y), i_z(_z), speed(_speed), i_path(_path)        {}

        void Initialize(Unit &);
        void Finalize(Unit &unit);
        void Reset(Unit &) {}
        bool Update(Unit &u, const uint32&);
        MovementGeneratorType GetMovementGeneratorType() { return POINT_MOTION_TYPE; }
        bool GetDestination(float& x, float& y, float& z) const { x=i_x; y=i_y; z=i_z; return true; }
        bool IsReachable() const { return (i_path) ? (i_path->getPathType() & PATHFIND_NORMAL) : true; }
    private:
        uint32 m_Id;
        float i_x, i_y, i_z;
        float speed;
        bool i_recalculateSpeed;
        bool i_recalculateTravel : 1;
        bool i_targetReached : 1;
        PathFinderMovementGenerator* i_path;
        void _setTargetLocation(Unit &);
};

#endif

