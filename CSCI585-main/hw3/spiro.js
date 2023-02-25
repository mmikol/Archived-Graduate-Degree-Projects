function generateSpirographPointSequence(x0, y0, n, R, r, a, scale) {
    const X = (t, R, r) => { return (R+r)*Math.cos((r/R)*t) - (a*Math.cos((1+r/R)*t)) }
    const Y = (t, R, r) => { return (R+r)*Math.sin((r/R)*t) - (a*Math.sin((1+r/R)*t)) }
    const limit = n * Math.PI
    const sequence = []

    for (let t = 0; t < limit; t+=0.01) {
        const [ x1, y1 ] = [ X(t, R, r) * scale, Y(t, R, r) * scale ]
        const coordinate = toKMLCoordinate(x0 + x1, y0 + y1, t < limit - 0.01)
        sequence.push(coordinate)
    }

    return sequence.join('')
}

function toKMLCoordinate(lon, lat, newLineNeeded) {
    return `${lon},${lat}${newLineNeeded ? '\n' : ''}` 
}

const tommy = [-118.28546825232443, 34.02077528067821]

console.log(generateSpirographPointSequence(...tommy, 16, 8, 1, 4, .0004))