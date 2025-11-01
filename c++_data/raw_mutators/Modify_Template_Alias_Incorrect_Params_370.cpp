//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Alias_Incorrect_Params_370
 */ 
class MutatorFrontendAction_370 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(370)

private:
    class MutatorASTConsumer_370 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_370(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_alias_incorrect_params_370.h"

// ========================================================================================================
#define MUT370_OUTPUT 1

void MutatorFrontendAction_370::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("TemplateAlias")) {
      //Filter nodes in header files
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
        return;
      //Get the source code text of target node
      auto aliasDecl = TA->getTemplatedDecl();
      std::string aliasText = stringutils::rangetoStr(*(Result.SourceManager), aliasDecl->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = "template <typename Unused> " + aliasText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(aliasDecl->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_370::MutatorASTConsumer_370::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("TemplateAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}