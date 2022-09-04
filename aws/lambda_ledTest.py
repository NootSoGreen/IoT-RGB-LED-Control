# publishes rgb values to topic
# based on https://stackoverflow.com/questions/65330225/publish-mqtt-message-to-aws-iot-core-from-lambda
import base64
import json
import boto3


def lambda_handler(event, context):
    params = json.loads(base64.b64decode(event.get('body', '')))

    # make sure data sent to this function has the required format, i..e includes red, green blue in dict
    paramTypes = ["red", "green", "blue"]
    for par in paramTypes:
        if par not in params:
            print(f"missing param: {par}")
            return{'statuscode': 404, 'body': f'{par} not specified'}

    print(
        f"Set LED to RGB: {params['red']}, {params['green']}, {params['blue']}")

    client = boto3.client('iot-data', region_name='ap-southeast-2')

    # Format text message from data
    data = {"red": params["red"],
            "green": params["green"], "blue": params["blue"]}

    # send MQTT message to device
    response = client.publish(
        topic='esp32/sub',
        qos=0,
        payload=json.dumps(data)
    )

    print(response)

    # sends message back to website
    return {
        'statusCode': 200,
        'body': json.dumps(f"Set LED to RGB: {params['red']}, {params['green']}, {params['blue']}")
    }
