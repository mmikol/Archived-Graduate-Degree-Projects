$(document).ready(function() {
    let currentArtistSelection = null
    $('#search-clear-button').click(() => $('#search-bar').val(''))
    $('form').submit((event) => {
        clearAndshowAnimation('.biography-wrapper', event)
        $.get('/search', $('form').serialize(), (response) => {
            appendArtistResults(response)
            $('.loading-animation-wrapper').hide()
            $('.results-wrapper').show()        
        })
    })
    $('.results-wrapper').on('click', '.result-artist', (event) => {
        $('.result-artist').attr('disabled','disabled');
        clearAndshowAnimation('.biography-wrapper', event)
        if (currentArtistSelection) $(currentArtistSelection).css('background-color', '#205375')
        currentArtistSelection = event.currentTarget
        $(currentArtistSelection).css('background-color', '#112B3C')
        $.get('/retrieve',{ artist_id: $(currentArtistSelection).attr('data-id') }, (response) => {
            appendArtistBiography(response)
            $('.loading-animation-wrapper').hide()
            $('.biography-wrapper').show()        
            $('.result-artist').removeAttr('disabled');
        })
    })
})

function appendArtistBiography(results) {
    const { biography, birthday, deathday, name, nationality } = results
    const missing = []
    if (!biography) missing.push('No Biography')
    if (!birthday) missing.push('No Birth Year')
    if (!deathday) missing.push('No Death Year')
    if (!name) missing.push('No Name')
    if (!nationality) missing.push('No Nationality')
    $('.biography-wrapper').append(
        `
        <p class="artist-info">
            <span class="artist-name">${name}</span>
            <span class="artist-birthyear">(${birthday} - </span>
            <span class="artist-deathyear">${deathday})</span>
        </p>
        <p class="artist-nationality">${nationality}</p>
        <p class="missing-artist-info">${missing.join(', ')}</p>
        <p class="artist-biography">${biography}</p>
        `
    )
}

function appendArtistResults(results) {
    $('.results-wrapper').empty()
    $('.results-wrapper').hide()
    if (results.length <= 0) {
        $('.results-wrapper').append('<span class="no-result-wrapper">No results found.</span>')
    } else {
        results.map((artist) => {
            let { id, image, name } = artist
            if (image === '/assets/shared/missing_image.png') image = '/static/artsy_logo.svg'
            $('.results-wrapper').append(
                `
                <button class="result-artist" data-id="${id}">
                    <img class="result-artist-image" src="${image}">
                    <span class="result-artist-name">${name}</span>
                </button>
                `
            );
        })
    }
}

function clearAndshowAnimation(element, event) {
    event.preventDefault()
    $(element).empty()
    $(element).hide()
    $('.loading-animation-wrapper').show()
}
