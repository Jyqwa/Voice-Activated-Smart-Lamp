import speech_recognition
import pyttsx3
import serial
import time

recognizer = speech_recognition.Recognizer()

arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1) 
def write_read(x): 
    arduino.write(bytes(x, 'utf-8')) 
    time.sleep(0.05) 
    data = arduino.readline() 
    return data 
while True: 
    num = input("Enter a number: ") # Taking input from user 
    value = write_read(num) 
    print(value) # printing the value 


while True:
    try:
        #Grabs Audio from mic
        with speech_recognition.Microphone() as mic:
            recognizer.adjust_for_ambient_noise(mic, duration=0.2)
            audio = recognizer.listen(mic)

            #Translates audio into text
            text = recognizer.recognize_google(audio)
            text = text.lower()

            print(f"recognized {text}")

    except speech_recognition.UnknownValueError:

        recognizer = speech_recognition.Recognizer()
        continue
