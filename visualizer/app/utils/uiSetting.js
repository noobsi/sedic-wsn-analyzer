// Prevent space scroll
window.addEventListener('keydown', function (e) {
  if (e.keyCode === 32) {
    e.preventDefault();
  }
});

$(function () {
  $('[data-toggle="tooltip"]').tooltip()
});
