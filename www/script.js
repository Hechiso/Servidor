/*
let contador = localStorage.getItem('contador') || 0;

document.getElementById('contador').innerText = contador;

function aumentarContador() {
    if (!localStorage.getItem('contadorAumentado')) {
        contador++;
        document.getElementById('contador').innerText = contador;
        localStorage.setItem('contador', contador);
        localStorage.setItem('contadorAumentado', true);
    } else {
	contador--;
	/*document.getElementById('contador').innerText = contador;
	localStorage.setItem('contador', contador);
	localStorage.setItem('contadorAumentado',true);    
       /* alert('Ya has aumentado el contador.');*/
/*	 
    }
}
*/

const Boton = document.querySelector(".boton");
let Icon = document.querySelector("#icono");
let cont = document.querySelector("#contador");

let clickar = false;

Boton.addEventListener("click", () => {
	if(!clickar){
	clickar = true;
	Icon.innerHTML =`<i class="fas fa-heart"></i> Me Gusta `
	cont.textContent++;	
	}else{
	clickar = false;
	Icon.innerHTML =`<i class="fas fa-heart"></i> Me Gusta `
	cont.textContent--;	
	}
});

