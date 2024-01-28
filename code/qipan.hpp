//
// Created by pigzhu on 2023/12/18.
//

#ifndef PROJECT2_QIPAN_HPP
#define PROJECT2_QIPAN_HPP

class Qipan : public Fl_Widget {
private:

    Fl_Image *now;
    Fl_Image *background;
    std::string ind[16] = {"0", "1", "2", "3", "4","5", "6", "7",
                           "8", "9","A", "B", "C", "D", "E", "F"};

public:

    //load background
    Qipan(int x, int y, int w, int h) :
        Fl_Widget(x, y, w, h) {
        background = new Fl_JPEG_Image("bg.jpeg");
        double zoom = std::max(double(w) / background->w(), double(h) / background->h());
        now = background->copy(int(background->w() * zoom), int(background->h() * zoom));
    }

    //draw background and qipan
    void draw() override {

        //draw background
        if (now != nullptr) {
            fl_push_clip(x(), y(), w(), h());
            int _x = x(), _y = y();
            if (now->w() > w()) _x = x() + (w() - now->w()) / 2;
            else if (now->h() > h()) _y = y() + (h() - now->h()) / 2;
            now->draw(_x, _y);
            fl_pop_clip();
        }

        //draw line and dot
        fl_color(FL_BLACK);
        int wid = (w() - 60) / 14;
        wid = std::min(wid, (h() - 60) / 14);
        for (int i = 0; i < 15; i++) {
            fl_line_style(FL_SOLID, 3);
            fl_line(x() + 30, y() + 30 + wid * i, x() + 30 + wid * 14, y() + 30 + wid * i);
            fl_line(x() + 30 + wid * i, y() + 30, x() + 30 + wid * i, y() + 30 + wid * 14);
        }
        fl_line_style(FL_SOLID, 2);
        fl_pie(x() + 30 + wid * 7 - 5, y() + 30 + wid * 7 - 5, 11, 11, 0, 360);
        fl_pie(x() + 30 + wid * 3 - 5, y() + 30 + wid * 3 - 5, 11, 11, 0, 360);
        fl_pie(x() + 30 + wid * 3 - 5, y() + 30 + wid * 11 - 5, 11, 11, 0, 360);
        fl_pie(x() + 30 + wid * 11 - 5, y() + 30 + wid * 11 - 5, 11, 11, 0, 360);
        fl_pie(x() + 30 + wid * 11 - 5, y() + 30 + wid * 3 - 5, 11, 11, 0, 360);

        //draw index
        for (int i = 1; i < 15; i++) {
            fl_font(FL_COURIER_BOLD, 20);
            fl_color(FL_BLACK);
            fl_draw(ind[i].c_str(), x() + 25 + wid * i, y() + 16);
            fl_draw(ind[i].c_str(), x() + 5, y() + 37 + wid * i);
        }
        fl_font(FL_COURIER_BOLD, 20);
        fl_draw("0", x() + 5, y() + 18);
    }

    //change background
    void add_image(const char *filename) {
        if (filename == nullptr) return;
        delete background;
        delete now;
        if(filename[strlen(filename)-2]=='n')  background = new Fl_PNG_Image(filename);
        else background = new Fl_JPEG_Image(filename);
        double zoom = std::max(double(w()) / background->w(), double(h()) / background->h());
        now = background->copy(int(background->w() * zoom), int(background->h() * zoom));
    }

    void size(int W, int H) {
        Fl_Widget::size(W, H);
        double zoom = std::max(double(W) / background->w(), double(H) / background->h());
        delete now;
        now = background->copy(int(background->w() * zoom), int(background->h() * zoom));
    }

};

#endif //PROJECT2_QIPAN_HPP
