#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <random>
#include <curl/curl.h>
#include <emscripten/fetch.h> //For webassembly

#define cimg_display 0
#include "CImg.h"

using namespace std;
using namespace cimg_library;

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

void downloadSucceeded(emscripten_fetch_t *fetch) {
  printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);
  // The data is now available at fetch->data[0] through fetch->data[fetch->numBytes-1];
  emscripten_fetch_close(fetch); // Free data associated with the fetch.
}

void downloadFailed(emscripten_fetch_t *fetch) {
  printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
  emscripten_fetch_close(fetch); // Also free data on failure.
}

int main(int argc, char *argv[])
{
    std::string s;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> dist(0,1);
    std::cout << "Enter phrase.\n";
    std::getline(std::cin, s);
    for(int i = 0; i < s.size(); ++i)
    {
        if(dist(generator) == 1) s[i] = toupper(s[i]);
    }
    std::cout << s << "\n";

    /************************************/

    char const *url = "https://i.kym-cdn.com/photos/images/original/001/255/479/85b.png";
    CURL *curl_handle;
    static const char *pagefilename = "meme.png";
    FILE *pagefile;
    
    if(argc > 1) {
        url = argv[1];
    }
    
    // curl_global_init(CURL_GLOBAL_ALL);
    
    // /* init the curl session */ 
    // curl_handle = curl_easy_init();
    
    // /* set URL to get here */ 
    // curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    
    // /* Switch on full protocol/debug output while testing */ 
    // curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
    
    // /* disable progress meter, set to 0L to enable it */ 
    // curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    
    // /* send all data to this function  */ 
    // curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
    
    // /* open the file */ 
    // pagefile = fopen(pagefilename, "wb");
    // if(pagefile) {
    
    //     /* write the page body to this file handle */ 
    //     curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
    
    //     /* get it! */ 
    //     curl_easy_perform(curl_handle);
    
    //     /* close the header file */ 
    //     fclose(pagefile);
    // }
    
    // /* cleanup curl stuff */ 
    // curl_easy_cleanup(curl_handle);
    
    // curl_global_cleanup();

    /************************************/
    // This part is for webassembly
    
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = downloadSucceeded;
    attr.onerror = downloadFailed;
    emscripten_fetch(&attr, url);



    /************************************/

    // CImg<unsigned char> image("meme.png");
    // int width = image.width();
    // int height = image.height();
    // cout << "*******************************************************************\n";
    // cout << "IMAGE DIMS: " << height << " x " << width << "\n";
    // int text_w, text_h;
    // unsigned char black[] = {0,0,0};
    // unsigned char white[] = {255,255,255};
    // bool centered = false;
    // int font = 50;
    // vector<pair<string, int>> chunks;
    // int count = 0;
    // while(!centered)
    // {
    //     CImg<unsigned char> imgtext;
    //     cout << "FONT SIZE: " << font << "\n";
    //     imgtext.draw_text(0, 0, s.c_str(), white, 0, 1, font);
    //     text_w = imgtext.width();
    //     text_h = imgtext.height();
    //     if(text_w > width - 16 && count < 10)
    //     {
    //         // Ratio of width of image to width of text (before splitting it)
    //         float fraction_over = (float) (width - 10) / (float) text_w;
    //         int cut_off = (int) (fraction_over * (float) s.size());

    //         // Check if number of chunks times text height is greater than the height of region of interest
    //         if((int) (1/fraction_over) * text_h + 1 > (height - text_h * 2))
    //         {
    //             // Ratio of heigh of image to height of text
    //             float height_amount_over = (float) (height) / ((int) (1/fraction_over) * text_h + 1);
    //             font = (int) (height_amount_over * font);
    //         }

    //         else
    //         {
    //             string temp = "" + s[0];
    //             for(int i = 1; i < s.size(); ++i)
    //             {
    //                 if(i%cut_off == 0)
    //                 {
    //                     CImg<unsigned char> chunktext;
    //                     chunktext.draw_text(0, 0, temp.c_str(), white, 0, 1, font);
    //                     int w = chunktext.width();
    //                     chunks.push_back(make_pair(temp, w));
    //                     temp = "";
    //                 }
    //                 temp += s[i];
    //             }
    //             CImg<unsigned char> chunktext;
    //             chunktext.draw_text(0, 0, temp.c_str(), white, 0, 1, font);
    //             int w = chunktext.width();
    //             chunks.push_back(make_pair(temp, w));
    //             centered = true;
    //         }
    //     }
    //     else 
    //     {
    //         chunks.push_back(make_pair(s, text_w));
    //         centered = true;
    //     }
    //     ++count;
    // }

    // cout << "NUMBER OF CHUNKS: " << chunks.size() << "\n";
    // cout << "CHUNKS:" << "\n";
    // for(auto chunk : chunks)
    // {
    //     cout << chunk.first << "\n";
    // }

    // for(int i = 0; i < chunks.size(); ++i)
    // {
    //     image.draw_rectangle(width/2 - chunks[i].second/2, height-text_h-(chunks.size()-i)*text_h, width/2 + chunks[i].second/2, height-text_h-(chunks.size()-i-1)*text_h, black, 0.5);
    //     image.draw_text(width/2 - chunks[i].second/2, height-text_h-(chunks.size()-i)*text_h, chunks[i].first.c_str(), white, 0, 1, font);
    // }
    // image.save_png("meme2.png");

}