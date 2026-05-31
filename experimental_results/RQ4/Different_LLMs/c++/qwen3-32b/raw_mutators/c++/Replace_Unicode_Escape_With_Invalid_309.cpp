//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Unicode_Escape_With_Invalid_309
 */ 
class MutatorFrontendAction_309 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(309)

private:
    class MutatorASTConsumer_309 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_309(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Replace_Unicode_Escape_With_Invalid_309.h"

// ========================================================================================================
#define MUT309_OUTPUT 1

void MutatorFrontendAction_309::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SL = Result.Nodes.getNodeAs<clang::StringLiteral>("stringLiteral")) {
        if (!SL || !Result.Context->getSourceManager().isWrittenInMainFile(SL->getLocation()))
            return;

        std::string originalText = stringutils::rangetoStr(*Result.SourceManager, SL->getSourceRange());
        std::regex unicodeRegex(R"(\\(u|U)\{([0-9a-fA-F]+)\})", std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::smatch matches;
        std::string::const_iterator searchStart(originalText.begin());
        std::vector<std::string> foundEscapes;

        while (std::regex_search(searchStart, originalText.end(), matches, unicodeRegex)) {
            foundEscapes.push_back(matches[0]);
            searchStart = matches.suffix().first;
        }

        if (foundEscapes.empty())
            return;

        int index = getrandom::getRandomIndex(foundEscapes.size());
        std::string originalEscape = foundEscapes[index];
        std::string mutatedEscape;
        int mutationType = getrandom::getRandomIndex(2);

        if (mutationType == 0) {
            size_t hexStart = originalEscape.find("{") + 1;
            size_t hexEnd = originalEscape.find("}");
            std::string hexPart = originalEscape.substr(hexStart, hexEnd - hexStart);
            if (!hexPart.empty()) {
                size_t pos = getrandom::getRandomIndex(hexPart.size());
                std::string nonHexChars = "ghijklmnopqrstuvwxyz";
                char newChar = nonHexChars[getrandom::getRandomIndex(nonHexChars.size())];
                hexPart[pos] = newChar;
                mutatedEscape = originalEscape.substr(0, hexStart) + hexPart + originalEscape.substr(hexEnd);
            }
        } else {
            std::string modifiedBraces = originalEscape;
            size_t closingBracePos = modifiedBraces.find("}");
            if (closingBracePos != std::string::npos) {
                modifiedBraces[closingBracePos] = 'g';
                mutatedEscape = modifiedBraces;
            } else {
                mutatedEscape = originalEscape + "}";
            }
        }

        std::string mutatedText = originalText;
        size_t startPos = originalText.find(originalEscape);
        if (startPos != std::string::npos) {
            mutatedText.replace(startPos, originalEscape.length(), mutatedEscape);
        }

        Rewrite.ReplaceText(SL->getSourceRange(), mutatedText);
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CharacterLiteral>("stringLiteral")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;

        std::string originalText = stringutils::rangetoStr(*Result.SourceManager, CL->getSourceRange());
        std::regex unicodeRegex(R"(\\(u|U)\{([0-9a-fA-F]+)\})", std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::smatch matches;
        std::string::const_iterator searchStart(originalText.begin());
        std::vector<std::string> foundEscapes;

        while (std::regex_search(searchStart, originalText.end(), matches, unicodeRegex)) {
            foundEscapes.push_back(matches[0]);
            searchStart = matches.suffix().first;
        }

        if (foundEscapes.empty())
            return;

        int index = getrandom::getRandomIndex(foundEscapes.size());
        std::string originalEscape = foundEscapes[index];
        std::string mutatedEscape;
        int mutationType = getrandom::getRandomIndex(2);

        if (mutationType == 0) {
            size_t hexStart = originalEscape.find("{") + 1;
            size_t hexEnd = originalEscape.find("}");
            std::string hexPart = originalEscape.substr(hexStart, hexEnd - hexStart);
            if (!hexPart.empty()) {
                size_t pos = getrandom::getRandomIndex(hexPart.size());
                std::string nonHexChars = "ghijklmnopqrstuvwxyz";
                char newChar = nonHexChars[getrandom::getRandomIndex(nonHexChars.size())];
                hexPart[pos] = newChar;
                mutatedEscape = originalEscape.substr(0, hexStart) + hexPart + originalEscape.substr(hexEnd);
            }
        } else {
            std::string modifiedBraces = originalEscape;
            size_t closingBracePos = modifiedBraces.find("}");
            if (closingBracePos != std::string::npos) {
                modifiedBraces[closingBracePos] = 'g';
                mutatedEscape = modifiedBraces;
            } else {
                mutatedEscape = originalEscape + "}";
            }
        }

        std::string mutatedText = originalText;
        size_t startPos = originalText.find(originalEscape);
        if (startPos != std::string::npos) {
            mutatedText.replace(startPos, originalEscape.length(), mutatedEscape);
        }

        Rewrite.ReplaceText(CL->getSourceRange(), mutatedText);
    }
}

void MutatorFrontendAction_309::MutatorASTConsumer_309::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = (stringLiteral().bind("stringLiteral") || characterLiteral().bind("stringLiteral"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}