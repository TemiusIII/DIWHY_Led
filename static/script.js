// Sakura Leaves Animation
const sakuraContainer = document.getElementById('sakura-container');

function createSakura() {
    if (sakuraContainer.children.length >= 10) return; // Limit to 10 sakura leaves
    const sakura = document.createElement('div');
    sakura.classList.add('sakura');
    const startX = Math.random() * window.innerWidth;
    const duration = Math.random() * 3 + 3;
    const size = Math.random() * 15 + 10;
    const rotate = Math.random() * 360;

    sakura.style.left = `${startX}px`;
    sakura.style.width = `${size}px`;
    sakura.style.height = `${size}px`;
    sakura.style.opacity = 1;
    const viewportHeight = window.innerHeight;
    sakura.style.setProperty('--fall-distance', `${viewportHeight}px`);
    sakura.style.animation = `fall ${duration}s linear forwards`;
    sakura.style.transform = `rotate(${rotate}deg)`;

    sakuraContainer.appendChild(sakura);
    setTimeout(() => sakura.remove(), duration * 1000);
}

setInterval(createSakura, 700);

// Anime Woman Animation Toggle
const animeWoman = document.getElementById('anime-woman');
const toggleButton = document.getElementById('toggle-animation');
let isAnimationEnabled = true;

if (toggleButton) { // Check if toggleButton exists
    toggleButton.addEventListener('click', () => {
        if (isAnimationEnabled) {
            animeWoman.style.animation = 'none';
            toggleButton.textContent = 'Enable Animation';
        } else {
            animeWoman.style.animation = 'jump 2s ease-in-out infinite';
            toggleButton.textContent = 'Disable Animation';
        }
        isAnimationEnabled = !isAnimationEnabled;
    });
} else {
    console.error('Toggle button not found!');
}

// Original Functionality
const currentModeEl = document.getElementById('current-mode');
const colorPickerEl = document.getElementById('colorPicker');
const speedInputEl = document.getElementById('speedInput');
const hueGapEl = document.getElementById('hue_gap');
const hueRangeEl = document.getElementById('hue_range');
const rippleStepEl = document.getElementById('ripple_step');
const hueGapValEl = document.getElementById('hue_gap_val');
const hueRangeValEl = document.getElementById('hue_range_val');
const rippleStepValEl = document.getElementById('ripple_step_val');

if (hueGapEl) hueGapEl.oninput = () => hueGapValEl.textContent = hueGapEl.value;
if (hueRangeEl) hueRangeEl.oninput = () => hueRangeValEl.textContent = hueRangeEl.value;
if (rippleStepEl) rippleStepEl.oninput = () => rippleStepValEl.textContent = rippleStepEl.value;

function showSection(sectionId) {
    document.querySelectorAll('.section').forEach(sec => sec.classList.remove('active'));
    document.getElementById(sectionId).classList.add('active');
    document.querySelectorAll('.tab-btn').forEach(btn => btn.classList.remove('active'));
    event.target.classList.add('active');
}

function switchMode(newMode) {
    fetch(`/set_mode/${newMode}`, { method: 'POST' })
        .then(response => response.json())
        .then(data => {
            currentModeEl.textContent = data.led_mode;
        });
}

function applyColor() {
    const color = colorPickerEl.value;
    fetch('/set_color', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ color })
    })
    .then(res => res.json())
    .then(data => {
        alert('Цвет обновлён: ' + data.custom_color);
    });
}

function applySpeed() {
    const newSpeed = parseInt(speedInputEl.value);
    fetch('/set_speed', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ speed: newSpeed })
    })
    .then(res => res.json())
    .then(data => {
        alert('Скорость обновлена: ' + data.speed);
    });
}

function applyRipple() {
    const hueGapVal = parseInt(hueGapEl.value);
    const hueRangeVal = parseInt(hueRangeEl.value);
    const rippleStepVal = parseInt(rippleStepEl.value);

    updateSlider('hue_gap', hueGapVal);
    updateSlider('hue_range', hueRangeVal);
    updateSlider('ripple_step', rippleStepVal);
}

function updateSlider(sliderName, sliderValue) {
    fetch('/update_slider', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            slider_name: sliderName,
            slider_value: sliderValue
        })
    })
    .then(res => res.json())
    .then(data => {
        console.log('Slider updated:', data);
    });
}

function fetchHSV() {
    fetch('/color_hsv')
        .then(res => res.json())
        .then(data => {
            document.getElementById('hsv-value').textContent =
                `H: ${data.h}, S: ${data.s}, V: ${data.v}`;
        });
}