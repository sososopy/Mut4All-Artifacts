//header file
#pragma once
#include "Mutator_base.h"

/**
 * Embed_Resource_In_Compound_Literal_393
 */ 
class MutatorFrontendAction_393 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(393)

private:
    class MutatorASTConsumer_393 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_393(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/embed_resource_in_compound_literal_393.h"

// ========================================================================================================
#define MUT393_OUTPUT 1

void MutatorFrontendAction_393::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CompoundLiteralExpr>("compoundLiteral")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        const auto *InitList = llvm::dyn_cast<clang::InitListExpr>(CE->getInitializer());
        if (!InitList)
            return;

        std::string typeStr = CE->getType().getAsString();
        std::string resourceName = "resource_" + std::to_string(CE->getBeginLoc().getRawEncoding());
        std::string resourceDecl = "static " + typeStr + " " + resourceName + " = ";

        std::string initListStr = stringutils::rangetoStr(*(Result.SourceManager), InitList->getSourceRange());
        resourceDecl += initListStr + ";";

        Rewrite.InsertTextBefore(CE->getBeginLoc(), resourceDecl + "\n");
        Rewrite.ReplaceText(CE->getSourceRange(), resourceName);
    }
}

void MutatorFrontendAction_393::MutatorASTConsumer_393::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = compoundLiteralExpr().bind("compoundLiteral");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}