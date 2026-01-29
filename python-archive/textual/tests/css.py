# app.py

from textual.app import App
from textual.widgets import Header, Footer, Input, Static

class QuickStart(App):
    CSS = """
    Input {
        border: tall green;
    }
    Static#output {
        color: magenta;
        padding: 1;
    }
    """

    def compose(self):
        yield Header()
        yield Input(placeholder="Type something...")
        yield Static(id="output")
        yield Footer()

    def on_input_submitted(self, message):
        output = self.query_one("#output", Static)
        output.update(f"You typed: {message.value}")

if __name__ == "__main__":
    QuickStart().run()
