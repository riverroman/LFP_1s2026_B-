import { useState, useRef } from "react";
import { indexService } from "./services";

// ─── Helpers ────────────────────────────────────────────────────
const TIPO_STYLES = {
  Lexico:     "bg-orange-100 text-orange-700",
  Sintactico: "bg-red-100 text-red-700",
};

const EJEMPLO = `TABLERO "Proyecto LFP" {
    COLUMNA "Por Hacer" {
        tarea: "Diseñar AFD" [
            prioridad: ALTA,
            responsable: "Jorge",
            fecha_limite: 2026-05-01
        ],
        tarea: "Implementar Lexer" [
            prioridad: ALTA,
            responsable: "Maria",
            fecha_limite: 2026-05-08
        ],
    };
};`;

// ─── Sub-componentes ─────────────────────────────────────────────

function StatusBanner({ resultado }) {
  if (!resultado) return null;
  return resultado.exitoso ? (
    <div className="flex items-center gap-2 px-4 py-2 rounded-lg bg-green-50 border border-green-200 text-green-700 text-sm font-medium">
      <span>✓</span>
      Análisis exitoso · {resultado.tokens?.length ?? 0} tokens encontrados
    </div>
  ) : (
    <div className="flex items-center gap-2 px-4 py-2 rounded-lg bg-red-50 border border-red-200 text-red-700 text-sm font-medium">
      <span>✗</span>
      {resultado.errores?.length ?? 0} error(es) detectado(s)
    </div>
  );
}

function TablaTokens({ tokens }) {
  if (!tokens?.length)
    return <p className="text-slate-400 italic text-sm">Sin tokens aún.</p>;

  return (
    <div className="overflow-auto max-h-96 rounded-lg border border-slate-200">
      <table className="w-full text-sm border-collapse">
        <thead className="sticky top-0 z-10">
          <tr className="bg-slate-800 text-slate-100">
            {["#", "Lexema", "Tipo", "Línea", "Col"].map((h) => (
              <th key={h} className="px-3 py-2 text-left font-medium text-xs tracking-wide">
                {h}
              </th>
            ))}
          </tr>
        </thead>
        <tbody>
          {tokens.map((t, i) => (
            <tr
              key={i}
              className={`border-b border-slate-100 hover:bg-blue-50 transition-colors ${
                i % 2 === 0 ? "bg-white" : "bg-slate-50"
              }`}
            >
              <td className="px-3 py-2 text-slate-400 text-xs">{t.numero}</td>
              <td className="px-3 py-2 font-mono text-blue-700 font-medium">{t.lexema}</td>
              <td className="px-3 py-2">
                <span className="font-mono text-xs bg-violet-100 text-violet-700 px-2 py-0.5 rounded">
                  {t.tipo}
                </span>
              </td>
              <td className="px-3 py-2 text-slate-500 tabular-nums">{t.linea}</td>
              <td className="px-3 py-2 text-slate-500 tabular-nums">{t.columna}</td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}

function TablaErrores({ errores }) {
  if (!errores?.length)
    return (
      <div className="flex flex-col items-center justify-center py-12 text-green-600 gap-2">
        <span className="text-4xl">✓</span>
        <p className="font-medium">Sin errores detectados</p>
      </div>
    );

  return (
    <div className="overflow-auto max-h-96 rounded-lg border border-slate-200">
      <table className="w-full text-sm border-collapse">
        <thead className="sticky top-0 z-10">
          <tr className="bg-red-700 text-white">
            {["#", "Lexema", "Tipo", "Descripción", "Línea", "Col"].map((h) => (
              <th key={h} className="px-3 py-2 text-left font-medium text-xs tracking-wide">
                {h}
              </th>
            ))}
          </tr>
        </thead>
        <tbody>
          {errores.map((e, i) => (
            <tr
              key={i}
              className={`border-b border-red-100 ${i % 2 === 0 ? "bg-red-50" : "bg-white"}`}
            >
              <td className="px-3 py-2 text-slate-400 text-xs">{e.numero}</td>
              <td className="px-3 py-2 font-mono text-red-600 font-medium">{e.lexema}</td>
              <td className="px-3 py-2">
                <span className={`text-xs px-2 py-0.5 rounded font-medium ${TIPO_STYLES[e.tipo] ?? ""}`}>
                  {e.tipo}
                </span>
              </td>
              <td className="px-3 py-2 text-slate-600 text-xs max-w-xs truncate" title={e.descripcion}>
                {e.descripcion}
              </td>
              <td className="px-3 py-2 text-slate-500 tabular-nums">{e.linea}</td>
              <td className="px-3 py-2 text-slate-500 tabular-nums">{e.columna}</td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}

function PanelDot({ dot }) {
  const [copiado, setCopiado] = useState(false);
  const copiar = () => {
    navigator.clipboard.writeText(dot);
    setCopiado(true);
    setTimeout(() => setCopiado(false), 2000);
  };
  return (
    <div className="flex flex-col gap-3">
      <div className="flex items-center justify-between">
        <p className="text-sm text-slate-500">
          Pega en{" "}
          <a
            href="https://dreampuf.github.io/GraphvizOnline/"
            target="_blank"
            rel="noreferrer"
            className="text-blue-600 underline hover:text-blue-800"
          >
            Graphviz Online
          </a>{" "}
          para visualizar el árbol.
        </p>
        <button
          onClick={copiar}
          className="text-xs px-3 py-1.5 rounded-lg border border-slate-300 hover:bg-slate-100 transition-colors text-slate-600 font-medium"
        >
          {copiado ? "✓ Copiado" : "Copiar DOT"}
        </button>
      </div>
      <pre className="bg-slate-900 text-slate-200 rounded-xl p-4 text-xs overflow-auto max-h-80 leading-relaxed">
        {dot || "// Sin árbol — revisa los errores sintácticos"}
      </pre>
    </div>
  );
}

// ─── Componente principal ────────────────────────────────────────
const TABS = [
  { id: "tokens",  label: "Tokens" },
  { id: "errores", label: "Errores" },
  { id: "dot",     label: "Árbol DOT" },
];

export default function App() {
  const [source, setSource]       = useState(EJEMPLO);
  const [resultado, setResultado] = useState(null);
  const [cargando, setCargando]   = useState(false);
  const [error, setError]         = useState(null);
  const [tabActiva, setTabActiva] = useState("tokens");
  const textareaRef               = useRef(null);

  const analizar = async () => {
    if (!source.trim()) return;
    setCargando(true);
    setError(null);
    setResultado(null);
    try {
      // indexService.analyze(source) llama: API.post("/analyze", { code: source })
      // El backend espera "source", así que ajusta services.js a:
      //   analyze: (code) => API.post("/analyze", { source: code })
      const { data } = await indexService.analyze(source);
      setResultado(data);
      setTabActiva(data.errores?.length ? "errores" : "tokens");
    } catch (e) {
      setError(
        e?.response
          ? `Error del servidor: ${e.response.status}`
          : "No se pudo conectar al servidor C++. ¿Está corriendo en localhost:8080?"
      );
    } finally {
      setCargando(false);
    }
  };

  const limpiar = () => {
    setSource("");
    setResultado(null);
    setError(null);
    textareaRef.current?.focus();
  };

  return (
    <div className="min-h-screen bg-slate-100 font-sans">

      {/* ── Header ── */}
      <header className="bg-slate-900 text-white px-6 py-4 flex items-center justify-between shadow-lg">
        <div className="flex items-center gap-3">
          <div className="w-8 h-8 rounded-lg bg-blue-500 flex items-center justify-center text-sm font-bold">
            TS
          </div>
          <div>
            <h1 className="text-base font-semibold leading-tight">TaskScript Analyzer</h1>
            <p className="text-xs text-slate-400">Lenguajes Formales · USAC</p>
          </div>
        </div>
        <StatusBanner resultado={resultado} />
      </header>

      <main className="max-w-7xl mx-auto p-6 grid grid-cols-1 lg:grid-cols-2 gap-6">

        {/* ── Panel izquierdo: Editor ── */}
        <div className="flex flex-col gap-4">
          <div className="bg-white rounded-2xl shadow-sm border border-slate-200 overflow-hidden">

            {/* Barra superior del editor */}
            <div className="flex items-center justify-between px-4 py-3 border-b border-slate-100 bg-slate-50">
              <div className="flex items-center gap-2">
                <span className="w-3 h-3 rounded-full bg-red-400" />
                <span className="w-3 h-3 rounded-full bg-yellow-400" />
                <span className="w-3 h-3 rounded-full bg-green-400" />
                <span className="ml-2 text-xs text-slate-400 font-mono">programa.task</span>
              </div>
              <button
                onClick={() => setSource(EJEMPLO)}
                className="text-xs text-blue-600 hover:text-blue-800 font-medium transition-colors"
              >
                Cargar ejemplo
              </button>
            </div>

            {/* Textarea */}
            <div className="relative">
              <textarea
                ref={textareaRef}
                value={source}
                onChange={(e) => setSource(e.target.value)}
                className="w-full h-96 font-mono text-sm bg-slate-900 text-slate-100
                           p-4 resize-none focus:outline-none leading-relaxed
                           placeholder:text-slate-600"
                placeholder="Escribe tu archivo .task aquí…"
                spellCheck={false}
              />
              <div className="absolute bottom-2 right-3 text-xs text-slate-500 font-mono">
                {source.split("\n").length} líneas
              </div>
            </div>
          </div>

          {/* Botones */}
          <div className="flex items-center gap-3">
            <button
              onClick={analizar}
              disabled={cargando || !source.trim()}
              className="flex-1 flex items-center justify-center gap-2 bg-blue-600
                         hover:bg-blue-700 disabled:opacity-50 disabled:cursor-not-allowed
                         text-white font-semibold py-3 rounded-xl transition-colors shadow-sm text-sm"
            >
              {cargando ? (
                <>
                  <svg className="animate-spin w-4 h-4" viewBox="0 0 24 24" fill="none">
                    <circle cx="12" cy="12" r="10" stroke="currentColor" strokeWidth="3" strokeOpacity="0.3"/>
                    <path d="M12 2a10 10 0 0 1 10 10" stroke="currentColor" strokeWidth="3" strokeLinecap="round"/>
                  </svg>
                  Analizando…
                </>
              ) : "▶  Analizar"}
            </button>
            <button
              onClick={limpiar}
              className="px-4 py-3 rounded-xl border border-slate-300 hover:bg-slate-200
                         text-slate-600 text-sm font-medium transition-colors"
            >
              Limpiar
            </button>
          </div>

          {/* Error de conexión */}
          {error && (
            <div className="bg-red-50 border border-red-200 text-red-700 rounded-xl px-4 py-3 text-sm">
              <strong>Error:</strong> {error}
            </div>
          )}

          {/* Aviso del service */}
          <div className="bg-amber-50 border border-amber-200 rounded-xl px-4 py-3 text-xs text-amber-800 leading-relaxed">
            <strong>Ajuste en services.js:</strong> cambia{" "}
            <code className="font-mono bg-amber-100 px-1 rounded">{"{ code }"}</code> por{" "}
            <code className="font-mono bg-amber-100 px-1 rounded">{"{ source: code }"}</code>{" "}
            para que coincida con el campo que espera el backend C++.
          </div>
        </div>

        {/* ── Panel derecho: Resultados ── */}
        <div className="flex flex-col gap-4">
          <div className="bg-white rounded-2xl shadow-sm border border-slate-200 overflow-hidden flex flex-col">

            {/* Tabs */}
            <div className="flex border-b border-slate-200 bg-slate-50">
              {TABS.map((tab) => {
                const esActiva  = tabActiva === tab.id;
                const errCount  = tab.id === "errores" ? (resultado?.errores?.length ?? 0) : 0;
                return (
                  <button
                    key={tab.id}
                    onClick={() => setTabActiva(tab.id)}
                    className={`relative flex items-center gap-1.5 px-5 py-3 text-sm font-medium
                                transition-colors border-b-2 -mb-px
                                ${esActiva
                                  ? "border-blue-600 text-blue-700 bg-white"
                                  : "border-transparent text-slate-500 hover:text-slate-700 hover:bg-slate-100"
                                }`}
                  >
                    {tab.label}
                    {errCount > 0 && (
                      <span className="bg-red-500 text-white text-xs rounded-full w-4 h-4 flex items-center justify-center leading-none">
                        {errCount}
                      </span>
                    )}
                  </button>
                );
              })}

              {resultado && (
                <div className="ml-auto px-4 flex items-center gap-3 text-xs text-slate-400">
                  <span>{resultado.tokens?.length ?? 0} tokens</span>
                  <span className={resultado.errores?.length ? "text-red-500 font-medium" : "text-green-500"}>
                    {resultado.errores?.length ?? 0} errores
                  </span>
                </div>
              )}
            </div>

            {/* Contenido de la tab */}
            <div className="p-4 flex-1 min-h-96">
              {!resultado ? (
                <div className="h-full flex flex-col items-center justify-center text-slate-400 gap-3">
                  <svg className="w-12 h-12 opacity-30" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={1.5}
                      d="M9 12h6m-6 4h6m2 5H7a2 2 0 01-2-2V5a2 2 0 012-2h5.586a1 1 0 01.707.293l5.414 5.414a1 1 0 01.293.707V19a2 2 0 01-2 2z"/>
                  </svg>
                  <p className="text-sm">Presiona <strong>Analizar</strong> para ver los resultados</p>
                </div>
              ) : (
                <>
                  {tabActiva === "tokens"  && <TablaTokens  tokens={resultado.tokens} />}
                  {tabActiva === "errores" && <TablaErrores errores={resultado.errores} />}
                  {tabActiva === "dot"     && <PanelDot     dot={resultado.dot} />}
                </>
              )}
            </div>
          </div>

          {/* Tarjetas de resumen */}
          {resultado && (
            <div className="grid grid-cols-3 gap-3">
              {[
                {
                  label: "Tokens",
                  value: resultado.tokens?.length ?? 0,
                  color: "text-blue-700",
                  bg: "bg-blue-50 border-blue-200",
                },
                {
                  label: "Errores léxicos",
                  value: resultado.errores?.filter((e) => e.tipo === "Lexico").length ?? 0,
                  color: "text-orange-700",
                  bg: "bg-orange-50 border-orange-200",
                },
                {
                  label: "Errores sintácticos",
                  value: resultado.errores?.filter((e) => e.tipo === "Sintactico").length ?? 0,
                  color: "text-red-700",
                  bg: "bg-red-50 border-red-200",
                },
              ].map((stat) => (
                <div key={stat.label} className={`rounded-xl border p-3 text-center ${stat.bg}`}>
                  <p className={`text-2xl font-bold tabular-nums ${stat.color}`}>{stat.value}</p>
                  <p className="text-xs text-slate-500 mt-0.5 leading-tight">{stat.label}</p>
                </div>
              ))}
            </div>
          )}
        </div>
      </main>
    </div>
  );
}