//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Specialization_With_Invalid_Base_152
 */ 
class MutatorFrontendAction_152 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(152)

private:
    class MutatorASTConsumer_152 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_152(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_specialization_with_invalid_base_152.h"

// ========================================================================================================
#define MUT152_OUTPUT 1

void MutatorFrontendAction_152::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedStruct")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;

        for (auto &Base : RD->bases()) {
            if (Base.getType()->isTemplateSpecializationType()) {
                std::string mutatedBase = "UndefinedType";
                auto baseRange = Base.getSourceRange();
                Rewrite.ReplaceText(baseRange, mutatedBase);
            }
        }
    }
}

void MutatorFrontendAction_152::MutatorASTConsumer_152::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isDerivedFrom(cxxRecordDecl(hasAnyTemplateArgument(anything())))).bind("DerivedStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}