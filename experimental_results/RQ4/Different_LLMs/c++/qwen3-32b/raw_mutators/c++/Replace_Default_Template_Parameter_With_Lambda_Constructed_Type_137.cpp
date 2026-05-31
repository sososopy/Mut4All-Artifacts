//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Template_Parameter_With_Lambda_Constructed_Type_137
 */ 
class MutatorFrontendAction_137 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(137)

private:
    class MutatorASTConsumer_137 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_137(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Default_Template_Parameter_With_Lambda_Constructed_Type_137.h"

// ========================================================================================================
#define MUT137_OUTPUT 1

void MutatorFrontendAction_137::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TTP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("tparam")) {
        if (!TTP || !Result.Context->getSourceManager().isWrittenInMainFile(TTP->getLocation()))
            return;

        clang::DeclContext *DC = TTP->getDeclContext();
        if (auto *TD = dyn_cast<clang::TemplateDecl>(DC)) {
            clang::TemplateParameterList *TPL = TD->getTemplateParameters();
            if (!TPL)
                return;

            clang::TemplateTypeParmDecl *FirstTTP = nullptr;
            for (const auto *Param : TPL->asWritten()->getTemplateParameters()) {
                if (auto *TTPD = dyn_cast<clang::TemplateTypeParmDecl>(Param)) {
                    FirstTTP = TTPD;
                    break;
                }
            }

            if (!FirstTTP)
                return;

            std::string firstTypeName = FirstTTP->getNameAsString();
            std::string lambdaType = "decltype([]{ return " + firstTypeName + "(); })";
            std::string newParam = lambdaType + " " + TTP->getNameAsString() + " = " + lambdaType;

            SourceRange range = TTP->getSourceRange();
            Rewrite.ReplaceText(range, newParam);
        }
    }
}

void MutatorFrontendAction_137::MutatorASTConsumer_137::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateTypeParmDecl(hasDefaultArgument()).bind("tparam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}