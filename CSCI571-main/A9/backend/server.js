const path = require('path'),
    express = require('express'),
    bodyParser = require('body-parser'),
    axios = require('axios'),
    cors = require('cors'),
    app = express()

app.use(cors())
app.use((req, res, next) => {
    const allowed = ['http://localhost:4200/', 'https://artistsearchfrontend.wl.r.appspot.com', 'https://artistsearchcarouselfrontend.wl.r.appspot.com'];
    const origin = req.headers.origin;
    if (allowed.includes(origin)) res.setHeader('Access-Control-Allow-Origin', origin);
    res.header('Access-Control-Allow-Methods', 'GET');
    res.header('Access-Control-Allow-Headers', 'Content-Type, Authorization');
    res.header('Access-Control-Allow-Credentials', true);
    return next();
});
app.use(bodyParser.urlencoded({ extended: true }));

const client = {
    'id': '6c4a035b4e7a9d2c8ead', 
    'secret': '3d692604e254e1063427540ad5933f8e'
 }

const authentication = async (id, secret) => {
    const url = 'https://api.artsy.net/api/tokens/xapp_token'
    const payload = { 'client_id': id, 'client_secret': secret }
    const config = { headers: { 'Content-Type': 'application/json' } }
    return await axios
        .post(url, payload, config)
        .then(response => { 
            const { data } = response
            return data['token'] 
        })
        .catch(error => { return error.message })
}

app.get('/', async (req, res) => res.json({message: 'You have successfully connected to the server.'}))

app.get('/search', async (req, res) => {
    const token = await authentication(client['id'], client['secret'])
    const config = {
        url: 'https://api.artsy.net/api/search',
        params: { 'q': req.query.q, 'size': 10 },
        headers: { 'X-Xapp-Token': token }
    }
    await axios(config)
        .then(response => {
            const data = response['data']['_embedded']['results']
                .filter(element => element['type'] == 'artist')
                .map(artist => {
                    return {
                        'name': artist['title'], 
                        'id': artist['_links']['self']['href'].split('/').pop(),
                        'image': artist['_links']['thumbnail']['href']
                    }
                })
            res.json(data)
        })
        .catch(error => res.json({'error_message': error.message }))
})

app.get('/artists', async (req, res) => {
    const token = await authentication(client['id'], client['secret'])
    const config = {
        url: `https://api.artsy.net/api/artists/${req.query.id}`,
        headers: { 'X-Xapp-Token': token }
    }
    await axios(config)
        .then(response => {
            res.json({
                'id': req.query.id,
                'name': response['data']['name'],
                'birthday': response['data']['birthday'],
                'deathday': response['data']['deathday'],
                'nationality': response['data']['nationality'],
                'biography': response['data']['biography']
            })
        })
        .catch(error => res.json({'error_message': error.message }))
})

app.get('/artworks', async (req, res) => {
    const token = await authentication(client['id'], client['secret'])
    const config = {
        url: 'https://api.artsy.net/api/artworks',
        params: { 'artist_id': req.query.id, 'size': 10 },
        headers: { 'X-Xapp-Token': token }
    }
    await axios(config)
        .then(response => {
            const data = response['data']['_embedded']['artworks']
                .map(artwork => {
                    return {
                        'id': artwork['id'],
                        'title': artwork['title'],
                        'date': artwork['date'],
                        'image': artwork['_links']['thumbnail']['href']
                    }
                })
            res.json(data)
        })
        .catch(error => res.json({ 'error_message': error.message }))
    })

app.get('/genes', async (req, res) => {
    const token = await authentication(client['id'], client['secret'])
    const config = {
        url: 'https://api.artsy.net/api/genes',
        params: { 'artwork_id': req.query.id },
        headers: { 'X-Xapp-Token': token }
    }
    await axios(config)
        .then(response => {
            const data = response['data']['_embedded']['genes']
            .map(gene => {
                return {
                    'name': gene['name'],
                    'image': gene['_links']['thumbnail']['href'],
                    'description': gene['description']
                }
            })
            res.json(data)
        })
        .catch(error => res.json({'error_message': error.message }))
})

const PORT = process.env.port || 8080
app.listen(PORT, () => console.log(`App listening on port ${PORT}`))