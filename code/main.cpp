#include "global.hpp"

//main window
Fl_Double_Window *m_w = new Fl_Double_Window(200, 100, 830, 855, "Five Chess");

//game page
Fl_Menu_Bar *menu = new Fl_Menu_Bar(0, 0, 780, 25);
Fl_Button *shrink = new Fl_Button(780, 0, 25, 25, "-");
Fl_Button *grow = new Fl_Button(805, 0, 25, 25, "+");
Qipan *qp = new Qipan(0, 25, 830, 830);
QiziGroup *qs = new QiziGroup(0, 25, 830, 830);

//home page
Home_page *hp = new Home_page(0, 0, 830, 855, qs, menu, shrink, grow);

//set icon
Fl_PNG_Image* _ic = new Fl_PNG_Image("256.png");

//set backgroud
void choose_bg_cb(Fl_Widget *o, void *v) {
    const char *l = fl_file_chooser("选择背景图片", "*", nullptr, 0);
    qp->add_image(l);
    qp->redraw();
    qs->redraw();
    Fl::flush();
}

//set chess color
void hc_cb(Fl_Widget *o, void *v) {
    uchar r, g, b;
    Fl_Box *temp = (Fl_Box *) v;
    if (!fl_color_chooser("New Hei Color", r, g, b, 3)) return;
    qs->set_color(fl_rgb_color(r, g, b), 0);
    uchar *p = new uchar[3 * 25 * 25];
    for (int i = 0; i < 3 * 25 * 25; i += 3) {
        p[i] = r;
        p[i + 1] = g;
        p[i + 2] = b;
    }
    Fl_Image *bbb = new Fl_RGB_Image(p, 25, 25);
    temp->image(bbb);
    temp->redraw();
    Fl::flush();
    delete[] p;
}

void bc_cb(Fl_Widget *o, void *v) {
    uchar r, g, b;
    Fl_Box *temp = (Fl_Box *) v;
    if (!fl_color_chooser("New Bai Color", r, g, b, 3)) return;
    qs->set_color(fl_rgb_color(r, g, b), 1);
    uchar *p = new uchar[3 * 25 * 25];
    for (int i = 0; i < 3 * 25 * 25; i += 3) {
        p[i] = r;
        p[i + 1] = g;
        p[i + 2] = b;
    }
    Fl_Image *bbb = new Fl_RGB_Image(p, 25, 25);
    temp->image(bbb);
    temp->redraw();
    Fl::flush();
    delete[] p;
}

//set chess background
void hg_cb(Fl_Widget *o, void *v) {
    const char *l = fl_file_chooser("New Hei Chess Image", "*", nullptr, 0);
    if (l == nullptr) return;
    Fl_Box *temp = (Fl_Box *) v;
    Fl_Image *bbb;
    if(l[strlen(l)-2]=='n')  bbb = new Fl_PNG_Image(l);
    else bbb = new Fl_JPEG_Image(l);
    qs->set_bg(bbb, 0);
    bbb = bbb->copy(25, 25);
    delete temp->image();
    temp->image(bbb);
    temp->redraw();
}

void bg_cb(Fl_Widget *o, void *v) {
    const char *l = fl_file_chooser("New Bai Chess Image", "*", nullptr, 0);
    if (l == nullptr) return;
    Fl_Box *temp = (Fl_Box *) v;
    Fl_Image *bbb;
    if(l[strlen(l)-2]=='n')  bbb = new Fl_PNG_Image(l);
    else bbb = new Fl_JPEG_Image(l);
    qs->set_bg(bbb, 1);
    bbb = bbb->copy(25, 25);
    delete temp->image();
    temp->image(bbb);
    temp->redraw();

}

//shrink and grow for different computer
void shrink_cb(Fl_Widget *o, void *v) {
    int new_w = m_w->w() - 14, new_h = m_w->h() - 14;
    m_w->size(new_w, new_h);
    qp->size(new_w, new_w);
    qs->size(new_w, false);
    hp->size(new_w, new_h);
    menu->size(new_w - 50, 25);
    shrink->position(new_w - 50, 0);
    grow->position(new_w - 25, 0);
    Fl::flush();
}

void grow_cb(Fl_Widget *o, void *v) {
    int new_w = m_w->w() + 14, new_h = m_w->h() + 14;
    m_w->size(new_w, new_h);
    qp->size(new_w, new_w);
    qs->size(new_w, true);
    hp->size(new_w, new_h);
    menu->size(new_w - 50, 25);
    shrink->position(new_w - 50, 0);
    grow->position(new_w - 25, 0);
    Fl::flush();
}

int main() {
    fl_register_images();

    //set ico
    m_w->icon(_ic);
    m_w->end();

    //set chess window
    Fl_Double_Window *chess_window = new Fl_Double_Window(m_w->x(), m_w->y() + 50, 355, 100, "Setting Chess");
    chess_window->icon(_ic);
    Fl_Button *hc_button = new Fl_Button(0, 10, 150, 25, "Set Hei Chess Color");
    Fl_Button *bc_button = new Fl_Button(0, 40, 150, 25, "Set Bai Chess Color");
    Fl_Button *ok_button = new Fl_Button(295, 70, 50, 25, "OK");
    Fl_Button *hg_button = new Fl_Button(150, 10, 150, 25, "Set Hei Chess Image");
    Fl_Button *bg_button = new Fl_Button(150, 40, 150, 25, "Set Bai Chess Image");
    Fl_Box *h_e = new Fl_Box(320, 10, 25, 25);
    Fl_Box *b_e = new Fl_Box(320, 40, 25, 25);
    h_e->box(FL_THIN_UP_BOX);
    h_e->align(FL_ALIGN_INSIDE);
    h_e->color(FL_BLACK);
    b_e->box(FL_THIN_DOWN_BOX);
    b_e->align(FL_ALIGN_INSIDE);
    b_e->color(FL_WHITE);
    hc_button->callback(hc_cb, h_e);
    bc_button->callback(bc_cb, b_e);
    hg_button->callback(hg_cb, h_e);
    bg_button->callback(bg_cb, b_e);
    ok_button->callback([](Fl_Widget *, void *v) {
        Fl_Double_Window *temp = (Fl_Double_Window *) v;
        temp->hide();
    }, chess_window);
    chess_window->end();

    //set top menu
    shrink->callback(shrink_cb, chess_window);
    grow->callback(grow_cb, chess_window);
    menu->add("Back", "^q", [](Fl_Widget *, void *v) {
        hp->show();
        qs->hide();
        menu->hide();
        grow->hide();
        shrink->hide();
    }, nullptr);
    menu->add("UI/Backgroud", "^a", choose_bg_cb);
    menu->add("UI/Chess", "^s", [](Fl_Widget *, void *v) {
        Fl_Double_Window *temp = (Fl_Double_Window *) v;
        temp->show();
    }, chess_window);
    menu->add("Game/Begin", FL_Enter, [](Fl_Widget *, void *v) { qs->set_begin(); }, nullptr);
    menu->add("Game/Stop", FL_End, [](Fl_Widget *, void *v) { qs->zhanting(); }, nullptr);
    menu->add("Game/Regret", FL_BackSpace, [](Fl_Widget *, void *v) { qs->ret_1(); }, nullptr);
    menu->add("Game/Clear", FL_Delete, [](Fl_Widget *, void *v) { qs->clear(); }, nullptr);
    menu->add("Mode/1_AI  ", 0, [](Fl_Widget *, void *v) { qs->set_ai(0); }, nullptr, FL_MENU_TOGGLE);
    menu->add("Mode/2_AI  ", 0, [](Fl_Widget *, void *v) { qs->set_ai(1); }, nullptr, FL_MENU_TOGGLE | FL_MENU_DIVIDER);
    menu->add("Mode/1_Level", 0, nullptr, nullptr, FL_SUBMENU);
    menu->add("Mode/1_Level/EEEasy", 0, [](Fl_Widget *, void *v) { qs->set_level(2, 0); }, nullptr, FL_MENU_RADIO | FL_MENU_VALUE);
    menu->add("Mode/1_Level/HHHard", 0, [](Fl_Widget *, void *v) { qs->set_level(3, 0); }, nullptr, FL_MENU_RADIO);
    menu->add("Mode/2_Level", 0, nullptr, nullptr, FL_SUBMENU);
    menu->add("Mode/2_Level/EEEasy", 0, [](Fl_Widget *, void *v) { qs->set_level(2, 1); }, nullptr, FL_MENU_RADIO | FL_MENU_VALUE);
    menu->add("Mode/2_Level/HHHard", 0, [](Fl_Widget *, void *v) { qs->set_level(3, 1); }, nullptr, FL_MENU_RADIO);

    //help window
    Fl_Window *he = new Fl_Window(300, 265, "Help");
    he->icon(_ic);
    Fl_Box *box = new Fl_Box(0, 2, 300, 265,
                             "Five Chess --by pigzhu\n"
                             "操作说明：\n"
                             "0.点击Back返回主界面\n"
                             "1.点击UI/Background设置背景\n"
                             "2.点击UI/Chess设置你喜欢的棋子样式\n"
                             "3.点击Game/Begin开始游戏\n"
                             "4.点击Game/Stop暂停游戏\n"
                             "5.点击Game/Regret悔棋\n"
                             "6.点击Game/Clear清空棋盘\n"
                             "7.点击Mode/1_AI设置1P为AI\n"
                             "8.点击Mode/1_Level/HHHard设置1P为困难AI\n"
                             "9.点击Back返回主界面\n"
                             "10.点击右上角'+'与'-'可以缩放窗口\n"
                             "上述功能部分可通过快捷键实现\n"
                             "更多信息见：\n"
                             "https://github.com/Peebinens/wtqjysj_project2");
    box->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT | FL_ALIGN_TOP);
    he->end();
    menu->add("Help", 0, [](Fl_Widget*, void *v) {
        Fl_Window *temp = (Fl_Window *) v;
        temp->show();
    }, he);

    //set show
    qs->hide();
    menu->hide();
    grow->hide();
    shrink->hide();
    menu->textfont(FL_TIMES_BOLD);
    m_w->show();

    return Fl::run();
}
