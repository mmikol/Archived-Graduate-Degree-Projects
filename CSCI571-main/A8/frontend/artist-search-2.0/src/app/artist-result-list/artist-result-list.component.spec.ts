import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ArtistResultListComponent } from './artist-result-list.component';

describe('ArtistResultListComponent', () => {
  let component: ArtistResultListComponent;
  let fixture: ComponentFixture<ArtistResultListComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ArtistResultListComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ArtistResultListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
