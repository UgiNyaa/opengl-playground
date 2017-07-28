#ifndef TIME_HPP
#define TIME_HPP

struct Time
{
    float full;
    float delta;

    Time()
        : full(0)
        , delta(0)
    { }

    void add(float delta)
    {
        this->delta = delta;
        full += delta;
    }
};

#endif /* end of include guard: TIME_HPP */
