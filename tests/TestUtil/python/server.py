"""
Brief:               Small server for testing. Will run until recieved one request and then stop.
                     Assumes request contain JSON, and will write it to the file sys.argv[1].
                     Ex:
                       python server.py /tmp/out.json


File name:           server.py
Python Version:      3.9
"""

import sys
import multiprocessing
from flask import Flask, request
import json

app = Flask(__name__)

queue = multiprocessing.Queue()

@app.route('/report', methods=['POST'])
def result():
    print(f"Recieved request on /report")

    print(f"JSON: {request.json}")
    with open(sys.argv[1], 'w') as out_file:
        print(f"Writing JSON to {sys.argv[1]}")
        out_file.write(json.dumps(request.json))

    # Notify that we can turn off the server
    queue.put("done")

    return "Hi from python"

def run_until_one_request():
    serverProcess = multiprocessing.Process(target=app.run)
    serverProcess.start()
    # Block until something in the queue
    queue.get(block=True)
    serverProcess.terminate()

if __name__ == '__main__':
    run_until_one_request()

