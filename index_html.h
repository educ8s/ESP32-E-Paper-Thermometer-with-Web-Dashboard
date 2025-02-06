const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Home Environment Dashboard</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f9;
            color: #333;
            text-align: center;
            margin: 0;
            padding: 0;
        }

        h1 {
            margin-top: 20px;
            font-size: 2rem;
        }

        .current-container {
            display: flex;
            flex-wrap: wrap;
            justify-content: center;
            margin: 20px 0;
        }

        .current-card {
            background: #ffffff;
            border-radius: 8px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
            padding: 20px;
            margin: 10px;
            width: 200px;
            text-align: center;
        }

        .current-value {
            font-size: 2rem;
            color: #007BFF;
        }

        .current-label {
            font-size: 1rem;
            color: #555;
        }

        .container {
            width: 90%;
            max-width: 1200px;
            margin: 0 auto;
            display: flex;
            flex-wrap: wrap;
            justify-content: space-around;
        }

        .chart-container {
            width: 100%;
            max-width: 400px;
            margin: 20px 0;
            background: #ffffff;
            border-radius: 8px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
            padding: 20px;
        }

        canvas {
            width: 100%;
            height: auto;
        }

        footer {
            margin-top: 20px;
            font-size: 0.8rem;
            color: #888;
        }
    </style>
</head>
<body>
    <h1>Home Environment Dashboard</h1>

    <!-- Current Values Section -->
    <div class="current-container">
        <div class="current-card">
            <div class="current-value" id="current-temperature">--</div>
            <div class="current-label" id="temperature-label">Temperature</div>
        </div>
        <div class="current-card">
            <div class="current-value" id="current-humidity">-- %</div>
            <div class="current-label">Humidity</div>
        </div>
        <div class="current-card">
            <div class="current-value" id="current-pressure">-- hPa</div>
            <div class="current-label">Pressure</div>
        </div>
    </div>

    <!-- Graphs Section -->
    <div class="container">
        <div class="chart-container">
            <h2 id="temp-chart-label">Temperature</h2>
            <canvas id="temperatureChart"></canvas>
        </div>
        <div class="chart-container">
            <h2>Humidity (%)</h2>
            <canvas id="humidityChart"></canvas>
        </div>
        <div class="chart-container">
            <h2>Pressure (hPa)</h2>
            <canvas id="pressureChart"></canvas>
        </div>
    </div>

    <footer>Data for the last 48 hours (one value per hour).</footer>

    <script>
        let isMetric = true; // Default, will be updated from /sensor

        // Labels for the last 48 hours
        const labels = Array.from({ length: 48 }, (_, i) => `${48 - i}h ago`);
        let temperatureData = Array(48).fill(null);
        let humidityData = Array(48).fill(null);
        let pressureData = Array(48).fill(null);

        // Fetch current sensor data
        async function fetchSensorData() {
            try {
                const response = await fetch('/sensor');
                if (!response.ok) {
                    throw new Error('Failed to fetch sensor data');
                }
                const data = await response.json();

                // Update the unit (C or F)
                isMetric = data.metric;
                let tempUnit = isMetric ? "°C" : "°F";

                // Update current values
                document.getElementById('current-temperature').textContent = `${data.temperature.toFixed(2)} ${tempUnit}`;
                document.getElementById('current-humidity').textContent = `${data.humidity.toFixed(2)} %`;
                document.getElementById('current-pressure').textContent = `${data.pressure.toFixed(2)} hPa`;

                // Update labels dynamically
                document.getElementById("temp-chart-label").textContent = `Temperature (${tempUnit})`;

            } catch (error) {
                console.error('Error fetching sensor data:', error);
            }
        }

        // Fetch historical data from /history endpoint
        async function fetchHistoricalData() {
            try {
                const response = await fetch('/history');
                if (!response.ok) {
                    throw new Error('Failed to fetch historical data');
                }
                const data = await response.json();

                // Populate graphs with historical data
                temperatureData = data.temperature;
                humidityData = data.humidity;
                pressureData = data.pressure;

                initializeCharts();
            } catch (error) {
                console.error('Error fetching historical data:', error);
            }
        }

        // Initialize charts
        let temperatureChart, humidityChart, pressureChart;
        function initializeCharts() {
            const chartOptions = {
                type: 'line',
                options: {
                    responsive: true,
                    scales: {
                        x: {
                            title: { display: true, text: 'Time' }
                        },
                        y: {
                            title: { display: true, text: 'Value' }
                        }
                    }
                }
            };

            temperatureChart = new Chart(document.getElementById('temperatureChart'), {
                ...chartOptions,
                data: {
                    labels,
                    datasets: [{
                        label: `Temperature (${isMetric ? "°C" : "°F"})`,
                        data: temperatureData,
                        borderColor: 'rgb(255, 99, 132)',
                        backgroundColor: 'rgba(255, 99, 132, 0.2)',
                        fill: true
                    }]
                }
            });

            humidityChart = new Chart(document.getElementById('humidityChart'), {
                ...chartOptions,
                data: {
                    labels,
                    datasets: [{
                        label: 'Humidity (%)',
                        data: humidityData,
                        borderColor: 'rgb(54, 162, 235)',
                        backgroundColor: 'rgba(54, 162, 235, 0.2)',
                        fill: true
                    }]
                }
            });

            pressureChart = new Chart(document.getElementById('pressureChart'), {
                ...chartOptions,
                data: {
                    labels,
                    datasets: [{
                        label: 'Pressure (hPa)',
                        data: pressureData,
                        borderColor: 'rgb(75, 192, 192)',
                        backgroundColor: 'rgba(75, 192, 192, 0.2)',
                        fill: true
                    }]
                }
            });
        }

        // Fetch data and update UI
        fetchHistoricalData();
        fetchSensorData();
        setInterval(fetchSensorData, 5000); // Refresh data every 5 seconds
    </script>
</body>
</html>)rawliteral";
