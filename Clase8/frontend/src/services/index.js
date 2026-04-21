// Libreria axios
import axios from "axios";

// Configuracion de la instancia de axios en base a una variable API
const API = axios.create({
    baseURL: "http://localhost:8080",
    headers: {
        "Content-Type": "application/json",
    },
});

// Exportamos un objeto con los servicios que se van a consumir en el frontend, cada servicio es una funcion que hace una peticion a la API utilizando la instancia de axios configurada anteriormente
export const indexService = {
    analyze : (code) => API.post("/analyze", { source : code}),
}