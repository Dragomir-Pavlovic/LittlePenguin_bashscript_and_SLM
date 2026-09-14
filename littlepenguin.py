import tkinter as tk
from tkinter import messagebox
from PIL import Image, ImageTk, ImageSequence
import os

WINDOW_WIDTH = 600
# %%
WINDOW_HEIGHT = 600

BG_COLOR = "black"
TEXT_COLOR = "yellow"
BUTTON_BG = "#202020"
BUTTON_ACTIVE = "#404040"
VERSION = "V1.0"

class LPSLMApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Little Penguin Small Learning Model")
        self.root.geometry(f"{WINDOW_WIDTH}x{WINDOW_HEIGHT}")
        self.root.resizable(False, False)
        self.root.configure(bg=BG_COLOR)

        self.gif_frames = []
        self.gif_index = 0
        self.gif_running = False

        self.create_gui()
        self.load_logo()
        self.load_gif()

    def create_gui(self):
        top_frame = tk.Frame(self.root, bg=BG_COLOR)
        top_frame.pack(fill="x", padx=15, pady=(12, 5))

        self.program_label = tk.Label(
            top_frame,
            text=f"LP-SLM {VERSION}",
            font=("Courier New", 18, "bold"),
            fg=TEXT_COLOR,
            bg=BG_COLOR
        )
        self.program_label.pack(side="left")

        info_frame = tk.Frame(self.root, bg=BG_COLOR)
        info_frame.pack(fill="x", padx=15, pady=5)

        self.logo_label = tk.Label(
            info_frame,
            text="LOGO",
            font=("Courier New", 10, "bold"),
            fg=TEXT_COLOR,
            bg=BG_COLOR,
            width=15,
            height=7
        )
        self.logo_label.pack(side="right", padx=(10, 0))

        description = (
            "This is a Small Learning Model - Little Penguin\n"
            "Created by: Dragomir Pavlovic\n"
            "Small College Graduation Project"
        )

        description_label = tk.Label(
            info_frame,
            text=description,
            font=("Courier New", 10),
            fg=TEXT_COLOR,
            bg=BG_COLOR,
            justify="left",
            anchor="nw"
        )
        description_label.pack(
            side="left",
            fill="both",
            expand=True,
            pady=5
        )

        separator = tk.Frame(
            self.root,
            bg=TEXT_COLOR,
            height=1
        )
        separator.pack(
            fill="x",
            padx=15,
            pady=(5, 10)
        )

        button_frame = tk.Frame(
            self.root,
            bg=BG_COLOR
        )
        button_frame.pack(
            fill="x",
            padx=20,
            pady=5
        )

        self.start_button = tk.Button(
            button_frame,
            text="START",
            command=self.start_sml,
            font=("Courier New", 13, "bold"),
            fg=TEXT_COLOR,
            bg=BUTTON_BG,
            activeforeground=TEXT_COLOR,
            activebackground=BUTTON_ACTIVE,
            relief="flat",
            bd=0,
            width=18,
            height=2,
            cursor="hand2"
        )
        self.start_button.pack(pady=5)

        self.stop_button = tk.Button(
            button_frame,
            text="STOP",
            command=self.stop_sml,
            font=("Courier New", 13, "bold"),
            fg=TEXT_COLOR,
            bg=BUTTON_BG,
            activeforeground=TEXT_COLOR,
            activebackground=BUTTON_ACTIVE,
            relief="flat",
            bd=0,
            width=18,
            height=2,
            cursor="hand2"
        )
        self.stop_button.pack(pady=5)

        self.exit_button = tk.Button(
            button_frame,
            text="SHUT DOWN",
            command=self.shutdown,
            font=("Courier New", 13, "bold"),
            fg=TEXT_COLOR,
            bg=BUTTON_BG,
            activeforeground=TEXT_COLOR,
            activebackground=BUTTON_ACTIVE,
            relief="flat",
            bd=0,
            width=18,
            height=2,
            cursor="hand2"
        )
        self.exit_button.pack(pady=5)

        self.status_label = tk.Label(
            self.root,
            text="STATUS: READY",
            font=("Courier New", 10, "bold"),
            fg=TEXT_COLOR,
            bg=BG_COLOR
        )
        self.status_label.pack(pady=(8, 2))

        bottom_frame = tk.Frame(
            self.root,
            bg=BG_COLOR
        )
        bottom_frame.pack(
            fill="both",
            expand=True,
            padx=15,
            pady=(5, 10)
        )

        self.gif_label = tk.Label(
            bottom_frame,
            text=" ",
            bg=BG_COLOR
        )
        self.gif_label.pack(
            side="right",
            anchor="se",
            padx=5,
            pady=5
        )

    def load_logo(self):
        logo_path = "logo.png"

        if not os.path.exists(logo_path):
            return

        try:
            image = Image.open(logo_path)
            image.thumbnail((130, 100))

            self.logo_image = ImageTk.PhotoImage(image)

            self.logo_label.configure(
                image=self.logo_image,
                text=" "
            )

        except Exception as error:
            print("Could not load logo:", error)

    def load_gif(self):
        # LP-SLM rotating atom logo
        gif_path = "LP-SLM_rotating_atom_logo.gif"

        if not os.path.exists(gif_path):
            print(f"Could not find GIF: {gif_path}")
            return

        try:
            gif = Image.open(gif_path)

            for frame in ImageSequence.Iterator(gif):
                frame = frame.convert("RGBA")

                frame.thumbnail((300, 250))

                self.gif_frames.append(
                    ImageTk.PhotoImage(frame)
                )

            if self.gif_frames:
                self.gif_running = True
                self.animate_gif()

        except Exception as error:
            print("Could not load GIF:", error)

    def animate_gif(self):
        if not self.gif_running or not self.gif_frames:
            return

        frame = self.gif_frames[self.gif_index]

        self.gif_label.configure(
            image=frame
        )

        self.gif_index = (
            self.gif_index + 1
        ) % len(self.gif_frames)

        self.root.after(
            70,
            self.animate_gif
        )

    def start_sml(self):
        self.status_label.configure(
            text="STATUS: SLM RUNNING"
        )

        slm_window = tk.Toplevel(self.root)

        slm_window.title(
            "LP-SLM - Small Learning Model"
        )

        slm_window.geometry("800x600")
        slm_window.resizable(True, True)
        slm_window.configure(bg=BG_COLOR)

        title = tk.Label(
            slm_window,
            text="Little Penguin Small Learning Model",
            font=("Courier New", 16, "bold"),
            fg=TEXT_COLOR,
            bg=BG_COLOR
        )
        title.pack(pady=15)

        slm_area = tk.Text(
            slm_window,
            bg="#050505",
            fg=TEXT_COLOR,
            insertbackground=TEXT_COLOR,
            font=("Courier New", 11),
            relief="flat",
            bd=0
        )
        slm_area.pack(
            fill="both",
            expand=True,
            padx=20,
            pady=10
        )

        try:
            with open(
                "learning.txt",
                "r",
                encoding="utf-8"
            ) as f:
                content = f.read()

            slm_area.insert(
                "1.0",
                content
            )

        except Exception as e:
            slm_area.insert(
                "1.0",
                f"Error loading learning.txt:\n{e}"
            )

        close_button = tk.Button(
            slm_window,
            text="CLOSE",
            command=slm_window.destroy,
            font=("Courier New", 11, "bold"),
            fg=TEXT_COLOR,
            bg=BUTTON_BG,
            activeforeground=TEXT_COLOR,
            activebackground=BUTTON_ACTIVE,
            relief="flat",
            width=15,
            cursor="hand2"
        )
        close_button.pack(pady=15)

    def stop_sml(self):
        self.status_label.configure(
            text="STATUS: STOP REQUESTED"
        )

        print("SML stop button pressed.")

    def shutdown(self):
        if messagebox.askyesno(
            "Shut Down",
            "Are you sure you want to shut down LP-SLM?"
        ):
            self.gif_running = False
            self.root.destroy()


if __name__ == "__main__":
    root = tk.Tk()
    app = LPSLMApp(root)
    root.mainloop()