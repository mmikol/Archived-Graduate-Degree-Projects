package com.example.artistsearch;

import android.os.Bundle;
import android.widget.TextView;
import androidx.fragment.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import androidx.recyclerview.widget.DividerItemDecoration;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import org.json.JSONArray;
import org.json.JSONException;
import java.util.ArrayList;

public class ArtworksFragment extends Fragment  {
    private static final String ARTWORKS_JSON_ARRAY = "artworks_json_array";
    private ArrayList<Artwork> artworks;

    public static ArtworksFragment newInstance(String artworksJSONArray) {
        ArtworksFragment fragment = new ArtworksFragment();
        Bundle args = new Bundle();
        args.putString(ARTWORKS_JSON_ARRAY, artworksJSONArray);
        fragment.setArguments(args);
        return fragment;
    }

    public ArtworksFragment() {}

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            String artworksJSONArrayString = getArguments().getString(ARTWORKS_JSON_ARRAY);
            try {
                JSONArray artworksJSONArray = new JSONArray(artworksJSONArrayString);
                artworks = new ArrayList<>();
                for (int i = 0; i < artworksJSONArray.length(); i++) {
                    artworks.add(new Artwork(artworksJSONArray.getJSONObject(i)));
                }
            } catch (JSONException e) {
                throw new RuntimeException(e);
            }
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_artworks, container, false);
        if (getArguments() != null) {
            showArtworks(view);
        }
        return view;
    }

    private void showArtworks(View view) {
        if (artworks != null && artworks.size() > 0) {
            RecyclerView artworksRecyclerView = view.findViewById(R.id.recycler_view_artworks);
            artworksRecyclerView.setLayoutManager(new LinearLayoutManager(getActivity()));
            artworksRecyclerView.addItemDecoration(new DividerItemDecoration(artworksRecyclerView.getContext(), DividerItemDecoration.VERTICAL));
            ArtworksAdapter artworksAdapter = new ArtworksAdapter(artworks);
            artworksRecyclerView.setAdapter(artworksAdapter);
        } else {
            final TextView message = view.findViewById(R.id.empty_result_message);
            message.setVisibility(View.VISIBLE);
        }
    }
}