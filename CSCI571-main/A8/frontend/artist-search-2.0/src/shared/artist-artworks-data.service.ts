import { Injectable } from '@angular/core';
import { BehaviorSubject } from 'rxjs';
import { Artwork } from 'src/models/artwork';

@Injectable({
  providedIn: 'root'
})

export class ArtistArtworksDataService {
  private source = new BehaviorSubject<Array<any>>([[], false])
  public message = this.source.asObservable()

  constructor() { }
  set(message: Array<any>) { this.source.next(message) }
}
