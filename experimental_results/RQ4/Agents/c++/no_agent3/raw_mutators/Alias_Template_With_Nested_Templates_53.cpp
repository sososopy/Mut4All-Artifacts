//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alias_Template_With_Nested_Templates_53
 */ 
class MutatorFrontendAction_53 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(53)

private:
    class MutatorASTConsumer_53 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_53(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/alias_template_with_nested_templates_53.h"

// ========================================================================================================
#define MUT53_OUTPUT 1

void MutatorFrontendAction_53::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AT = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasTemplate")) {
      if (!AT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AT->getLocation()))
        return;
      auto aliasDecl = AT->getTemplatedDecl();
      if (!aliasDecl)
        return;
      auto underlyingType = aliasDecl->getUnderlyingType().getAsString();
      auto aliasName = aliasDecl->getNameAsString();
      auto templateParams = AT->getTemplateParameters()->getAsString();
      
      std::string wrapperStruct = "template" + templateParams + " struct Wrapper_" + aliasName + " { using type = " + underlyingType + "; };";
      std::string newAlias = "template" + templateParams + " using " + aliasName + " = Wrapper_" + aliasName + "<T>::type;";
      
      Rewrite.ReplaceText(AT->getSourceRange(), "/*mut53*/" + wrapperStruct + "\n" + newAlias);
    }
}
  
void MutatorFrontendAction_53::MutatorASTConsumer_53::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}