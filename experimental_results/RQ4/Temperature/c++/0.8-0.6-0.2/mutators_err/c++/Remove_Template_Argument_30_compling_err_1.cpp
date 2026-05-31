//header file
#pragma once
#include "Mutator_base.h"

/**
 * remove_template_argument_30
 */ 
class MutatorFrontendAction_30 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(30)

private:
    class MutatorASTConsumer_30 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_30(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT30_OUTPUT 1

void MutatorFrontendAction_30::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TU = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAlias")) {
        if (!TU || !Result.Context->getSourceManager().isWrittenInMainFile(
                TU->getLocation()))
            return;

        if (auto *TS = llvm::dyn_cast<clang::TemplateSpecializationType>(
                TU->getUnderlyingType().getTypePtr())) {
            if (TS->getNumArgs() > 1) {
                std::string original = TS->getTemplateName().getAsTemplateDecl()->getNameAsString();
                std::string mutated;
                for (unsigned i = 0; i < TS->getNumArgs(); ++i) {
                    if (i != 1) { // Remove the second argument as an example
                        if (!mutated.empty()) {
                            mutated += ", ";
                        }
                        mutated += TS->getArg(i).getAsType().getAsString();
                    }
                }
                std::string replacement = original + "<" + mutated + ">";
                Rewrite.ReplaceText(TU->getSourceRange(), replacement);
            }
        }
    }
}
  
void MutatorFrontendAction_30::MutatorASTConsumer_30::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasDecl(
        hasType(templateSpecializationType(hasAnyTemplateArgument(anything())))
    ).bind("TypeAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}