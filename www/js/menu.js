// --------------------  menu Celular  -----------------------//
/*
document.addEventListener('DOMContentLoaded', function() {
  var menu = document.getElementById('MenuCelular');
  var listaReproduccion = document.getElementById('listaReproduccion');

//-------------------------------------------------------------


//------------------------------------------------------------
  menu.addEventListener('click', function(){
    // Crear una lista desordenada (ul)
    var ul = document.createElement('ul');

    // Agregar elementos de lista (li)
    var canciones = ['ListaPP','Anime','Peliculas']; // Aquí puedes poner los nombres de las canciones
    canciones.forEach(function(cancion) {
      var li = document.createElement('li');
      li.textContent = cancion;
      ul.appendChild(li);
    }); //foreach

    // Agregar la lista al contenedor en el HTML
    listaReproduccion.appendChild(ul);
   
  });//eventlistener



});//fin document
*/

/*
document.addEventListener('DOMContentLoaded', function() {
  var menu = document.getElementById('MenuCelular');
  var listaReproduccion = document.getElementById('listaReproduccion');

  menu.addEventListener('click', function(){
    // Crear una lista desordenada (ul)
    var ul = document.createElement('ul');

    // Agregar elementos de lista (li) con enlaces (a)
    var canciones = [
      { nombre: 'ListaPP', enlace: 'pagina1.html' },
      { nombre: 'Anime', enlace: 'anime.html' },
      { nombre: 'Peliculas', enlace: 'Pelis.html' }
    ]; // Aquí puedes poner los nombres de las canciones y sus enlaces

    canciones.forEach(function(cancion) {
      var li = document.createElement('li');
      var a = document.createElement('a');
      a.textContent = cancion.nombre;
      a.href = cancion.enlace;
      li.appendChild(a);
      ul.appendChild(li);
    });

    // Agregar la lista al contenedor en el HTML
    listaReproduccion.appendChild(ul);
  });
});

*/

document.addEventListener('DOMContentLoaded', function() {
  var menu = document.getElementById('MenuCelular');
  var listaReproduccion = document.getElementById('listaReproduccion');
  var listaVisible = false; // Variable para rastrear si la lista está visible o no

  menu.addEventListener('click', function(){
    // Si la lista ya está visible, la eliminamos
    if (listaVisible) {
      listaReproduccion.innerHTML = ''; // Elimina el contenido de la lista
      listaVisible = false; // Actualiza el estado de visibilidad
      return; // Sale de la función
    }

    // Si la lista no está visible, la creamos
    var ul = document.createElement('ul');

    var canciones = [
      { nombre: 'ListaPP', enlace: 'pagina1.html' },
      { nombre: 'Anime', enlace: 'anime.html' },
      { nombre: 'Peliculas', enlace: 'Pelis.html' }
    ];

    canciones.forEach(function(cancion) {
      var li = document.createElement('li');
      var a = document.createElement('a');
      a.textContent = cancion.nombre;
      a.href = cancion.enlace;
      li.appendChild(a);
      ul.appendChild(li);
    });

    // Agregar la lista al contenedor en el HTML
    listaReproduccion.appendChild(ul);

    listaVisible = true; // Actualiza el estado de visibilidad
  });
});



