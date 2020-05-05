#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <curl/curl.h>
#include "gumbo.h"
#include <tgbot/tgbot.h>

using namespace std;
using namespace TgBot;

int main() {
    Bot bot("1123398491:AAGWVjoytWLjqlWhcg4iBUhJmbIcdN22h6A");

    CURL* curl;
    curl = curl_easy_init();

    InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> row0;
    InlineKeyboardButton::Ptr choicekBut(new InlineKeyboardButton);

    choicekBut->text = "Выбрать город";
    choicekBut->callbackData = "choice";
    row0.push_back(choicekBut);

    keyboard->inlineKeyboard.push_back(row0);

    bot.getEvents().onCommand("start", [&bot, &keyboard](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Привет!", false, 0, keyboard);
    });
    bot.getEvents().onCommand("choice", [&bot, &keyboard](Message::Ptr messageKey) {
    	bot.getApi().sendMessage(messageKey->chat->id, "Введите название населнного пункта");
    	bot.getEvents().onCallbackQuery([&bot]((CallbackQuery::Ptr query){

    	});
    });

    const string photoFilePath = "1.jpg";
    const string photoMimeType = "1.jpg";

    bot.getEvents().onCommand("photo", [&bot, &photoFilePath, &photoMimeType](Message::Ptr message) {
    bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile(photoFilePath, photoMimeType));
        });

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (exception& e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}
