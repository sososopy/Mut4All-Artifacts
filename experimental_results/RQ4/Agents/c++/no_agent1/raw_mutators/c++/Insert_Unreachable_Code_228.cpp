//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_228
 */ 
class MutatorFrontendAction_228 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(228)

private:
    class MutatorASTConsumer_228 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_228(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_code_228.h"

// ========================================================================================================
#define MUT228_OUTPUT 1

void MutatorFrontendAction_228::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
        return;

      if (FS->hasBody()) {
        //Get the source code text of target node
        auto body = stringutils::rangetoStr(*(Result.SourceManager),
                                            FS->getBody()->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string unreachable_code = "\n/*mut228*/if(false) { while(true) {} }\n";
        body.insert(body.find("{") + 1, unreachable_code);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getBody()->getSourceRange()), body);
      }
    }
}

void MutatorFrontendAction_228::MutatorASTConsumer_228::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}