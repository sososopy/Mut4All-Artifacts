//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Typedef_Conflict_421
 */ 
class MutatorFrontendAction_421 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(421)

private:
    class MutatorASTConsumer_421 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_421(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> typedefNames;
    };
};

//source file
#include "../include/Mutator_Struct_Typedef_Conflict_421.h"

// ========================================================================================================
#define MUT421_OUTPUT 1

void MutatorFrontendAction_421::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("Typedef")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        std::string typedefName = TD->getNameAsString();
        typedefNames.insert(typedefName);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        for (const auto &typedefName : typedefNames) {
            std::string structDecl = "struct " + typedefName + " { int x; };";
            Rewrite.InsertTextAfterToken(FD->getBody()->getBeginLoc(), "\n/*mut421*/" + structDecl + "\n");
        }
    }
}
  
void MutatorFrontendAction_421::MutatorASTConsumer_421::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher typedefMatcher = typedefDecl().bind("Typedef");
    DeclarationMatcher functionMatcher = functionDecl(hasBody()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(typedefMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}