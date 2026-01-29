# app.py

from textual.app import App
from textual.widgets import Header, Footer, Static

class QuickStart(App):
    def compose(self):
        yield Header()
        yield Static("Press 'q' to quit.", id="message")
        yield Footer()

    def on_key(self, event):
        if event.key == "q":
            self.exit()

if __name__ == "__main__":
    QuickStart().run()
