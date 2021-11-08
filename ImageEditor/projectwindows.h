#ifndef PROJECT_WINDOWS_H
#define PROJECT_WINDOWS_H

#include <vector>

class ProjectWide {
public:
    static void *g_iV;
    static void *g_fContrast;
    static void *g_histogramWnd;
    static std::vector<int> g_brightnessVector;
    static bool oneLinePaint;
};

#endif // PROJECT_WINDOWS_H
