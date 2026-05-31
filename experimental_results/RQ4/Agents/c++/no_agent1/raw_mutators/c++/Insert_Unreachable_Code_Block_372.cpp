//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_Block_372
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
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/insert_unreachable_code_block_372.h"

// ========================================================================================================
#define MUT372_OUTPUT 1

void MutatorFrontendAction_372::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
        return;
      if (FS->hasBody()) {
        //Get the source code text of target node
        auto body = FS->getBody();
        auto bodyText = stringutils::rangetoStr(*(Result.SourceManager),
                                                body->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string unreachableCode = "\n/*mut372*/if (false) { int unreachable = 0; }";
        bodyText.insert(bodyText.find("{") + 1, unreachableCode);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
      }
    }
}
  
void MutatorFrontendAction_372::MutatorASTConsumer_372::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}