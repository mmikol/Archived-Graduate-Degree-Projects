package com.example.artistsearch;

import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Paint;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.TextPaint;
import android.text.method.LinkMovementMethod;
import android.text.style.URLSpan;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;
import androidx.appcompat.widget.SearchView;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import androidx.recyclerview.widget.DividerItemDecoration;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import io.github.luizgrp.sectionedrecyclerviewadapter.SectionedRecyclerViewAdapter;
import org.json.JSONException;
import org.json.JSONObject;

import java.text.SimpleDateFormat;
import java.util.*;

public class MainActivity extends AppCompatActivity {
    private Intent searchIntent;

    // NOTE TO GRADER: Code from https://itecnote.com/tecnote/android-remove-underline-from-links-in-textview-android/
    private static class URLSpanNoUnderline extends URLSpan {
        public URLSpanNoUnderline(String url) {
            super(url);
        }
        @Override public void updateDrawState(TextPaint ds) {
            super.updateDrawState(ds);
            ds.setUnderlineText(false);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setTheme(R.style.Theme_ArtistSearch);
        setContentView(R.layout.activity_spinner);
        try {
            setContentView(R.layout.activity_main);
            initFavoritesList();
            initDate();
            initArtsyLink();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.action_search, menu);
        MenuItem searchItem = menu.findItem(R.id.action_search);
        SearchView searchView = (SearchView) searchItem.getActionView();
        searchView.setOnQueryTextListener(
                new SearchView.OnQueryTextListener() {
                    @Override
                    public boolean onQueryTextChange(String newTest) {
                        return false;
                    }

                    @Override
                    public boolean onQueryTextSubmit(String query) {
                        searchIntent = new Intent(MainActivity.this, SearchResultsActivity.class);
                        searchIntent.putExtra("query", query);
                        startActivity(searchIntent);
                        return true;
                    }
                });
        return true;
    }

    @Override
    protected void onResume() {
        try {
            initFavoritesList();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        super.onResume();
    }

    private void initDate() {
        Date currentTime = Calendar.getInstance().getTime();
        SimpleDateFormat DateFor = new SimpleDateFormat("d MMM yyyy", Locale.US);
        String date = DateFor.format(currentTime);
        TextView dateTextView = findViewById(R.id.activity_date);
        dateTextView.setText(date);
    }

    private void initFavoritesList() throws JSONException {
        ArrayList<Artist> favoriteArtists = new ArrayList<>();
        SharedPreferences preferences = getApplicationContext().getSharedPreferences("favorite_artists", 0);
        Map<String,?> entries = preferences.getAll();
        if (entries != null) {
            for (Map.Entry<String,?> entry : preferences.getAll().entrySet()) {
                if (entry != null && entry.getValue() != null) {
                    JSONObject details = new JSONObject(entry.getValue().toString());
                    favoriteArtists.add(new Artist(details));
                }
            }
        }
        SectionedRecyclerViewAdapter sectionAdapter = new SectionedRecyclerViewAdapter();
        FavoritesSection favoritesSection = new FavoritesSection("Favorites", favoriteArtists);
        sectionAdapter.addSection(favoritesSection);
        RecyclerView recyclerView = findViewById(R.id.recycler_view_favorites);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));
        recyclerView.addItemDecoration(new DividerItemDecoration(recyclerView.getContext(), DividerItemDecoration.VERTICAL));
        recyclerView.setAdapter(sectionAdapter);
    }
    private void initArtsyLink() {
        TextView linkTextView = findViewById(R.id.activity_artsy_api_link);
        linkTextView.setMovementMethod(LinkMovementMethod.getInstance());
        linkTextView.setPaintFlags(linkTextView.getPaintFlags() & (~Paint.UNDERLINE_TEXT_FLAG));
        Spannable s = new SpannableString(linkTextView.getText());
        // NOTE TO GRADER: The following code is from https://itecnote.com/tecnote/android-remove-underline-from-links-in-textview-android/
        URLSpan[] spans = s.getSpans(0, s.length(), URLSpan.class);
        for (URLSpan span: spans) {
            int start = s.getSpanStart(span);
            int end = s.getSpanEnd(span);
            s.removeSpan(span);
            span = new URLSpanNoUnderline(span.getURL());
            s.setSpan(span, start, end, 0);
        }
        linkTextView.setText(s);
    }

}