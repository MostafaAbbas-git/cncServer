
import math


def scale_list(list, out_min, out_max):
    result = []
    list_max = max(list)
    list_min = min(list)

    for item in list:
        calc = (item - list_min) * (out_max - out_min) / \
            (list_max - list_min) + out_min
        result.append(calc)

    return result


def get_distance(x1, y1, x2, y2):
    y = x2 - x1
    x = y2 - y1

    return math.sqrt((x * x) + (y * y))


def filter_result(x_array, y_array, commands_array):

    x = x_array
    y = y_array
    g = commands_array

    i = 0
    while i < (len(x) - 1):

        distance = get_distance(x[i], y[i], x[i + 1], y[i + 1])

        if (distance < 15):

            del x[i + 1]
            del y[i + 1]
            del g[i + 1]
            i = i - 1

            if (i < 0):
                i = 0
        i = i + 1

    return x, y, g


def extract_gcode_file(gcode_file_path):

    # Using readlines()
    file1 = open(gcode_file_path, 'r')
    Lines = file1.readlines()

    count = 0
    x_values = []
    y_values = []
    commands = []

    # Strips the newline character
    for line in Lines:
        count += 1
        text_in_line = line.strip()
        splitted_line = text_in_line.split()

        x_value = splitted_line[1][1:]
        y_value = splitted_line[2][2:]
        commmand = splitted_line[0][1:]

        if x_value == '':
            x_value = '0'

        if y_value == '':
            y_value = '0'

        if commmand == '':
            commmand = '0'

        x_values.append(x_value)
        y_values.append(y_value)
        commands.append(commmand)

    x_values = [int(x) for x in x_values]
    y_values = [int(x) for x in y_values]
    commands = [int(x) for x in commands]

    return x_values, y_values, commands


def floor_values(list):
    result = []

    for item in list:
        item = math.floor(item)
        result.append(item)
        
    return result
