//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Concept_Satisfaction_In_Template_432
 */ 
class MutatorFrontendAction_432 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(432)

private:
    class MutatorASTConsumer_432 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_432(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_concept_satisfaction_in_template_432.h"

// ========================================================================================================
#define MUT432_OUTPUT 1

void MutatorFrontendAction_432::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;

        for (auto *Ctor : CT->ctors()) {
            if (Ctor->getBody()) {
                auto ctorSource = stringutils::rangetoStr(*(Result.SourceManager), Ctor->getSourceRange());
                std::string staticAssert = "\nstatic_assert(sizeof(T) > 4, \"T must be larger than 4 bytes\");\n";
                size_t insertPos = ctorSource.find("{") + 1;
                ctorSource.insert(insertPos, staticAssert);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(Ctor->getSourceRange()), ctorSource);
            }
        }
    }
}
  
void MutatorFrontendAction_432::MutatorASTConsumer_432::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isTemplateInstantiation()).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}