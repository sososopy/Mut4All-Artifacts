//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_specialization_confusion_513
 */ 
class MutatorFrontendAction_513 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(513)

private:
    class MutatorASTConsumer_513 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_513(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_specialization_confusion_513.h"

// ========================================================================================================
#define MUT513_OUTPUT 1

void MutatorFrontendAction_513::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunc")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());

        size_t pos = declaration.find("std::enable_if_t<");
        if (pos != std::string::npos) {
            pos = declaration.find(",", pos);
            if (pos != std::string::npos) {
                std::string nestedCalc = "template <typename X> struct NestedCalc { static const bool value = sizeof(X) > 0; };";
                std::string newCondition = "std::enable_if_t<(Condition && NestedCalc<U>::value), int>";

                declaration.insert(0, nestedCalc + "\n");
                declaration.replace(pos + 1, std::string::npos, newCondition);
            }
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_513::MutatorASTConsumer_513::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(
        hasTemplateParameterList(
            has(
                templateTypeParmDecl().bind("TemplateFunc")
            )
        )
    ).bind("TemplateFunc");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}