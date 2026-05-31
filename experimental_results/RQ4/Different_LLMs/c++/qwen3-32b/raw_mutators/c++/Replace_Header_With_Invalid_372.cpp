//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Header_With_Invalid_372
 */ 
class MutatorFrontendAction_372 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(372)

private:
    class MutatorASTConsumer_372 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_372(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
        bool hasTemplateOrConcept = false;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Replace_Header_With_Invalid_372.h"

// ========================================================================================================
#define MUT372_OUTPUT 1

void MutatorFrontendAction_372::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::Decl>("decl")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       MT->getLocation()))
            return;
        hasTemplateOrConcept = true;
    }
}

void MutatorFrontendAction_372::MutatorASTConsumer_372::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = (templateDecl() || conceptDefinitionDecl()).bind("decl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);

    if (callback.hasTemplateOrConcept) {
        SourceManager &SM = Context.getSourceManager();
        FileID mainFile = SM.getMainFileID();
        if (!mainFile.isValid())
            return;

        StringRef buffer = SM.getBufferData(mainFile);
        const char *bufferStart = buffer.data();
        size_t bufferLength = buffer.size();

        const char *includeStart = std::strstr(bufferStart, "#include");
        if (includeStart) {
            const char *lineEnd = includeStart;
            while (lineEnd < bufferStart + bufferLength && *lineEnd != '\n' && *lineEnd != '\r') {
                ++lineEnd;
            }

            size_t startOffset = includeStart - bufferStart;
            size_t endOffset = lineEnd - bufferStart;

            SourceLocation startLoc = SM.getLocForStartOfFile(mainFile).getLocWithOffset(startOffset);
            SourceLocation endLoc = SM.getLocForStartOfFile(mainFile).getLocWithOffset(endOffset);

            TheRewriter.ReplaceText(SourceRange(startLoc, endLoc), "#include <invalid_header>");
        }
    }
}