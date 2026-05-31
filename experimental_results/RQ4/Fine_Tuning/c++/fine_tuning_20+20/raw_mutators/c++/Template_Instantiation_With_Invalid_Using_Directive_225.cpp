//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Instantiation_With_Invalid_Using_Directive_225
 */ 
class MutatorFrontendAction_225 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(225)

private:
    class MutatorASTConsumer_225 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_225(Rewriter &R) : TheRewriter(R) {}
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
        const clang::TypeAliasDecl *cur_template = nullptr;
    };
};

//source file
#include "../include/template_instantiation_with_invalid_using_directive_225.h"

// ========================================================================================================
#define MUT225_OUTPUT 1

void MutatorFrontendAction_225::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->getBeginLoc() == MT->getEndLoc())
        return;
      if (MT->getTypeAsWritten()->isInstantiationDependentType() == false)
        return;
      if (MT->getTypeAsWritten()->isDependentType() == false)
        return;
      cur_template = MT;
    } else if (auto *UT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>(
                   "AliasInst")) {
      if (!UT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UT->getLocation()))
        return;
      if (UT->getBeginLoc() == UT->getEndLoc())
        return;
      if (cur_template == nullptr)
        return;
      if (UT->getNameAsString() != cur_template->getNameAsString())
        return;
      auto alias_content = stringutils::rangetoStr(
          *(Result.SourceManager), UT->getSourceRange());
      llvm::outs() << alias_content << '\n';
      auto pos = alias_content.find('=');
      if (pos == string::npos)
        return;
      alias_content.replace(pos + 1, alias_content.size() - pos - 1,
                            " /*mut225*/UnknownType");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(UT->getSourceRange()),
                          alias_content);
    }
}
  
void MutatorFrontendAction_225::MutatorASTConsumer_225::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = typeAliasDecl().bind("Alias");
    auto matcher_inst =
        typeAliasDecl(hasType(qualType(unless(elaboratedType()))))
            .bind("AliasInst");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(matcher_inst, &callback);
    matchFinder.matchAST(Context);
}