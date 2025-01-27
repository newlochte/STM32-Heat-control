import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import time

# Constants for the plots
PLOT_DURATION = 300  # 5 minutes in seconds
TEMP_Y_AXIS_MIN = 25
TEMP_Y_AXIS_MAX = 80
SIGNAL_Y_AXIS_MIN = -10
SIGNAL_Y_AXIS_MAX = 10

# Initialize plot data
def setup_plots(parent_frame):
    figure, (ax_temp, ax_signal) = plt.subplots(2, 1, figsize=(12, 8), dpi=100, gridspec_kw={"height_ratios": [2, 1]})

    # Temperature plot
    ax_temp.set_xlim(0, PLOT_DURATION)
    ax_temp.set_ylim(TEMP_Y_AXIS_MIN, TEMP_Y_AXIS_MAX)
    ax_temp.set_xlabel("Time (s)")
    ax_temp.set_ylabel("Temperature (°C)")
    ax_temp.grid()
    temperature_line, = ax_temp.plot([], [], label="Temperature", color="red")
    # setpoint_line, = ax_temp.plot([], [], label="Setpoint", color="green")
    # setpoint_error_plus_line, = ax_temp.plot([], [], linestyle="--", label="Error", color="blue")
    # setpoint_error_minus_line, = ax_temp.plot([], [], linestyle="--", color="blue")
    ax_temp.legend()

    # Signal plot
    ax_signal.set_xlim(0, PLOT_DURATION)
    ax_signal.set_ylim(SIGNAL_Y_AXIS_MIN, SIGNAL_Y_AXIS_MAX)
    ax_signal.set_xlabel("Time (s)")
    ax_signal.set_ylabel("Signal")
    ax_signal.grid()
    signal_line, = ax_signal.plot([], [], label="Signal", color="purple")
    ax_signal.legend()

    canvas = FigureCanvasTkAgg(figure, master=parent_frame)
    canvas.get_tk_widget().pack(fill="both", expand=True)
    canvas.draw()

    plot_lines = {
        "temperature": [],
        "time_temp": [],
        "signal": [],
        "time_signal": [],
        "temperature_line": temperature_line,
        # "setpoint_line": setpoint_line,
        # "setpoint_error_plus_line": setpoint_error_plus_line,
        # "setpoint_error_minus_line": setpoint_error_minus_line,
        "signal_line": signal_line,
        "ax_temp": ax_temp,
        "ax_signal": ax_signal,
        "canvas": canvas,
    }
    return canvas, plot_lines

# Update the plots dynamically
def update_plot_lines(plot_lines, setpoint):
    print("update")
    current_time = time.time()

    # Update temperature plot
    plot_lines["time_temp"] = [t for t in plot_lines["time_temp"] if current_time - t <= PLOT_DURATION]
    plot_lines["temperature"] = plot_lines["temperature"][-len(plot_lines["time_temp"]):]

    x_data_temp = [current_time - t for t in plot_lines["time_temp"]]
    plot_lines["temperature_line"].set_data(x_data_temp, plot_lines["temperature"])

    # Update setpoint lines
    y_data_setpoint = [setpoint] * len(x_data_temp)
    y_data_error_plus = [setpoint + 0.4] * len(x_data_temp)
    y_data_error_minus = [setpoint - 0.4] * len(x_data_temp)

    # plot_lines["setpoint_line"].set_data(x_data_temp, y_data_setpoint)
    # plot_lines["setpoint_error_plus_line"].set_data(x_data_temp, y_data_error_plus)
    # plot_lines["setpoint_error_minus_line"].set_data(x_data_temp, y_data_error_minus)

    # Adjust temperature x-axis dynamically
    if plot_lines["time_temp"]:
        plot_lines["ax_temp"].set_xlim(0, max(x_data_temp) + 5)

    # Update signal plot
    plot_lines["time_signal"] = [t for t in plot_lines["time_signal"] if current_time - t <= PLOT_DURATION]
    plot_lines["signal"] = plot_lines["signal"][-len(plot_lines["time_signal"]):]

    x_data_signal = [current_time - t for t in plot_lines["time_signal"]]
    plot_lines["signal_line"].set_data(x_data_signal, plot_lines["signal"])

    # Adjust signal x-axis dynamically
    if plot_lines["time_signal"]:
        plot_lines["ax_signal"].set_xlim(0, max(x_data_signal) + 5)

    plot_lines["canvas"].draw()