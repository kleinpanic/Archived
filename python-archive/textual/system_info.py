# system_info.py

from textual.widget import Widget
from textual.reactive import Reactive
import psutil
import time
import os


class SystemInfo(Widget):
    """Widget to display system information."""

    data: Reactive[str] = Reactive("Loading system info...")

    def on_mount(self) -> None:
        self.set_interval(1, self.refresh_data)

    def render(self) -> str:
        return self.data

    def refresh_data(self) -> None:
        cpu_usage = psutil.cpu_percent()
        load_avg = psutil.getloadavg()[0]
        memory = psutil.virtual_memory()
        disk = psutil.disk_usage('/')
        uptime_seconds = time.time() - psutil.boot_time()
        uptime_string = time.strftime('%H:%M:%S', time.gmtime(uptime_seconds))
        packages = self.get_package_count()
        shell = self.get_shell()

        self.data = f"""
CPU Usage: {cpu_usage}%
Load Average: {load_avg}
Memory Usage: {memory.percent}%
Disk Usage: {disk.percent}%
Packages Installed: {packages}
Shell: {shell}
Uptime: {uptime_string}
        """

    def get_package_count(self):
        # Example for Debian-based systems
        try:
            import subprocess
            result = subprocess.run(['dpkg', '--list'], stdout=subprocess.PIPE)
            output = result.stdout.decode()
            package_count = len([line for line in output.split('\n') if line.startswith('ii')])
            return package_count
        except:
            return "N/A"

    def get_shell(self):
        return os.environ.get('SHELL', 'N/A')
