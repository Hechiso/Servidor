// Obtener elementos del DOM
const playlist = document.getElementById('playlist');
const audioPlayer = document.getElementById('audioPlayer');
const imagenAlbum = document.getElementById('imagenAlbum');

// Definir contador y lógica para el cambio de imagen al finalizar una canción
let currentSong = 0;

playlist.addEventListener('click', function(event) {
  if (event.target.tagName === 'LI') {
    const songSrc = event.target.getAttribute('data-src');
    const albumImgSrc = event.target.getAttribute('data-img');
    audioPlayer.src = songSrc;
    imagenAlbum.src = albumImgSrc;
    audioPlayer.play();
  }
});

audioPlayer.addEventListener('ended', function() {
  currentSong++; // Aumentar contador después de que la canción termine
  const songs = playlist.getElementsByTagName('li');
  if (currentSong < songs.length) {
    const albumImgSrc = songs[currentSong].getAttribute('data-img');
    imagenAlbum.src = albumImgSrc;
    audioPlayer.src = songs[currentSong].getAttribute('data-src');
    audioPlayer.play();
  } else {
    // Resto del código para reiniciar la reproducción al final de la lista, si es necesario
    currentSong = 0; // Reiniciar al inicio de la lista
    const albumImgSrc = songs[currentSong].getAttribute('data-img');
    imagenAlbum.src = albumImgSrc;
    audioPlayer.src = songs[currentSong].getAttribute('data-src');
    audioPlayer.play();
  }
});

