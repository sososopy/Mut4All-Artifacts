//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_FunctionLike_Macro_With_Invalid_266
 */ 
class MutatorFrontendAction_266 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(266)

private:
    class MutatorASTConsumer_266 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_266(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_FunctionLike_Macro_With_Invalid_266.h"

// ========================================================================================================
#define MUT266_OUTPUT 1

void MutatorFrontendAction_266::Callback::run(const MatchFinder::MatchResult &Result) {
    // Intentionally left empty - macro mutation occurs via source text processing
}

void MutatorFrontendAction_266::MutatorASTConsumer_266::HandleTranslationUnit(ASTContext &Context) {
    SourceManager &SM = Context.getSourceManager();
    const FileEntry *FE = SM.getFileEntryForID(SM.getMainFileID());
    if (!FE) return;

    std::string buffer;
    {
        std::ifstream t(FE->getName().str());
        buffer.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    }

    std::istringstream iss(buffer);
    std::string line;
    std::vector<std::pair<unsigned, unsigned>> macroDefs;

    unsigned lineNo = 0;
    while (std::getline(iss, line)) {
        lineNo++;
        if (line.find("#define") == 0) {
            size_t openParen = line.find('(');
            if (openParen != std::string::npos) {
                macroDefs.push_back({lineNo, openParen});
            }
        }
    }

    if (!macroDefs.empty()) {
        auto [lineNum, _] = macroDefs[0];
        SourceLocation loc = SM.getLocForLineCol(SM.getMainFileID(), lineNum);
        if (loc.isValid()) {
            CharSourceRange range = CharSourceRange::getLineRange(loc, SM);
            std::string macroLine = TheRewriter.getRewrittenText(range);
            
            std::istringstream macroLineStream(macroLine);
            std::string token;
            std::string macroName;
            int tokenCount = 0;
            
            while (macroLineStream >> token) {
                if (tokenCount == 1) {
                    macroName = token;
                    break;
                }
                tokenCount++;
            }
            
            if (!macroName.empty()) {
                std::string newMacro = "#define " + macroName + " #f";
                TheRewriter.ReplaceText(range, newMacro);
            }
        }
    }
}