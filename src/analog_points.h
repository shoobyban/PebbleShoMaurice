#pragma once
#include <pebble.h>

#ifdef PBL_COLOR

const GPathInfo HOUR_HAND_POINTS = {
    6,
    (GPoint []) {
        { 5, 0},
        { 4, -25},
        { 2, -36},
        {-2, -36},
        {-4, -25},
        {-5, 0},
    }
};

const GPathInfo MINUTE_HAND_POINTS = {
    6,
    (GPoint []) {
        { 4, 0},
        { 3, -25},
        { 2, -47},
        {-2, -47},
        {-3, -25},
        {-4, 0},
    }
};

#else

const GPathInfo HOUR_HAND_POINTS = {
    6,
    (GPoint []) {
        { 7, 0},
        { 5, -25},
        { 3, -36},
        {-3, -36},
        {-5, -25},
        {-7, 0},
    }
};

const GPathInfo MINUTE_HAND_POINTS = {
    6,
    (GPoint []) {
        { 5, 0},
        { 4, -27},
        { 3, -50},
        {-3, -50},
        {-4, -27},
        {-5, 0},
    }
};

#endif

const GPathInfo HOUR_IN_POINTS = {
    6,
    (GPoint []) {
        { 5, 0},
        { 3, -20},
        { 2, -31},
        {-2, -31},
        {-3, -20},
        {-5, 0},
    }
};

const GPathInfo SEC_POINTS = {
    2,
    (GPoint []) {
        {0, -7},
        {0, -54}
    }
};
