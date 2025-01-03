/*
document.addEventListener('DOMContentLoaded', function() {
  var searchBox = document.getElementById('searchBox');
  var playlist = document.getElementById('playlist').getElementsByTagName('li');
  var playButton = document.getElementById('playButton');
  var currentAudio = null; // Mantener una referencia al audio actualmente reproduciéndose

  playButton.addEventListener('click', function() {
    var searchTerm = searchBox.value.toLowerCase();

    Array.from(playlist).forEach(function(song) {
      var songName = song.textContent.toLowerCase();
      var songSource = song.getAttribute('data-src').toLowerCase();

      if (songName.includes(searchTerm) || songSource.includes(searchTerm)) {
        if (currentAudio) {
          currentAudio.pause(); // Detener la pista actual si hay alguna reproduciéndose
        }
        
        var audio = new Audio(song.getAttribute('data-src'));
        audio.play(); // Reproducir la nueva pista
        currentAudio = audio; // Establecer la nueva pista como la pista actual
      }
    });
  });
});
*/

/*
document.addEventListener('DOMContentLoaded', function() {
  var searchBox = document.getElementById('searchBox');
  var playlist = document.getElementById('playlist').getElementsByTagName('li');
  var playButton = document.getElementById('playButton');
  var stopButton = document.getElementById('stopButton');
  var currentAudio = null;

  playButton.addEventListener('click', function() {
    var searchTerm = searchBox.value.toLowerCase();

    Array.from(playlist).forEach(function(song) {
      var songName = song.textContent.toLowerCase();
      var songSource = song.getAttribute('data-src').toLowerCase();

      if (songName.includes(searchTerm) || songSource.includes(searchTerm)) {
        if (currentAudio) {
          currentAudio.pause();
        }

        var audio = new Audio(song.getAttribute('data-src'));
        audio.play();
        currentAudio = audio;
      }
    });
  });

  stopButton.addEventListener('click', function() {
    if (currentAudio) {
      currentAudio.pause(); // Detener la pista actual si existe alguna reproduciéndose
    }
  });
});
*/

/*
document.addEventListener('DOMContentLoaded', function() {
  var searchBox = document.getElementById('searchBox');
  var playlist = document.getElementById('playlist').getElementsByTagName('li');
  var playButton = document.getElementById('playButton');
  var stopButton = document.getElementById('stopButton');
  var nextButton = document.getElementById('nextButton');
  var currentAudio = null;
  var currentSongIndex = 0;

  playButton.addEventListener('click', function() {
    var searchTerm = searchBox.value.toLowerCase();

    Array.from(playlist).forEach(function(song, index) {
      var songName = song.textContent.toLowerCase();
      var songSource = song.getAttribute('data-src').toLowerCase();

      if (songName.includes(searchTerm) || songSource.includes(searchTerm)) {
        if (currentAudio) {
          currentAudio.pause();
        }

        var audio = new Audio(song.getAttribute('data-src'));
        audio.play();
        currentAudio = audio;
        currentSongIndex = index;
      }
    });
  });

  stopButton.addEventListener('click', function() {
    if (currentAudio) {
      currentAudio.pause();
    }
  });

  nextButton.addEventListener('click', function() {
    var nextIndex = (currentSongIndex + 1) % playlist.length; // Obtener el índice de la siguiente canción
    var nextSong = playlist[nextIndex];

    if (currentAudio) {
      currentAudio.pause();
    }

    var audio = new Audio(nextSong.getAttribute('data-src'));
    audio.play();
    currentAudio = audio;
    currentSongIndex = nextIndex;
  });
});
*/


/*
document.addEventListener('DOMContentLoaded', function() {
  var searchBox = document.getElementById('searchBox');
  var playlist = document.getElementById('playlist').getElementsByTagName('li');
  var playButton = document.getElementById('playButton');
  var stopButton = document.getElementById('stopButton');
  var nextButton = document.getElementById('nextButton');
  var prevButton = document.getElementById('prevButton');
  var currentAudio = null;
  var currentSongIndex = 0;

  playButton.addEventListener('click', function() {
    var searchTerm = searchBox.value.toLowerCase();

    Array.from(playlist).forEach(function(song, index) {
      var songName = song.textContent.toLowerCase();
      var songSource = song.getAttribute('data-src').toLowerCase();

      if (songName.includes(searchTerm) || songSource.includes(searchTerm)) {
        if (currentAudio) {
          currentAudio.pause();
        }

        var audio = new Audio(song.getAttribute('data-src'));
        audio.play();
        currentAudio = audio;
        currentSongIndex = index;
      }
    });
  });

  stopButton.addEventListener('click', function() {
    if (currentAudio) {
      currentAudio.pause();
    }
  });

  nextButton.addEventListener('click', function() {
    var nextIndex = (currentSongIndex + 1) % playlist.length; // Obtener el índice de la siguiente canción
    var nextSong = playlist[nextIndex];

    if (currentAudio) {
      currentAudio.pause();
    }

    var audio = new Audio(nextSong.getAttribute('data-src'));
    audio.play();
    currentAudio = audio;
    currentSongIndex = nextIndex;
  });

  prevButton.addEventListener('click', function() {
    var prevIndex = (currentSongIndex - 1 + playlist.length) % playlist.length; // Obtener el índice de la canción anterior
    var prevSong = playlist[prevIndex];

    if (currentAudio) {
      currentAudio.pause();
    }

    var audio = new Audio(prevSong.getAttribute('data-src'));
    audio.play();
    currentAudio = audio;
    currentSongIndex = prevIndex;
  });
});
*/


/*
document.addEventListener('DOMContentLoaded', function() {
  var searchBox = document.getElementById('searchBox');
  var playlist = document.getElementById('playlist').getElementsByTagName('li');
  var playButton = document.getElementById('playButton');
  var stopButton = document.getElementById('stopButton');
  var nextButton = document.getElementById('nextButton');
  var prevButton = document.getElementById('prevButton');
  var currentAudio = null;
  var currentSongIndex = 0;

  function playAudio(src) {
    if (currentAudio && !currentAudio.paused && src === currentAudio.src) {
      currentAudio.pause();
    } else {
      if (currentAudio) {
        currentAudio.pause();
      }

      var audio = new Audio(src);
      audio.play();
      currentAudio = audio;
    }
  }

  playButton.addEventListener('click', function() {
    var searchTerm = searchBox.value.toLowerCase();

    Array.from(playlist).forEach(function(song, index) {
      var songName = song.textContent.toLowerCase();
      var songSource = song.getAttribute('data-src').toLowerCase();

      if (songName.includes(searchTerm) || songSource.includes(searchTerm)) {
        playAudio(song.getAttribute('data-src'));
        currentSongIndex = index;
      }
    });
  });

  stopButton.addEventListener('click', function() {
    if (currentAudio) {
      currentAudio.pause();
      currentAudio.currentTime = 0;
    }
  });

  nextButton.addEventListener('click', function() {
    var nextIndex = (currentSongIndex + 1) % playlist.length;
    var nextSong = playlist[nextIndex];

    playAudio(nextSong.getAttribute('data-src'));
    currentSongIndex = nextIndex;
  });

  prevButton.addEventListener('click', function() {
    var prevIndex = (currentSongIndex - 1 + playlist.length) % playlist.length;
    var prevSong = playlist[prevIndex];

    playAudio(prevSong.getAttribute('data-src'));
    currentSongIndex = prevIndex;
  });
});
*/


document.addEventListener('DOMContentLoaded', function() {
  var searchBox = document.getElementById('searchBox');
  var playlist = document.getElementById('playlist').getElementsByTagName('li');
  var playPauseButton = document.getElementById('playPauseButton');
  var nextButton = document.getElementById('nextButton');
  var prevButton = document.getElementById('prevButton');
  var currentAudio = null;
  var currentSongIndex = 0;

  function playAudio(src) {
    if (currentAudio && !currentAudio.paused && src === currentAudio.src) {
      currentAudio.pause();
    } else {
      if (currentAudio) {
        currentAudio.pause();
      }

      var audio = new Audio(src);
      audio.play();
      currentAudio = audio;
      playPauseButton.src = 'Icono/pause.png'; // Cambia el ícono a pausa al reproducir una pista nueva
    }
  }

  function pauseAudio() {
    if (currentAudio) {
      currentAudio.pause();
      playPauseButton.src = 'Icono/play.png'; // Cambia el ícono a reproducción al pausar la pista
    }
  }

  playPauseButton.addEventListener('click', function() {
    if (currentAudio && !currentAudio.paused) {
      pauseAudio();
    } else if (currentAudio && currentAudio.paused) {
      currentAudio.play();
      playPauseButton.src = 'Icono/pause.png'; // Cambia el ícono a pausa al reanudar la pista
    }
  });

  searchBox.addEventListener('input', function() {
    var searchTerm = searchBox.value.toLowerCase();

    Array.from(playlist).forEach(function(song, index) {
      var songName = song.textContent.toLowerCase();
      var songSource = song.getAttribute('data-src').toLowerCase();

      if (songName.includes(searchTerm) || songSource.includes(searchTerm)) {
        playAudio(song.getAttribute('data-src'));
        currentSongIndex = index;
      }
    });
  });

  nextButton.addEventListener('click', function() {
    var nextIndex = (currentSongIndex + 1) % playlist.length;
    var nextSong = playlist[nextIndex];

    playAudio(nextSong.getAttribute('data-src'));
    currentSongIndex = nextIndex;
  });

  prevButton.addEventListener('click', function() {
    var prevIndex = (currentSongIndex - 1 + playlist.length) % playlist.length;
    var prevSong = playlist[prevIndex];

    playAudio(prevSong.getAttribute('data-src'));
    currentSongIndex = prevIndex;
  });
});



