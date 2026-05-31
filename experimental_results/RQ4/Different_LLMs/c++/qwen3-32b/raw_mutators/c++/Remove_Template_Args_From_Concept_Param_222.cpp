//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/Type.h"

/**
 * Remove_Template_Args_From_Concept_Param_222
 */ 
class MutatorFrontendAction_222 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(222)

private:
    class MutatorASTConsumer_222 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_222(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Template_Args_From_Concept_Param_222.h"
#include "clang/AST/TypeLoc.h"
#include "clang/Basic/SourceManager.h"
#include "stringutils.h"

// ========================================================================================================
#define MUT222_OUTPUT 1

void MutatorFrontendAction_222::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NTTP = Result.Nodes.getNodeAs<clang::NonTypeTemplateParmDecl>("TemplateParam")) {
        if (!NTTP || !Result.Context->getSourceManager().isWrittenInMainFile(NTTP->getLocation()))
            return;

        QualType T = NTTP->getType();
        if (T.isNull())
            return;

        if (auto *TST = T->getAs<clang::TemplateSpecializationType>()) {
            clang::TypeSourceInfo *TSI = NTTP->getTypeSourceInfo();
            if (!TSI)
                return;

            clang::TypeLoc TL = TSI->getTypeLoc();
            if (TL.getTypePtr()->getAs<clang::TemplateSpecializationType>()) {
                clang::SourceRange typeRange = TL.getSourceRange();
                if (!typeRange.isValid())
                    return;

                std::string typeText = stringutils::rangetoStr(*Result.SourceManager, typeRange);
                size_t pos = typeText.find('<');
                if (pos != std::string::npos) {
                    std::string nameText = typeText.substr(0, pos);
                    Rewrite.ReplaceText(typeRange, nameText);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_222::MutatorASTConsumer_222::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = nonTypeTemplateParmDecl().bind("TemplateParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}