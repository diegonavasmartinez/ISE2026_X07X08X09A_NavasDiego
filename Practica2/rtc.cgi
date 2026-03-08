t <html><head><title>RTC Clock</title></head>

i pg_header.inc

t <h2 align=center><br>RTC Date and Time</h2>

t <p align=center><font size="3">
t This page shows the current <b>date</b> and <b>time</b> from the RTC.
t </font></p>

t <table border=0 width=60% align=center>
t <tr bgcolor=#aaccff>
t  <th width=50%>Item</th>
t  <th width=50%>Value</th>
t </tr>

t <tr>
t  <td><img src=pabb.gif>Current Time:</td>
c h 0 <td>%s</td>
t </tr>

t <tr>
t  <td><img src=pabb.gif>Current Date:</td>
c h 1 <td>%s</td>
t </tr>

t </table>

i pg_footer.inc
.