#ifndef REGULARCUBEMAP_H
#define REGULARCUBEMAP_H

#include <QObject>
#include <QLabel>


enum class Edge: ushort {
        U, R, D, L
};

class Side : public QPixmap
{

public:


    Side(QColor color, ushort dimension);
    ~Side();

    void set_stickers(const std::vector<QPixmap>& stickers_to_paint, Edge trg_edge);
    void redraw(const QPixmap& other = QPixmap(0,0));
    ushort get_rotation() { return rotation; }
    void set_rotation(ushort r) { rotation = r; }
    std::vector<QPixmap> get_temp_stickers();
    std::vector<QPixmap> get_stickers(Edge source_edge);
    QPixmap get_color_sticker() { return *base_color_sticker; }
    void rotate(bool prim = false);
    void restart();
    ushort get_width() { return dimension * sticker_width; }
    ushort get_height() { return dimension * sticker_height; }


private:
    ushort dimension;
    std::vector<std::vector<QPixmap*>> stickers;
    std::vector<QPixmap> temp_stickers;
    QPixmap *base_color_sticker;
    ushort rotation = 0;

    ushort sticker_width;
    ushort sticker_height;

    void set_temp_stickers(const std::vector<QPixmap*>& st);
};

class RegularCubeMap :public QLabel
{
    Q_OBJECT
public:

    RegularCubeMap(ushort cube_dimension);
    ~RegularCubeMap();

    void make_move(const QString& move);
    void rotate_side(ushort side_index, bool prim = false);
    void paint_edge(ushort side_index, std::vector<QPixmap> stickers, Edge target_edge, bool should_reverse = false);
    void redraw();
    void restart() {
        for(auto i: sides)
            i->restart();
    }
private:
    enum colors: ushort {
        white,
        green,
        red,
        blue,
        orange,
        yellow
    };

    ushort side_width;
    ushort side_height;
    Side *sides[6];
    QPixmap *cube_map;
    ushort dimension;
};

#endif // REGULARCUBEMAP_H
