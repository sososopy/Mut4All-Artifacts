//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Requires_Initialization_With_Requires_Expression_93
 */ 
class MutatorFrontendAction_93 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(93)

private:
    class MutatorASTConsumer_93 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_93(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_requires_initialization_with_requires_expression_93.h"

// ========================================================================================================
#define MUT93_OUTPUT 1

void MutatorFrontendAction_93::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("Var")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->getType()->isDependentType())
        return;
      if (!DL->getTypeSourceInfo()->getTypeLoc().getAs<DependentNameTypeLoc>())
        return;
      const auto *init = DL->getInit();
      if (!init)
        return;
      if (!DL->isConstexpr())
        return;
      if (!DL->isImplicit() && DL->isConstexpr() &&
          DL->getType()->isDependentType()) {
        //Get the source code text of target node
        auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                            DL->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        decl = "/*mut93*/" + decl;
        decl = decl.substr(0, decl.find('=')) + ';';
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), decl);
      }
    }
}
  
void MutatorFrontendAction_93::MutatorASTConsumer_93::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl().bind("Var");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}