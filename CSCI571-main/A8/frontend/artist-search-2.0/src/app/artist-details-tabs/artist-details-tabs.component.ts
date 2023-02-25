import { Component, OnInit } from '@angular/core';
import { ArtistDetails } from 'src/models/artist-details';
import { Artwork } from 'src/models/artwork';
import { ArtistGene } from 'src/models/artist-gene';
import { ArtistDetailsDataService } from 'src/shared/artist-details-data.service';
import { ArtistArtworksDataService } from 'src/shared/artist-artworks-data.service';

@Component({
  selector: 'app-artist-details-tabs',
  templateUrl: './artist-details-tabs.component.html',
  styleUrls: ['./artist-details-tabs.component.css']
})
export class ArtistDetailsTabsComponent implements OnInit {
  public artworks: Array<Artwork> = []
  public genes: Array<ArtistGene> = []
  public details: ArtistDetails = new ArtistDetails({})
  public missing: string = ''
  public modalTitle: string = ''
  public modalThumbnail: string = ''
  public modalYear: string = ''
  public detailsLoaded: boolean = false
  public artworksLoaded: boolean = false
  public genesRequested: boolean = false
  public showSpinner: boolean = false

  constructor(private artistDetailsData: ArtistDetailsDataService, private artistArtworksData: ArtistArtworksDataService) { }

  ngOnInit(): void {
    this.artistDetailsData.message.subscribe(([data, detailsLoaded])  => {
      this.detailsLoaded = detailsLoaded
      this.details = data  
    })
    this.artistArtworksData.message.subscribe(([data, artworksLoaded])  => {
      this.artworksLoaded = artworksLoaded
      this.artworks = data
    })
  }

  private toggleSpinner() {
    this.showSpinner = !this.showSpinner
  }

  private clearArtworkGenes() {
    if (this.genesRequested) this.genesRequested = !this.genesRequested;
    this.genes = []
  }
  
  public async getArtistCategories(query: string) {
    const config = [
      `https://artistsearchbackend.wl.r.appspot.com/genes?id=${query}`,
      { method: 'GET', headers: { 'Content-Type': 'application/json' } },  
    ] as const
    return await fetch(...config)
      .then(response => { return response.json() })
      .then(parsed => parsed.map((item: Object) => { return new ArtistGene(item) }))
  }

  public async expandArtistGenes(artistID: string, artworkTitle: string, artworkImage: string, artworkYear: string) {
    this.clearArtworkGenes()
    this.toggleSpinner();
    [this.modalTitle, this.modalThumbnail, this.modalYear] = [artworkTitle, artworkImage, artworkYear]
    this.genes = await this.getArtistCategories(artistID)
    this.genesRequested = true
    this.toggleSpinner()
  }
}
