//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateInstantiationConstantToType_375
 */ 
class MutatorFrontendAction_TemplateInstantiationConstantToType_375 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(TemplateInstantiationConstantToType_375)

private:
    class MutatorASTConsumer_TemplateInstantiationConstantToType_375 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_TemplateInstantiationConstantToType_375(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_TemplateInstantiationConstantToType_375.h"

// ========================================================================================================
#define MUTTEMPLATEINSTANTIATIONCONSTANTTOTYPE_375_OUTPUT 1

void MutatorFrontendAction_TemplateInstantiationConstantToType_375::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateArgument>("TemplateArgument")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getAsExpr()->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getAsExpr()->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      if (MT->getKind() == TemplateArgument::ArgKind::Integral) {
        // Replace constant with a type
        declaration = "int";
      } else if (MT->getKind() == TemplateArgument::ArgKind::Type) {
        // Replace type with a constant
        declaration = "5";
      }
      Rewrite.ReplaceText(MT->getAsExpr()->getSourceRange(), declaration);
    }
}
  
void MutatorFrontendAction_TemplateInstantiationConstantToType_375::MutatorASTConsumer_TemplateInstantiationConstantToType_375::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = templateArgument().bind("TemplateArgument");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}