/** Brownian Motion 2015
 * \date :180915
 put on gitHub 191015
 * \file :pianola.cpp

 * \brief :pianola - a little piano - manipulating sounds with delays and threads
 based on AlexanderX.net/Tutorials/Create a simple piano/main.cpp
 all the sound files to play this are in the data/ directory


 openal32.dll is needed in the code folder
*/

#include "pianola.hpp"


namespace pianissimo
{
int msec = 15; ///used in the thread callee - changed in render()
float pitch = 1;
string noteDisplay = "ABC";

void Pianola::octave()
{   if (pitch == 1) { pitch = 2; pitchButton->SetLabel("X2");}
    else {pitch = 1; pitchButton->SetLabel("X1");} }

string noteName (string& str)
{   string dpStr = DP;
    size_t dpLen = dpStr.length();
    size_t pos = str.find("-");
    string mid = str.substr(dpLen, pos - dpLen);
    return mid; }

int thisTone(sf::SoundBuffer& buff)     /// called by thread in pE
{   sf::Sound note;                     /// NOT in pianlola class
    note.setBuffer(buff);
    note.setPitch(pitch);
    note.play();
    for (int i = 100 ; i > 0; --i )
    {   sf::sleep(sf::milliseconds(msec));
        note.setVolume(i); }
    note.setVolume(100);
    note.stop();
    return 0; }

void Pianola::run()
{   SFGSetup();
    textPanel();
    loadKeys();
    Widgets();
    render(); }

void Pianola::SFGSetup()
{   padding = 5;
    desktop.SetProperty("*", "FontName","HORATIOM.TTF");
    desktop.LoadThemeFromFile("Cezanne.theme");
    desktop.SetProperty( "*", "FontSize", 25 ); }

void Pianola::textPanel()
{   font.loadFromFile("HORATIOM.TTF");
    text.setFont(font);
    text.setCharacterSize(72);
    text.setString(noteDisplay); }

void Pianola::loadKeys()
{   allKeys = keyArray.size();
    for (int j = 0 ; j < allKeys; ++j)
    {   keyArray[j].keyName = noteName( keyArray[j].filename);
        if (keyArray[j].keycolor == white) ++whiteKeys; }   /// don't need to count black keys - they get sorted out below
    for (int i = 0 ; i <keyArray.size(); ++i )
    {   keyArray[i].buffer.loadFromFile(keyArray[i].filename);
        keyArray[i].sound.setBuffer(keyArray[i].buffer);
        keyArray[i].keyShape.setOutlineThickness(1);
        keyArray[i].keyShape.setOutlineColor(sf::Color::Black);
        if (keyArray[i].keycolor == white)
        {   keyArray[i].keyShape.setSize(whiteKeySize);
            keyArray[i].keyShape.setFillColor(sf::Color::White); }
        if (keyArray[i].keycolor == black)
        {   keyArray[i].keyShape.setSize(blackKeySize);
            keyArray[i].keyShape.setFillColor(sf::Color::Black);
            keyArray[i].keyShape.setOrigin(blackKeySize.x/2, 0); } }
    pianoSize = {whiteKeySize.x * whiteKeys, whiteKeySize.y }; }

void Pianola::Widgets()
{   slider = sfg::Scale::Create(5,100, 5, sfg::Scale::Orientation::HORIZONTAL);
    slider->SetRequisition({ 150, 20 });
    slider->SetValue(20);
    sliderTitle = sfg::Label::Create();

    auto divV = sfg::Separator::Create( sfg::Separator::Orientation::VERTICAL );
    auto divH = sfg::Separator::Create( sfg::Separator::Orientation::HORIZONTAL );
    pianoFrame = sfg::Frame::Create();
    pianoFrame->SetRequisition(pianoSize);
    menuFrame = sfg::Frame::Create();

    textCanvas = sfg::Canvas::Create();

    textWindow = sfg::Window::Create();
    textWindow->SetStyle(0);
    textWindow->Add(textCanvas);
    menuFrame->Add(textWindow);

    pitchButton = sfg::Button::Create("X1");
    pitchButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &Pianola::octave, this));

    auto controlBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, padding);
    controlBox->Pack(menuFrame);
    controlBox->Pack(sliderTitle, false);
    controlBox->Pack(slider, false);
    controlBox->Pack(pitchButton, false);

    auto leftBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, padding);
    leftBox->Pack(pianoFrame, false);
    auto mainBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, padding);
    mainBox->Pack(leftBox);
    mainBox->Pack(divV);
    mainBox->Pack(controlBox);

    auto sfgWindow = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
    sfgWindow->Add(mainBox);
    desktop.Add(sfgWindow);
    SFwindowSize = sfgWindow->GetAllocation();

    view.reset({0, 0, pianoSize.x, pianoSize.y } );
    view.setViewport(
    {   pianoFrame->GetAbsolutePosition().x / SFwindowSize.width,
        pianoFrame->GetAbsolutePosition().y / SFwindowSize.height,
        pianoFrame->GetAllocation().width   / SFwindowSize.width,
        pianoFrame->GetAllocation().height  / SFwindowSize.height }); }

void Pianola::drawKeys()
{   keyrank = 0;
    for (int k = 0; k < allKeys; ++k)
    {   if (keyArray[k].keycolor == black)
        {   keyArray[k].keyShape.setPosition(keyrank * whiteKeySize.x, 0);
            SFwindow.draw(keyArray[k].keyShape);  }
        if (keyArray[k].keycolor == white)
        {   keyArray[k].keyShape.setPosition(whiteKeySize.x * keyrank, 0);
            SFwindow.draw(keyArray[k].keyShape);
            ++keyrank; } }
    for (int k = 0; k < allKeys; ++k)       /// redraw the blacks
    {   if (keyArray[k].keycolor == black)
            SFwindow.draw(keyArray[k].keyShape);  } }

void Pianola::render()
{   sf::Clock clock;
    SFwindow.create(sf::VideoMode(SFwindowSize.width,SFwindowSize.height), "Pianola 17");
        SFwindow.setIcon( Pink_Piano.width,  Pink_Piano.height,  Pink_Piano.pixel_data );
    while (SFwindow.isOpen())
    {   msec = slider->GetValue();
        sliderTitle->SetText (to_string(msec) + "00 ms");
        desktop.Update(clock.restart().asSeconds());
        SFwindow.resetGLStates();
        SFwindow.clear();
        m_sfgui.Display(SFwindow);
        SFwindow.pushGLStates();
        {   processEvents();
            drawKeys();
            SFwindow.setView(view); }
        SFwindow.popGLStates();
        textCanvas->Bind();
        textCanvas->Clear( sf::Color( 0, 0, 0, 0 ) );
        textCanvas->Draw(text);
        textCanvas->Display();
        textCanvas->Unbind();
        SFwindow.display();  } }

void Pianola::processEvents()
{   sf::Event event;
    while(SFwindow.pollEvent(event))
    {   desktop.HandleEvent(event); ///desktop defined in 'widgets' - needed to activate SFGUI events
        switch (event.type)
        {   case sf::Event::Closed: SFwindow.close(); break;
            case sf::Event::KeyPressed:
            {   for (int i = 0 ; i < allKeys ; ++i )
                {   if (event.key.code == sf::Keyboard::Key(keyArray[i].keyNumber) && keyreleased)
                    {   noteDisplay = keyArray[i].keyName;
                        text.setString(noteDisplay);
                        keyArray[i].keyShape.setFillColor(sf::Color::Red);
                        thread th1(&thisTone, ref(keyArray[i].buffer));        ///this is the thread caller
                        th1.detach(); } } ; break; }
            case sf::Event::KeyReleased:
            {   for (int j = 0 ; j < allKeys; ++j )
                {   if (keyArray[j].keycolor == white) keyArray[j].keyShape.setFillColor(sf::Color::White);
                    if (keyArray[j].keycolor == black) keyArray[j].keyShape.setFillColor(sf::Color::Black); }

            }
            default: break; }             /// end of switch
    } }   /// end pE function and while poll event - return to caller (render function)

} ///end of namespace

