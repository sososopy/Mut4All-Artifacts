//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Parameters_In_Specialization_1
 */ 
class MutatorFrontendAction_1 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(1)

private:
    class MutatorASTConsumer_1 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_1(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_parameters_in_specialization_1.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateSpecialization")) {
        if (const auto *TS = FD->getTemplateSpecializationInfo()) {
            if (!Result.Context->getSourceManager().isWrittenInMainFile(
                    FD->getLocation()))
                return;

            SourceLocation StartLoc = TS->getTemplateArgsAsWritten()->getLAngleLoc();
            SourceLocation EndLoc = TS->getTemplateArgsAsWritten()->getRAngleLoc();

            CharSourceRange Range(SourceRange(StartLoc, EndLoc), false);
            std::string NewTemplateArgs = "<char, double>"; // Modify template parameters to introduce inconsistency

            Rewrite.ReplaceText(Range, NewTemplateArgs);
        }
    }
}
  
void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl(has(functionTemplateSpecializationInfo())).bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}