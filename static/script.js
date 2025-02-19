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
// const sakuraContainer = document.getElementById('sakura-container');
//
// function createSakura() {
//     const sakura = document.createElement('div');
//     sakura.classList.add('sakura');
//     sakura.style.left = Math.random() * window.innerWidth + 'px';
//     sakura.style.animationDuration = Math.random() * 5 + 3 + 's'; // Случайная скорость падения
//     sakura.style.opacity = Math.random(); // Случайная прозрачность
//     sakuraContainer.appendChild(sakura);
//
//     setTimeout(() => {
//         sakura.remove();
//     }, 8000); // Удаляем лепесток после падения
// }
//
// setInterval(createSakura, 100);