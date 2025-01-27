import tkinter as tk
from tkinter import messagebox
import serial
import threading
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import time

setpoint_lines = []

def read_serial():
    """Function to read data from the serial port."""
    global setpoint_value
    while True:
        try:
            if ser.is_open:
                line = ser.readline().decode('utf-8').strip()
                if line.startswith("Temperature:"):
                    temperature = line.split(":")[1].strip()
                    update_temperature(temperature)
                    update_plot(float(temperature))
                elif line.startswith("Setpoint:"):
                    print(line)
                    setpoint_value = float(line.split(":")[1].strip())
                    setpoint_label.config(text=f"Setpoint: {setpoint_value} °C")
                    print(setpoint_value)
                elif line.startswith("Signal:"):
                    pass
                else:
                    update_message_list(line)
        except Exception as e:
            print("line: ", line)
            print(f"Error reading serial data: {e}")
            break

def update_temperature(temp):
    """Update the temperature label in the GUI."""
    temperature_label.config(text=f"Temperature: {temp} °C")

def update_message_list(message):
    """Update the listbox with the latest messages."""
    if len(messages) >= 5:
        messages.pop(0)  # Remove the oldest message if the list exceeds 5
    messages.append(message)
    message_listbox.delete(0, tk.END)  # Clear the listbox
    for msg in messages:
        message_listbox.insert(tk.END, msg)  # Insert updated messages

def update_plot(temp):
    """Update the plot with the new temperature data."""
    current_time = time.time()
    temperatures.append((current_time, temp))

    # Remove points older than 5 minutes
    while temperatures and (current_time - temperatures[0][0] > 300):
        temperatures.pop(0)

    times = [t - temperatures[0][0] for t, _ in temperatures]  # Relative times
    temps = [t for _, t in temperatures]

    plot_line.set_xdata(times)
    plot_line.set_ydata(temps)
    ax.relim()
    ax.set_xlim(0, 300)  # Fixed x-axis for the last 5 minutes
    ax.set_ylim(0, 100)  # Fixed y-axis from 0 to 100

    # Remove old axhlines
    global setpoint_lines  # Ensure setpoint_lines is accessible
    for line in setpoint_lines:
        line.remove()  # Remove the line from the plot
    setpoint_lines = []  # Clear the list of old lines

    # Add new setpoint line and ±1°C lines
    if setpoint_value is not None:
        setpoint_lines.append(ax.axhline(setpoint_value, color='green', linewidth=2))
        setpoint_lines.append(ax.axhline(setpoint_value + 1, color='blue', linestyle='--', linewidth=2))
        setpoint_lines.append(ax.axhline(setpoint_value - 1, color='blue', linestyle='--', linewidth=2))

    canvas.draw()

def send_command(command = None):
    """Send a command to the serial port."""
    try:
        if ser.is_open:
            if not command:
                command = command_entry.get().ljust(12)  # Pad the command to 12 characters
            print(command, 'newline')
            ser.write(command.encode('ascii'))
            # command_entry.delete(0, tk.END)  # Clear the entry field
        else:
            messagebox.showwarning("Warning", "Serial port is not connected.")
    except Exception as e:
        messagebox.showerror("Error", f"Failed to send command: {e}")

def set_setpoint():
    """Send the setpoint value to the serial port."""
    global setpoint_value
    try:
        if ser.is_open:
            setpoint_value = float(setpoint_entry.get())
            command = f"w s {setpoint_value}".ljust(12)  # Format the setpoint command
            ser.write((command).encode('utf-8'))
            setpoint_label.config(text=f"Setpoint: {setpoint_value} °C")
        else:
            messagebox.showwarning("Warning", "Serial port is not connected.")
    except Exception as e:
        messagebox.showerror("Error", f"Failed to send setpoint: {e}")

def connect_to_serial():
    """Connect to the specified serial port."""
    global ser
    try:
        port = port_entry.get()
        baudrate = baudrate_entry.get()
        ser = serial.Serial(port, int(baudrate), timeout=1)
        threading.Thread(target=read_serial, daemon=True).start()
        # messagebox.showinfo("Connection Status", "Connected to the serial port successfully!")
        connect_button.config(state=tk.DISABLED)
        disconnect_button.config(state=tk.NORMAL)
        send_command("r s")
    except Exception as e:
        messagebox.showerror("Connection Error", f"Failed to connect to serial port: {e}")

def disconnect_from_serial():
    """Disconnect from the serial port."""
    global ser
    try:
        if ser.is_open:
            ser.close()
        # messagebox.showinfo("Connection Status", "Disconnected from the serial port.")
        connect_button.config(state=tk.NORMAL)
        disconnect_button.config(state=tk.DISABLED)
    except Exception as e:
        messagebox.showerror("Disconnection Error", f"Failed to disconnect: {e}")

# Initialize the main tkinter window
root = tk.Tk()
root.title("Serial Port Temperature Reader")
root.geometry("1280x720")

# Main frame to hold control and plot area
main_frame = tk.Frame(root)
main_frame.pack(fill=tk.BOTH, expand=True)

# Left side frame for controls
control_frame = tk.Frame(main_frame, width=400)
control_frame.pack(side=tk.LEFT, fill=tk.Y, padx=10, pady=10)

# Serial Port Frame
serial_frame = tk.Frame(control_frame)
serial_frame.pack(pady=10)

# Port Entry
tk.Label(serial_frame, text="Port:").grid(row=0, column=0, padx=5)
port_entry = tk.Entry(serial_frame, width=10)
port_entry.grid(row=0, column=1, padx=5)
port_entry.insert(0, "COM4")  # Default value

# Baudrate Entry
tk.Label(serial_frame, text="Baudrate:").grid(row=1, column=0, padx=5)
baudrate_entry = tk.Entry(serial_frame, width=10)
baudrate_entry.grid(row=1, column=1, padx=5)
baudrate_entry.insert(0, "115200")  # Default value

# Connect Button
connect_button = tk.Button(serial_frame, text="Connect", command=connect_to_serial)
connect_button.grid(row=2, column=0, columnspan=2, pady=10)

# Disconnect Button
disconnect_button = tk.Button(serial_frame, text="Disconnect", command=disconnect_from_serial, state=tk.DISABLED)
disconnect_button.grid(row=3, column=0, columnspan=2, pady=5)

# Temperature Display
temperature_label = tk.Label(control_frame, text="Temperature: -- °C", font=("Arial", 16))
temperature_label.pack(pady=10)

# Command Entry Frame
command_frame = tk.Frame(control_frame)
command_frame.pack(pady=10)

# Command Entry
tk.Label(command_frame, text="Command:").grid(row=0, column=0, padx=5)
command_entry = tk.Entry(command_frame, width=30)
command_entry.grid(row=0, column=1, padx=5)

# Send Button
send_button = tk.Button(command_frame, text="Send", command=send_command)
send_button.grid(row=0, column=2, padx=5)

# Setpoint Frame
setpoint_frame = tk.Frame(control_frame)
setpoint_frame.pack(pady=10)

# Setpoint Entry
tk.Label(setpoint_frame, text="Setpoint:").grid(row=0, column=0, padx=5)
setpoint_entry = tk.Entry(setpoint_frame, width=10)
setpoint_entry.grid(row=0, column=1, padx=5)

# Set Setpoint Button
set_setpoint_button = tk.Button(setpoint_frame, text="Set Setpoint", command=set_setpoint)
set_setpoint_button.grid(row=0, column=2, padx=5)

# Setpoint Display
setpoint_label = tk.Label(control_frame, text="Setpoint: -- °C", font=("Arial", 14))
setpoint_label.pack(pady=5)

# Message Listbox
tk.Label(control_frame, text="Last 5 Messages:").pack(pady=5)
messages = []  # List to store the last 5 messages
message_listbox = tk.Listbox(control_frame, height=5, width=50)
message_listbox.pack(pady=5)

# Temperature Plot
plot_frame = tk.Frame(main_frame)
plot_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, pady=10)

fig, ax = plt.subplots(figsize=(8, 5))  # Larger plot size
temperatures = []
plot_line, = ax.plot([], [], label="Temperature", color="red")
ax.set_title("Temperature Over Time", fontsize=12)
ax.set_xlabel("Time (seconds)", fontsize=10)
ax.set_ylabel("Temperature (°C)", fontsize=10)
ax.legend(fontsize=10)
ax.grid()

canvas = FigureCanvasTkAgg(fig, master=plot_frame)
canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

# Setpoint variable
setpoint_value = None

# Start the Tkinter event loop
root.mainloop()
