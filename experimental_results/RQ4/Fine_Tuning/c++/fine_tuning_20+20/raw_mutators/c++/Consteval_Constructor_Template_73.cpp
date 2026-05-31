//header file
#pragma once
#include "Mutator_base.h"

/**
 * Consteval_Constructor_Template_73
 */ 
class MutatorFrontendAction_73 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(73)

private:
    class MutatorASTConsumer_73 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_73(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/consteval_constructor_template_73.h"

// ========================================================================================================
#define MUT73_OUTPUT 1

void MutatorFrontendAction_73::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isTemplated() && MT->isExplicitlyDefaulted() == false &&
          MT->isExplicitlyDeleted() == false && MT->isConstexpr() == false) {
        //Get the source code text of target node
        auto method = stringutils::rangetoStr(*(Result.SourceManager),
                                              MT->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        method = "/*mut73*/consteval " + method;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), method);
      }
    }
}
  
void MutatorFrontendAction_73::MutatorASTConsumer_73::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl().bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}