#ifndef SFML_PIANO_HPP
#define SFML_PIANO_HPP

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include "icons.hpp"

using namespace std;

namespace pianissimo
{
#define DP "data/"      ///this is the location of the piano wavs

int thisTone(sf::SoundBuffer& buff);  /// forward declaration of thread callee

enum keyColor {white, black };

struct Key
{   string filename;
    int keyNumber;
    keyColor keycolor;
    sf::RectangleShape keyShape;
    sf::SoundBuffer buffer;
    sf::Sound sound;
    string keyName; };

class Pianola
{
public:
    void loadKeys();
    void drawKeys();
    void textPanel();
    void processEvents();
    void render();
    void run();
    void SFGSetup();
    void Signals();
    void Widgets();
    void octave();

private:
    vector<Key> keyArray =
    {   {DP "C3-49-96.wav", 0, white },
        {DP "Db3-49-96.wav", 22, black },
        {DP "D3-49-96.wav", 18, white },
        {DP "Eb3-49-96.wav", 4, black },
        {DP "E3-49-96.wav", 3, white },
        {DP "F3-49-96.wav", 5, white },
        {DP "Gb3-49-96.wav", 19, black },
        {DP "G3-49-96.wav", 6, white },
        {DP "Ab3-97-127.wav", 24, black },
        {DP "A3-49-96.wav", 7, white },
        {DP "Bb3-49-96.wav", 20, black },
        {DP "B3-49-96.wav", 9, white },
        {DP "C4-49-96.wav", 10, white },
        {DP "Db4-49-96.wav", 14, black },
        {DP "D4-49-96.wav", 11, white },
        {DP "Eb4-49-96.wav", 15, black },
        {DP "E4-49-96.wav", 48, white } };
    bool keyreleased = true;
    int allKeys = 0;
    int keyrank = 0;
    int padding = 5;
    int whiteKeys = 0;
    sf::FloatRect SFwindowSize;
    sf::Font font;
    sf::RenderWindow SFwindow;
    sf::Text text;
    sf::Vector2f blackKeySize {40, 120 };
    sf::Vector2f pianoSize;
    sf::Vector2f whiteKeySize = {80, 230 };
    sf::View view;
    sfg::Adjustment::Ptr sliderLevel;
    sfg::Button::Ptr pitchButton;
    sfg::Button::Ptr button2;
    sfg::Canvas::Ptr canvas1;
    sfg::Desktop desktop;
    sfg::Frame::Ptr menuFrame;
    sfg::Frame::Ptr pianoFrame;
    sfg::Label::Ptr titleLabel;
    sfg::Label::Ptr noteLabel;
    sfg::Label::Ptr m_value;
    sfg::Label::Ptr readOut;
    sfg::Label::Ptr sliderTitle;
    sfg::Scale::Ptr slider;
    sfg::SFGUI m_sfgui;
    sfg::Canvas::Ptr textCanvas;
    sfg::Window::Ptr textWindow; };

} ///end namespace

#endif ///#ifndef SFML_PIANO_HPP
