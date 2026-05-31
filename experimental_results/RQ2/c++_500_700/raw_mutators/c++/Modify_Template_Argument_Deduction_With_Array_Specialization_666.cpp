//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_argument_deduction_with_array_specialization_666
 */ 
class MutatorFrontendAction_666 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(666)

private:
    class MutatorASTConsumer_666 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_666(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_argument_deduction_with_array_specialization_666.h"

// ========================================================================================================
#define MUT666_OUTPUT 1

void MutatorFrontendAction_666::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto templateDecl = stringutils::rangetoStr(*(Result.SourceManager),
                                                  FTD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t pos = templateDecl.find("template<");
      if (pos != std::string::npos) {
          templateDecl.insert(pos + 9, "int, ");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), templateDecl);
    }
}
  
void MutatorFrontendAction_666::MutatorASTConsumer_666::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(nonTypeTemplateParmDecl())).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}