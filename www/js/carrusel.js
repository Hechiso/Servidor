/* script para el carrusel */

const imagenes = document.querySelectorAll('section div.contenedor img');
let indiceImagen = 0;

function mostrarSiguienteImagen(){
   imagenes.forEach(img => {
      img.style.opacity = 0;
   });
   
   indiceImagen = (indiceImagen + 1) % imagenes.length;
   imagenes[indiceImagen].style.opacity = 1;
}

setInterval(mostrarSiguienteImagen, 10000);
