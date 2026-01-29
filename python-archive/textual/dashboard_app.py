# dashboard_app.py

from textual.app import App
from textual.containers import Grid
from textual.widgets import Static

from system_info import SystemInfo
from weather_info import WeatherInfo
from tasks_info import TasksInfo
from stocks_info import StocksInfo


class DashboardApp(App):
    """A super simple dashboard application with 4 quadrants for system info, weather, tasks, and stocks."""

    CSS = """
    Screen {
        layout: grid;
        grid-size: 2 2;
        grid-gutter: 1;
    }

    Static {
        border: solid white;
        padding: 1;
    }
    """

    def compose(self):
        # Create a grid layout with 2 rows and 2 columns
        grid = Grid()
        grid.add_column("col1")
        grid.add_column("col2")
        grid.add_row("row1")
        grid.add_row("row2")

        # Place each info widget in its respective cell in the grid
        grid.place(
            SystemInfo(id="system_info"),
            WeatherInfo(id="weather_info"),
            TasksInfo(id="tasks_info"),
            StocksInfo(id="stocks_info")
        )

        yield grid


if __name__ == "__main__":
    app = DashboardApp()
    app.run()
