import os
import time
from datetime import datetime
import tkinter as tk
from tkinter import filedialog, messagebox
from tkinter import ttk
import serial
from serial.tools import list_ports
from threading import Thread

# 初始化串口
arduino_serial = None

def list_serial_ports():
    """获取当前可用的串口列表"""
    ports = list_ports.comports()
    return [port.device for port in ports]

def refresh_serial_ports():
    """刷新串口列表并更新下拉菜单"""
    ports = list_serial_ports()
    port_combobox['values'] = ports
    if ports:
        port_combobox.current(0)  # 默认选择第一个可用端口
    else:
        port_combobox.set("No ports available")

def initialize_serial_connection():
    """根据用户选择的串口进行连接初始化"""
    global arduino_serial
    selected_port = port_combobox.get()
    if not selected_port or selected_port == "No ports available":
        messagebox.showerror("Serial Error", "Please select a valid port.")
        return

    try:
        arduino_serial = serial.Serial(selected_port, baudrate=115200, timeout=1)
        messagebox.showinfo("Serial Info", f"Connected to {selected_port} at 115200 baud.")
    except serial.SerialException as e:
        messagebox.showerror("Serial Error", f"Failed to connect to {selected_port}: {e}")
        arduino_serial = None

def disconnect_serial():
    """断开当前串口连接"""
    global arduino_serial
    if arduino_serial:
        try:
            arduino_serial.close()
            arduino_serial = None
            messagebox.showinfo("Serial Info", "Disconnected from the serial port.")
        except serial.SerialException as e:
            messagebox.showerror("Serial Error", f"Failed to disconnect: {e}")
    else:
        messagebox.showinfo("Serial Info", "No serial port is currently connected.")
preview_process = None

def start_preview():
    global preview_process
    if preview_process is None:
        command = "libcamera-still -t 0 --preview 0,0,1280,960"
        preview_process = os.popen(command)
    else:
        messagebox.showinfo("Info", "Preview is already running.")


def send_to_arduino(command):
    """向 Arduino 发送串口指令"""
    if arduino_serial:
        try:
            arduino_serial.write(command.encode() + b"\n")
            response = arduino_serial.readline().decode().strip()
            # print(f"Arduino response: {response}")
        except serial.SerialException as e:
            messagebox.showerror("Serial Error", f"Failed to send command: {e}")
    else:
        messagebox.showerror("Serial Error", "Arduino is not connected.")

def stop_preview():
    global preview_process
    if preview_process is not None:
        preview_process.close()
        preview_process = None
    else:
        messagebox.showinfo("Info", "No preview to stop.")

def start_timelapse():
    def timelapse_task():
        stop_preview()  # 停止预览避免冲突

        output_dir = folder_path.get()
        interval = int(interval_entry.get())
        total_duration = int(duration_entry.get())
        shutter = int(shutter_entry.get())
        gain = float(gain_entry.get())
        width = int(width_entry.get())
        height = int(height_entry.get())

        if not output_dir:
            messagebox.showerror("Error", "Please select an output folder.")
            return

        os.makedirs(output_dir, exist_ok=True)
        total_photos = total_duration // interval

        for i in range(total_photos):
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            filename = f"{output_dir}/{i}.jpg"
            command = f"libcamera-still -o {filename} --shutter {shutter} --gain {gain} --width {width} --height {height} --timeout 1 --awb auto --ev 1"
            os.system(command)

            # 每拍一张照片后控制电机前进一步
            if arduino_serial:
                try:
                    arduino_serial.write(b"c2f1\n")  # 前进1步
                    response = arduino_serial.readline().decode().strip()
                    print(f"Arduino response: {response}")  # 打印响应
                except serial.SerialException as e:
                    print(f"Serial communication error: {e}")

            time.sleep(interval)

        messagebox.showinfo("Done", "Timelapse photography complete.")

    Thread(target=timelapse_task).start()

def browse_folder():
    folder_selected = filedialog.askdirectory()
    folder_path.set(folder_selected)

def on_closing():
    """释放资源并退出程序"""
    if arduino_serial:
        arduino_serial.close()
    root.destroy()
def send_custom_command():
    """发送用户输入的指令到串口"""
    command = command_entry.get().strip()
    if not command:
        messagebox.showwarning("Input Error", "Command cannot be empty.")
        return

    if arduino_serial:
        try:
            arduino_serial.write((command + "\n").encode())
            response = arduino_serial.readline().decode().strip()
            print(f"Arduino response: {response}")
            messagebox.showinfo("Command Sent", f"Response: {response}")
        except serial.SerialException as e:
            messagebox.showerror("Serial Error", f"Failed to send command: {e}")
    else:
        messagebox.showerror("Serial Error", "Arduino is not connected.")
# 主程序
root = tk.Tk()
root.title("Timelapse Photography with Arduino Motor Control")

folder_path = tk.StringVar()

tk.Label(root, text="Output Folder:").grid(row=0, column=0, padx=5, pady=5, sticky='e')
tk.Entry(root, textvariable=folder_path, width=50).grid(row=0, column=1, padx=5, pady=5)
tk.Button(root, text="Browse", command=browse_folder).grid(row=0, column=2, padx=5, pady=5)

tk.Label(root, text="Interval (seconds):").grid(row=1, column=0, padx=5, pady=5, sticky='e')
interval_entry = tk.Entry(root)
interval_entry.grid(row=1, column=1, padx=5, pady=5)
interval_entry.insert(0, "10")

tk.Label(root, text="Total Duration (seconds):").grid(row=2, column=0, padx=5, pady=5, sticky='e')
duration_entry = tk.Entry(root)
duration_entry.grid(row=2, column=1, padx=5, pady=5)
duration_entry.insert(0, "3600")

tk.Label(root, text="Shutter (microseconds):").grid(row=3, column=0, padx=5, pady=5, sticky='e')
shutter_entry = tk.Entry(root)
shutter_entry.grid(row=3, column=1, padx=5, pady=5)
shutter_entry.insert(0, "100000")

tk.Label(root, text="Gain:").grid(row=4, column=0, padx=5, pady=5, sticky='e')
gain_entry = tk.Entry(root)
gain_entry.grid(row=4, column=1, padx=5, pady=5)
gain_entry.insert(0, "5.0")

tk.Label(root, text="Width:").grid(row=5, column=0, padx=5, pady=5, sticky='e')
width_entry = tk.Entry(root)
width_entry.grid(row=5, column=1, padx=5, pady=5)
width_entry.insert(0, "2028")

tk.Label(root, text="Height:").grid(row=6, column=0, padx=5, pady=5, sticky='e')
height_entry = tk.Entry(root)
height_entry.grid(row=6, column=1, padx=5, pady=5)
height_entry.insert(0, "1080")

tk.Button(root, text="Open Camera Preview", command=start_preview).grid(row=7, column=1, columnspan=1, pady=10)
tk.Button(root, text="Stop Camera Preview", command=stop_preview).grid(row=8, column=1, columnspan=1, pady=10)
tk.Button(root, text="Start Timelapse", command=start_timelapse).grid(row=9, column=1, columnspan=1, pady=10)

# 串口选择组件
tk.Label(root, text="Select Arduino Port:").grid(row=11, column=0, padx=5, pady=5, sticky='e')
port_combobox = ttk.Combobox(root, state="readonly", width=40)
port_combobox.grid(row=11, column=1, padx=5, pady=5)
port_combobox.set("Select a port")
tk.Button(root, text="Refresh Ports", command=refresh_serial_ports).grid(row=12, column=2, padx=5, pady=5)
tk.Button(root, text="Connect to Arduino", command=initialize_serial_connection).grid(row=12, column=0, columnspan=2, pady=10)
tk.Button(root, text="Disconnect Port", command=disconnect_serial).grid(row=12, column=1, columnspan=2, pady=10)

tk.Label(root, text="Send Command:").grid(row=15, column=0, padx=5, pady=5, sticky='e')
command_entry = tk.Entry(root, width=40)
command_entry.grid(row=15, column=1, padx=5, pady=5)
tk.Button(root, text="Send", command=send_custom_command).grid(row=15, column=2, padx=5, pady=5)
# 加载可用串口
refresh_serial_ports()

root.protocol("WM_DELETE_WINDOW", on_closing)

root.mainloop()
