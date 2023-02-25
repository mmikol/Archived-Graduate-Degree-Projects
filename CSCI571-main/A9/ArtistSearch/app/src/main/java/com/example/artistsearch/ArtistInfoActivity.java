package com.example.artistsearch;

import android.annotation.SuppressLint;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Toast;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;
import androidx.viewpager2.widget.ViewPager2;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.google.android.material.tabs.TabLayout;
import com.google.android.material.tabs.TabLayoutMediator;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class ArtistInfoActivity extends AppCompatActivity implements TabLayoutMediator.TabConfigurationStrategy{
    private final String[] TAB_TITLES = new String[]{ "DETAILS", "ARTWORK" };
    private DetailsFragment detailsFragment = new DetailsFragment();
    private ArtworksFragment artworksFragment = new ArtworksFragment();
    private ViewPager2 pager;
    private MenuItem favorite, unfavorite;
    private boolean isFavoriteArtist;
    private Artist artist;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_artist_info);
        Bundle extras = getIntent().getExtras();
        isFavoriteArtist = extras.getBoolean("isFavoriteArtist");
        setTitle(extras.getString("artistName"));
        pager = findViewById(R.id.pager);
        TabLayout tabLayout = findViewById(R.id.tab_layout);
        setViewPagerAdapter(detailsFragment, artworksFragment);
        new TabLayoutMediator(tabLayout, pager, this).attach();
        getArtist(extras.getString("artistID"));
        getArtworks(extras.getString("artistID"));
    }

    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
        favorite = menu.findItem(R.id.action_favorite);
        unfavorite = menu.findItem(R.id.action_unfavorite);
        favorite.setVisible(isFavoriteArtist);
        unfavorite.setVisible(!isFavoriteArtist);
        return true;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.action_favorite, menu);
        return true;
    }

    @SuppressLint("NonConstantResourceId")
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        SharedPreferences preferences = getApplicationContext().getSharedPreferences("favorite_artists", 0);
        SharedPreferences.Editor editor = preferences.edit();
        switch (item.getItemId()) {
            case android.R.id.home:
                this.finish();
                return true;
            case R.id.action_favorite:
                showToast(false);
                editor.remove(artist.getID());
                editor.commit();
                item.setVisible(false);
                unfavorite.setVisible(true);
                return true;
            case R.id.action_unfavorite:
                showToast(true);
                JSONObject artistDetails = new JSONObject();
                try {
                    artistDetails.put("id", artist.getID());
                    artistDetails.put("name", artist.getName());
                    artistDetails.put("birthday", artist.getBirthday());
                    artistDetails.put("nationality", artist.getNationality());
                } catch (JSONException e) {
                    throw new RuntimeException(e);
                }
                editor.putString(artist.getID(), artistDetails.toString());
                editor.commit();
                item.setVisible(false);
                favorite.setVisible(true);
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    @SuppressLint("ResourceAsColor")
    @Override
    public void onConfigureTab(@NonNull TabLayout.Tab tab, int position) {
        tab.setText(TAB_TITLES[position]);
        switch (position) {
            case 0:
                tab.setIcon(R.drawable.ic_action_details);
                break;
            case 1:
                tab.setIcon(R.drawable.ic_action_artwork);
                break;
        }
    }

    public void setViewPagerAdapter(DetailsFragment detailsFragment, ArtworksFragment artworksFragment) {
        InfoPagerAdapter infoPagerAdapter = new InfoPagerAdapter(this);
        final Fragment[] tab_fragments = new Fragment[]{ detailsFragment, artworksFragment };
        infoPagerAdapter.setData(tab_fragments);
        pager.setAdapter(infoPagerAdapter);
    }

    private void getArtist(String query) {
        RequestQueue queue = Volley.newRequestQueue(this);
        String url = String.format("https://artistsearchbackend.wl.r.appspot.com/artists?id=%s", query);
        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                response -> {
                    try {
                        JSONObject artistDetails = new JSONObject(response);
                        artist = new Artist(artistDetails);
                        detailsFragment = DetailsFragment.newInstance(artist);
                    } catch (JSONException e) { throw new RuntimeException(e); }
                },
                error -> { throw new RuntimeException(error); });
        queue.add(stringRequest);
    }

    private void getArtworks(String query) {
        RequestQueue queue = Volley.newRequestQueue(this);
        String url = String.format("https://artistsearchbackend.wl.r.appspot.com/artworks?id=%s", query);
        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                response -> {
                    try {
                        JSONArray artistArtworks = new JSONArray(response);
                        artworksFragment = ArtworksFragment.newInstance(artistArtworks.toString());
                        setViewPagerAdapter(detailsFragment, artworksFragment);
                    } catch (JSONException e) { throw new RuntimeException(e); }
                },
                error -> {
                    error.printStackTrace();
                    throw new RuntimeException(error);
                });
        queue.add(stringRequest);
    }

    private void showToast(boolean favorited) {
        String message = (favorited) ? "%s is added to favorites" : "%s is removed from favorites";
        Toast toast = Toast.makeText(this, String.format(message, artist.getName()), Toast.LENGTH_SHORT);
        toast.show();
    }
}

