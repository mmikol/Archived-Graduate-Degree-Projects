import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { AppComponent } from './app.component';
import { FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';
import { ArtistSearchFormComponent } from './artist-search-form/artist-search-form.component';
import { ArtistResultListComponent } from './artist-result-list/artist-result-list.component';
import { ArtistDetailsTabsComponent } from './artist-details-tabs/artist-details-tabs.component';

@NgModule({
  declarations: [
    AppComponent,
    ArtistSearchFormComponent,
    ArtistResultListComponent,
    ArtistDetailsTabsComponent
  ],
  imports: [
    BrowserModule,
    HttpClientModule,
    FormsModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
