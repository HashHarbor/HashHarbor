import json
import requests
from openai import OpenAI

def get_api_key():
    with open('config.json') as f:
        config = json.load(f)
    return config['api_key']

def toolTip(input_msg):
    apiToken = get_api_key()
    client = OpenAI(
    base_url = "https://integrate.api.nvidia.com/v1",
    api_key = apiToken
    )

    # Prompt the user to enter the message
    # user_message = input("Enter your message: ")

    completion = client.chat.completions.create(
    model="meta/codellama-70b",
    messages=[{"role":"user","content":input_msg}],
    temperature=0.1,
    top_p=1,
    max_tokens=1024,
    stream=True
    )

    output = ""
    for chunk in completion:
        if chunk.choices[0].delta.content is not None:
            output += chunk.choices[0].delta.content

    # print(output)
    return output