import tkinter as tk
from tkinter import messagebox
from plot_handler import setup_plots, update_plot_lines
from serial_handler import setup_serial_reader, send_serial_command, connect_to_serial, disconnect_from_serial, request_setpoint

# Initialize the main tkinter window
root = tk.Tk()
root.title("Serial Port Temperature Reader")
root.geometry("1280x720")

# Left Control Frame
control_frame = tk.Frame(root)
control_frame.pack(side=tk.LEFT, padx=10, pady=10, fill=tk.Y)

# Serial Port Frame
serial_frame = tk.Frame(control_frame)
serial_frame.pack(pady=10, fill=tk.X)

# Port Entry
tk.Label(serial_frame, text="Port:").grid(row=0, column=0, padx=5, pady=5)
port_entry = tk.Entry(serial_frame, width=10)
port_entry.grid(row=0, column=1, padx=5, pady=5)
port_entry.insert(0, "COM4")  # Default value

# Baudrate Entry
tk.Label(serial_frame, text="Baudrate:").grid(row=1, column=0, padx=5, pady=5)
baudrate_entry = tk.Entry(serial_frame, width=10)
baudrate_entry.grid(row=1, column=1, padx=5, pady=5)
baudrate_entry.insert(0, "115200")  # Default value

# Connect Button
connect_button = tk.Button(serial_frame, text="Connect", command=lambda: connect_to_serial(port_entry.get(), baudrate_entry.get(), connect_button, disconnect_button, update_setpoint_callback))
connect_button.grid(row=2, column=0, columnspan=2, pady=5)

# Disconnect Button
disconnect_button = tk.Button(serial_frame, text="Disconnect", command=disconnect_from_serial, state=tk.DISABLED)
disconnect_button.grid(row=3, column=0, columnspan=2, pady=5)

# Temperature Display
temperature_label = tk.Label(control_frame, text="Temperature: -- °C", font=("Arial", 16))
temperature_label.pack(pady=10)

# Setpoint Display
setpoint_label = tk.Label(control_frame, text="Setpoint: -- °C", font=("Arial", 16))
setpoint_label.pack(pady=10)

# Command Entry Frame
command_frame = tk.Frame(control_frame)
command_frame.pack(pady=10, fill=tk.X)

# Command Entry
tk.Label(command_frame, text="Command:").grid(row=0, column=0, padx=5, pady=5)
command_entry = tk.Entry(command_frame, width=20)
command_entry.grid(row=0, column=1, padx=5, pady=5)

# Send Button
send_button = tk.Button(command_frame, text="Send", command=lambda: send_serial_command(command_entry.get()))
send_button.grid(row=0, column=2, padx=5, pady=5)

# Message Listbox
tk.Label(control_frame, text="Last 5 Messages:").pack(pady=5)
message_listbox = tk.Listbox(control_frame, height=5, width=30)
message_listbox.pack(pady=5)

# Right Plot Frame
plot_frame = tk.Frame(root)
plot_frame.pack(side=tk.RIGHT, padx=10, pady=10, expand=True, fill=tk.BOTH)

# Plot Initialization
plot_canvas, plot_lines = setup_plots(plot_frame)

# Setpoint callback
def update_setpoint_callback(new_setpoint):
    setpoint_label.config(text=f"Setpoint: {new_setpoint:.1f} °C")
    update_plot_lines(plot_lines, new_setpoint)

# Start Serial Reader
setup_serial_reader(temperature_label, message_listbox, plot_lines, update_setpoint_callback)

# Start the Tkinter event loop
root.mainloop()