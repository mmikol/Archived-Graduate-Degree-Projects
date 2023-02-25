import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ArtistDetailsTabsComponent } from './artist-details-tabs.component';

describe('ArtistDetailsTabsComponent', () => {
  let component: ArtistDetailsTabsComponent;
  let fixture: ComponentFixture<ArtistDetailsTabsComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ArtistDetailsTabsComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ArtistDetailsTabsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
