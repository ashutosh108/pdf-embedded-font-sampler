#include <iostream>
#include <memory>

#include "FontInfo.h"
#include "GlobalParams.h"
#include "PDFDoc.h"
#include "PDFDocFactory.h"

std::vector<std::unique_ptr<FontInfo>> get_embedded_fonts(PDFDoc &doc) {
	std::vector<std::unique_ptr<FontInfo>> fonts;

	int pages_count = doc.getNumPages();
	FontInfoScanner scanner(&doc);
	GooList *fonts_list;
	fonts_list = scanner.scan(pages_count);
	for (int i = 0; i < fonts_list->getLength(); ++i) {
		std::unique_ptr<FontInfo> font { (FontInfo *) fonts_list->get(i) };
		if (font->getEmbedded()) {
			fonts.push_back(std::move(font));
		}
	}
	delete fonts_list;

	return fonts;
}

void print_font_page(PDFDoc &doc, FontInfo const &f) {

}

void add_sampler_to_pdf(std::unique_ptr<PDFDoc> const & doc) {
	std::vector<std::unique_ptr<FontInfo> > fonts = get_embedded_fonts(*doc);
	std::cout << "Embedded fonts from " << doc->getFileName()->toStr() << " (" << fonts.size() << ")\n";
	for (auto& font : fonts) {
		std::cout << "Font id: " << font->getRef().num << ' ' << font->getRef().gen << '\n';
		print_font_page(*doc, *font);
	}
}

class Params {
public:
	~Params() {
		std::cout << "~Params()\n";
	}
};

template<typename T, typename... Args>
class RAIINullGlobalPointer {
public:
	// "T *&" is a reference to the pointer to T
	RAIINullGlobalPointer(T *&t, Args... a) : t_(t)  {
		t_ = new T(a...);
		u_ptr = std::unique_ptr<T>(t_);
	};

	~RAIINullGlobalPointer() {
		t_ = nullptr;
	}
private:
	T *&t_;
	std::unique_ptr<T> u_ptr;
};

void add_sampler_to_pdf_file(const char * filename) {
	RAIINullGlobalPointer<GlobalParams> g(globalParams);
	GooString filename_g(filename);
	std::unique_ptr<PDFDoc> doc(PDFDocFactory().createPDFDoc(filename_g));
	add_sampler_to_pdf(doc);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: pdf-font-sampler filename.pdf\n";
		exit(1);
	}
	add_sampler_to_pdf_file(argv[1]);
	return 0;
}
