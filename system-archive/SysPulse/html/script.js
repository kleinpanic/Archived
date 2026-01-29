function fetchData(endpoint, elementId, formatFn) {
    fetch(endpoint)
        .then(response => response.json())
        .then(data => {
            document.getElementById(elementId).innerText = formatFn(data);
        })
        .catch(err => {
            document.getElementById(elementId).innerText = 'Error';
        });
}

function updateStats() {
    fetchData('/api/uptime', 'uptime', data =>
        data.uptime ? data.uptime + " seconds" : "N/A"
    );
    fetchData('/api/load', 'load', data =>
        data.load1 && data.load5 && data.load15 ? 
        data.load1 + ", " + data.load5 + ", " + data.load15 : "N/A"
    );
    fetchData('/api/mem', 'mem', data =>
        data.mem_total && data.mem_free ? 
        "Total: " + data.mem_total + " kB, Free: " + data.mem_free + " kB" : "N/A"
    );
    fetchData('/api/cpu', 'cpu', data =>
        data.cpu_stats ? data.cpu_stats : "N/A"
    );
    fetchData('/api/disk', 'disk', data =>
        data.disk_total && data.disk_free ? 
        "Total: " + data.disk_total + " kB, Free: " + data.disk_free + " kB" : "N/A"
    );
}

setInterval(updateStats, 5000);
updateStats();

