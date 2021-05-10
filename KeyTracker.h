#ifndef KEYTRACKER_H
#define KEYTRACKER_H

#include <map>

class KeyTracker
{
public:
    static KeyTracker &get()
    {
        static KeyTracker instance;
        return instance;
    }
    KeyTracker(KeyTracker const &) = delete;
    void operator=(KeyTracker const &) = delete;

    void KeyDown(int key) { keys[key] = true; }
    void KeyUp(int key) { keys[key] = false; }

    bool IsDown(int key)
    {
        if (keys.find(key) == keys.end())
            return false;
        return keys[key];
    }

private:
    KeyTracker() {}
    std::map<int, bool> keys;
};

#endif