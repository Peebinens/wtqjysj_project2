//
// Created by pigzhu on 2023/12/20.
//

#ifndef PROJECT2_HOME_PAGE_HPP
#define PROJECT2_HOME_PAGE_HPP

class Home_page : public Fl_Widget {

    Fl_Image *background;
    QiziGroup *a;
    Fl_Menu_Bar *b;
    Fl_Button *c, *d;

public:

    //load home background
    Home_page(int x, int y, int w, int h, QiziGroup *_a, Fl_Menu_Bar *_b, Fl_Button *_c, Fl_Button *_d) :
        Fl_Widget(x, y, w, h), a(_a), b(_b), c(_c), d(_d) {
        background = new Fl_PNG_Image("home.png");
//        background = Fl_Shared_Image::get("home.png");
    }

    //draw background
    void draw() override {
        background = background->copy(w(), h());
        background->draw(0, 0);
    }

    int handle(int event) override {
        if (event == FL_PUSH) {
            int x = Fl::event_x(), y = Fl::event_y();

            //enter game
            if (x < 0.58 * w() && x > 0.37 * w() && y > 0.4 * h() && y < 0.48 * h()) {
                a->show();
                b->show();
                c->show();
                d->show();
                this->hide();
            }

            //exit game
            else if (x < 0.58 * w() && x > 0.37 * w() && y > 0.52 * h() && y < 0.6 * h()) exit(0);
        }
        return Fl_Widget::handle(event);
    }

};

#endif //PROJECT2_HOME_PAGE_HPP
