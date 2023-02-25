import { Component, Injectable, OnInit } from '@angular/core';
import { Artist } from 'src/models/artist';
import { ArtistsSearchDataService } from 'src/shared/artists-search-data.service';

@Component({
  selector: 'app-artist-search-form',
  templateUrl: './artist-search-form.component.html',
  styleUrls: ['./artist-search-form.component.css']
})
@Injectable({
  providedIn:  'root'
})
export class ArtistSearchFormComponent implements OnInit {
  private artists!: Array<Artist>
  public searchBar!: string
  public showSpinner: boolean = false
  public artistResultsLoaded: boolean = false 
  public artistResultsFound: boolean = false
  
  constructor(private artistSearchData: ArtistsSearchDataService) {}  
  ngOnInit(): void {}

  private toggleSpinner() {
    this.showSpinner = !this.showSpinner
  }

  private async getArtists(query: string) { 
    const config = [
      `https://artistsearchbackend.wl.r.appspot.com/search?q=${query}`,
      { method: 'GET', headers: { 'Content-Type': 'application/json' } },  
    ] as const
    const artists = await fetch(...config)
      .then(response => { return response.json() })
      .then(parsed => parsed.map((item: Artist) => { return new Artist(item) }))
    return artists
  }

  public async search()  { 
    this.toggleSpinner()
    this.artists = await this.getArtists(this.searchBar)
    this.artistResultsFound = this.artists.length > 0
    if (!this.artistResultsLoaded) this.artistResultsLoaded = !this.artistResultsLoaded
    this.artistSearchData.set([this.artists, this.artistResultsLoaded])
    this.toggleSpinner()
  }

  public clear() { 
    this.searchBar = ''
    if (this.artistResultsLoaded) {
      this.artistResultsLoaded = !this.artistResultsLoaded
      this.artistSearchData.set([[], this.artistResultsLoaded])
    }
  }  
}