//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Constrained_Constructor_Template_127
 */ 
class MutatorFrontendAction_127 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(127)

private:
    class MutatorASTConsumer_127 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_127(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_constrained_constructor_template_127.h"

// ========================================================================================================
#define MUT127_OUTPUT 1

void MutatorFrontendAction_127::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;

      if (CD->getDescribedFunctionTemplate() && CD->getConstraints()) {
        //Get the source code text of target node
        auto constraintExpr = CD->getConstraints()->getSourceRange();
        auto constraintText = stringutils::rangetoStr(*(Result.SourceManager), constraintExpr);
        
        //Perform mutation on the source code text by applying string replacement
        std::string newConstraint = constraintText + " && sizeof...(args) > 1";
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(constraintExpr), newConstraint);
      }
    }
}
  
void MutatorFrontendAction_127::MutatorASTConsumer_127::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(isTemplateInstantiation(), hasConstraints()).bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}