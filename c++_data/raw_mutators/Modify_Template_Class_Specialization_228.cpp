//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Class_Specialization_228
 */ 
class MutatorFrontendAction_228 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(228)

private:
    class MutatorASTConsumer_228 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_228(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Class_Specialization_228.h"

// ========================================================================================================
#define MUT228_OUTPUT 1

void MutatorFrontendAction_228::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      TD->getLocation()))
            return;

        for (auto *Spec : TD->specializations()) {
            if (Spec->isExplicitSpecialization()) {
                std::string specializationText = "template<> class " + TD->getNameAsString() + "<int, void>; /*mut228*/\n";
                SourceLocation insertLoc = Spec->getEndLoc().getLocWithOffset(1);
                Rewrite.InsertText(insertLoc, specializationText, true, true);
                break;
            }
        }
    }
}

void MutatorFrontendAction_228::MutatorASTConsumer_228::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(hasName("basic_ios")).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}