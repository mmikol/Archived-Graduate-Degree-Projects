import { TestBed } from '@angular/core/testing';

import { ArtistDetailsDataService } from './artist-details-data.service';

describe('ArtistDetailsDataService', () => {
  let service: ArtistDetailsDataService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(ArtistDetailsDataService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
