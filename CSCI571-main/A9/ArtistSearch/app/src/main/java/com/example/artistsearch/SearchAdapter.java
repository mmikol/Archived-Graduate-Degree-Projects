package com.example.artistsearch;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;
import com.squareup.picasso.Picasso;

import java.util.ArrayList;

public class SearchAdapter extends RecyclerView.Adapter<SearchAdapter.ViewHolder> {
    private final ArrayList<ArtistSearchResult> data;

    public static class ViewHolder extends RecyclerView.ViewHolder {
        private final TextView artistName;
        private final ImageView artistImage;

        public ViewHolder(View view) {
            super(view);
            artistName = view.findViewById(R.id.artist_name);
            artistImage = view.findViewById(R.id.artist_image);
        }

        public TextView getArtistNameTextView() {
            return artistName;
        }

        public ImageView getArtistImageImageView() {
            return artistImage;
        }
    }

    public SearchAdapter(ArrayList<ArtistSearchResult> data) {
        this.data = data;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(ViewGroup viewGroup, int viewType) {
        View view = LayoutInflater.from(viewGroup.getContext())
                .inflate(R.layout.row_item_search_result, viewGroup, false);
        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(ViewHolder viewHolder, final int position) {
        String artistName = data.get(position).getName();
        String artistImage = data.get(position).getImage();
        String artistID = data.get(position).getID();
        viewHolder.getArtistNameTextView().setText(artistName);
        Picasso.get()
                .load(artistImage)
                .resize(700, 1100)
                .into(viewHolder.getArtistImageImageView());
        viewHolder.getArtistImageImageView().setOnClickListener(view -> {
            Context context = view.getContext();
            Intent infoIntent = new Intent(context, ArtistInfoActivity.class);
            SharedPreferences preferences = context.getSharedPreferences("favorite_artists", 0);
            infoIntent.putExtra("isFavoriteArtist", preferences.contains(artistID));
            infoIntent.putExtra("artistName", artistName);
            infoIntent.putExtra("artistID", artistID);
            context.startActivity(infoIntent);
        });
    }

    @Override
    public int getItemCount() {
        return data.size();
    }
}
