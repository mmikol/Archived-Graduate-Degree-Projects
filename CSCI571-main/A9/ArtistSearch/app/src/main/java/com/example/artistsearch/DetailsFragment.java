package com.example.artistsearch;

import android.os.Bundle;
import android.widget.TableLayout;
import android.widget.TextView;
import androidx.fragment.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class DetailsFragment extends Fragment {
    private static final String NAME = "name";
    private static final String NATIONALITY = "nationality";
    private static final String BIRTHDAY = "birthday";
    private static final String DEATHDAY = "deathday";
    private static final String BIOGRAPHY = "biography";

    private String artistName;
    private String artistNationality;
    private String artistBirthday;
    private String artistDeathday;
    private String artistBiography;

    public DetailsFragment() {}

    public static DetailsFragment newInstance(Artist artist) {
        DetailsFragment fragment = new DetailsFragment();
        Bundle args = new Bundle();
        args.putString(NAME, artist.getName());
        args.putString(NATIONALITY, artist.getNationality());
        args.putString(BIRTHDAY, artist.getBirthday());
        args.putString(DEATHDAY, artist.getDeathday());
        args.putString(BIOGRAPHY, artist.getBiography());
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            artistName = getArguments().getString(NAME);
            artistNationality = getArguments().getString(NATIONALITY);
            artistBirthday = getArguments().getString(BIRTHDAY);
            artistDeathday = getArguments().getString(DEATHDAY);
            artistBiography = getArguments().getString(BIOGRAPHY);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_details, container, false);
        if (getArguments() != null) {
            showArtistDetails(view);
        }
        return view;
    }

    private void showArtistDetails(View view) {
        view.findViewById(R.id.details_progress_bar).setVisibility(View.GONE);
        view.findViewById(R.id.details_progress_text).setVisibility(View.GONE);
        TableLayout tableLayout = view.findViewById(R.id.artist_details);
        tableLayout.setVisibility(View.VISIBLE);
        ((TextView)view.findViewById(R.id.artist_name)).setText(artistName);
        ((TextView)view.findViewById(R.id.artist_nationality)).setText(artistNationality);
        ((TextView)view.findViewById(R.id.artist_birthday)).setText(artistBirthday);
        ((TextView)view.findViewById(R.id.artist_deathday)).setText(artistDeathday);
        ((TextView)view.findViewById(R.id.artist_biography)).setText(artistBiography);
    }
}