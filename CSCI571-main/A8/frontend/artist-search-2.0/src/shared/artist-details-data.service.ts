import { Injectable } from '@angular/core';
import { BehaviorSubject } from 'rxjs';
import { ArtistDetails } from 'src/models/artist-details';

@Injectable({
  providedIn: 'root'
})
export class ArtistDetailsDataService {
  private source = new BehaviorSubject<Array<any>>([new ArtistDetails({}), false])
  public message = this.source.asObservable()
  constructor() { }
  set(message: Array<any>) { this.source.next(message) }
}
