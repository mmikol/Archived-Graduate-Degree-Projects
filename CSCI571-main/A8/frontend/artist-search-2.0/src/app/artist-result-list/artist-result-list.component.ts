import { Component, OnInit } from '@angular/core';
import { ArtistDetails } from 'src/models/artist-details';
import { ArtistsSearchDataService } from 'src/shared/artists-search-data.service';
import { ArtistDetailsDataService } from 'src/shared/artist-details-data.service';
import { ArtistArtworksDataService } from 'src/shared/artist-artworks-data.service';
import { Artwork } from 'src/models/artwork';
import { Artist } from 'src/models/artist';

@Component({
  selector: 'app-artist-result-list',
  templateUrl: './artist-result-list.component.html',
  styleUrls: ['./artist-result-list.component.css']
})
export class ArtistResultListComponent implements OnInit {
  public chosen!: any
  public artists!: Array<Artist>
  public artistResultsLoaded: boolean = false
  public artistResultsFound: boolean = false
  public artistExpansionLoaded: boolean = false
  public showSpinner: boolean = false

  constructor(
    private artistSearchData: ArtistsSearchDataService, 
    private artistDetailsData: ArtistDetailsDataService, 
    private artistArtworksData: ArtistArtworksDataService) {}
  
  ngOnInit(): void {
    this.artistSearchData.message.subscribe(([data, resultsLoaded]) => {
      this.clearArtistExpansion() 
      this.chosen = null
      this.artistResultsLoaded = resultsLoaded
      this.artistResultsFound = data.length > 0
      this.artists = data
    })
  }

  private toggleSpinner() {
    this.showSpinner = !this.showSpinner
  }

  private clearArtistExpansion() {
    if (this.artistExpansionLoaded) {
      this.artistDetailsData.set([new ArtistDetails({}), !this.artistExpansionLoaded])
      this.artistArtworksData.set([[], !this.artistExpansionLoaded])
    } else this.artistExpansionLoaded = true; 
  }

  public async getArtistDetails(query: string) {
    const config = [
      `https://artistsearchbackend.wl.r.appspot.com/artists?id=${query}`,
      { method: 'GET', headers: { 'Content-Type': 'application/json' } },  
    ] as const
    return await fetch(...config)
      .then(response => { return response.json() })
      .then(parsed => new ArtistDetails(parsed))
  }

  public async getArtistArtworks(query: string) {
    const config = [
      `https://artistsearchbackend.wl.r.appspot.com/artworks?id=${query}`,
      { method: 'GET', headers: { 'Content-Type': 'application/json' } },  
    ] as const
    return await fetch(...config)
      .then(response => { return response.json() })
      .then(parsed =>parsed.map((item: Object) => { return new Artwork(item) }))
  }

  public async expandArtist(artistID: string) {
    this.clearArtistExpansion()
    this.toggleSpinner()
    const [artistDetails, artistArtworks] = [await this.getArtistDetails(artistID), await this.getArtistArtworks(artistID)]
    this.toggleSpinner()
    this.artistDetailsData.set([artistDetails, this.artistExpansionLoaded]) // render details
    this.artistArtworksData.set([artistArtworks, this.artistExpansionLoaded]) // render artworks
  }
}
