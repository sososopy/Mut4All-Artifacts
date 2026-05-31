//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_407
 */ 
class MutatorFrontendAction_407 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(407)

private:
    class MutatorASTConsumer_407 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_407(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_code_407.h"

// ========================================================================================================
#define MUT407_OUTPUT 1

void MutatorFrontendAction_407::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
        return;
      if (FS->hasBody()) {
        //Get the source code text of target node
        auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FS->getBody()->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string unreachableCode = "\n/*mut407*/if (false) { throw \"Unreachable code executed\"; }\n";
        functionBody.insert(functionBody.find("{") + 1, unreachableCode);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getBody()->getSourceRange()), functionBody);
      }
    }
}
  
void MutatorFrontendAction_407::MutatorASTConsumer_407::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasBody()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}