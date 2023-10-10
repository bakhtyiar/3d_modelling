const express = require('express');
const bodyParser = require('body-parser');
const path = require('path');
const addon = require('./build/Release/addon');

const app = express();
const port = process.env.PORT || 3000;

app.use(express.static(path.join(__dirname, 'public')));
app.use(bodyParser.urlencoded({ extended: true }));

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

app.post('/generate_cone', (req, res) => {
    const height = parseFloat(req.body.height);
    const radius = parseFloat(req.body.radius);
    const segments = parseFloat(req.body.segments);

    if (isNaN(height) || isNaN(radius) || isNaN(segments)) {
        return res.status(400).json({ error: 'Invalid input. All values must be numbers.' });
    }

    const coords = addon.generateConeData(height, radius, segments);
    const data = { ...coords };
    res.json(data);
});

app.listen(port, () => {
    console.log(`Server started at port ${port}`);
});