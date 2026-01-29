import cv2
import os
import tkinter as tk
from datetime import datetime

class CameraApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Camera App")
        self.root.bind('<Escape>', self.clean_exit)

        self.label = tk.Label(root, text="Choose an option:")
        self.label.pack(pady=10)

        self.photo_button = tk.Button(root, text="Take a Picture", command=self.take_picture)
        self.photo_button.pack(pady=10)

        self.video_button = tk.Button(root, text="Record a Video", command=self.record_video)
        self.video_button.pack(pady=10)

    def take_picture(self):
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            print("Error: Could not open camera.")
            return

        ret, frame = cap.read()
        if ret:
            if not os.path.exists(os.path.expanduser("~/Pictures")):
                os.makedirs(os.path.expanduser("~/Pictures"))
            filename = os.path.expanduser(f"~/Pictures/photo_{datetime.now().strftime('%Y%m%d_%H%M%S')}.png")
            cv2.imwrite(filename, frame)
            print(f"Picture saved as {filename}")

        cap.release()

    def record_video(self):
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            print("Error: Could not open camera.")
            return

        fourcc = cv2.VideoWriter_fourcc(*'XVID')
        if not os.path.exists(os.path.expanduser("~/Videos")):
            os.makedirs(os.path.expanduser("~/Videos"))
        filename = os.path.expanduser(f"~/Videos/video_{datetime.now().strftime('%Y%m%d_%H%M%S')}.avi")
        out = cv2.VideoWriter(filename, fourcc, 20.0, (640, 480))

        print("Press 'q' to stop recording.")
        while cap.isOpened():
            ret, frame = cap.read()
            if ret:
                out.write(frame)
                cv2.imshow('Recording', frame)

                if cv2.waitKey(1) & 0xFF == ord('q'):
                    break
            else:
                break

        cap.release()
        out.release()
        cv2.destroyAllWindows()
        print(f"Video saved as {filename}")

    def clean_exit(self, event=None):
        print("Exiting application...")
        self.root.quit()

if __name__ == "__main__":
    root = tk.Tk()
    app = CameraApp(root)
    root.mainloop()
