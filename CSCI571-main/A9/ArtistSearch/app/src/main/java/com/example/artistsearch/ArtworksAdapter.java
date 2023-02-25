package com.example.artistsearch;

import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TableLayout;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.squareup.picasso.Picasso;
import org.json.JSONArray;
import org.json.JSONException;

import java.util.ArrayList;

public class ArtworksAdapter extends RecyclerView.Adapter<ArtworksAdapter.ViewHolder> {
    private ItemClickListener clickListener;
    private final ArrayList<Artwork> data;

    public class ViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
        private final TextView artworkTitle;
        private final ImageView artworkImage;

        public ViewHolder(View view) {
            super(view);
            artworkTitle = view.findViewById(R.id.artwork_title);
            artworkImage = view.findViewById(R.id.artwork_image);
            itemView.setOnClickListener(this);
        }

        public TextView getArtworkTitleTextView() {
            return artworkTitle;
        }

        public ImageView getArtworkImageImageView() {
            return artworkImage;
        }

        @Override
        public void onClick(View view) {
            if (clickListener != null) {
                clickListener.onItemClick(view, getBindingAdapterPosition());
            }
        }
    }

    public ArtworksAdapter(ArrayList<Artwork> data) {
        this.data = data;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(ViewGroup viewGroup, int viewType) {
        View view = LayoutInflater.from(viewGroup.getContext())
                .inflate(R.layout.row_item_artwork, viewGroup, false);
        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(ViewHolder viewHolder, final int position) {
        String artworkTitle = data.get(position).getTitle();
        String artworkImage = data.get(position).getImage();
        String artworkID = data.get(position).getID();
        viewHolder.getArtworkTitleTextView().setText(artworkTitle);
        Picasso.get()
                .load(artworkImage)
                .resize(1000, 800)
                .into(viewHolder.getArtworkImageImageView());
        viewHolder.getArtworkImageImageView().setOnClickListener(view -> {
            Context context = view.getContext();
            RequestQueue queue = Volley.newRequestQueue(context);
            String url = String.format("https://artistsearchbackend.wl.r.appspot.com/genes?id=%s", artworkID);
            StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                    response -> {
                        try {
                            JSONArray JSONResponse = new JSONArray(response);
                            Dialog dialog = new Dialog(context);
                            dialog.setContentView(R.layout.dialogue_artwork_genes);
                            if (JSONResponse.length() > 0) {
                                Gene gene = new Gene(JSONResponse.getJSONObject(0));
                                TableLayout geneHeader = dialog.findViewById(R.id.gene_header);
                                TableLayout geneDetails = dialog.findViewById(R.id.gene_details);
                                TextView geneName = dialog.findViewById(R.id.gene_name);
                                TextView geneDescription = dialog.findViewById(R.id.gene_description);
                                ImageView geneImage = dialog.findViewById(R.id.gene_image);
                                geneName.setText(String.format("Category\n%s", gene.getName()));
                                geneDescription.setText(gene.getDescription());
                                Picasso.get()
                                        .load(gene.getImage())
                                        .resize(200, 200)
                                        .into(geneImage);
                                geneHeader.setVisibility(View.VISIBLE);
                                geneDetails.setVisibility(View.VISIBLE);
                            } else {
                                final TextView message = dialog.findViewById(R.id.empty_result_message);
                                message.setVisibility(View.VISIBLE);
                            }
                            dialog.show();
                        } catch (JSONException e) { throw new RuntimeException(e); }
                    },
                    error -> { throw new RuntimeException(error); });
            queue.add(stringRequest);












        });

    }

    @Override
    public int getItemCount() {
        return data.size();
    }

    Artwork getItem(int id) {
        return data.get(id);
    }

    void setClickListener(ItemClickListener clickListener) {
        this.clickListener = clickListener;
    }

    public interface ItemClickListener {
        void onItemClick(View view, int position);
    }

}
