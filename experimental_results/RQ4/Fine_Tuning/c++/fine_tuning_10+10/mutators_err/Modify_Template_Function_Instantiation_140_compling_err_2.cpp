//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Function_Instantiation_140
 */ 
class MutatorFrontendAction_140 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(140)

private:
    class MutatorASTConsumer_140 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_140(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_function_instantiation_140.h"

// ========================================================================================================
#define MUT140_OUTPUT 1

void MutatorFrontendAction_140::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("templateCall")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(CallExpr->getBeginLoc()))
            return;

        const TemplateArgumentList *TemplateArgs = CallExpr->getDirectCallee()->getTemplateSpecializationArgs();
        if (!TemplateArgs)
            return;

        std::string modifiedCall = Lexer::getSourceText(CharSourceRange::getTokenRange(CallExpr->getSourceRange()), 
                                                        *Result.SourceManager, 
                                                        Result.Context->getLangOpts()).str();

        size_t angleBracketPos = modifiedCall.find_last_of('>');
        if (angleBracketPos != std::string::npos) {
            modifiedCall.insert(angleBracketPos, ", int");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(CallExpr->getSourceRange()), modifiedCall);
        }
    }
}

void MutatorFrontendAction_140::MutatorASTConsumer_140::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasAnyTemplateArgument()).bind("templateCall")));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}