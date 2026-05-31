//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Statement_433
 */ 
class MutatorFrontendAction_433 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(433)

private:
    class MutatorASTConsumer_433 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_433(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_statement_433.h"

// ========================================================================================================
#define MUT433_OUTPUT 1

void MutatorFrontendAction_433::Callback::run(const MatchFinder::MatchResult &Result) {
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
        std::string unreachableStmt = "\n/*mut433*/__builtin_unreachable();\n";
        bodyText.insert(bodyText.find("{") + 1, unreachableStmt);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
      }
    }
}
  
void MutatorFrontendAction_433::MutatorASTConsumer_433::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}