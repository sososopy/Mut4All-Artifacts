//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_NonType_Param_With_TypeTrait_494
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
#include "../include/Replace_NonType_Param_With_TypeTrait_494.h"

// ========================================================================================================
#define MUT494_OUTPUT 1

void MutatorFrontendAction_494::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DG = Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>("deductionGuide")) {
        if (!DG || !Result.Context->getSourceManager().isWrittenInMainFile(DG->getLocation()))
            return;

        auto constructorDecl = DG->getTargetDeductionGuide();
        if (!constructorDecl || !constructorDecl->isCXXConstructorDecl())
            return;

        auto constructor = cast<clang::CXXConstructorDecl>(constructorDecl);
        if (constructor->getNumParams() < 1)
            return;

        auto param = constructor->getParamDecl(0);
        std::string T_str = param->getType().getAsString();

        std::string sourceCode = stringutils::rangetoStr(*Result.SourceManager, DG->getSourceRange());
        size_t openAngle = sourceCode.find('<');
        size_t closeAngle = sourceCode.find('>');

        if (openAngle != std::string::npos && closeAngle != std::string::npos) {
            std::string args = sourceCode.substr(openAngle + 1, closeAngle - openAngle - 1);
            size_t lastComma = args.rfind(',');
            if (lastComma != std::string::npos) {
                std::string firstPart = args.substr(0, lastComma);
                std::string newLastArg = "alignof(" + T_str + ") + 1";
                std::string newArgs = firstPart + ", " + newLastArg;

                std::string newSourceCode = sourceCode.substr(0, openAngle + 1) + newArgs + sourceCode.substr(closeAngle);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(DG->getSourceRange()), newSourceCode);
            }
        }
    }
}
  
void MutatorFrontendAction_494::MutatorASTConsumer_494::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxDeductionGuideDecl(isConstructor(), isExplicit()).bind("deductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}