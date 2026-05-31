//header file
#pragma once
#include "Mutator_base.h"

/**
 * incorrect_template_argument_deduction_1
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
#include "../include/incorrect_template_argument_deduction_1.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        if (!FD || !FD->isFunctionTemplateSpecialization() || FD->getPrimaryTemplate() == nullptr) 
            return;
        
        if (!Result.Context->getSourceManager().isWrittenInMainFile(FD->getBeginLoc()))
            return;
        
        const auto *TM = FD->getTemplateSpecializationArgs();
        if (!TM || TM->size() == 0)
            return;
        
        std::string newTemplateArgs;
        for (unsigned i = 0; i < TM->size(); ++i) {
            if (i == 0) {
                newTemplateArgs += "double";
            } else {
                newTemplateArgs += ", int";
            }
        }

        auto funcCall = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
        size_t angleBracketPos = funcCall.find('<');
        if (angleBracketPos != std::string::npos) {
            size_t angleBracketClosePos = funcCall.find('>', angleBracketPos);
            if (angleBracketClosePos != std::string::npos) {
                funcCall.replace(angleBracketPos + 1, angleBracketClosePos - angleBracketPos - 1, newTemplateArgs);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcCall);
            }
        }
    }
}

void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isFunctionTemplateSpecialization()).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// The name of the mutation operator implemented