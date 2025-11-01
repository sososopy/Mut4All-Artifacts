//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Requires_With_Lambda_108
 */ 
class MutatorFrontendAction_108 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(108)

private:
    class MutatorASTConsumer_108 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_108(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Requires_With_Lambda_108.h"

// ========================================================================================================
#define MUT108_OUTPUT 1

void MutatorFrontendAction_108::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;

      if (auto *RC = FT->getTemplatedDecl()->getRequiresClause()) {
        //Get the source code text of target node
        auto requiresClause = stringutils::rangetoStr(*(Result.SourceManager),
                                                      RC->getSourceRange());
        auto functionBodyStart = FT->getTemplatedDecl()->getBody()->getBeginLoc();
        
        //Perform mutation on the source code text by applying string replacement
        std::string lambdaReplacement = "[] { static_assert(" + requiresClause + ", \"Constraint failed\"); }();\n";
        Rewrite.InsertTextBefore(functionBodyStart, lambdaReplacement);
        
        //Replace the original AST node with the mutated one
        Rewrite.RemoveText(RC->getSourceRange());
      }
    }
}
  
void MutatorFrontendAction_108::MutatorASTConsumer_108::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasRequiresClause()).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}