//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Extra_Template_Less_Than_164
 */ 
class MutatorFrontendAction_164 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(164)

private:
    class MutatorASTConsumer_164 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_164(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Extra_Template_Less_Than_164.h"

// ========================================================================================================
#define MUT164_OUTPUT 1

void MutatorFrontendAction_164::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("templateVar")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        auto *TSI = VD->getTypeSourceInfo();
        if (!TSI)
            return;
        auto TL = TSI->getTypeLoc();
        if (auto *TST = TL.getType()->getAs<clang::TemplateSpecializationType>()) {
            SourceRange typeRange = TL.getSourceRange();
            std::string originalText = stringutils::rangetoStr(*Result.SourceManager, typeRange);
            size_t lessThanPos = originalText.find('<');
            if (lessThanPos != std::string::npos) {
                originalText.insert(lessThanPos + 1, "<");
                Rewrite.ReplaceText(typeRange, originalText);
            }
        }
    }
}
  
void MutatorFrontendAction_164::MutatorASTConsumer_164::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(templateSpecializationType())).bind("templateVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}