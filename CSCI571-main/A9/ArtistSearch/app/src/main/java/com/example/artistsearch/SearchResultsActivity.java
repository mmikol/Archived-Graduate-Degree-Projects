package com.example.artistsearch;

import android.os.Bundle;
import android.view.MenuItem;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import org.json.JSONArray;
import org.json.JSONException;
import java.util.ArrayList;

public class SearchResultsActivity extends AppCompatActivity {
    private ArrayList<ArtistSearchResult> artistSearchResults;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_spinner);
        Bundle extras = getIntent().getExtras();
        String query = extras.getString("query");
        setTitle(query.toUpperCase());
        getSearchResults(query);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            this.finish();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private void getSearchResults(String query) {
        RequestQueue queue = Volley.newRequestQueue(this);
        String url = String.format("https://artistsearchbackend.wl.r.appspot.com/search?q=%s", query);
        StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                response -> {
                    try {
                        JSONArray JSONResponse = new JSONArray(response);
                        artistSearchResults = new ArrayList<>();
                        for (int i = 0; i < JSONResponse.length(); i++) {
                            artistSearchResults.add(new ArtistSearchResult(JSONResponse.getJSONObject(i)));
                        }
                        showSearchResults();
                    } catch (JSONException e) {
                        e.printStackTrace();
                        throw new RuntimeException(e);
                    }
                },
                error -> {
                    error.printStackTrace();
                    throw new RuntimeException(error);
                });
        queue.add(stringRequest);
    }
    private void showSearchResults() {
        if (artistSearchResults != null && artistSearchResults.size() > 0) {
            setContentView(R.layout.recycler_view_search_results);
            RecyclerView recyclerView = findViewById(R.id.recycler_view_search_results);
            recyclerView.setLayoutManager(new LinearLayoutManager(this));
            SearchAdapter searchAdapter = new SearchAdapter(artistSearchResults);
            recyclerView.setAdapter(searchAdapter);
        } else {
            setContentView(R.layout.activity_empty_results);
            final TextView message = findViewById(R.id.empty_result_message);
            message.setText(R.string.no_search_results);
        }
    }
}
