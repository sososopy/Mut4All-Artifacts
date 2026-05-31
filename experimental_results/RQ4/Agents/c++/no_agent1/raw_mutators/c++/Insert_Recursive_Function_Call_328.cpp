//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Function_Call_328
 */ 
class MutatorFrontendAction_328 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(328)

private:
    class MutatorASTConsumer_328 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_328(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_recursive_function_call_328.h"

// ========================================================================================================
#define MUT328_OUTPUT 1

void MutatorFrontendAction_328::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody() && !FD->isMain()) {
        auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FD->getBody()->getSourceRange());
        
        std::string recursiveCall = FD->getNameAsString() + "();";
        functionBody.insert(functionBody.find("{") + 1, "\n/*mut328*/" + recursiveCall + "\n");

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), functionBody);
      }
    }
}
  
void MutatorFrontendAction_328::MutatorASTConsumer_328::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isMain())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}