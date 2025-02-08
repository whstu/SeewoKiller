import requests
from os import system

API_BASE_URL = "https://api.cloudflare.com/client/v4/accounts/***/ai/run/"
headers = {"Authorization": "Bearer ***"}


def run(model, inputs):
    input = { "messages": inputs }
    response = requests.post(f"{API_BASE_URL}{model}", headers=headers, json=input)
    return response.json()
while True:
    content=input("Chat with llama:")
    inputs = [
        { "role": "system", "content": "你是一个乐于助人的助理。你要用简体中文回答用户的问题。" },#You are a helpful assistant
        { "role": "user", "content": content}#"Can you speak Chinese?"
    ];
    print("getting results...")
    output = run("@cf/meta/llama-3-8b-instruct", inputs)
    print(output)
    response_content = output['result']['response']
    print("\n结果：")
    print(response_content)
