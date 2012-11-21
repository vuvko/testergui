#ifndef FIELD_H
#define FIELD_H

#include <string>

#include "errors.h"

namespace mmp
{
namespace ui
{
    struct Point
    {
        int x, y;
        bool operator< (const Point &p) const
        {
                if (x < p.x) return true;
                if (x > p.x) return false;
                if (y < p.y) return true;
                return false;
        }
        Point(int x_, int y_) : x(x_), y(y_) {}
        Point() {}
    };

    class Star
    {
    public:
        virtual Point getPoint() const = 0;
        virtual ~Star() {}
    };

    class Block
    {
    public:
        virtual Point getPoint() const = 0;
        virtual ~Block() {}
    };

    class Empty
    {
    public:
        virtual Point getPoint() const = 0;
        virtual ~Empty() {}
    };

    class Number
    {
    public:
        virtual Point getPoint() const = 0;
        virtual int getTeamId() const = 0;
        virtual ~Number() {}

        bool operator< (const Number &other) const
        {
             return false;
        }
    };

    class Checker
    {
    public:
        virtual Point getPoint() const = 0;
        virtual int getTeamId() const = 0;
        virtual ~Checker() {}

        bool operator< (const Checker &other) const
        {
             return false;
        }
    };

    class IMMPui
    {
    public:
        static void ShowError(const std::string error);

        virtual void SetStar(const Star *star) = 0;
        virtual void SetBlock(const Block *block) = 0;
        virtual void SetEmpty(const Empty *empty) = 0;
        virtual void SetNumber(const Number *number) = 0;
        virtual void SetChecker(const Checker *checker) = 0;

        virtual void Clear() = 0;
        virtual void Paint() = 0;
        virtual void BeginPaint() = 0;
        virtual void EndPaint() = 0;

        virtual ~IMMPui() {}
    };
} // end of ui namespace
} // end of mmp namespace

#endif // FIELD_H
