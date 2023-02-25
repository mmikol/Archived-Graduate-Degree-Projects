package com.example.artistsearch;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;
import androidx.viewpager2.adapter.FragmentStateAdapter;

public class InfoPagerAdapter extends FragmentStateAdapter {
    private Fragment[] fragments;


    public InfoPagerAdapter(@NonNull FragmentActivity fragmentActivity) {
        super(fragmentActivity);
    }

    @NonNull
    @Override
    public Fragment createFragment(int position) {
        return fragments[position];
    }

    @Override
    public int getItemCount() {
        return 2;
    }

    public void setData(Fragment[] fragments) {
        this.fragments = fragments;
    }
}
