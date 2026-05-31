//header file
#pragma once
#include "Mutator_base.h"

/**
 * enum_initialization_with_non_integral_expression_658
 */ 
class MutatorFrontendAction_658 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(658)

private:
    class MutatorASTConsumer_658 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_658(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/enum_initialization_with_non_integral_expression_658.h"

// ========================================================================================================
#define MUT658_OUTPUT 1

void MutatorFrontendAction_658::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;

      for (auto *EI : ED->enumerators()) {
        //Get the source code text of target node
        auto enumName = EI->getNameAsString();
        std::string newInit = "(\"" + enumName.substr(0, 1) + "\" * " + std::to_string(EI->getInitVal().getLimitedValue()) + ")";
        //Perform mutation on the source code text by applying string replacement
        Rewrite.ReplaceText(EI->getInitExpr()->getSourceRange(), newInit);
      }
    }
}
  
void MutatorFrontendAction_658::MutatorASTConsumer_658::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("enumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}