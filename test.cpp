#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>

#include <tgbot/tgbot.h>

using namespace std;
using namespace TgBot;

int main() {
    Bot bot("1123398491:AAGWVjoytWLjqlWhcg4iBUhJmbIcdN22h6A");

    InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> row0, row1;
    InlineKeyboardButton::Ptr checkButton(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr helloButton(new InlineKeyboardButton);

    checkButton->text = "check";
    checkButton->callbackData = "check";
    helloButton->text = "hello";
    helloButton->callbackData = "hello";
    row0.push_back(checkButton);
    row1.push_back(helloButton);

    keyboard->inlineKeyboard.push_back(row0);
    keyboard->inlineKeyboard.push_back(row1);

    bot.getEvents().onCommand("start", [&bot, &keyboard](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi!", false, 0, keyboard);
    });
    bot.getEvents().onCommand("check", [&bot, &keyboard](Message::Ptr message) {
        string response = "ok";
        bot.getApi().sendMessage(message->chat->id, response, false, 0, keyboard, "Markdown");
    });
    bot.getEvents().onCallbackQuery([&bot, &keyboard](CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "check")) {
            string response = "ok";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboard, "Markdown");
        }
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
