//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_255
 */ 
class MutatorFrontendAction_255 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(255)

private:
    class MutatorASTConsumer_255 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_255(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_code_255.h"

// ========================================================================================================
#define MUT255_OUTPUT 1

void MutatorFrontendAction_255::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
        return;

      if (FS->hasBody()) {
        //Get the source code text of target node
        auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FS->getBody()->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string unreachableCode = "\n/*mut255*/if(false){while(true){}}";
        functionBody.insert(functionBody.find("{") + 1, unreachableCode);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getBody()->getSourceRange()), functionBody);
      }
    }
}
  
void MutatorFrontendAction_255::MutatorASTConsumer_255::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}