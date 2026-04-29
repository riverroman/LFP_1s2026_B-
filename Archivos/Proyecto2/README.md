# Archivos léxicos
*(el parser debe acumular todos los errores y continuar)*

- **test01** — Tablero de Sistema de Ventas.  
  Errores léxicos en valores de `responsable`:  
  `Lucia@Dev`, `Marco$Admin`, `Carlos&Backend`, `Team~QA`, `Carlos^Ops`.  
  Caracteres inválidos `@`, `$`, `&`, `~`, `^` dentro de las cadenas o como parte de identificadores sin comillas.  
  Son **5 errores** distribuidos en distintas columnas.

- **test02** — Plataforma Educativa.  
  Errores en:  
  `Estudio&Media`, `Admin|Portal`, `DevTeam#3`, `Técnico~Red`, `UX@Designer`, `IT$Admin`, `DBA_Senior` *(si el guión bajo no es válido en tu AFD)*, `RRHH^Dept`.  
  Más denso, unos **7-8 errores léxicos**.

- **test03** — App Móvil Finanzas.  
  Es el más cargado:  
  `iOS@Dev`, `Flutter~Dev`, `Seguridad$Team`, `Backend^API`, `DevTeam!Internacional`, `QA~iOS`, `QA&A11y`, `Marketing@Team`.  
  Aproximadamente **8 errores léxicos** bien distribuidos en 5 columnas.

---

# Archivos sintácticos
*(el parser debe detenerse en el primer error)*

- **test04** — En la tercera tarea de la columna "Análisis", falta el `:` después de `tarea`.  
  La línea dice `tarea "Benchmarking..."` en lugar de `tarea: "Benchmarking..."`.  
  El programa debe detenerse ahí y no procesar el resto.

- **test05** — En la columna "Planificación", el cierre del bloque dice `;` en lugar de `};`.  
  Le falta la llave de cierre `}` antes del punto y coma.  
  El parser debe detectar que encontró `;` cuando esperaba `}` y detenerse.
