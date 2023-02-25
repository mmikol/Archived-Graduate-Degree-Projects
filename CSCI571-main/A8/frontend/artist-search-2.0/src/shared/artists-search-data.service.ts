import { Injectable } from '@angular/core';
import { BehaviorSubject } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class ArtistsSearchDataService {
  private source = new BehaviorSubject<Array<any>>([[], false])
  public message = this.source.asObservable()
  constructor() { }
  set(message: Array<any>) { this.source.next(message) }
} 
