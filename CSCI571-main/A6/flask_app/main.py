from flask import Flask, request, jsonify
from requests import post, get

app = Flask(__name__)

client = {
   'id': '6c4a035b4e7a9d2c8ead', 
   'secret': '3d692604e254e1063427540ad5933f8e'
}

@app.route('/')
def home():
   return app.send_static_file('index.html')

def authenticate(client_id, client_secret):
   url = 'https://api.artsy.net/api/tokens/xapp_token'
   payload = { 'client_id': client_id, 'client_secret': client_secret }
   headers = { 'Content-Type': 'application/json' }
   response = post(url, headers=headers, params=payload).json()
   client['token'] = response['token']

@app.route('/search', methods=['GET'])
def search():
   authenticate(client['id'], client['secret'])
   data = request.args['query']
   url = 'https://api.artsy.net/api/search'
   headers = { 'X-Xapp-Token': client['token'] }
   args = { 'q': data, 'size': 10 }
   response = get(url, headers=headers, params=args).json()
   artists = filter(lambda r: r['type'] == 'artist', response['_embedded']['results'])
   return jsonify([{
      'name': artist['title'], 
      'id': artist['_links']['self']['href'].split('/')[-1], 
      'image': artist['_links']['thumbnail']['href']
   } for artist in artists])

@app.route('/retrieve', methods=['GET'])
def retrieve():
   authenticate(client['id'], client['secret'])   
   data = request.args['artist_id']
   url = f'https://api.artsy.net/api/artists/{data}'
   headers = { 'X-Xapp-Token': client['token'] }
   response = get(url, headers=headers).json()
   return jsonify({
      'name': response['name'],
      'birthday': response['birthday'],
      'deathday': response['deathday'],
      'nationality': response['nationality'],
      'biography': response['biography']
   })

if __name__ == '__main__':
   app.run()

