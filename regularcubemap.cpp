#include "regularcubemap.h"

#include <QPixmap>
#include <QPainter>
#include <QGraphicsRotation>
#include <algorithm>


/*
 * ===================================================
 *      SIDE CLASS FUCTION DEFINITIONS
 * ===================================================
 */

Side::Side(QColor color, ushort dim) :  QPixmap( dim * (30 / ((float)dim / 3)), dim * (30 / ((float)dim / 3)) )
{
    sticker_width = sticker_height = width() / dim;
    QPainter *painter = new QPainter();
    dimension = dim;

    temp_stickers.reserve(dimension);


    stickers.reserve(dimension);
    for(int i = 0; i < dimension; i++)
    {
        stickers.push_back(std::vector<QPixmap*>());
        temp_stickers.push_back(QPixmap());

        stickers[i].reserve(dimension);
        for(int j = 0; j < dimension; j++)
        {
                stickers[i].push_back(new QPixmap(sticker_width, sticker_height));


                painter->begin(stickers[i][j]);
                painter->setBrush(Qt::black);
                painter->drawRect(0,0,sticker_width,sticker_height);
                painter->fillRect(1,1, sticker_width - 2 , sticker_height - 2, color);
                painter->end();

                painter->begin(this);
                int x = j * sticker_width;
                int y = i * sticker_height;
                painter->drawPixmap(x,y, sticker_width, sticker_height, *stickers[0][0], 0, 0, sticker_width, sticker_height);
                painter->end();
        }
    }
    base_color_sticker = new QPixmap(*stickers[0][0]);

    delete painter;
}
void Side::redraw(const QPixmap& other)
{

    QPainter * painter = new QPainter();
    fill(Qt::white);

    painter->begin(this);

    if(!other.isNull())
    {
        painter->drawPixmap(0,0, width(), height(), other, 0,0, other.width(), other.height());
        painter->end();
        return;
    }

    for(int i = 0; i < dimension; i++)
    {
        for(int j = 0; j < dimension; j++)
        {

                painter->drawPixmap(j * sticker_width ,i * sticker_height, sticker_width, sticker_height, *stickers[i][j], 0, 0, sticker_width, sticker_height);
        }
    }
    painter->end();
}

void Side::set_temp_stickers(const std::vector<QPixmap*>& st)
{
        for(std::vector<QPixmap*>::size_type i = 0; i < st.size(); i++)
            temp_stickers[i] = *st[i];

}

void Side::set_stickers(const std::vector<QPixmap>& stickers_to_paint, Edge trg_edge)
{
    using index_type = std::vector<QPixmap>::size_type;
    std::vector<QPixmap*> temp;
     switch (trg_edge)
     {
         case Edge::U:
         set_temp_stickers(stickers[0]);
         for(index_type i = 0; i < stickers_to_paint.size(); i++)
                *stickers[0][i] = stickers_to_paint[i];

         break;
         case Edge::R:
                 for(index_type i = 0; i < stickers.size(); i++)
                        temp.push_back(stickers[i].back());
                 set_temp_stickers(temp);
                 for(index_type i = 0; i < stickers.size(); i++)
                        *(stickers[i].back()) = stickers_to_paint[i];
         break;
         case Edge::D:
                 set_temp_stickers(stickers.back());
                for(index_type i = 0; i < stickers_to_paint.size(); i++)
                        *stickers.back()[i] = stickers_to_paint[i];
         break;
         case Edge::L:
                 for(index_type i = 0; i < stickers.size(); i++)
                        temp.push_back(stickers[i][0]);
                 set_temp_stickers(temp);
                 for(index_type i = 0; i < stickers.size(); i++)
                        *stickers[i][0] = stickers_to_paint[i];
     }
}

std::vector<QPixmap> Side::get_temp_stickers()
{
        return temp_stickers;
}


std::vector<QPixmap> Side::get_stickers(Edge source_edge)
{
     using index_type = std::vector<QPixmap>::size_type;
     std::vector<QPixmap> result;
     switch (source_edge)
     {
         case Edge::U:

         for(auto i: stickers[0])
             result.push_back(*i);
         break;

         case Edge::R:

         for(index_type i = 0; i < stickers.size(); i++)
             result.push_back(*(stickers[i].back()));
         break;

         case Edge::D:

         for(auto i: stickers.back())
             result.push_back(*i);
         break;

         case Edge::L:

         for(index_type i = 0; i < stickers.size(); i++)
             result.push_back(*(stickers[i].front()));
         break;
     }
     return result;
}

/*
 *      void Side::rotate(bool prim = false)
 *
 *      This function rotates the Side object. If the prim parameter is true the rotation is done 90 degrees clockwise
 *      otherwise it's done 90 degress counterclockwise.
 *
 */

void Side::rotate(bool prim)
{
    /* Transponsing a matrix */
        for(int i = 0; i < dimension; i++)
            for(int j = i+1; j < dimension; j++)
                std::swap(*stickers[i][j], *stickers[j][i]);

    if(!prim)
    {

        /* Reversing rows */
        for(int i = 0; i < dimension; i++)
                std::swap(*stickers[i][0], *stickers[i][dimension-1]);
    }
    else
    {
        /* Reversing columns */
        for(int i = 0; i < dimension; i++)
            std::swap(*stickers[0][i], *stickers[dimension-1][i]);
    }
}

void Side::restart()
{
    for(int i = 0; i < dimension; i++)
        for(int j = 0; j < dimension; j++)
            *stickers[i][j] = *base_color_sticker;
}

/*
 * ===================================================
 *      RegularCubeMap CLASS FUCTION DEFINITIONS
 * ===================================================
 */

RegularCubeMap::RegularCubeMap(ushort cube_dimension) :QLabel()
{
    QPainter *painter = new QPainter;


    QColor colors[6] = {
      Qt::white,
      Qt::green,
      Qt::red,
      Qt::blue,
      QColor("orange"),
      Qt::yellow
    };

    for(int i = 0; i < 6; i++)
    {
        sides[i] = new Side(colors[i], cube_dimension);
    }
    side_width = side_height = sides[0]->get_width();

    cube_map = new QPixmap(4 * side_width, 3 * side_height);
    cube_map->fill(Qt::white);


    painter->begin(cube_map);

    painter->setPen(Qt::black);

    painter->drawPixmap(QPointF(0, side_height), *sides[0]);
    painter->drawPixmap(QPointF(side_width, side_height), *sides[1]);
    painter->drawPixmap(QPointF(side_width, 0), *sides[2]);
    painter->drawPixmap(QPointF(3 * side_width, side_height), *sides[3]);
    painter->drawPixmap(QPointF(side_width, 2 * side_height), *sides[4]);
    painter->drawPixmap(QPointF(2 * side_width, side_height), *sides[5]);

    painter->end();
    delete painter;
    setPixmap(*cube_map);
}

void RegularCubeMap::make_move(const QString& move)
{
        if(move == "R")
        {
                paint_edge(white, sides[green]->get_stickers(Edge::U),  Edge::U);
                paint_edge(green, sides[yellow]->get_stickers(Edge::U),  Edge::U);
                paint_edge(yellow, sides[blue]->get_stickers(Edge::U),  Edge::U);
                paint_edge(blue, sides[white]->get_temp_stickers(),  Edge::U);
                rotate_side(red);
        }
        else if(move == "R\'")
        {
                paint_edge(green, sides[white]->get_stickers(Edge::U),  Edge::U);
                paint_edge(yellow, sides[green]->get_temp_stickers(),  Edge::U);
                paint_edge(blue, sides[yellow]->get_temp_stickers(),  Edge::U );
                paint_edge(white, sides[blue]->get_temp_stickers(),  Edge::U);
                rotate_side(red, true);
        }
        else if(move == "R2")
        {
                paint_edge(white, sides[yellow]->get_stickers(Edge::U),  Edge::U);
                paint_edge(green, sides[blue]->get_stickers(Edge::U),  Edge::U);
                paint_edge(yellow, sides[white]->get_temp_stickers(),  Edge::U);
                paint_edge(blue, sides[green]->get_temp_stickers(),  Edge::U);
                rotate_side(red);
                rotate_side(red);
        }
         else if(move == "U")
         {
                 paint_edge(red, sides[blue]->get_stickers(Edge::R),  Edge::L, true);
                 paint_edge(green, sides[red]->get_temp_stickers(),  Edge::L);
                 paint_edge(orange, sides[green]->get_temp_stickers(),  Edge::L);
                 paint_edge(blue, sides[orange]->get_temp_stickers(),  Edge::R, true);
                 rotate_side(white);
         }
         else if(move == "U\'")
         {
                 paint_edge(red, sides[green]->get_stickers(Edge::L),  Edge::L);
                 paint_edge(green, sides[orange]->get_stickers(Edge::L),  Edge::L);
                 paint_edge(orange, sides[blue]->get_stickers(Edge::R),  Edge::L, true);
                 paint_edge(blue, sides[red]->get_temp_stickers(),  Edge::R, true);
                 rotate_side(white, true);
         }
         else if(move == "U2")
         {
                 paint_edge(red, sides[orange]->get_stickers(Edge::L),  Edge::L);
                 paint_edge(green, sides[blue]->get_stickers(Edge::R),  Edge::L, true);
                 paint_edge(orange, sides[red]->get_temp_stickers(),  Edge::L);
                 paint_edge(blue, sides[green]->get_temp_stickers(),  Edge::R, true);
                 rotate_side(white);
                 rotate_side(white);
         }
         else if(move == "L")
         {
                 paint_edge(green, sides[white]->get_stickers(Edge::D),  Edge::D);
                 paint_edge(yellow, sides[green]->get_temp_stickers(),  Edge::D);
                 paint_edge(blue, sides[yellow]->get_temp_stickers(),  Edge::D );
                 paint_edge(white, sides[blue]->get_temp_stickers(),  Edge::D );
                 rotate_side(orange);
         }
         else if(move == "L\'")
         {
                 paint_edge(white, sides[green]->get_stickers(Edge::D),  Edge::D);
                 paint_edge(green, sides[yellow]->get_stickers(Edge::D),  Edge::D);
                 paint_edge(yellow, sides[blue]->get_stickers(Edge::D),  Edge::D );
                 paint_edge(blue, sides[white]->get_temp_stickers(),  Edge::D);
                 rotate_side(orange, true);
         }
         else if(move == "L2")
         {
                 paint_edge(white, sides[yellow]->get_stickers(Edge::D),  Edge::D);
                 paint_edge(green, sides[blue]->get_stickers(Edge::D),  Edge::D);
                 paint_edge(yellow, sides[white]->get_temp_stickers(),  Edge::D);
                 paint_edge(blue, sides[green]->get_temp_stickers(),  Edge::D);
                 rotate_side(orange);
                 rotate_side(orange);
         }
         else if(move == "D")
         {
                 paint_edge(green, sides[orange]->get_stickers(Edge::R),  Edge::R);
                 paint_edge(red, sides[green]->get_temp_stickers(),  Edge::R);
                 paint_edge(orange, sides[blue]->get_stickers(Edge::L),  Edge::R, true);
                 paint_edge(blue, sides[red]->get_temp_stickers(),  Edge::L, true);
                 rotate_side(yellow);
         }
         else if(move == "D\'")
         {
                 paint_edge(green, sides[red]->get_stickers(Edge::R),  Edge::R);
                 paint_edge(orange, sides[green]->get_temp_stickers(), Edge::R);
                 paint_edge(red, sides[blue]->get_stickers(Edge::L),  Edge::R, true);
                 paint_edge(blue, sides[orange]->get_temp_stickers(),  Edge::L, true);
                 rotate_side(yellow, true);
         }
         else if(move == "D2")
         {
                 paint_edge(green, sides[blue]->get_stickers(Edge::L),  Edge::R, true);
                 paint_edge(orange, sides[red]->get_stickers(Edge::R),  Edge::R);
                 paint_edge(red, sides[orange]->get_temp_stickers(),  Edge::R);
                 paint_edge(blue, sides[green]->get_temp_stickers(),  Edge::L, true);
                 rotate_side(yellow);
                 rotate_side(yellow);
         }
         else if(move == "F")
         {
                 paint_edge(white, sides[orange]->get_stickers(Edge::U),  Edge::R);
                 paint_edge(red, sides[white]->get_temp_stickers(),  Edge::D, true);
                 paint_edge(yellow, sides[red]->get_temp_stickers(),  Edge::L);
                 paint_edge(orange, sides[yellow]->get_temp_stickers(),  Edge::U, true);
                 rotate_side(green);
         }
         else if(move == "F\'")
         {
                 paint_edge(white, sides[red]->get_stickers(Edge::D),  Edge::R, true);
                 paint_edge(orange, sides[white]->get_temp_stickers(),  Edge::U);
                 paint_edge(yellow, sides[orange]->get_temp_stickers(),  Edge::L, true);
                 paint_edge(red, sides[yellow]->get_temp_stickers(),  Edge::D);
                 rotate_side(green,true);
         }
         else if(move == "F2")
         {
                 paint_edge(white, sides[yellow]->get_stickers(Edge::L),  Edge::R, true);
                 paint_edge(orange, sides[red]->get_stickers(Edge::D),  Edge::U, true);
                 paint_edge(yellow, sides[white]->get_temp_stickers(),  Edge::L, true);
                 paint_edge(red, sides[orange]->get_temp_stickers(),  Edge::D, true);
                 rotate_side(green);
                 rotate_side(green);
         }
         else if(move == "B")
         {
                 paint_edge(yellow, sides[orange]->get_stickers(Edge::D), Edge::R, true);
                 paint_edge(orange, sides[white]->get_stickers(Edge::L), Edge::D);
                 paint_edge(red, sides[yellow]->get_temp_stickers(), Edge::U);
                 paint_edge(white, sides[red]->get_temp_stickers(), Edge::L, true);
                 rotate_side(blue);
         }
         else if(move == "B\'")
         {
                 paint_edge(yellow, sides[red]->get_stickers(Edge::U), Edge::R);
                 paint_edge(orange, sides[yellow]->get_temp_stickers(), Edge::D, true);
                 paint_edge(red, sides[white]->get_stickers(Edge::L), Edge::U, true);
                 paint_edge(white, sides[orange]->get_temp_stickers(), Edge::L);
                 rotate_side(blue, true);
         }
         else if(move == "B2")
         {
                 paint_edge(yellow, sides[white]->get_stickers(Edge::L), Edge::R, true);
                 paint_edge(orange, sides[red]->get_stickers(Edge::U), Edge::D, true);
                 paint_edge(red, sides[orange]->get_temp_stickers(), Edge::U, true);
                 paint_edge(white, sides[yellow]->get_temp_stickers(), Edge::L, true);

                 rotate_side(blue);
                 rotate_side(blue);
         }
}

void RegularCubeMap::rotate_side(ushort side_index, bool prim)
{
        sides[side_index]->rotate(prim);
}

void RegularCubeMap::paint_edge(ushort side_index, std::vector<QPixmap> stickers, Edge target_edge, bool should_reverse)
{
    if(should_reverse)
        std::reverse(stickers.begin(), stickers.end());
    sides[side_index]->set_stickers(stickers, target_edge);
}
void RegularCubeMap::redraw()
{
   QPainter *painter = new QPainter();
   cube_map->fill(Qt::white);
   for(auto i: sides)
       i->redraw();

    painter->begin(cube_map);

    painter->setPen(Qt::black);

    painter->drawPixmap(QPointF(0, side_height), *sides[0]);
    painter->drawPixmap(QPointF(side_width, side_height), *sides[1]);
    painter->drawPixmap(QPointF(side_width, 0), *sides[2]);
    painter->drawPixmap(QPointF(3 * side_width, side_height), *sides[3]);
    painter->drawPixmap(QPointF(side_width, 2 * side_height), *sides[4]);
    painter->drawPixmap(QPointF(2 * side_width, side_height), *sides[5]);

    painter->end();
    setPixmap(*cube_map);
}
