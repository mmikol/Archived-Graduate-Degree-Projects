import { TestBed } from '@angular/core/testing';

import { ArtistArtworksDataService } from './artist-artworks-data.service';

describe('ArtistArtworksDataService', () => {
  let service: ArtistArtworksDataService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(ArtistArtworksDataService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
