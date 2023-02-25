export class ArtistDetails {
    public id: string = ''
    public name: string = ''
    public birthday: string = ''
    public deathday: string = ''
    public nationality: string = ''
    public biography: string = ''
    constructor(data: Object) { Object.assign(this, data)}
}
