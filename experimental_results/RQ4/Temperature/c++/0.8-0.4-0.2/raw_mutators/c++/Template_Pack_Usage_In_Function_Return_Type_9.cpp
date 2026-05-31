//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Pack_Usage_In_Function_Return_Type_9
 */ 
class MutatorFrontendAction_9 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(9)

private:
    class MutatorASTConsumer_9 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_9(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Template_Pack_Usage_In_Function_Return_Type_9.h"

// ========================================================================================================
#define MUT9_OUTPUT 1

void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 TD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCall = "Example<>::getValue<0>();";
      if (declaration.find("Example<int, double>::getValue<0>();") != std::string::npos) {
          declaration.replace(declaration.find("Example<int, double>::getValue<0>();"), 
                              std::string("Example<int, double>::getValue<0>();").length(), 
                              mutatedCall);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(hasName("Example")).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}