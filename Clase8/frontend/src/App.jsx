import { useState } from "react";
import { indexService } from "./services";

function App(){
  const [code, setCode] = useState("{\n  \"name\": \"John\",\n  \"age\": 30,\n  \"city\": \"New York\"\n}");

  return(
    <>
      <div className="flex flex-col justify-between items-center h-screen p-4">
        <header className="py-4 px-2 bg-slate-900 text-white">
          <h1 className="text-2xl">Analizador Lexico | Sintáctico</h1>
        </header>
        <section>
               <textarea name="code" id="code" value={code} onChange={(e) => setCode(e.target.value)}
              >
              </textarea>
        </section>
        <footer className="bg-slate-900  text-white p-2">
             <p>
              Todos los derechos reservados &copy; 2026 | Hecho por: River
             </p>
        </footer>
      </div>

    </>
  )
}

export default App;