#include "View.hpp"

View::View() {
    size = 25;
    scale = 2;
    map.resize(size, std::vector<std::string>(size, "."));
    x = -10;
    y = -10;
}

View::View(const View &v) {
    size = v.size;
    scale = v.scale;
    map.clear();
    map.resize(size, std::vector<std::string>(size, "."));
    map = v.map;
    x = v.x;
    y = v.y;
}

View &View::operator=(const View &v) {
    if(this == &v) {
        return *this;
    }
    size = v.size;
    scale = v.scale;
    map.clear();
    map.resize(size, std::vector<std::string>(size, "."));
    x = v.x;
    y = v.y;
    return *this;
}

void View::default_size() {
    size = 25;
    scale = 2;
    x = -10;
    y = -10;
}

void View::new_size(int s) {
    if(s < 7) {
        std::cout << "ERROR: New map size is too small." << std::endl;
        return;
    }
    if(s > 30) {
        std::cout << "ERROR: New map size is too big." << std::endl;
        return;
    }
    size = s;
    map.resize(size, std::vector<std::string>(size, "."));
}

void View::new_zoom(double z) {
    if(z < 0) {
        std::cout << "ERROR: New map scale must be positive." << std::endl;
        return;
    }
    scale = z;
}

void View::pan(double xx, double yy) {
    this->x = xx;
    this->y = yy;
}

void View::show() {
    std::cout << "Display size: " << size << ", scale: " << std::fixed << std::setprecision(2) << scale;
    std::cout << ", origin: " << std::setprecision(2) << "(" << x << ", " << y << ")" << std::endl;

    std::vector<std::vector<std::string>> matrix = rotate_matrix();

    double axis = y + (scale * (size - 1));
    int tmp = size - 1;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            if(j != 0) {
                std::cout << " ";
            }
            else if(tmp%3 == 0){
                std::cout << std::setw(3) << std::left << (int)axis;
            }
            else {
                std::cout << std::setw(3) << std::left << " ";
            }
            std::cout << std::left << std::setw(2) << matrix[i][j];
        }
        std::cout << std::endl;
        tmp--;
        axis -= scale;
    }

    std::cout << std::setw(3) << std::left << " ";

    axis = x;
    for(int i = 0; i < size; i++) {
        if(i%3 == 0) {
            std::cout << std::setw(3) << std::left << (int)axis;
        }
        else {
            std::cout << std::setw(3) << std::left << " ";
        }
        axis += scale;
    }
    std::cout << std::endl << std::endl;
}

void View::insert_obj(double i, double j, std::string name) {
    // Convert world coordinates to view grid indices
    int row = static_cast<int>((j - y) / scale);
    int col = static_cast<int>((i - x) / scale);

    // Check bounds before inserting into the map
    if (row >= 0 && row < size && col >= 0 && col < size) {
        map[row][col] = std::move(name);
    }
}

void View::clear() {
    map.clear();
    map.resize(size, std::vector<std::string>(size, "."));
}

std::vector<std::vector<std::string>> View::rotate_matrix() {
    std::vector<std::vector<std::string>> tmp(size, std::vector<std::string>(size, "."));
    for(int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            tmp[size - j - 1][i] = map[i][j];
        }
    }
    return tmp;
}

int View::getSize() const {
    return size;
}

double View::getScale() const {
    return scale;
}

double View::getX() const {
    return x;
}

double View::getY() const {
    return y;
}