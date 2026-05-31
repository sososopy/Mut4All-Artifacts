//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Template_Deduction_Guide_280
 */ 
class MutatorFrontendAction_280 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(280)

private:
    class MutatorASTConsumer_280 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_280(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_280.h"

// ========================================================================================================
#define MUT280_OUTPUT 1

void MutatorFrontendAction_280::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto className = TD->getNameAsString();
      std::string deductionGuide = className + "(unknown_type) -> " + className + "<int>;";
      
      //Perform mutation on the source code text by applying string replacement
      SourceLocation insertLoc = TD->getEndLoc().getLocWithOffset(1);
      Rewrite.InsertText(insertLoc, "\n/*mut280*/" + deductionGuide, true, true);
    }
}
  
void MutatorFrontendAction_280::MutatorASTConsumer_280::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}