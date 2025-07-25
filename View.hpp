#ifndef VIEW_H
#define VIEW_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

class View {
private:
    std::vector<std::vector<std::string>> map;
    int size;
    double scale;
    double x;
    double y;

    std::vector<std::vector<std::string>> rotate_matrix();

public:
    View();
    View(const View &v);
    ~View() = default;
    View &operator=(const View &v);

    void default_size();
    void new_size(int s);
    void new_zoom(double z);
    void pan(double xx, double yy);
    void show();
    void insert_obj(double i, double j, std::string name);
    void clear();

    int getSize() const;
    double getScale() const;
    double getX() const;
    double getY() const;
};

#endif // VIEW_H
