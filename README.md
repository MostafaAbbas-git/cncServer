# cncServer

### Instructions:

<br>
1- Make sure you have all the required modules installed (requirements.txt)

<br>

2- Write down your IpV4(host) in: `app.run(debug=True, host="{YOUR_IP}", port=4000)`

<br>

3- Run `app.py` using:

```sh
 py app.py
 or
 python app.py
```

### Endpoints:

<br>

1- `/flutter` -> POST method for flutter app to send an image.

<br>

2- `/instructions` GET method for ESP to get the filtered instructions.
<br>
**PS**: Instructions by default reads the content of the file `./instructions/result.json `
