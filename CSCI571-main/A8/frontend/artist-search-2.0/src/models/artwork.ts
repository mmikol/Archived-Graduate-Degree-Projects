export class Artwork {
    public id: string = ''
    public title: string = ''
    public date: string = ''
    public image: string = ''
    constructor(data: Object) { Object.assign(this, data) }
}
