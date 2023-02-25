export class Artist {
    public id: string = ''
    public name: string = ''
    public image: string = ''
    constructor(data: Object) {
        Object.assign(this, data);
        if (this.image == '/assets/shared/missing_image.png') this.image = './assets/artsy_logo.svg'
    }
}
