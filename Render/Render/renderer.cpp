#include "renderer.hpp"
#include <algorithm>
#include <cmath>
bool initialize(SDL_Window*& window, SDL_Renderer*& renderer, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return false;
    }
    window = SDL_CreateWindow("SDL Drawing", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        return false;
    }
    return true;
}
void cleanup(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void drawScreen(SDL_Renderer* renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, NULL);
}
void drawRectangle(SDL_Renderer* renderer, int x, int y, int width, int height, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &rect);
}
void drawCircleOutline(SDL_Renderer* renderer, int centerX, int centerY, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    int x = radius;
    int y = 0;
    int p = 1 - radius;   
    if (radius == 0) {
        SDL_RenderDrawPoint(renderer, centerX, centerY);
        return;
    }
    SDL_RenderDrawPoint(renderer, centerX + radius, centerY);
    SDL_RenderDrawPoint(renderer, centerX - radius, centerY);
    SDL_RenderDrawPoint(renderer, centerX, centerY + radius);
    SDL_RenderDrawPoint(renderer, centerX, centerY - radius);
    while (x > y) {
        y++;
        if (p <= 0) {
            p = p + 2 * y + 1;
        }
        else {
            x--;
            p = p + 2 * y - 2 * x + 1;
        }
        if (x < y) {
            break;
        }
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        if (x != y) {
            SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
            SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
            SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
            SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
        }
    }
}
void drawRectangleOutline(SDL_Renderer* renderer, float x, float y, float width, float height, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x, y, x + width, y);
    SDL_RenderDrawLine(renderer, x, y + height, x + width, y + height);
    SDL_RenderDrawLine(renderer, x, y, x, y + height);
    SDL_RenderDrawLine(renderer, x + width, y, x + width, y + height);
}
void drawCircle(SDL_Renderer* renderer, int x_center, int y_center, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer, x_center + x, y_center + y);
            }
        }
    }
}
void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}
void drawRotatedRectangleOutline(SDL_Renderer* renderer, int x_center, int y_center, int width, int height, float angleInDegrees, SDL_Color color) {
    float s = sin(angleInDegrees * M_PI / 180.0f);
    float c = cos(angleInDegrees * M_PI / 180.0f);
    int half_width = width / 2;
    int half_height = height / 2;
    int corners[4][2] = {
        {-half_width, -half_height},
        { half_width, -half_height},
        { half_width,  half_height},
        {-half_width,  half_height}
    };
    for (int i = 0; i < 4; i++) {
        int x = corners[i][0];
        int y = corners[i][1];
        corners[i][0] = x * c - y * s + x_center;
        corners[i][1] = x * s + y * c + y_center;
    }
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int i = 0; i < 4; i++) {
        SDL_RenderDrawLine(renderer, corners[i][0], corners[i][1], corners[(i + 1) % 4][0], corners[(i + 1) % 4][1]);
    }
}
bool pointInRectangle(int x, int y, int rectCorners[4][2]) {
    int count = 0;
    for (int i = 0; i < 4; i++) {
        int xi = rectCorners[i][0], yi = rectCorners[i][1];
        int xj = rectCorners[(i + 1) % 4][0], yj = rectCorners[(i + 1) % 4][1];
        if ((yi > y) != (yj > y) &&
            (x < (xj - xi) * (y - yi) / (yj - yi) + xi)) {
            count++;
        }
    }
    return count & 1;
}

void drawRotatedRectangle(SDL_Renderer* renderer, int x_center, int y_center, int width, int height, float angleInDegrees, SDL_Color color) {
    float s = sin(angleInDegrees * M_PI / 180.0f);
    float c = cos(angleInDegrees * M_PI / 180.0f);
    int half_width = width / 2;
    int half_height = height / 2;
    int corners[4][2] = {
        {-half_width, -half_height},
        { half_width, -half_height},
        { half_width,  half_height},
        {-half_width,  half_height}
    };
    int minX = x_center + half_width;
    int minY = y_center + half_height;
    int maxX = x_center - half_width;
    int maxY = y_center - half_height;
    for (int i = 0; i < 4; i++) {
        int x = corners[i][0];
        int y = corners[i][1];
        corners[i][0] = x * c - y * s + x_center;
        corners[i][1] = x * s + y * c + y_center;
        if (corners[i][0] < minX) minX = corners[i][0];
        if (corners[i][1] < minY) minY = corners[i][1];
        if (corners[i][0] > maxX) maxX = corners[i][0];
        if (corners[i][1] > maxY) maxY = corners[i][1];
    }
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            if (pointInRectangle(x, y, corners)) {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}
void drawTriangleOutline(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
    SDL_RenderDrawLine(renderer, x3, y3, x1, y1);
}
void drawTriangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    if (y1 > y2) {
        std::swap(y1, y2);
        std::swap(x1, x2);
    }
    if (y1 > y3) {
        std::swap(y1, y3);
        std::swap(x1, x3);
    }
    if (y2 > y3) {
        std::swap(y2, y3);
        std::swap(x2, x3);
    }
    if (y2 == y3) { 
        for (int i = y1; i <= y3; i++) {
            int xa = x1 + (x3 - x1) * (i - y1) / (y3 - y1);
            int xb = x1 + (x2 - x1) * (i - y1) / (y2 - y1);
            SDL_RenderDrawLine(renderer, xa, i, xb, i);
        }
    }
    else if (y1 == y2) { 
        for (int i = y1; i <= y3; i++) {
            int xa = x2 + (x3 - x2) * (i - y2) / (y3 - y2);
            int xb = x1 + (x3 - x1) * (i - y1) / (y3 - y1);
            SDL_RenderDrawLine(renderer, xa, i, xb, i);
        }
    }
    else { 
        int xm = x1 + (x3 - x1) * (y2 - y1) / (y3 - y1);
        for (int i = y1; i <= y2-1; i++) {
            int xa = x1 + (xm - x1) * (i - y1) / (y2 - y1);
            int xb = x1 + (x2 - x1) * (i - y1) / (y2 - y1);
            SDL_RenderDrawLine(renderer, xa, i, xb, i);
        }
        for (int i = y2; i <= y3; i++) {
            int xa = xm + (x3 - xm) * (i - y2) / (y3 - y2);
            int xb = x2 + (x3 - x2) * (i - y2) / (y3 - y2);
            SDL_RenderDrawLine(renderer, xa, i, xb, i);
        }
    }
}
void drawEllipse(SDL_Renderer* renderer, int x_center, int y_center, int a, int b, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int y = y_center - b; y <= y_center + b; y++) {
        for (int x = x_center - a; x <= x_center + a; x++) {
            if ((x - x_center) * (x - x_center) * b * b +
                (y - y_center) * (y - y_center) * a * a <= a * a * b * b) {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}
void drawEllipseOutline(SDL_Renderer* renderer, int x_center, int y_center, int a, int b, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    int lastX = x_center + a; 
    int lastY = y_center;
    const int numSegments = 100;  
    const float angleIncrement = 2.0f * M_PI / numSegments;
    for (int i = 1; i <= numSegments; i++) {
        float angle = i * angleIncrement;
        int x = x_center + a * cos(angle);
        int y = y_center + b * sin(angle);
        SDL_RenderDrawLine(renderer, lastX, lastY, x, y);
        lastX = x;
        lastY = y;
    }
}
void drawPoint(SDL_Renderer* renderer, int x, int y, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(renderer, x, y);
}
void drawThickLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int thickness, SDL_Color color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    float length = sqrt(dx * dx + dy * dy);
    float angle = atan2(dy, dx) * (180.0 / M_PI);
    int midX = (x1 + x2) / 2;
    int midY = (y1 + y2) / 2;
    drawRotatedRectangle(renderer, midX, midY, (int)length, thickness, angle, color);
}