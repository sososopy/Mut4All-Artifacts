//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Parameter_Mismatch_In_Partial_Specialization_376
 */ 
class MutatorFrontendAction_376 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(376)

private:
    class MutatorASTConsumer_376 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_376(Rewriter &R) : TheRewriter(R) {}
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
        const clang::ClassTemplateSpecializationDecl *Specialization = nullptr;
    };
};

//source file
#include "../include/Template_Parameter_Mismatch_In_Partial_Specialization_376.h"

// ========================================================================================================
#define MUT376_OUTPUT 1

void MutatorFrontendAction_376::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *PS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("PartialSpec")) {
        if (!PS || !Result.SourceManager->isWrittenInMainFile(PS->getLocation()))
            return;

        Specialization = PS;
        auto specializationText = Lexer::getSourceText(CharSourceRange::getTokenRange(PS->getSourceRange()), 
                                                       *Result.SourceManager, 
                                                       Result.Context->getLangOpts());
        std::string mutatedText = specializationText.str();
        std::string baseClass = "Base<Derived<int>>";
        size_t pos = mutatedText.find(":");
        if (pos != std::string::npos) {
            mutatedText.replace(pos, std::string::npos, ": " + baseClass + " {}");
        }
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(PS->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_376::MutatorASTConsumer_376::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl(
        isExplicitSpecialization()).bind("PartialSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}