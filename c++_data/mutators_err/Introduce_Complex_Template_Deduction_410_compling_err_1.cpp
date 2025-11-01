//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Complex_Template_Deduction_410
 */ 
class MutatorFrontendAction_410 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(410)

private:
    class MutatorASTConsumer_410 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_410(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/introduce_complex_template_deduction_410.h"

// ========================================================================================================
#define MUT410_OUTPUT 1

void MutatorFrontendAction_410::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TemplateAlias")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      //Get the source code text of target node
      auto aliasText = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string newAliasText = aliasText + "\n/*mut410*/\n";
      newAliasText += "template <typename T>\n";
      newAliasText += "using complexAlias = " + TD->getNameAsString() + "<sizeof(T)>;\n";

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), newAliasText);
    }
}
  
void MutatorFrontendAction_410::MutatorASTConsumer_410::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasDecl(hasTemplateArgument()).bind("TemplateAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}