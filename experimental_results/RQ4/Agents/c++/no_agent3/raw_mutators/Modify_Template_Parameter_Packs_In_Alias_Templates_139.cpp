//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Parameter_Packs_In_Alias_Templates_139
 */ 
class MutatorFrontendAction_139 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(139)

private:
    class MutatorASTConsumer_139 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_139(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_parameter_packs_in_alias_templates_139.h"

// ========================================================================================================
#define MUT139_OUTPUT 1

void MutatorFrontendAction_139::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AliasTemplate = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasTemplate")) {
      //Filter nodes in header files
      if (!AliasTemplate || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AliasTemplate->getLocation()))
        return;

      //Get the source code text of target node
      auto aliasRange = AliasTemplate->getSourceRange();
      auto aliasText = stringutils::rangetoStr(*(Result.SourceManager), aliasRange);

      //Perform mutation on the source code text by applying string replacement
      std::string mutation = "requires(sizeof...(Items) > 0 && (std::is_integral_v<Items> && ...)) ";
      size_t pos = aliasText.find("using");
      if (pos != std::string::npos) {
          aliasText.insert(pos, mutation);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(aliasRange), aliasText);
    }
}
  
void MutatorFrontendAction_139::MutatorASTConsumer_139::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl(hasTypeAliasDecl(hasTemplateParameterList(
        hasTemplateParameterCount(greaterThan(1)),
        hasTemplateParameter(1, templateTypeParmDecl().bind("Items"))
    ))).bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}