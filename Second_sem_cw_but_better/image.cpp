#include "image.h"
#include <QDebug>

Color::Color()
    : r(0), g(0), b(0) {}

Color::Color(float r, float g, float b)
    : r(r), g(g), b(b) {}

Color::~Color() {}

Image::Image()
    : m_width(0), m_height(0), m_colors(std::vector<Color>(0)) {}

Image::Image(int width, int height)
    : m_width(width), m_height(height), m_colors(std::vector<Color>(width * height)) {}

Image::~Image()
{
    delete pixmap;
}

// PIXEL FUNCTIONS

Color Image::GetColor(int x, int y) const
{
    return m_colors[y*m_width + x];
}

void Image::SetColor(const Color &color, int x, int y)
{
    m_colors[y*m_width+x] = color;
}

// EMPTINESS CHECK FUNCTION
int Image::isEmpty()
{
    return m_height == 0 || m_width == 0;
}

// I/O MANAGING FUNCTIONS

void Image::Read(QString filepath)
{
    QFile file(filepath);
    if(file.open(QIODevice::ReadOnly))
    {
        // READING INTO READ BUFFER //
        unsigned char fileHeaderRead[14];
        unsigned char infoHeaderRead[15];

        file.read(reinterpret_cast<char*>(fileHeaderRead), fileHeaderSize);
        if (fileHeaderRead[0] != 'B' || fileHeaderRead[1] != 'M')
        {
            lastRead = false;
        }
        else
        {

            file.read(reinterpret_cast<char*>(infoHeaderRead), 15);
            if (infoHeaderRead[14] != 24)
            {
                lastRead = false;
            }
            else
            {
                // COPYING IN OBJECT & READING FURTHER //
                for (int i = 0; i < 14; i++)
                {
                    fileHeader[i] = fileHeaderRead[i];
                    infoHeader[i] = infoHeaderRead[i];
                }
                infoHeader[14] = infoHeaderRead[14];

                infoHeaderSize = infoHeader[0] + (infoHeader[1] << 8) + (infoHeader[2] << 16) + (infoHeader[3] << 24);

                unsigned char * ptr = &infoHeader[15];

                file.read(reinterpret_cast<char*>(ptr), infoHeaderSize - 15);

                m_width = infoHeader[4] + (infoHeader[5] << 8) + (infoHeader[6] << 16) + (infoHeader[7] << 24);
                m_height = infoHeader[8] + (infoHeader[9] << 8) + (infoHeader[10] << 16) + (infoHeader[11] << 24);

                m_colors.resize(m_width * m_height);

                paddingAmount = (4 - (m_width * 3) % 4) % 4;

                for (int y = 0; y < m_height; y++)
                    {
                        for (int x = 0; x < m_width; ++x)
                        {
                            unsigned char color[3];
                            file.read(reinterpret_cast<char*>(color), 3);
                            m_colors[y * m_width + x].r = static_cast<float>(color[2]) / 255.0f;
                            m_colors[y * m_width + x].g = static_cast<float>(color[1]) / 255.0f;
                            m_colors[y * m_width + x].b = static_cast<float>(color[0]) / 255.0f;
                        }
                        file.skip(paddingAmount);
                    }
                if (pixmap != nullptr)
                    delete pixmap;
                pixmap = new QPixmap(filepath);

                fpath = filepath;
                lastRead = true;
            }
        }
    }
    else
        lastRead = false;

    file.close();


}

void Image::Export(QString filepath)
{
    QFile file(filepath);

    if(file.open(QIODevice::WriteOnly))
    {
        unsigned char bmpPad[3] = {0, 0, 0};

        file.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
        file.write(reinterpret_cast<char*>(infoHeader), infoHeaderSize);

        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
                unsigned char r = static_cast<unsigned char>(GetColor(x, y).r * 255.0f);
                unsigned char g = static_cast<unsigned char>(GetColor(x, y).g * 255.0f);
                unsigned char b = static_cast<unsigned char>(GetColor(x, y).b * 255.0f);

                unsigned char color[] = {b, g, r};

                file.write(reinterpret_cast<char*>(color), 3);
            }
            file.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
        }
    }

    file.close();
}

void Image::Export()
{
    QFile file(fpath);

    if(file.open(QIODevice::WriteOnly))
    {
        unsigned char bmpPad[3] = {0, 0, 0};

        file.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
        file.write(reinterpret_cast<char*>(infoHeader), infoHeaderSize);

        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
                unsigned char r = static_cast<unsigned char>(GetColor(x, y).r * 255.0f);
                unsigned char g = static_cast<unsigned char>(GetColor(x, y).g * 255.0f);
                unsigned char b = static_cast<unsigned char>(GetColor(x, y).b * 255.0f);

                unsigned char color[] = {b, g, r};

                file.write(reinterpret_cast<char*>(color), 3);
            }
            file.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
        }
    }

    file.close();
}

// EDIT FUNCTIONS

void Image::setRGB(QString colour, float value)
{
    for (int x = 0; x < m_width; x++)
        for (int y = 0; y < m_height; y++)
        {
            if (colour == "Red")
                m_colors[y*m_width + x].r = value / 255.0f;
            else if (colour == "Green")
                m_colors[y*m_width + x].g = value / 255.0f;
            else if (colour == "Blue")
                m_colors[y*m_width + x].b = value / 255.0f;
        }
    updatePixmap();
}

struct cmpColor {
    bool operator()(const Color& a, const Color& b) const {
        return a.r < b.r;
    }
};

void Image::majorColorChange(Color change)
{
    std::map <Color, int, cmpColor> map;
    for (int x = 0; x < m_width; x++)
        for (int y = 0; y < m_height; y++)
        {
            auto it = map.find(m_colors[y*m_width + x]);
            if (it != map.end())
            {
                //found in there
                it->second++;
            }
            else
            {
                map.insert({m_colors[y*m_width + x], 1});
            }
        }

    int max = 0;
    Color common;

    for (auto it = map.cbegin(); it != map.cend(); it++)
    {
        if (it->second > max)
        {
            max = it->second;
            common = it->first;
        }
    }
    for (int x = 0; x < m_width; x++)
        for (int y = 0; y < m_height; y++)
        {
            if (common.r == m_colors[y*m_width + x].r && common.g == m_colors[y*m_width + x].g && common.b == m_colors[y*m_width + x].b)
                m_colors[y*m_width + x] = change;
        }
    updatePixmap();
}

void Image::rectangleDraw(QPoint start, QPoint end, Color color, int pts)
{
    for (int p = 0; p < pts; p++)
    {
        for (int x = start.x(); x <= end.x(); x++)
        {
            SetColor(color, x, start.y()-p);
            SetColor(color, x, end.y()+p);
        }
        for (int y = start.y(); y >= end.y(); y--)
        {
            SetColor(color, start.x()+p, y);
            SetColor(color, end.x()-p, y);
        }
    }
    updatePixmap();
}

void Image::rectangleFill(QPoint start, QPoint end, Color color)
{
    for (int x = start.x(); x <= end.x(); x++)
        for (int y = start.y(); y >= end.y(); y--)
            SetColor(color, x, y);

    updatePixmap();
}

void Image::rectangleShuffle(QPoint start, QPoint end, int flag)
{
    int y_half = (start.y() - end.y()) / 2;
    int x_half = (end.x() - start.x()) / 2;
    Color color;
    if (flag == 0)
    {
        for (int x = start.x(); x < end.x(); x++)
            for (int y = end.y() + y_half; y < start.y(); y++)
            {
                if (x - start.x() < x_half)
                {
                    color = m_colors[y * m_width + x];
                    m_colors[y * m_width + x] = m_colors[(y - y_half) * m_width + (x + x_half)];
                    m_colors[(y - y_half) * m_width + (x + x_half)] = color;
                }
                else if (x - start.x() >= x_half)
                {
                    color = m_colors[y * m_width + x];
                    m_colors[y * m_width + x] = m_colors[(y - y_half) * m_width + (x - x_half)];
                    m_colors[(y - y_half) * m_width + (x - x_half)] = color;
                }
            }
    }
    else if (flag == 1)
    {
        for (int x = start.x(); x < end.x(); x++)
            for (int y = end.y() + y_half; y < start.y(); y++)
            {
                color = m_colors[y * m_width + x];
                m_colors[y * m_width + x] = m_colors[(y - y_half) * m_width + x];
                m_colors[(y - y_half) * m_width + x] = color;
            }
        for (int x = start.x() + x_half; x < end.x(); x++)
            for (int y = end.y() + y_half; y < start.y(); y++)
            {
                color = m_colors[y * m_width + x];
                m_colors[y * m_width + x] = m_colors[(y - y_half) * m_width + (x - x_half)];
                m_colors[(y - y_half) * m_width + (x - x_half)] = color;
            }

    }
    updatePixmap();
}

void Image::fill(QPoint start, QPoint end)
{
    std::vector<Color> copy;
    int c_width = end.x()-start.x()+1;
    int c_height = start.y() - end.y()+1;
    copy.resize(c_width*c_height);
    for (int x = start.x(); x <= end.x(); x++)
        for (int y = end.y(); y <= start.y(); y++)
        {
            copy[(y-end.y()) * c_width + x - start.x()] = m_colors[y * m_width + x];
        }
    for (int x = 0; x < m_width; x++)
        for (int y = 0; y < m_height; y++)
        {
            m_colors[y * m_width + x] = copy[(y % c_height) * c_width + (x % c_width)];
        }
    updatePixmap();
}

void Image::rectangleFillOutside(QPoint start, QPoint end, Color color)
{
    for (int x = 0; x < m_width; x++)
        for (int y = 0; y < m_height; y++)
        {
                if (!(x >= start.x() && x <= end.x() && y <= start.y() && y >= end.y()))
                    SetColor(color, x, y);
        }
    updatePixmap();
}

void Image::split(int h, int v, int pts, Color color)
{
    int width = m_width / v;
    int height = m_height / h;

    for (int p = 0; p < pts; p++)
    {
        for (int y = height-1; (y + p) < m_height; y += height)
            {
                for (int x = 0; x < m_width; x++)
                    SetColor(color, x, y + p);
            }
        for (int x = width-1; (x + p) < m_width; x += width)
            {
                for (int y = 0; y < m_height; y++)
                    SetColor(color, x + p, y);
            }
    }
    updatePixmap();
}

void Image::split(int h, int v, QString filepath)
{
    int width = m_width / v;
    int height = m_height / h;
    int num = 0;
    for (int j = 1; j <= h; j++)
    {
        for (int i = 1; i <= v; i++)
        {
            num++;
            Image piece;

            piece.m_height = height;
            piece.m_width = width;
            piece.m_colors.resize(width * height);

            for (int k = 0; k < fileHeaderSize; k++)
                piece.fileHeader[k] = fileHeader[k];

            for (int k = 0; k < infoHeaderSize; k++)
                piece.infoHeader[k] = infoHeader[k];
            piece.infoHeaderSize = infoHeaderSize;

            piece.paddingAmount = (4 - (width * 3) % 4) % 4;

            int fileSize = piece.fileHeaderSize + piece.infoHeaderSize + width * height * 3 + piece.paddingAmount * height;

            // Piece file size
            piece.fileHeader[2] = fileSize;
            piece.fileHeader[3] = fileSize >> 8;
            piece.fileHeader[4] = fileSize >> 16;
            piece.fileHeader[5] = fileSize >> 24;
            piece.fileHeader[10] = fileHeaderSize + infoHeaderSize;

            piece.infoHeader[0] = piece.infoHeaderSize;
            // Piece width
            piece.infoHeader[4] = width;
            piece.infoHeader[5] = width >> 8;
            piece.infoHeader[6] = width >> 16;
            piece.infoHeader[7] = width >> 24;
            // Piece height
            piece.infoHeader[8] = height;
            piece.infoHeader[9] = height >> 8;
            piece.infoHeader[10] = height >> 16;
            piece.infoHeader[11] = height >> 24;
            for (int x = ((i-1) * width); x < (i * width) && x < m_width; x++)
                for (int y = ((j-1) * height); y < (j * height) && y < m_height; y++)
                {
                    piece.m_colors[(y - ((j-1) * height)) * width + (x - ((i-1) * width))] = m_colors[y * m_width + x];
                }
            piece.fpath = filepath;
            QString append = QString("/%1.bmp").arg(QString::number(num, 10));
            piece.fpath.append(append);
            piece.Export();
        }
    }
    updatePixmap();
}

void Image::merge(Image * merge, int flag)
{
    if (flag)
    {
        int height = m_height + merge->m_height; // 2*image_half + 2*merge_half
        int x_start = (m_width - merge->m_width) / 2;
        qDebug() << height;
        std::vector<Color> merge_colors;
        merge_colors.resize(height * m_width, Color( 1, 1, 1));
        //int half = height / 2; // image_half + merge_half
        int merge_half = merge->m_height / 2;
        int image_half = m_height / 2;
        for (int y = 0; y < image_half - merge_half; y++)
            for (int x = 0; x < m_width; x++)
            {
                merge_colors[y * m_width + x] = GetColor(x, y);
            }
        for (int y = height - image_half + merge_half; y < height; y++)
            for (int x = 0; x < m_width; x++)
            {
                merge_colors[y * m_width + x] = GetColor(x, image_half + merge_half + (y % (image_half + 3*merge_half)));
            }
        int i = 0;
        int m = 0;
        for (int y = image_half - merge_half; y < image_half + 3*merge_half - 2; y++)
        {

                if (y % 2 == 0)
                {
                    for (int x = 0; x < m_width; x++)
                        merge_colors[y * m_width + x] = GetColor(x, image_half - merge_half + i);
                    i++;
                }
                else
                {
                    for (int x = 0; x < merge->m_width; x++)
                        merge_colors[y * m_width + x_start +x] = merge->GetColor(x, m);
                    m++;
                }

        }
        qDebug() << height - image_half + merge_half;

        m_height = height;
        infoHeader[8] = m_height;
        infoHeader[9] = m_height >> 8;
        infoHeader[10] = m_height >> 16;
        infoHeader[11] = m_height >> 24;
        m_colors.resize(height * m_width);
        m_colors.swap(merge_colors);
    }
    else
    {
        int width = m_width + merge->m_width;
        std::vector<Color> merge_colors;
        merge_colors.resize(m_height * width, Color( 1, 1, 1));
        for (int y = 0; y < m_height; y++)
            for (int x = 0; x < m_width; x++)
            {
                merge_colors[y * width + x] = GetColor(x, y);
            }
        for (int y = 0; y < merge->m_height; y++)
            for (int x = m_width; x < width; x++)
            {
                merge_colors[y * width + x] = merge->GetColor(x - m_width, y);
            }
        paddingAmount = (4 - (width * 3) % 4) % 4;

        m_width = width; //m_width = infoHeader[4] + (infoHeader[5] << 8) + (infoHeader[6] << 16) + (infoHeader[7] << 24);
        infoHeader[4] = m_width;
        infoHeader[5] = m_width >> 8;
        infoHeader[6] = m_width >> 16;
        infoHeader[7] = m_width >> 24;
        m_colors.resize(m_height * m_width);
        m_colors.swap(merge_colors);
    }

    updatePixmap();
}



// REFRESH FUNCTION

void Image::updatePixmap()
{
    QString tmp = "tmp.bmp";
    if (QFile::exists(tmp))
    {
        QString backup = "backup.bmp";
        QFile::remove(backup);
        QFile::copy(tmp, backup);
    }
    Export(tmp);
    delete pixmap;
    pixmap = new QPixmap(tmp);
}

