//
// Created by Justin.Suess on 1/25/2022.
//

#include <gtest/gtest.h>
#include <libtcod/random.h>
#include <math.h>
#include "Util.h"
#include "GlobalGameState.h"
#include <list>
#include <libtcod/mersenne.hpp>
#include <libtcod/noise.hpp>

namespace {
    TEST(Util, substituteResources) {
        //std::cout << substituteResources("Hello {0} this is {1}. {0}.",{"df","34"},GlobalGameState::getInstance());
    }
    TEST(RandomNoise, noise) {
        TCODRandom* c = new TCODRandom(2332);
        TCODNoise* noise = new TCODNoise(2, .5, 10,c);
        std::cout<<std::cout.precision(3);
        for (int i = 32000; i < 32100; i++) {
            for (int j = 32000; j < 32100; j++) {
                float p[2] = {float(i) / 1024, float(j) / 1024};
                std::cout << p[0] << " : " << p[1] << " / " << pow(((noise->get(p)/2.)+.9),2)*59 << std::endl;
            }
        }
    }
}