//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Doxygen_Self_In_Non_Class_Context_342
 */ 
class MutatorFrontendAction_342 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(342)

private:
    class MutatorASTConsumer_342 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_342(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalid_Doxygen_Self_In_Non_Class_Context_342.h"

// ========================================================================================================
#define MUT342_OUTPUT 1

void MutatorFrontendAction_342::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *decl = Result.Nodes.getNodeAs<clang::Decl>("decl")) {
        if (!decl || !Result.Context->getSourceManager().isWrittenInMainFile(decl->getLocation()))
            return;

        const DeclContext *ctx = decl->getDeclContext();
        bool insideClass = false;
        while (ctx) {
            if (ctx->getDeclContext() && ctx->getDeclContext()->isRecord()) {
                insideClass = true;
                break;
            }
            ctx = ctx->getParent();
        }
        if (insideClass)
            return;

        const RawComment *comment = decl->getRawComment();
        if (!comment)
            return;

        SourceRange commentRange = comment->getSourceRange();
        if (commentRange.isInvalid())
            return;

        std::string originalText = Lexer::getSourceText(CharSourceRange::getCharRange(commentRange), *Result.SourceManager, getLangOpts());
        if (originalText.find("///") != 0 && originalText.find("/**") != 0)
            return;

        std::string mutatedText = "/// Adds two @doc.self values using bitwise operations\n";
        Rewrite.ReplaceText(commentRange, mutatedText);
    }
}

void MutatorFrontendAction_342::MutatorASTConsumer_342::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = 
        (functionDecl(unless(cxxMethodDecl())).bind("decl") || 
         varDecl(unless(isCXXClassMember())).bind("decl") || 
         namespaceDecl().bind("decl"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}