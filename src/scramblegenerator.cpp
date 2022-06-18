#include "../include/scramblegenerator.hpp"

#include <QRandomGenerator>
#include <QPushButton>
#include <QIntValidator>

ScrambleGenerator::ScrambleGenerator(QWidget *parent) : QDockWidget("Scramble Generator", parent)
{

        main_widget = new QWidget(this);
        layout = new QVBoxLayout(main_widget);
        layout->setContentsMargins(1,1,1,1);
        main_widget->setLayout(layout);
        heading_font = new QFont("Roboto Mono", 20);
        heading_font->setBold(true);
        setWidget(main_widget);


        QFont regular("Roboto Mono", 15);
        regular.setBold(false);

        QHBoxLayout *cube_type_layout = new QHBoxLayout();
        cube_type_layout->setContentsMargins(1,0,1,0);
        layout->addLayout(cube_type_layout);

        choose_cube_type = new QComboBox(this);
        choose_cube_type->addItem("2x2");
        choose_cube_type->addItem("3x3");
        choose_cube_type->addItem("4x4");
        choose_cube_type->addItem("5x5");
        choose_cube_type->addItem("6x6");
        choose_cube_type->addItem("7x7");
        choose_cube_type->setCurrentIndex(1);

        cube_type = new QLabel("Cube: ");
        cube_type->setFont(regular);
        cube_type_layout->addWidget(cube_type);
        cube_type_layout->addWidget(choose_cube_type);

        scrambled_cube_map = new RegularCubeMap(3);
        scrambled_cube_map->setMinimumSize( scrambled_cube_map->minimumSizeHint());
        layout->addWidget(scrambled_cube_map);


        QIntValidator *validator = new QIntValidator(1,30);
        length_edit = new QLineEdit();
        length_edit->setValidator(validator);
        delete validator;
        length_edit->setText("22");
        length_edit->setPlaceholderText("Enter the length of the scramble");
        layout->addWidget(length_edit);

        generate_button = new ScrambleButton("Generate", this);
        generate_button->setFocusPolicy(Qt::ClickFocus);
        layout->addWidget(generate_button);

        scramble = new QLabel("");
        scramble->setFont(*heading_font);
        scramble->setWordWrap(true);
        layout->addWidget(scramble);

        QObject::connect(generate_button, &QPushButton::clicked, this, &ScrambleGenerator::generate);
        QObject::connect(length_edit, &QLineEdit::editingFinished, this, &ScrambleGenerator::generate);
        QObject::connect(choose_cube_type, &QComboBox::currentTextChanged, this, &ScrambleGenerator::cube_type_changed);

        current_cube = CubeType::three_x_three;


}

ScrambleGenerator::~ScrambleGenerator()
{
        delete heading_font;
}

void ScrambleGenerator::generate_scramble(ushort length)
{
        QString result;
        QChar previous;
        for(int i = 0; i < length; i++)
        {
                ushort pseudorandom;
                do
                {
                        pseudorandom = QRandomGenerator::global()->generate() % (sizeof(moves_for_3x3) / sizeof(QString));
                }
                while(moves_for_3x3[pseudorandom][0] == previous);
                result += moves_for_3x3[pseudorandom] + " ";
                previous = moves_for_3x3[pseudorandom][0];
        }
        scramble->setText(result);

}

/* =============================== SLOTS ==================================== */

void ScrambleGenerator::generate()
{
 generate_scramble(length_edit->text().toUInt());

 scrambled_cube_map->restart();

 QStringList scramble_moves = scramble->text().split(" ");

    for(auto& i: scramble_moves)
         scrambled_cube_map->make_move(i);
   scrambled_cube_map->redraw();
}

void ScrambleGenerator::cube_type_changed()
{
        ushort index = layout->indexOf(scrambled_cube_map);
        ushort next_dimension = choose_cube_type->currentText().at(2).digitValue();
        delete scrambled_cube_map;
        scrambled_cube_map = new RegularCubeMap(next_dimension);
        layout->insertWidget(index, scrambled_cube_map);
        generate();
}
