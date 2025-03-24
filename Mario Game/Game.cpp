#include "Game.h"
#include <thread>




// -----------------------------------------
//           Konstruktor Game
// -----------------------------------------
Game::Game()
    : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Mario", sf::Style::Close),
    lag(0),
    frame_start_time(std::chrono::steady_clock::now()),
    game_previous_time(std::chrono::steady_clock::now()),
    mapa(1),                        
    mario(),                    
    menu(SCREEN_WIDTH, SCREEN_HEIGHT),
    menu_open(true),
    isRunning(true),
    gameEnded(false),
    totalTime(0)
{
    window.setFramerateLimit(60);
}

// -----------------------------------------
//                run()
// -----------------------------------------
void Game::run()
{
    gameStartTime = std::chrono::steady_clock::now();
    mapa.loadMap(enemies); // Pierwsze wczytanie mapy i przeciwników

    while (window.isOpen() && isRunning)
    {
        // Obliczamy delta_time
        auto current_time = std::chrono::steady_clock::now();
        auto delta_time = std::chrono::duration_cast<std::chrono::microseconds>(current_time - frame_start_time);
        frame_start_time = current_time;
        lag += delta_time;

        processEvents();  // Obs³uga zdarzeñ

        // Jeœli menu nie jest otwarte i gra nie jest zakoñczona, aktualizujemy grê
        if (!menu_open && !gameEnded)
        {
            if (!mario.getDead()) // Tylko wtedy naliczamy czas
            {
                totalTime += std::chrono::duration_cast<std::chrono::milliseconds>(current_time - game_previous_time);
            }
            game_previous_time = current_time;

            // Wywo³ania update w sta³ych krokach
            while (lag >= FRAME_DURATION)
            {
                lag -= FRAME_DURATION;
                update();
            }
        }
        else
        {
            // Jeœli gra jest zakoñczona lub menu otwarte, zerujemy lag,
            // aby nie przeskoczyæ w czasie po powrocie z menu
            game_previous_time = std::chrono::steady_clock::now();
            lag = std::chrono::microseconds(0);
        }

        // Renderujemy
        render();
    }
}

// -----------------------------------------
//           processEvents()
// -----------------------------------------
void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        // Zamkniêcie okna
        if (event.type == sf::Event::Closed)
        {
            window.close();
            return;
        }

        // --- OBS£UGA MENU ---
        if (menu_open)
        {
            // W pierwszej kolejnoœci pozwól menu obs³u¿yæ eventy
            menu.handleEvent(event, menu_open, isRunning, players);

            // Jeœli chcesz, by Enter wychodzi³ z menu:
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            {
                // Zamykamy menu
                menu_open = false;
            }
        }
        // --- OBS£UGA GAME OVER ---
        else if (gameEnded)
        {
            // Jeœli przegraliœmy lub wygraliœmy (gameEnded = true),
            // Enter -> restart gry
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            {
                resetGame();
            }
            // Escape -> otwarcie menu
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                menu_open = true;
                updateMenuStartOption();
            }
        }
        // --- NORMALNA GRA (nie ma menu, nie ma gameOver) ---
        else
        {
            // Escape -> otwarcie/ zamkniêcie menu
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                menu_open = true;
                updateMenuStartOption();
            }
        }
    }
}

// -----------------------------------------
//                update()
// -----------------------------------------
void Game::update()
{
    // Aktualizacja Mario
    mario.update(mapa);

    // Wielow¹tkowa aktualizacja przeciwników
    const int watki = 4;
    std::vector<std::thread> threads;
    int liczbpostaci = static_cast<int>(enemies.size());
    int postacienawatek = (liczbpostaci + watki - 1) / watki;

    for (int i = 0; i < watki; ++i)
    {
        int start = i * postacienawatek;
        int end = std::min(start + postacienawatek, liczbpostaci);

        threads.emplace_back([this, start, end]()
            {
                for (int j = start; j < end; ++j)
                {
                    enemies[j]->update(mario.getX(), enemies, mapa, mario);
                }
            });
    }
    for (auto& thread : threads)
        thread.join();

    // Usuwanie martwych przeciwników
    for (unsigned short a = 0; a < enemies.size(); a++)
    {
        if (1 == enemies[a]->getDead(1))
        {
            enemies.erase(a + enemies.begin());
            a--;
        }
    }

    // Scroll mapy
    mapa.updateView(window, mario.getX());

    // Sprawdzenie warunku wygrania
    if (mapa.isEndReached(mario.getX()))
    {
        gameEnded = true;
        saveScoreToFile(); // Zapisz wynik
       
        menu_open = true; 
    }

    // Sprawdzenie œmierci Mario
    if (mario.getDead())
    {
        gameEnded = true;
		
		
    }
}

// -----------------------------------------
//               render()
// -----------------------------------------
void Game::render()
{
    window.clear(sf::Color(135, 206, 235));

    // Rysowanie mapy i przeciwników
    mapa.drawMap(window, mario.getX());
    for (auto& enemy : enemies)
    {
        enemy->draw(mario.getX(), window);
    }

    // Rysowanie Mario
    mario.draw(window);

    // Rysowanie up³ywu czasu
    drawTime(window);

    if (gameEnded)
    {
        displayGameOver();
    }
  
    else if (menu_open)
    {
        sf::View originalView = window.getView();
        window.setView(window.getDefaultView());

        sf::RectangleShape overlay(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);

        menu.draw(window);
        window.setView(originalView);
    }

    // Statystyki Mario (np. punkty, ¿ycie)
    mario.drawstats(window);

    window.display();
}

// -----------------------------------------
//       Formatowanie czasu w napisie
// -----------------------------------------
std::string Game::getFormattedTime() const
{
    auto millis = totalTime.count() % 1000;
    auto seconds = (totalTime.count() / 1000) % 60;
    auto minutes = (totalTime.count() / (1000 * 60)) % 60;

    // Jeœli Mario ma "pointstimeminus" jako karê w sekundach:
    unsigned int adjustedSeconds = (seconds >= mario.pointstimeminus)
        ? (seconds - mario.pointstimeminus)
        : 0;

    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%02u:%02u:%03u",
        minutes, adjustedSeconds, millis);
    return std::string(buffer);
}

// -----------------------------------------
//         Zapis wyniku do pliku
// -----------------------------------------
void Game::saveScoreToFile()
{
    std::string username = menu.loggedInUser;
    auto it = std::find_if(menu.ranking.begin(), menu.ranking.end(),
        [&username](const Player& player) {
            return player.username == username;
        });
    if (it != menu.ranking.end())
    {
        std::string password = it->password;
        std::string score = getFormattedTime();

        Player currentPlayer(username, password, score);
        players.push_back(currentPlayer);

        std::ofstream file("players.txt", std::ios::app);
        if (file.is_open())
        {
            for (const auto& player : players)
            {
                file << player.username << " "
                    << player.password << " "
                    << player.score << std::endl;
            }
            file.close();
        }
    }
}

// -----------------------------------------
//            Rysowanie czasu
// -----------------------------------------
void Game::drawTime(sf::RenderWindow& window)
{
    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());

    sf::Font font;
    font.loadFromFile("Resources/Alexandriaflf.ttf");

    sf::Text timeText;
    timeText.setFont(font);
    timeText.setCharacterSize(24);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(10, 440);
    timeText.setString(getFormattedTime());

    window.draw(timeText);

    window.setView(originalView);
}

// -----------------------------------------
//     Wyœwietlanie ekranu Game Over
// -----------------------------------------
void Game::displayGameOver()
{
   
    sf::Font font;
    font.loadFromFile("Resources/Alexandriaflf.ttf");

    sf::Text gameOverText("GAME OVER", font, 50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(
        SCREEN_WIDTH / 2.f - gameOverText.getLocalBounds().width / 2.f,
        SCREEN_HEIGHT / 2.f - 100
    );

    sf::Text retryText("Press ENTER to Retry", font, 30);
    retryText.setFillColor(sf::Color::White);
    retryText.setPosition(
        SCREEN_WIDTH / 2.f - retryText.getLocalBounds().width / 2.f,
        SCREEN_HEIGHT / 2.f
    );

    window.draw(gameOverText);
    window.draw(retryText);
}

// -----------------------------------------
//       Reset stanu gry (po przegranej)
// -----------------------------------------
void Game::resetGame()
{
    
    gameEnded = false;

    // Ponowne wczytanie mapy (usuwa poprzednich wrogów i ³aduje od nowa)
    mapa.loadMap(enemies);

   
    mario = Mario();

    // Zerujemy czas
    totalTime = std::chrono::milliseconds(0);

    // Zamykamy menu (opcjonalnie)
    menu_open = false;
}

// -----------------------------------------
//   Uaktualnia napis startowy w menu
// -----------------------------------------
void Game::updateMenuStartOption()
{
    if (menu_open)
    {
        // Tekst przycisku (np. "Wznów" lub "Rozpocznij")
        menu.updateMenuOptionText(0, isRunning ? "Wznow" : "Rozpocznij");
    }
}