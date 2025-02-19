// document.addEventListener('DOMContentLoaded', function () {
//     const sliders = document.querySelectorAll('input[type="range"]');
//
//     sliders.forEach(slider => {
//         const sliderValue = document.getElementById(slider.id + '_val');
//         slider.addEventListener('input', function () {
//             const value = slider.value;
//             sliderValue.textContent = value;
//
//             // Отправляем данные на сервер
//             fetch('/update_slider', {
//                 method: 'POST',
//                 headers: {
//                     'Content-Type': 'application/json'
//                 },
//                 body: JSON.stringify({ slider_name: slider.id, slider_value: value })
//             })
//             .then(response => {
//                 if (!response.ok) {
//                     throw new Error('Network response was not ok');
//                 }
//                 return response.json();
//             })
//             .then(data => {
//                 serverResponse.textContent = data.updated_value;
//             })
//             .catch(error => {
//                 console.error('There was a problem with the fetch operation:', error);
//             });
//         });
//     });
// });
