//
// Created by pigzhu on 2023/12/18.
//

#ifndef PROJECT2_QIZI_HPP
#define PROJECT2_QIZI_HPP

#include "game_tree.hpp"

class QiziGroup : public Fl_Box {
private:
    struct Qizi {
        int pos[2];
        Fl_Color color;
        Fl_Image *bg;

        Qizi(int x, int y, Fl_Color _color, Fl_Image *_bg = nullptr) : color(_color), bg(_bg) { pos[0] = x; pos[1] = y; }

    };

    std::vector<Qizi> all;
    int ge;
    int _level[2] = {2, 2};
    char **_status;
    bool _1_2, begin, win;
    bool _ai[2] = {false, false};
    Fl_Color _color[2] = {FL_BLACK, FL_WHITE};
    Fl_Image *_bg[2] = {nullptr, nullptr};
    Fl_Image *_bbg[2] = {nullptr, nullptr};
    Fl_Image *_sbg[2] = {nullptr, nullptr};
    std::stack<char **> log;

public:

    //set empty chess
    QiziGroup(int x, int y, int w, int h) :
            Fl_Box(x, y, w, h), _1_2(true), begin(false), win(false) {
        _status = new char *[15];
        ge = (h - 60) / 14;
        for (int i = 0; i < 15; i++) {
            _status[i] = new char[15];
            for (int j = 0; j < 15; j++) _status[i][j] = '0';
        }
    }

    //for resize
    //da: true--grow; false--shrink
    void size(int _w, bool da) {
        delete _bbg[0];
        delete _bbg[1];
        delete _sbg[0];
        delete _sbg[1];
        for (auto &qz: all) {
            int i = (qz.pos[0] - 30 - x()) / ge, j = (qz.pos[1] - 30 - y()) / ge;
            qz.pos[0] += da ? i : -i;
            qz.pos[1] += da ? j : -j;
        }
        da ? ge++ : ge--;
        for(int i = 0; i < 2; i++) {
            if (_bg[i] != nullptr) {
                int mid = int(ge * 0.27);
                double zoom = std::max(double(2 * mid + 1) / _bg[i]->w(), double(2 * mid + 1) / _bg[i]->h());
                _bbg[i] = _bg[i]->copy(int(_bg[i]->w() * zoom), int(_bg[i]->h() * zoom));
                mid = int(ge * 0.15);
                zoom = std::max(double(2 * mid + 1) / _bg[i]->w(), double(2 * mid + 1) / _bg[i]->h());
                _sbg[i] = _bg[i]->copy(int(_bg[i]->w() * zoom), int(_bg[i]->h() * zoom));
                for (int j = i; j < all.size(); j += 2) all[j].bg = _bbg[i];
            }
        }
        Fl_Box::size(_w, _w);
    }

    //draw chess
    void draw() override {
        Fl_Box::draw();
        int mid = int(ge * 0.27);

        //draw all chess
        for (auto i: all) {
            if (i.bg == nullptr) {
                fl_color(i.color);
                fl_pie(i.pos[0] - mid, i.pos[1] - mid, 2 * mid + 1, 2 * mid + 1, 0, 360);
            }
            else {
                fl_push_clip(i.pos[0] - mid, i.pos[1] - mid, 2 * mid + 1, 2 * mid + 1);
                int xx = i.pos[0] - mid, yy = i.pos[1] - mid;
                if (i.bg->w() > i.bg->h()) xx += (2 * mid + 1 - i.bg->w()) / 2;
                else yy += (2 * mid + 1 - i.bg->h()) / 2;
                i.bg->draw(xx, yy);
                fl_pop_clip();
            }
        }

        //draw the just added chess
        if (!all.empty()) {
            auto i = all.back();
            if (i.bg == nullptr) {
                fl_color(FL_RED);
                fl_line_style(FL_DOT, 3);
                fl_circle(i.pos[0], i.pos[1], mid - 2);
            }
            else {
                fl_color(FL_RED);
                fl_line_style(FL_DOT, 3);
                fl_rect(i.pos[0] - mid + 2, i.pos[1] - mid + 2, 2 * mid - 3, 2 * mid - 3);
            }
        }
    }

    //set the color of chess
    //ind: 1--hei; 2--bai
    void set_color(Fl_Color color, int ind) {
        _color[ind] = color;
        for (int i = ind; i < all.size(); i += 2) {
            all[i].bg = nullptr;
            all[i].color = color;
        }
        delete _bg[ind];
        delete _bbg[ind];
        delete _sbg[ind];
        _bg[ind] = nullptr;
        _bbg[ind] = nullptr;
        _sbg[ind] = nullptr;
        parent()->redraw();
    }

    //set the background of chess
    //ind: 1--hei; 2--bai
    void set_bg(Fl_Image *p, int ind) {
        delete _bg[ind];
        delete _bbg[ind];
        delete _sbg[ind];
        _bg[ind] = p;
        int mid = int(ge * 0.27);
        double zoom = std::max(double(2 * mid + 1) / p->w(), double(2 * mid + 1) / p->h());
        Fl_Image *temp = p->copy(int(p->w() * zoom), int(p->h() * zoom));
        _bbg[ind] = temp;
        mid = int(ge * 0.15);
        zoom = std::max(double(2 * mid + 1) / p->w(), double(2 * mid + 1) / p->h());
        temp = p->copy(int(p->w() * zoom), int(p->h() * zoom));
        _sbg[ind] = temp;
        for (int i = ind; i < all.size(); i += 2) all[i].bg = _bbg[ind];
        parent()->redraw();
    }

    //add chess and check
    //ind: 1--hei; 2--bai
    bool _add(int _x, int _y, int ind) {
        if (_status[_x][_y] != '0') return false;
        all.emplace_back(_x * ge + 30 + x(), _y * ge + 30 + y(), _color[ind], _bbg[ind]);
        _status[_x][_y] = ind == 0 ? 'h' : 'b';
        redraw();
        Fl::flush();
        get_win(_x, _y, ind);
        return true;
    }

    //begin game
    void set_begin() {
        if (win) clear();
        begin = true;
        if (all.empty()) _1_2 = true;
    }

    //set ai level
    //ind: 1--hei; 2--bai
    void set_level(int _l, int ind) { _level[ind] = _l; }

    //set ai or person
    //ind: 1--hei; 2--bai
    void set_ai(int ind) { _ai[ind] = !_ai[ind]; }

    //for stop
    void zhanting() {
        begin = false;
        redraw();
    }

    //regret one step
    void ret_1() {
        if (win) win = false;
        if (log.empty()) return;
        char **de = log.top();
        log.pop();
        for (int i = 0; i < 15; i++) delete[] de[i];
        delete[] de;
        if (log.empty()) {
            _status[(all.front().pos[0] - 30 - x()) / ge][(all.front().pos[1] - 30 - y()) / ge] = '0';
            all.clear();
        }
        else {
            for (int i = 0; i < 15; i++)
                for (int j = 0; j < 15; j++) _status[i][j] = log.top()[i][j];
            Qizi temp = all.back();
            if (!all.empty() && _status[(temp.pos[0] - 30 - x()) / ge][(temp.pos[1] - 30 - y()) / ge] == '0')
                all.pop_back();
        }
        _1_2 = !_1_2;
        begin = false;
        parent()->redraw();
    }

    //clear all chess
    void clear() {
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) _status[i][j] = '0';
        }
        all.clear();
        win = begin = false;
        while (!log.empty()) {
            char **de = log.top();
            log.pop();
            for (int i = 0; i < 15; i++) delete[] de[i];
            delete[] de;
        }
        _1_2 = true;
        parent()->redraw();
    }

    //check win or draw
    //ind: 1--hei; 2--bai
    void get_win(int _x, int _y, int ind) {
        _1_2 = !_1_2;

        //check
        int x_min = std::max(0, _x - 4), x_max = std::min(14, _x + 4);
        int y_min = std::max(0, _y - 4), y_max = std::min(14, _y + 4);
        std::string check;
        for (int i = x_min; i <= x_max; i++) check.push_back(_status[i][_y]);
        check.push_back('0');
        for (int i = y_min; i <= y_max; i++) check.push_back(_status[_x][i]);
        check.push_back('0');
        int _i = _x - std::min(_x - x_min, _y - y_min), _j = _y - std::min(_x - x_min, _y - y_min);
        while (_i <= x_max && _j <= y_max) {
            check.push_back(_status[_i][_j]);
            _i++;
            _j++;
        }
        check.push_back('0');
        _i = _x + std::min(x_max - _x, _y - y_min), _j = _y - std::min(x_max - _x, _y - y_min);
        while (_i >= x_min && _j <= y_max) {
            check.push_back(_status[_i][_j]);
            _i--;
            _j++;
        }

        //win
        if (ind == 0 && check.find("hhhhh") != std::string::npos) {
            begin = false;
            win = true;
            fl_alert("先手获胜！");
            _1_2 = true;
        }
        else if (ind == 1 && check.find("bbbbb") != std::string::npos) {
            begin = false;
            win = true;
            fl_alert("后手获胜！");
            _1_2 = true;
        }

        //draw
        bool flag = true;
        for (int i = 0; i < 15 && flag; i++)
            for (int j = 0; j < 15 && flag; j++)
                if (_status[i][j] == '0') flag = false;
        if (flag) {
            begin = false;
            win = true;
            fl_alert("平局！");
            _1_2 = true;
        }
    }

    //solve mouse event
    int handle(int event) {

        //shortcut
        if(event == FL_KEYDOWN) {
            if (Fl::event_key() == FL_BackSpace) ret_1();
            else if(Fl::event_key() == FL_Enter) set_begin();
            else if(Fl::event_key() == FL_Delete) clear();
            else if(Fl::event_key() == FL_End) zhanting();
            return 1;
        }

        //not begin
        if (!begin) return 1;

        //first or second
        int ind = _1_2 ? 0 : 1;

        //ai
        if (_ai[ind]) {
            GameTree *tree;
            std::pair<int, int> ans;
            tree = new GameTree(1, _level[ind], _status, _1_2);
            tree->run();
            ans = tree->get_next();
            delete tree;
            _add(ans.first, ans.second, ind);
            char **temp = new char *[15];
            for (int i = 0; i < 15; i++) {
                temp[i] = new char[15];
                for (int j = 0; j < 15; j++) temp[i][j] = _status[i][j];
            }
            log.push(temp);
        }

        //person
        else {
            int _x = (Fl::event_x() + ge / 2 - x() - 30) / ge;
            int _y = (Fl::event_y() + ge / 2 - y() - 30) / ge;
            if (_x < 0 || _x > 14 || _y < 0 || _y > 14) return 0;

            //push
            if (event == FL_PUSH) {
                if (Fl::event_button() == FL_LEFT_MOUSE) {
                    if (_add(_x, _y, ind)) {
                        char **temp = new char *[15];
                        for (int i = 0; i < 15; i++) {
                            temp[i] = new char[15];
                            for (int j = 0; j < 15; j++) temp[i][j] = _status[i][j];
                        }
                        log.push(temp);
                    }
                }
            }

            //move
            else if (event == FL_MOVE) {
                redraw();
                Fl::flush();
                int mid = int(ge * 0.15);
                if (_status[_x][_y] != '0') return 1;
                if (_sbg[ind] != nullptr) {
                    fl_push_clip(_x * ge + 30 + x() - mid, _y * ge + 30 + y() - mid, 2 * mid + 1, 2 * mid + 1);
                    int xx = _x * ge + 30 + x() - mid, yy = _y * ge + 30 + y() - mid;
                    if (_sbg[ind]->w() > _sbg[ind]->h()) xx += (2 * mid + 1 - _sbg[ind]->w()) / 2;
                    else yy += (2 * mid + 1 - _sbg[ind]->h()) / 2;
                    _sbg[ind]->draw(xx, yy);
                    fl_pop_clip();
                }
                else {
                    fl_color(_color[ind]);
                    fl_pie(_x * ge + 30 + x() - mid, _y * ge + 30 + y() - mid, 2 * mid + 1, 2 * mid + 1, 0, 360);
                }
            }
        }
        std::cout << Fl::event_x() << " " << Fl::event_y() << std::endl;
        return 0;
    }

};

#endif //PROJECT2_QIZI_HPP
