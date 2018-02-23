

import edu.uci.ics.crawler4j.crawler.WebCrawler;

import com.google.common.io.Files;
import java.io.File;
import java.util.Set;
import java.util.regex.Pattern;

import org.apache.http.Header;
import org.apache.http.HttpStatus;
import org.apache.poi.ss.usermodel.Workbook;

import edu.uci.ics.crawler4j.crawler.Page;
import edu.uci.ics.crawler4j.crawler.WebCrawler;
import edu.uci.ics.crawler4j.fetcher.PageFetchResult;
import edu.uci.ics.crawler4j.parser.HtmlParseData;
import edu.uci.ics.crawler4j.url.WebURL;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Set;
import java.util.regex.Pattern;

public class MyCrawler extends WebCrawler {
	
	private final static Pattern FILTERS = Pattern.compile(".*(\\.(css|js" + "|mp3|mp3|zip|gz))$");
	
	CrawlState crawlState;

    public MyCrawler() {
        crawlState = new CrawlState();
    }
	
    private static File storageFolder;

    public static void configure(String storageFolderName) {
        storageFolder = new File(storageFolderName);
        if (!storageFolder.exists()) {
            storageFolder.mkdirs();
        }
    }

	
	 @Override
	    public boolean shouldVisit(Page page, WebURL url) {
	        String href = url.getURL().toLowerCase();
	        String type = "OutNews";
	        if (href.startsWith("http://www.bostonglobe.com")||href.startsWith("https://www.bostonglobe.com") ) {
	            type = "OK";
	        } 
	        else if (!href.startsWith("https://www.bostonglobe.com")||!href.startsWith("http://www.bostonglobe.com") ) {
	            type = "N_OK";
	        }
	        System.out.println("discoveredURLs $$ "+url);
	        crawlState.discoveredUrls.add(new UrlInfo(href, type));
	        return !FILTERS.matcher(href).matches() && type.equals("OK" ) && (href.startsWith("http://www.bostonglobe.com") || href.startsWith("https://www.bostonglobe.com"));
	    }
	
	/* This function is called when a page is fetched and ready * to be processed by your program. */
	 @Override
	    public void visit(Page page) {
	        String url = page.getWebURL().getURL();
	        String contentType = page.getContentType().split(";")[0];
	        System.out.println("contentType $$ "+contentType);
	        ArrayList<String> outgoingUrls = new ArrayList<String>();
	        System.out.println("URL: " + url);

	        
	        UrlInfo urlInfo;
	        if (contentType.equals("text/html")) { // html
	            if (page.getParseData() instanceof HtmlParseData) {
	                HtmlParseData htmlParseData = (HtmlParseData) page.getParseData();
	                Set<WebURL> links = htmlParseData.getOutgoingUrls();
	                for (WebURL link : links) {
	                    outgoingUrls.add(link.getURL());
	                }
	                urlInfo = new UrlInfo(url, page.getContentData().length, outgoingUrls, "text/html", ".html");
	                crawlState.visitedUrls.add(urlInfo);
	            } else {
	                urlInfo = new UrlInfo(url, page.getContentData().length, outgoingUrls, "text/html", ".html");
	                crawlState.visitedUrls.add(urlInfo);
	            }
	        } else if (contentType.equals("application/msword")) { // doc
	            urlInfo = new UrlInfo(url, page.getContentData().length, outgoingUrls, "application/msword", ".doc");
	            crawlState.visitedUrls.add(urlInfo);
	        } else if (contentType.equals("application/pdf")) { // pdf
	            urlInfo = new UrlInfo(url, page.getContentData().length, outgoingUrls, "application/pdf", ".pdf");
	            crawlState.visitedUrls.add(urlInfo);
	        } else if (contentType.equals("application/vnd.openxmlformats-officedocument.wordprocessingml.document")) {
	            urlInfo = new UrlInfo(url, page.getContentData().length, outgoingUrls, "application/vnd.openxmlformats-officedocument.wordprocessingml.document", ".docx");
	            crawlState.visitedUrls.add(urlInfo);
	            
	        }else if (contentType.equals("image/gif")) {
	            urlInfo = new UrlInfo(url, page.getContentData().length, outgoingUrls, "image/gif", ".gif");
	            crawlState.visitedUrls.add(urlInfo); 
	        }else if (contentType.equals("image/jpeg")) {
	            urlInfo = new UrlInfo(url, page.getContentData().length, outgoingUrls, "image/jpeg", ".jpeg");
	            crawlState.visitedUrls.add(urlInfo); 
	        }else if (contentType.equals("image/png")) {
	            urlInfo = new UrlInfo(url, page.getContentData().length, outgoingUrls, "image/png", ".png");
	            crawlState.visitedUrls.add(urlInfo); 
	        } else {
	            urlInfo = new UrlInfo(url, page.getContentData().length, outgoingUrls, "unknown", "");
	            crawlState.visitedUrls.add(urlInfo);
	        }

	        if (!urlInfo.extension.equals("")) {
	            String filename = storageFolder.getAbsolutePath() + "/" + urlInfo.hash + urlInfo.extension;
	            try {
	                Files.write(page.getContentData(), new File(filename));
	            } catch (IOException iox) {
	                System.out.println("Failed to write file: " + filename);
	            }
	        }
	    }
	
	 @Override
	    protected void handlePageStatusCode(WebURL webUrl, int statusCode, String statusDescription) {
	        crawlState.attemptUrls.add(new UrlInfo(webUrl.getURL(), statusCode));
	    }

	    @Override
	    public Object getMyLocalData() {
	        return crawlState;
	    }
}
