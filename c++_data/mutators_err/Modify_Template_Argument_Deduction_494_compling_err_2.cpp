//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Argument_Deduction_494
 */ 
class MutatorFrontendAction_494 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(494)

private:
    class MutatorASTConsumer_494 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_494(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_argument_deduction_494.h"

// ========================================================================================================
#define MUT494_OUTPUT 1

void MutatorFrontendAction_494::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CT->getLocation()))
            return;

        for (auto *spec : CT->specializations()) {
            if (spec->isExplicitSpecialization())
                continue;

            if (auto *ctor = spec->getSpecializedTemplate()->getConstructor()) {
                auto ctorSource = stringutils::rangetoStr(*(Result.SourceManager), ctor->getSourceRange());

                // Perform mutation by changing the constructor parameter type to a pointer type
                std::string mutatedCtor = ctorSource;
                size_t pos = mutatedCtor.find('(');
                if (pos != std::string::npos) {
                    mutatedCtor.insert(pos + 1, "T* ");
                }

                Rewrite.ReplaceText(CharSourceRange::getTokenRange(ctor->getSourceRange()), mutatedCtor);
            }
        }
    }
}

void MutatorFrontendAction_494::MutatorASTConsumer_494::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(has(cxxConstructorDecl())).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}