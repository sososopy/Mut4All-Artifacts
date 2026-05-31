//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_instantiation_with_mismatched_arguments_601
 */ 
class MutatorFrontendAction_601 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(601)

private:
    class MutatorASTConsumer_601 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_601(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_601.h"

// ========================================================================================================
#define MUT601_OUTPUT 1

void MutatorFrontendAction_601::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("templateInstance")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        const TemplateArgumentList &args = MT->getTemplateArgs();
        if (args.size() == 1) {
            std::string newInstantiation = "B<int, double>";
            Rewrite.ReplaceText(MT->getSourceRange(), newInstantiation);
        }
    }
}
  
void MutatorFrontendAction_601::MutatorASTConsumer_601::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl(hasName("B")).bind("templateInstance");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}