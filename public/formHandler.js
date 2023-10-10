const form = document.getElementById('cone_form');

function encodeData(formElement) {
    const formData = new FormData(form);
    const data = {};
    for (let [name, value] of formData) {
        data[name] = value;
    }
    return (Object.keys(data)
        .map(key => `${encodeURIComponent(key)}=${encodeURIComponent(data[key])}`)
        .join('&'));
}

function calcCoordinates(event) {
    event.preventDefault();

    return fetch('/generate_cone', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded;charset=UTF-8'
        },
        body: encodeData(form)
    }).then(function(response) {
        return response.json();
    }).then(function(data) {
        setMesh(data);
        return data;
    });
}

form.addEventListener('submit', calcCoordinates);