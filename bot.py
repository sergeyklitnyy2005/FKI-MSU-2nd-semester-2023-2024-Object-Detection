import subprocess
import os

import telebot
from PIL import Image
import io
from telebot import types
import smtplib


class ImageBot:
    def __init__(self, token):
        self.bot = telebot.TeleBot(token)
        self.user_state = {}

    def main_menu(self, message):
        markup = types.ReplyKeyboardMarkup(row_width=2)
        itembtn1 = types.KeyboardButton('Выделить объекты на изображении')
        itembtn4 = types.KeyboardButton('Помочь автору создавать')
        itembtn5 = types.KeyboardButton('Оставить отзыв/жалобу')
        itembtn6 = types.KeyboardButton('Получить границы изображения')
        itembtn7 = types.KeyboardButton('меню')
        markup.add(itembtn1, itembtn4, itembtn5, itembtn6, itembtn7)
        self.bot.send_message(message.chat.id, "Выберите действие:", reply_markup=markup)

    def run_make(self):
        path = r"C:\Users\ksv\Documents\GitHub\FKI-MSU-2nd-semester-2023-2024-Object-Detection"
        os.chdir(path)
        subprocess.run("mingw32-make", shell=True)

    def convert_and_save_image(self, message):
        try:
            file_info = self.bot.get_file(message.photo[-1].file_id)
            downloaded_file = self.bot.download_file(file_info.file_path)

            img = Image.open(io.BytesIO(downloaded_file))
            img = img.convert('RGBA')
            img.save(r'skull.png', 'PNG')
            self.run_make()
            self.bot.reply_to(message, "Фото успешно преобразовано")
        except TypeError:
            self.bot.reply_to(message, "Пожалуйста, отправьте фотографию.")

    def send_image(self, message):
        with open(r'output.png', 'rb') as photo:
            self.bot.send_photo(message.chat.id, photo)

    def send_image2(self, message):
        with open(r'output2.png', 'rb') as photo:
            self.bot.send_photo(message.chat.id, photo)

    def default_message(self, message):
        self.bot.reply_to(message, "Извините, я не понимаю эту команду.")

    def send_email(self, message):
        try:
            sender_email = '__@gmail.com'
            password = ''
            receiver_email = 'login_our_input_email'
            server = smtplib.SMTP('smtp.gmail.com', 587)
            server.ehlo()
            server.starttls()
            server.login(sender_email, password)
            message = f'Subject: Отзыв/жалоба на телеграмм бот по подготовке к егэ по русскому языку\nContent-Type: text/plain; charset=utf-8\n\n{message.text}'
            server.sendmail(sender_email, receiver_email, message.encode('utf-8'))
            print('Email sent successfully')
        except Exception as e:
            print(f'Error: {e}')



    def start(self):
        @self.bot.message_handler(commands=['start'])
        def send_welcome(message):
            self.main_menu(message)
            self.bot.send_message(message.chat.id,
                                  "Добрый день,спасибо, что пользуетесь данным ботом.")

        @self.bot.message_handler(content_types=['photo'])
        def handle_photo(message):
            if self.user_state.get(message.chat.id) == 'Image':
                self.convert_and_save_image(message)
                self.send_image(message)
                self.user_state[message.chat.id] = 'NONE'
            elif self.user_state.get(message.chat.id) == 'Image2':
                self.convert_and_save_image(message)
                self.send_image2(message)
                self.user_state[message.chat.id] = 'NONE'
            else:
                self.bot.reply_to(message, "Я не ожидал получить изображение. Пожалуйста, выберите действие.")

        @self.bot.message_handler(func=lambda m: True)
        def handle_message(message):
            if message.text == 'Выделить объекты на изображении':
                self.bot.send_message(message.chat.id, "Отправьте изображение.")
                self.user_state[message.chat.id] = 'Image'

            elif message.text == "Помочь автору создавать":
                self.bot.send_message(message.chat.id,
                                      "Спасибо за вашу поддержку!\n Ваш вклад помогает мне продолжать создавать и делиться своим творчеством. Вы можете поддержать меня, сделав перевод на мой банковский счет. Номер моей карты: 2200 2407 3514 1034. Благодарю вас за поддержку!")

            elif message.text == 'Оставить отзыв/жалобу':
                if message.text == "меню":
                    self.user_state[message.chat.id] = 'NONE'
                else:
                    self.bot.send_message(message.chat.id, "Пожалуйста, напишите отзыв/жалобу в следующем сообщении")
                    self.user_state[message.chat.id] = 'SEND_MAIL'

            elif self.user_state.get(message.chat.id) == "SEND_MAIL":
                self.user_state[message.chat.id] = 'NONE'
                if message.text == 'меню':
                    self.main_menu(message)
                else:
                    self.bot.send_message(message.chat.id, "Пожалуйста, подождите, Ваш отзыв отправляется")
                    self.send_email(message)
                    self.bot.send_message(message.chat.id, "Спасибо! Ваша поддержка и отзывы помогают нам совершенствоваться.")

            elif message.text == 'меню':
                self.main_menu(message)
            elif message.text == 'Получить границы изображения':
                self.user_state[message.chat.id] = 'Image2'
            else:
                self.bot.reply_to(message, "Извините, я не понимаю эту команду.")

        self.bot.polling()




if __name__ == '__main__':
    bot = ImageBot('your_tg_token')
    bot.start()
