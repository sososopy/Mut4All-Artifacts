//header file
#pragma once
#include "Mutator_base.h"

/**
 * replace_defaulted_comparison_operator_685
 */ 
class MutatorFrontendAction_685 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(685)

private:
    class MutatorASTConsumer_685 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_685(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_defaulted_comparison_operator_685.h"

// ========================================================================================================
#define MUT685_OUTPUT 1

void MutatorFrontendAction_685::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("comparisonOp")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      if (MT->getOverloadedOperator() == OO_EqualEqual || MT->getOverloadedOperator() == OO_ExclaimEqual || MT->getOverloadedOperator() == OO_Spaceship) {
        std::string newOp = "operator+";
        std::string newBody = "{ return *new " + MT->getParent()->getNameAsString() + "(); }";
        size_t pos = declaration.find("operator");
        if (pos != std::string::npos) {
          declaration.replace(pos, 10, newOp);
          pos = declaration.find("= default");
          if (pos != std::string::npos) {
            declaration.replace(pos, 9, newBody);
          }
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_685::MutatorASTConsumer_685::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isDefaulted(), isOverloadedOperator(), anyOf(hasOverloadedOperatorName("=="), hasOverloadedOperatorName("!="), hasOverloadedOperatorName("<=>"))).bind("comparisonOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}