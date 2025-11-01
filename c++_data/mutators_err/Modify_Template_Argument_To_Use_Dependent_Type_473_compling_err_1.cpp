//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Argument_To_Use_Dependent_Type_473
 */ 
class MutatorFrontendAction_473 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(473)

private:
    class MutatorASTConsumer_473 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_473(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_473.h"

// ========================================================================================================
#define MUT473_OUTPUT 1

void MutatorFrontendAction_473::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
        return;

      const TemplateArgumentList *TemplateArgs = TA->getTemplateArgs();
      if (!TemplateArgs || TemplateArgs->size() != 1)
        return;

      const TemplateArgument &Arg = TemplateArgs->get(0);
      if (Arg.getKind() != TemplateArgument::Type)
        return;

      const Type *ArgType = Arg.getAsType().getTypePtr();
      if (!ArgType || !ArgType->isDependentType())
        return;

      std::string OldAlias = stringutils::rangetoStr(*(Result.SourceManager),
                                                     TA->getSourceRange());

      std::string NewAlias = "MyStruct<Wrapper<T>::x>";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TA->getSourceRange()), NewAlias);
    }
}
  
void MutatorFrontendAction_473::MutatorASTConsumer_473::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasDecl(hasTemplateArgument(0, templateArgument().bind("Alias"))).bind("Alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}