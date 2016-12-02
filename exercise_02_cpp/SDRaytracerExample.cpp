#include <chrono>
#include <iostream>
#include <vector>


#include "SDRaytracerExample.h"
#include "Triangle.h"
#include "RGB.h"
#include "Light.h"
#include "Vec3D.h"
#include "SDRaytracer.h"
#include "Matrix.h"


/* Really bad c / c++ mixture but i was lazy*/
#include <stdlib.h>
#include <SDL2/SDL.h>

template <typename T>
void Append(std::vector<T>& a, const std::vector<T>& b)
{
  a.reserve(a.size() + b.size());
  a.insert(a.end(), b.begin(), b.end());
}

namespace SDRaytracerExample {

  void SDRaytracerExample::updateScene() {

    Matrix mRx = Matrix::createXRotation((float) (sd.x_angle_factor * PI_F / 16));
    Matrix mRy = Matrix::createYRotation((float) (sd.y_angle_factor * PI_F / 16));
    Matrix mT = Matrix::createTranslation(0, 0, 200);
    Matrix m = mT.mult(mRx).mult(mRy);
    m.print();
    m.apply(sd.triangles);

  }

  void SDRaytracerExample::createScene() {

    sd.triangles.clear();

    Append(sd.triangles, Triangle::createCube(0, 35, 0, 10, 10, 10, RGB(0.3f, 0, 0), 0.4f));
    Append(sd.triangles, Triangle::createCube(-70, -20, -20, 20, 100, 100, RGB(0.0f, 0, 0.3f), .4f));
    Append(sd.triangles, Triangle::createCube(-30, 30, 40, 20, 20, 20, RGB(0, 0.4f, 0), 0.2f));
    Append(sd.triangles, Triangle::createCube(50, -20, -40, 10, 80, 100, RGB(.5f, .5f, .5f), 0.2f));
    Append(sd.triangles, Triangle::createCube(-70, -26, -40, 130, 3, 40, RGB(.5f, .5f, .5f), 0.2f));

    sd.lights.clear();
    sd.lights.push_back(Light(Vec3D(0, 100, 0), RGB(0.1f, 0.1f, 0.1f)));
    sd.lights.push_back(Light(Vec3D(100, 200, 300), RGB(0.5f, 0, 0.0f)));
    sd.lights.push_back(Light(Vec3D(-100, 200, 300), RGB(0.0f, 0, 0.5f)));
    // ,new Light(new Vec3D(-100,0,0), new RGB(0.0f,0.8f,0.0f))

    sd.fovx = (float) 0.628;
    sd.fovy = (float) 0.628;
    sd.ambient_color = RGB(0.01f, 0.01f, 0.01f);
    sd.background_color = RGB(0.05f, 0.05f, 0.05f);

    updateScene();


  }

  SDRaytracerExample::SDRaytracerExample() {

    sd.profiling = true;

    sd.y_angle_factor = 4;
    sd.x_angle_factor = -4;

    createScene();

    if (!sd.profiling) {
      sd.renderImage();
    } else {
      sd.profileRenderImage();
    }

    std::cout << "fovx=" << sd.fovx << ", fovy=" << sd.fovy << ", xangle=" << sd.x_angle_factor << ", yangle=" << sd.y_angle_factor << std::endl;

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;


    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(sd.height, sd.width, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < sd.width; ++i) {
      for (int j = 0; j < sd.height; ++j) {
        SDL_SetRenderDrawColor(renderer, sd.image[i][j].getred(), sd.image[i][j].getgreen(), sd.image[i][j].getblue(), 255);
        SDL_RenderDrawPoint(renderer, i, j);

      }
    }

    SDL_RenderPresent(renderer);

    std::cout << "I'm going into endless loop" << std::endl;

    while (1) {
      if (SDL_PollEvent(&event)) {
        switch(event.type) {
          case SDL_QUIT:
          return;
          break;

          case SDL_KEYDOWN:

          switch( event.key.keysym.sym ){
            case SDLK_LEFT:
            sd.y_angle_factor-=1;
            break;
            case SDLK_RIGHT:
            sd.y_angle_factor+=1;
            break;
            case SDLK_UP:
            sd.x_angle_factor-=1;
            break;
            case SDLK_DOWN:
            sd.x_angle_factor+=1;
            break;
            default:
            break;
          }

          createScene();
          sd.renderImage();
          std::cout << "fovx=" << sd.fovx << ", fovy=" << sd.fovy << ", xangle=" << sd.x_angle_factor << ", yangle=" << sd.y_angle_factor << std::endl;

          for (int i = 0; i < sd.width; ++i) {
            for (int j = 0; j < sd.height; ++j) {
              SDL_SetRenderDrawColor(renderer, sd.image[i][j].getred(), sd.image[i][j].getgreen(), sd.image[i][j].getblue(), 255);
              SDL_RenderDrawPoint(renderer, i, j);

            }
          }

          SDL_RenderPresent(renderer);

          break;
        }
      }

    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

  }

}

int main() {

  std::chrono::milliseconds start = std::chrono::duration_cast< std::chrono::milliseconds >(
    std::chrono::system_clock::now().time_since_epoch()
  );
  SDRaytracerExample::SDRaytracerExample* sdr = new SDRaytracerExample::SDRaytracerExample();
  std::chrono::milliseconds end = std::chrono::duration_cast< std::chrono::milliseconds >(
    std::chrono::system_clock::now().time_since_epoch()
  );
  std::chrono::milliseconds time = end - start;
  std::cout << "time: " << time.count() << " ms" << std::endl;
  std::cout << "nrprocs=" << sdr->sd.nrOfProcessors << std::endl;

}
