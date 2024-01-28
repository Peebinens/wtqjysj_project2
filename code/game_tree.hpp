//
// Created by pigzhu on 2023/12/18.
//

#ifndef PROJECT2_GAME_TREE_HPP
#define PROJECT2_GAME_TREE_HPP

#include <unordered_map>
#include <stack>
#include <cstdint>

//score
static std::unordered_map<std::string, int> sc = {{"h0000", 1},
                                                  {"0h000", 1},
                                                  {"00h00", 1},
                                                  {"000h0", 1},
                                                  {"0000h", 1},
                                                  {"hh000", 10},
                                                  {"0hh00", 10},
                                                  {"00hh0", 10},
                                                  {"000hh", 10},
                                                  {"h0h00", 10},
                                                  {"0h0h0", 10},
                                                  {"00h0h", 10},
                                                  {"h00h0", 10},
                                                  {"0h00h", 10},
                                                  {"h000h", 10},
                                                  {"hhh00", 100},
                                                  {"0hhh0", 150},
                                                  {"00hhh", 100},
                                                  {"hh0h0", 100},
                                                  {"0hh0h", 100},
                                                  {"h0hh0", 100},
                                                  {"0h0hh", 100},
                                                  {"hh00h", 100},
                                                  {"h00hh", 100},
                                                  {"h0h0h", 100},
                                                  {"hhhh0", 10000},
                                                  {"hhh0h", 10000},
                                                  {"hh0hh", 10000},
                                                  {"h0hhh", 10000},
                                                  {"0hhhh", 10000},
                                                  {"hhhhh", 100000000},
                                                  {"b0000", -1},
                                                  {"0b000", -1},
                                                  {"00b00", -1},
                                                  {"000b0", -1},
                                                  {"0000b", -1},
                                                  {"bb000", -10},
                                                  {"0bb00", -10},
                                                  {"00bb0", -10},
                                                  {"000bb", -10},
                                                  {"b0b00", -10},
                                                  {"0b0b0", -10},
                                                  {"00b0b", -10},
                                                  {"b00b0", -10},
                                                  {"0b00b", -10},
                                                  {"b000b", -10},
                                                  {"bbb00", -1000},
                                                  {"0bbb0", -2000},
                                                  {"00bbb", -1000},
                                                  {"bb0b0", -1000},
                                                  {"0bb0b", -1000},
                                                  {"b0bb0", -1000},
                                                  {"0b0bb", -1000},
                                                  {"bb00b", -1000},
                                                  {"b00bb", -1000},
                                                  {"b0b0b", -1000},
                                                  {"bbbb0", -100000},
                                                  {"bbb0b", -100000},
                                                  {"bb0bb", -100000},
                                                  {"b0bbb", -100000},
                                                  {"0bbbb", -100000},
                                                  {"bbbbb", -100000000}};

//direction
static int dir[4][2] = {{0, 1},
                        {1, 0},
                        {1, 1},
                        {1, -1}};

class GameTree {
private:

    struct GameNode {

        int pos[2], value, depth, c_cnt;
        char status[15][15];
        GameNode *fa;

        GameNode() : value(INT32_MIN), fa(nullptr), depth(0), c_cnt(0) {
            pos[0] = 7;
            pos[1] = 7;
        }

        GameNode(int x, int y, GameNode *parent = nullptr) : fa(parent), c_cnt(0) {
            pos[0] = x;
            pos[1] = y;
            depth = parent->depth + 1;
            value = depth % 2 ? INT32_MAX : INT32_MIN;
            memcpy(status, parent->status, sizeof(status));
            status[x][y] = (depth % 2) ? 'h' : 'b';
        }

    };

    int radius, dep;
    std::stack<GameNode *> down;
    GameNode *root;

    //alpha-beta cut
    static bool ab_cut(GameNode *node) {
        if (node == nullptr || node->fa == nullptr) return false;
        if ((node->depth % 2 && node->value < node->fa->value) ||
            (!(node->depth % 2) && node->value > node->fa->value))
            return true;
        return false;
    }

public:

    //struct
    GameTree(int radius, int depth, char **s, bool hei) : radius(radius), dep(depth) {
        root = new GameNode();
        //hei
        if (hei) {
            for (int i = 0; i < 15; i++) {
                for (int j = 0; j < 15; j++) root->status[i][j] = s[i][j];
            }
        }
        //bai
        else {
            for (int i = 0; i < 15; i++) {
                for (int j = 0; j < 15; j++) {
                    if (s[i][j] == 'h') root->status[i][j] = 'b';
                    else if (s[i][j] == 'b') root->status[i][j] = 'h';
                    else root->status[i][j] = s[i][j];
                }
            }
        }
        down.push(root);
    }

    //destruct
    ~GameTree() { delete root; }

    //run
    void run() {
        while (!down.empty()) {
            GameNode *temp = down.top();
            down.pop();
            //alpha-beta cut and update
            if (ab_cut(temp->fa)) {
                auto t = temp;
                temp = temp->fa;
                delete t;
                temp->c_cnt--;
                while (temp->fa != nullptr && temp->c_cnt == 0) {
                    if (temp->depth % 2) {
                        if (temp->value > temp->fa->value) {
                            temp->fa->value = temp->value;
                            if (temp->fa == root) root->pos[0] = temp->pos[0], root->pos[1] = temp->pos[1];
                        }
                    }
                    else temp->fa->value = std::min(temp->fa->value, temp->value);
                    temp->fa->c_cnt--;
                    t = temp;
                    temp = temp->fa;
                    delete t;
                }
                continue;
            }

            //generate son
            if (temp->depth < dep) {
                bool have[15][15];
                memset(have, false, sizeof(have));
                for (int i = 0; i < 15; i++) {
                    for (int j = 0; j < 15; j++) {
                        if (temp->status[i][j] == '0') continue;
                        have[i][j] = true;
                        int x1 = std::max(0, i - radius), x2 = std::min(14, i + radius);
                        int y1 = std::max(0, j - radius), y2 = std::min(14, j + radius);
                        for (int x = x1; x <= x2; x++) {
                            for (int y = y1; y <= y2; y++) {
                                if (temp->status[x][y] == '0' && !have[x][y]) {
                                    have[x][y] = true;
                                    GameNode *t = new GameNode(x, y, temp);
                                    temp->c_cnt++;
                                    down.push(t);
                                }
                            }
                        }
                    }
                }
                if (temp->c_cnt != 0) continue;
            }

            //evaluate
            temp->value = 0;
            for (int i = 0; i < 15; i++) {
                for (int j = 0; j < 15; j++) {
                    for (auto &k: dir) {
                        if (i + 4 * k[0] < 0 || i + 4 * k[0] > 14 || j + 4 * k[1] < 0 || j + 4 * k[1] > 14) continue;
                        std::string s;
                        for (int l = 0; l < 5; l++) s.push_back(temp->status[i + l * k[0]][j + l * k[1]]);
                        if (sc.find(s) != sc.end()) temp->value += sc[s];
                    }
                }
            }

            //update
            while (temp->fa != nullptr && temp->c_cnt == 0) {
                if (temp->depth % 2) {
                    if (temp->value > temp->fa->value) {
                        temp->fa->value = temp->value;
                        if (temp->fa == root) root->pos[0] = temp->pos[0], root->pos[1] = temp->pos[1];
                    }
                }
                else temp->fa->value = std::min(temp->fa->value, temp->value);
                temp->fa->c_cnt--;
                auto t = temp;
                temp = temp->fa;
                delete t;
            }
        }
    }

    //get next pos
    std::pair<int, int> get_next() { return std::make_pair(root->pos[0], root->pos[1]); }

};

#endif //PROJECT2_GAME_TREE_HPP
