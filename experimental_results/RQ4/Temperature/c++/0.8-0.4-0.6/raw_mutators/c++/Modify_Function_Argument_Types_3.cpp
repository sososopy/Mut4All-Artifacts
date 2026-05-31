//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_function_argument_types_3
 */ 
class MutatorFrontendAction_3 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(3)

private:
    class MutatorASTConsumer_3 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_3(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_function_argument_types_3.h"

// ========================================================================================================
#define MUT3_OUTPUT 1

void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("makeTupleCall")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getExprLoc()))
            return;

        auto *DRE = dyn_cast<DeclRefExpr>(MT->getCallee()->IgnoreImplicit());
        if (!DRE || DRE->getNameInfo().getAsString() != "make_tuple")
            return;

        auto *TSI = MT->getDirectCallee()->getTemplateSpecializationArgs();
        if (!TSI || TSI->size() < 2)
            return;

        const TemplateArgument &arg1 = TSI->get(0);
        const TemplateArgument &arg2 = TSI->get(1);

        if (arg1.getKind() == TemplateArgument::Type && arg2.getKind() == TemplateArgument::Type) {
            std::string newType1 = "std::string";
            std::string newType2 = "int";

            std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
            std::string mutatedText = originalText;
            mutatedText.replace(mutatedText.find(arg1.getAsType().getAsString()), arg1.getAsType().getAsString().length(), newType1);
            mutatedText.replace(mutatedText.find(arg2.getAsType().getAsString()), arg2.getAsType().getAsString().length(), newType2);

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
        }
    }
}

void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasName("std::make_tuple")))).bind("makeTupleCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}