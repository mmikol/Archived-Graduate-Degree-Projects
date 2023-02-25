package com.example.artistsearch;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;
import io.github.luizgrp.sectionedrecyclerviewadapter.Section;
import io.github.luizgrp.sectionedrecyclerviewadapter.SectionParameters;

import java.util.List;

class FavoritesSection extends Section {
    private final String title;
    private final List<Artist> artists;

    public FavoritesSection(@NonNull final String title, @NonNull final List<Artist> artists) {
        super(SectionParameters.builder()
                .itemResourceId(R.layout.section_item_favorites)
                .headerResourceId(R.layout.section_header_favorites)
                .build());
        this.title = title;
        this.artists = artists;
    }

    public static class HeaderViewHolder extends RecyclerView.ViewHolder {
        private final TextView header;

        public HeaderViewHolder(View view) {
            super(view);
            header = view.findViewById(R.id.favorites_header);
        }

        public TextView getHeaderTextView() {
            return header;
        }
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        private final TextView artistName;
        private final TextView artistBirthday;
        private final TextView artistNationality;
        private final ImageView icon;

        public ViewHolder(View view) {
            super(view);
            artistName = view.findViewById(R.id.artist_name);
            artistBirthday = view.findViewById(R.id.artist_birthday);
            artistNationality = view.findViewById(R.id.artist_nationality);
            icon = view.findViewById(R.id.favorites_icon);
        }

        public TextView getArtistNameTextView() {
            return artistName;
        }

        public TextView getArtistBirthdayTextView() {
            return artistBirthday;
        }

        public TextView getArtistNationalityTextView() {
            return artistNationality;
        }

        public ImageView getIconImageView() {
            return icon;
        }

    }

    @Override
    public int getContentItemsTotal() {
        return artists.size();
    }

    @Override
    public RecyclerView.ViewHolder getItemViewHolder(View view) {
        return new ViewHolder(view);
    }

    @Override
    public void onBindItemViewHolder(RecyclerView.ViewHolder holder, int position) {
        ViewHolder itemHolder = (ViewHolder) holder;
        itemHolder.getArtistNameTextView().setText(artists.get(position).getName());
        itemHolder.getArtistBirthdayTextView().setText(artists.get(position).getBirthday());
        itemHolder.getArtistNationalityTextView().setText(artists.get(position).getNationality());
        itemHolder.getIconImageView().setImageResource(R.drawable.ic_action_view_favorite);
        itemHolder.getIconImageView().setOnClickListener(view -> {
            Context context = view.getContext();
            String artistName = artists.get(position).getName();
            String artistID = artists.get(position).getID();
            try {
                Intent infoIntent = new Intent(context, ArtistInfoActivity.class);
                SharedPreferences preferences = context.getSharedPreferences("favorite_artists", 0);
                infoIntent.putExtra("isFavoriteArtist", preferences.contains(artistID));
                infoIntent.putExtra("artistName", artistName);
                infoIntent.putExtra("artistID", artistID);
                context.startActivity(infoIntent);
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
        });
    }

    @Override
    public void onBindHeaderViewHolder(final RecyclerView.ViewHolder holder) {
        final HeaderViewHolder headerHolder = (HeaderViewHolder) holder;
        headerHolder.getHeaderTextView().setText(title);
    }
    @Override
    public RecyclerView.ViewHolder getHeaderViewHolder(View view) {
        return new HeaderViewHolder(view);
    }
}