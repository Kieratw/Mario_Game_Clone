#include "Menu.h"

Menu::Menu(unsigned short width, unsigned short height)
    : isLoginScreen(false), isRegisterScreen(false), isEnteringLogin(false),
    isEnteringPassword(false), isEnteringUsername(false), rankingScreen(false), loggedInUser("")
{
    menuItems.resize(MAX_NUMBER_OF_ITEMS);
    font.loadFromFile("Resources/Alexandriaflf.ttf");

    menuItems[0].setFont(font);
    menuItems[0].setFillColor(sf::Color::Red);
    menuItems[0].setString("Rozpocznij");
    menuItems[0].setCharacterSize(70);
    sf::FloatRect textRect = menuItems[0].getLocalBounds();
    menuItems[0].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    menuItems[0].setPosition(width / 2.0f, height / (MAX_NUMBER_OF_ITEMS + 1) * 1);

    menuItems[1].setFont(font);
    menuItems[1].setFillColor(sf::Color::White);
    menuItems[1].setString("Logowanie");
    menuItems[1].setCharacterSize(70);
    textRect = menuItems[1].getLocalBounds();
    menuItems[1].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    menuItems[1].setPosition(width / 2.0f, height / (MAX_NUMBER_OF_ITEMS + 1) * 2);

    menuItems[2].setFont(font);
    menuItems[2].setFillColor(sf::Color::White);
    menuItems[2].setString("Rejestracja");
    menuItems[2].setCharacterSize(70);
    textRect = menuItems[2].getLocalBounds();
    menuItems[2].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    menuItems[2].setPosition(width / 2.0f, height / (MAX_NUMBER_OF_ITEMS + 1) * 3);

    menuItems[3].setFont(font);
    menuItems[3].setFillColor(sf::Color::White);
    menuItems[3].setString("Ranking");
    menuItems[3].setCharacterSize(70);
    textRect = menuItems[3].getLocalBounds();
    menuItems[3].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    menuItems[3].setPosition(width / 2.0f, height / (MAX_NUMBER_OF_ITEMS + 1) * 4);

    menuItems[4].setFont(font);
    menuItems[4].setFillColor(sf::Color::White);
    menuItems[4].setString("Wyjscie");
    menuItems[4].setCharacterSize(70);
    textRect = menuItems[4].getLocalBounds();
    menuItems[4].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    menuItems[4].setPosition(width / 2.0f, height / (MAX_NUMBER_OF_ITEMS + 1) * 5);

    selectedItemIndex = 0;

    loginText.setFont(font);
    loginText.setString("Wpisz login:");
    loginText.setCharacterSize(50);
    loginText.setFillColor(sf::Color::White);
    loginText.setPosition(width / 2.0f - 300, height / 4.0f);

    passwordText.setFont(font);
    passwordText.setString("Wpisz haslo:");
    passwordText.setCharacterSize(50);
    passwordText.setFillColor(sf::Color::White);
    passwordText.setPosition(width / 2.0f - 300, height / 2.0f);

    loginInput.setFont(font);
    loginInput.setCharacterSize(50);
    loginInput.setFillColor(sf::Color::White);
    loginInput.setPosition(width / 2.0f, height / 4.0f);

    passwordInput.setFont(font);
    passwordInput.setCharacterSize(50);
    passwordInput.setFillColor(sf::Color::White);
    passwordInput.setPosition(width / 2.0f, height / 2.0f);

    messageText.setFont(font);
    messageText.setCharacterSize(40);
    messageText.setFillColor(sf::Color::Red);
    messageText.setPosition(50, height * 3 / 4.0f);

    loggedInUserText.setFont(font);
    loggedInUserText.setCharacterSize(20);
    loggedInUserText.setFillColor(sf::Color::Green);
    loggedInUserText.setPosition(550, 450);

    loadPlayersFromFile();
}


void Menu::draw(sf::RenderWindow& window)
{
    if (isLoginScreen || isRegisterScreen)
    {
        if (isLoginScreen)
        {
            drawLoginScreen(window);
        }
        else if (isRegisterScreen)
        {
            drawRegisterScreen(window);
        }
        window.draw(messageText);
    }
    else if (rankingScreen)
    {
        drawRanking(window);
    }
    else
    {
        for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
        {
            window.draw(menuItems[i]);
        }
        if (!loggedInUser.empty())
        {
            window.draw(loggedInUserText);
        }

    }
}

void Menu::drawLoginScreen(sf::RenderWindow& window)
{
    window.draw(loginText);
    window.draw(loginInput);
    if (isEnteringPassword)
    {
        window.draw(passwordText);
        window.draw(passwordInput);
    }
}

void Menu::drawRegisterScreen(sf::RenderWindow& window)
{
    window.draw(loginText);
    window.draw(loginInput);
    if (isEnteringPassword)
    {
        window.draw(passwordText);
        window.draw(passwordInput);
    }
}

bool Menu::validatePassword(const std::string& password)
{
    std::regex passwordRegex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d).+$");
    return std::regex_match(password, passwordRegex);
}

bool Menu::userExists(const std::string& username)
{
    for (const auto& player : ranking)
    {
        if (player.username == username)
        {
            return true;
        }
    }
    return false;
}

bool Menu::validateLogin(const std::string& username, const std::string& password)
{
    for (const auto& player : ranking)
    {
        if (player.username == username && player.password == password)
        {
            return true;
        }
    }
    return false;
}

void Menu::login()
{
    if (userExists(inputLogin) && validateLogin(inputLogin, inputPassword))
    {
        loggedInUser = inputLogin;
        loggedInUserText.setString(loggedInUser);
        messageText.setFillColor(sf::Color::Green);
        isLogged = true;
        messageText.setString("Logowanie udane.");
    }
    else
    {
        messageText.setFillColor(sf::Color::Red);
        messageText.setString("Niepoprawny login lub haslo.");
    }
    inputLogin.clear();
    inputPassword.clear();
}

void Menu::registerUser()
{
    if (userExists(inputLogin))
    {
        messageText.setFillColor(sf::Color::Red);
        messageText.setString("Uzytkownik o takiej nazwie juz istnieje.");
    }
    else if (validatePassword(inputPassword))
    {
        Player newPlayer(inputLogin, inputPassword, "100000000000");
        ranking.emplace_back(newPlayer);

        loggedInUser = inputLogin;
        isLogged = true;
        loggedInUserText.setString(loggedInUser);
        messageText.setFillColor(sf::Color::Green);
        messageText.setString("Rejestracja udana. Zalogowano.");
    }
    else
    {
        messageText.setFillColor(sf::Color::Red);
        messageText.setString("Haslo musi zawierac .A.a.2.");
    }
    inputLogin.clear();
    inputPassword.clear();
}

void Menu::handleEvent(sf::Event& event, bool& isMenuOpen, bool& isRunning, std::vector<Player>& players)
{
    if (isLoginScreen || isRegisterScreen)
    {
        if (event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode < 128)
            {
                char enteredChar = static_cast<char>(event.text.unicode);
                if (isEnteringUsername)
                {
                    if (enteredChar == '\b' && !inputLogin.empty())
                    {
                        inputLogin.pop_back();
                    }
                    else if (enteredChar != '\b' && enteredChar != '\r')
                    {
                        inputLogin += enteredChar;
                    }
                    loginInput.setString(inputLogin);
                }
                else if (isEnteringPassword)
                {
                    if (enteredChar == '\b' && !inputPassword.empty())
                    {
                        inputPassword.pop_back();
                    }
                    else if (enteredChar != '\b' && enteredChar != '\r')
                    {
                        inputPassword += enteredChar;
                    }
                    passwordInput.setString(inputPassword);
                }
            }
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Enter)
            {
                if (isEnteringUsername)
                {
                    isEnteringUsername = false;
                    isEnteringPassword = true;
                }
                else if (isEnteringPassword)
                {
                    if (isLoginScreen)
                    {
                        login();
                    }
                    else if (isRegisterScreen)
                    {
                        registerUser();
                    }
                    isEnteringPassword = false;
                }
            }
            else if (event.key.code == sf::Keyboard::Escape)
            {
                isLoginScreen = false;
                isRegisterScreen = false;
                isEnteringUsername = false;
                isEnteringPassword = false;
                messageText.setString("");
                loginInput.setString("");
                passwordInput.setString("");

            }

        }
    }
    else if (rankingScreen)
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            rankingScreen = false;
        }
    }
    else
    {
        if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::Up)
            {
                if (selectedItemIndex - 1 >= 0)
                {
                    menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                    selectedItemIndex--;
                    menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                }
            }
            else if (event.key.code == sf::Keyboard::Down)
            {
                if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
                {
                    menuItems[selectedItemIndex].setFillColor(sf::Color::White);
                    selectedItemIndex++;
                    menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
                }
            }
            else if (event.key.code == sf::Keyboard::Return)
            {
                if (selectedItemIndex == 0) // Rozpocznij
                {
                    if (isLogged)
                    {
                        isMenuOpen = false;

                    }
                    else
                    {
                        messageText.setFillColor(sf::Color::Red);
                        messageText.setString("Musisz sie zalogowac.");
                    }
                }
                else if (selectedItemIndex == 1) // Logowanie
                {
                    isLoginScreen = true;
                    isEnteringUsername = true;
                    isEnteringPassword = false;
                }
                else if (selectedItemIndex == 2) // Rejestracja
                {
                    isRegisterScreen = true;
                    isEnteringUsername = true;
                    isEnteringPassword = false;
                }
                else if (selectedItemIndex == 3) // Ranking
                {
                    rankingScreen = true;
                }
                else if (selectedItemIndex == 4) // Wyjœcie
                {
                    isRunning = false;
                }
            }
        }
    }
}

void Menu::loadPlayersFromFile()
{
    std::ifstream file("players.txt");
    std::string username, password;
    std::string score;
    if (file.is_open()) {
        while (file >> username >> password >> score)
        {
            Player player(username, password, score);
            ranking.push_back(player);
        }
        file.close();
    }
}
void Menu::sortRanking()
{
    std::sort(ranking.begin(), ranking.end(), [](const Player& a, const Player& b)
        {
            return a.score < b.score;
        });
}

void Menu::drawRanking(sf::RenderWindow& window)
{
    sortRanking();

    sf::Text rankingTitle;
    rankingTitle.setFont(font);
    rankingTitle.setString("Top graczy:");
    rankingTitle.setCharacterSize(50);
    rankingTitle.setFillColor(sf::Color::White);
    rankingTitle.setPosition(100, 100);
    window.draw(rankingTitle);

    for (int i = 0; i < 3 && i < ranking.size(); ++i)
    {
        sf::Text playerText;
        playerText.setFont(font);
        playerText.setString(ranking[i].username + ": " + ranking[i].score);
        playerText.setCharacterSize(40);
        playerText.setFillColor(sf::Color::White);
        playerText.setPosition(100, 200 + i * 50);
        window.draw(playerText);
    }
}

void Menu::updateMenuOptionText(int index, const std::string& newText)
{
    if (index >= 0 && index < menuItems.size())
    {
        menuItems[index].setString(newText);
        sf::FloatRect textRect = menuItems[index].getLocalBounds();
        menuItems[index].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    }
}