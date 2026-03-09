t <html>
t <head>
t <title>Monitor RTC</title>
t <script language=JavaScript type="text/javascript" src="xml_http.js"></script>
t <script language=JavaScript type="text/javascript">
t var formUpdate = new periodicObj("rtc.cgx", 1000);
t function plotRTCGraph() {
t  hora = document.getElementById("hora_rtc").value;
t  fecha = document.getElementById("fecha_rtc").value;
t }
t function periodicUpdateRTC() {
t  if(document.getElementById("RTCChkBox").checked == true) {
t   updateMultiple(formUpdate,plotRTCGraph);
t   RTC_elTime = setTimeout(periodicUpdateRTC, formUpdate.period);
t  }
t  else
t   clearTimeout(RTC_elTime);
t }
t </script>
t <style>
t body {
t   background-color: #121212;
t   font-family: 'Segoe UI', Roboto, Helvetica, Arial, sans-serif;
t   color: #e0e0e0;
t }
t .rtcBox {
t   width: 450px;
t   margin: 50px auto;
t   background: #1e1e24;
t   border-radius: 12px;
t   padding: 30px;
t   box-shadow: 0 10px 25px rgba(0,0,0,0.5);
t   border-top: 4px solid #4CAF50;
t }
t .title {
t   text-align: center;
t   color: #ffffff;
t   margin-top: 0;
t   font-weight: 400;
t   letter-spacing: 1px;
t }
t .subtitle {
t   text-align: center;
t   color: #888;
t   font-size: 0.9em;
t   margin-bottom: 25px;
t }
t .field {
t   margin-bottom: 20px;
t }
t .field label {
t   display: block;
t   font-size: 0.85em;
t   text-transform: uppercase;
t   color: #4CAF50;
t   margin-bottom: 8px;
t   letter-spacing: 1px;
t }
t .field input[type=text] {
t   width: 100%;
t   box-sizing: border-box;
t   padding: 12px;
t   background: #0a0a0c;
t   border: 1px solid #333;
t   border-radius: 6px;
t   color: #4CAF50;
t   font-family: 'Courier New', Courier, monospace;
t   font-size: 1.2em;
t   text-align: center;
t   transition: border 0.3s ease;
t }
t .field input[type=text]:focus {
t   border-color: #4CAF50;
t   outline: none;
t }
t .updateBox {
t   margin-top: 30px;
t   display: flex;
t   align-items: center;
t   justify-content: space-between;
t   background: #2a2a32;
t   padding: 15px 20px;
t   border-radius: 8px;
t }
t .updateBox span {
t   font-weight: 500;
t }
t /* Estilo para convertir el checkbox en un Switch moderno */
t .switch {
t   position: relative;
t   display: inline-block;
t   width: 46px;
t   height: 24px;
t }
t .switch input {
t   opacity: 0;
t   width: 0;
t   height: 0;
t }
t .slider {
t   position: absolute;
t   cursor: pointer;
t   top: 0; left: 0; right: 0; bottom: 0;
t   background-color: #555;
t   transition: .4s;
t   border-radius: 24px;
t }
t .slider:before {
t   position: absolute;
t   content: "";
t   height: 16px;
t   width: 16px;
t   left: 4px;
t   bottom: 4px;
t   background-color: white;
t   transition: .4s;
t   border-radius: 50%;
t }
t input:checked + .slider {
t   background-color: #4CAF50;
t }
t input:checked + .slider:before {
t   transform: translateX(22px);
t }
t </style>
t </head>
i pg_header.inc
t <body>
t <div class="rtcBox">
t <h2 class="title">Monitor de Hora RTC</h2>
t <div class="subtitle">Visualización del reloj en tiempo real del sistema</div>
t <form action=rtc.cgi method=post name=cgi>
t <input type=hidden value="lang" name=pg>
t <div class="field">
t  <label>Hora del sistema</label>
c h 1 <input type=text name=rtc1 id="hora_rtc" maxlength=20 value="%s" readonly>
t </div>
t <div class="field">
t  <label>Fecha del sistema</label>
c h 2 <input type=text name=rtc2 id="fecha_rtc" maxlength=20 value="%s" readonly>
t </div>
t <div class="updateBox">
t  <span>Actualización automática</span>
t  <label class="switch">
t   <input type="checkbox" id="RTCChkBox" onclick="periodicUpdateRTC()">
t   <span class="slider"></span>
t  </label>
t </div>
t </form>
t </div>
t </body>
i pg_footer.inc
t </html>
.