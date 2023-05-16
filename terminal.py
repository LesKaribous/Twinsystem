import serial
import serial.tools.list_ports
import threading
import queue
import curses

def find_teensy():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if "16C0:0483" in port.hwid:  # Modify the keyword if needed to match your Teensy device
            return port.device
    return None

def read_from_serial(ser, command_queue, output_win, input_win):
    while True:
        if ser.in_waiting > 0:
            response = ser.readline().decode().strip()

            # save cursor position
            cursor_y, cursor_x = input_win.getyx()

            # print output and refresh
            output_win.addstr(response + '\n')
            output_win.refresh()

            # return cursor to the saved position
            input_win.move(cursor_y, cursor_x)
            input_win.refresh()

        if not command_queue.empty():
            command = command_queue.get()
            if command == "exit":
                break
            ser.write(command.encode() + b'\n')

def user_input(command_queue, input_win):
    command = ''
    while True:
        c = input_win.getch()
        if c == ord('\n'):  # Enter key
            command_queue.put(command)
            if command == "exit":
                break
            command = ''
            input_win.clear()
        elif c == curses.KEY_BACKSPACE or c == 127:  # Backspace key
            command = command[:-1]
            input_win.clear()
            input_win.addstr("> " + command)
        elif c != -1:  # if a character is entered
            command += chr(c)
            input_win.addstr(chr(c))

def main(stdscr):
    com_port = find_teensy()
    if com_port is None:
        print("No teensy board found")
        return

    stdscr.addstr(f"Teensy found at {com_port}\n")
    stdscr.refresh()

    with serial.Serial(com_port, 115200, timeout=1) as ser:  # Modify baud rate and timeout as needed
        command_queue = queue.Queue()

        # create two windows for input and output
        output_win = curses.newwin(curses.LINES-3, curses.COLS, 0, 0)
        input_win = curses.newwin(3, curses.COLS, curses.LINES-3, 0)
        input_win.nodelay(True)  # make getch non-blocking

        # start a new thread to read from the serial and send commands
        threading.Thread(target=read_from_serial, args=(ser, command_queue, output_win, input_win), daemon=True).start()

        # start another thread to accept user input
        threading.Thread(target=user_input, args=(command_queue, input_win), daemon=True).start()

        while True:
            if not command_queue.empty() and "exit" == command_queue.queue[0]:
                break

if __name__ == "__main__":
    curses.wrapper(main)
