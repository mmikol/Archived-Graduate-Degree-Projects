import { TestBed } from '@angular/core/testing';

import { ArtistsSearchDataService } from './artists-search-data.service';

describe('ArtistsSearchDataService', () => {
  let service: ArtistsSearchDataService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(ArtistsSearchDataService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
