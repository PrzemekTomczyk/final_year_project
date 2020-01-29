// stdafx.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef STDAFX_H
#define STDAFX_H

// add headers that you want to pre-compile here

//std
#include <random>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <utility>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <list>
#include <queue>
#include <functional>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <cstdlib>


//boost
#include "boost/thread.hpp"
#include "boost/thread/scoped_thread.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/lock_types.hpp"

//SFML
#include <SFML/Graphics.hpp>

//imgui
#include "imgui/imgui.h"
#include "imguiSFML/imgui-SFML.h"


//some external libraries
#include <Thor/Vectors.hpp>
#include <Thor/Time.hpp>


//other
//#include "Input.h" //for keyboard input
//#include "XBOX360CONTROLLER.h" //for controller input
//#include "FPS.h" //for fps

//more stuff

#endif //STDAFX_H
