#ifndef IMAGE_H
#define IMAGE_H
#include <QString>
#include <QPixmap>
#include <vector>
#include <QString>
#include <QFile>

///////////////////////////////////////////////////////////////
/// The function definition lines in cpp file are indicated ///
/// in the comment next to the function declaration.        ///
///////////////////////////////////////////////////////////////

struct Color
{
    float r, g, b;
    Color();
    Color(float r, float g, float b);
    ~Color();
};

class Image
{
public:
    Image();
    Image(int width, int height);
    ~Image();

    QPixmap * pixmap = nullptr;

    QString fpath;
    int m_width;
    int m_height;

    bool lastRead; // States whether the last attempt of opening a new file was successful or not

    // PIXEL FUNCTIONS
    Color GetColor(int x, int y) const; // line 24
    void SetColor(const Color& color, int x, int y); // line 29

    // EMPTINESS CHECK FUNCTION
    int isEmpty(); // line 35

    // I/O MANAGING FUNCTIONS

    void Read(QString filepath); // line 42
    void Export(QString filepath); // line 116
    void Export(); // line 146

    // EDIT FUNCTIONS

    void setRGB(QString colour, float value); // line 178
    void majorColorChange(Color change); // line 193
    void rectangleDraw(QPoint start, QPoint end, Color color, int pts); // line 237
    void rectangleFill(QPoint start, QPoint end, Color color); // line 255
    void rectangleShuffle(QPoint start, QPoint end, int flag); // line 264
    void fill(QPoint start, QPoint end); // line 309
    void rectangleFillOutside(QPoint start, QPoint end, Color color); // line 328
    void split(int n, int m, int pts, Color color); // line 339
    void split(int n, int m, QString filpath); // line 360
    void merge(Image * merge, int flag);

    // REFRESH FUNCTION
    void updatePixmap(); // line 421


private:

    // PIXEL MAP
    std::vector<Color> m_colors;
    int paddingAmount;

    const int fileHeaderSize = 14;
    int infoHeaderSize;

    unsigned char fileHeader[14];
    unsigned char infoHeader[124];
};
#endif // IMAGE_H
