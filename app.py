import os
from flask import Flask, request, jsonify
from flask_cors import CORS
from subprocess import call
from utils.helper import extract_gcode_file, filter_result,  scale_list, floor_values
import json


app = Flask(__name__)
CORS(app)


@app.route('/')
def index():
    return "<h1>CNC task (#3)</h1>"


@app.route('/flutter', methods=["POST"])
def flutter():
    if request.method == "POST":
        # get the image from flutter app
        imagefile = request.files['image']
        filename = imagefile.filename
        saved_img_path = "./uploadedimages/" + filename
        imagefile.save(saved_img_path)

        print(f"Image recieved and saved successfully in {saved_img_path}")

        # Convert the saved image to gcode
        gcode_file_path = f"./gcodeFiles/{filename}.gcode"

        call(["py", "image_to_gcode.py", "--input", saved_img_path,
              "--output", gcode_file_path, "--threshold", "100"])

        print(f"Gcode file created successfully in {gcode_file_path}")

        # Read the gcode file
        x_values, y_values, commands = extract_gcode_file(gcode_file_path)

        print(f"Gcode file read successfully and x values: {len(x_values)}")

        # Scale results to [0, 400]
        x_values = scale_list(x_values, 0, 400)
        y_values = scale_list(y_values, 0, 400)

        print(f"x_values scaled successfully with max value: {max(x_values)}")

        # filter results to remove close values
        x_values, y_values, commands = filter_result(
            x_values, y_values, commands)

        print(f"x_values filtered successfully, new length: {len(x_values)}")

        # floor all values
        x_values = floor_values(x_values)
        y_values = floor_values(y_values)

        result = {
            'x': x_values,
            'y': y_values,
            'state': commands
        }

        # Store result in json file localy
        result_json_path = f"./instructions/{filename}.json"

        with open(result_json_path, 'w') as json_file:
            json.dump(result, json_file)

        # Save the last result again in result.json to get it later easily.
        with open("./instructions/result.json", 'w') as json_file:
            json.dump(result, json_file)

        print(f"Result stored successfully in {result_json_path}.")

        # Send result
        return jsonify({
            'message': "Done"
        })


@app.route('/instructions', methods=["GET"])
def get_instructions():

    x_values = []
    y_values = []
    state = []

    # Opening JSON file
    # PS: User has to specify the path manually if
    file = open("./instructions/result.json")

    data = json.load(file)

    # Iterating through the json list
    for x in data["x"]:
        x_values.append(x)

    for y in data["y"]:
        y_values.append(y)

    for s in data["state"]:
        state.append(s)

    # Closing file
    file.close()

    print("Fetching the data from result.json has completed successfully")

    return jsonify({
        "x": x_values,
        "y": y_values,
        "state": state
    })


if __name__ == "__main__":
    os.environ['FLASK_ENV'] = 'development'
    app.run(debug=True, host="192.168.1.13", port=4000)
