import serial
import threading
import time
import tkinter as tk
from tkinter import messagebox
import re
import traceback

ser = None

messages = []  # Last 5 messages
temperatures = []  # (timestamp, temperature)
signals = []  # (timestamp, signals)

# Start serial reader
def setup_serial_reader(temp_label, message_listbox, plot_lines, setpoint_callback):
    def read_serial():
        while True:
            try:
                if ser and ser.is_open:
                    line = ser.readline().decode('utf-8').strip()
                    if line.startswith("Temperature:"):
                        temp = float(line.split(":")[1].strip())
                        temp_label.config(text=f"Temperature: {temp} °C")
                        temperatures.append((time.time(), temp))
                        plot_lines["temperature"].append(temp)
                    elif line.startswith("Setpoint:"):
                        new_setpoint = float(line.split(":")[1].strip())
                        setpoint_callback(new_setpoint)
                    elif line.startswith("Signal:"):
                        parts = line.split(":")
                        if len(parts) > 1:
                            value = re.split(r'\s+', parts[1].strip(), maxsplit=1)
                            if len(value) > 1:
                                signal = float(value[1])
                                if value[0] == '-':
                                    signal = -1 * signal
                                signals.append((time.time(), signal))
                                plot_lines["signal"].append(signal)
                    else:
                    # Update messages
                      messages.append(line)
                    if len(messages) > 5:
                        messages.pop(0)
                    message_listbox.delete(0, tk.END)
                    for msg in messages:
                        message_listbox.insert(tk.END, msg)
            except Exception as e:
                print(f"Serial error: {e}")
                traceback.print_exc()
    threading.Thread(target=read_serial, daemon=True).start()


def connect_to_serial(port, baudrate, connect_btn, disconnect_btn, setpoint_callback):
    global ser
    try:
        ser = serial.Serial(port, int(baudrate), timeout=1)
        threading.Thread(target=setup_serial_reader, args=(connect_btn, disconnect_btn, setpoint_callback), daemon=True).start()
        connect_btn.config(state=tk.DISABLED)
        disconnect_btn.config(state=tk.NORMAL)
        # Request initial setpoint
        request_setpoint()
        print("Connected to serial port")
    except Exception as e:
        messagebox.showerror("Connection Error", f"Failed to connect: {e}")


def disconnect_from_serial():
    global ser
    if ser and ser.is_open:
        ser.close()
        print("Disconnected from serial port")

def send_serial_command(command):
    if ser and ser.is_open:
        command = command.ljust(12)  # Ensure command is 12 chars long
        ser.write(command.encode('utf-8'))

def request_setpoint():
    send_serial_command("r s")  # Request current setpoint
