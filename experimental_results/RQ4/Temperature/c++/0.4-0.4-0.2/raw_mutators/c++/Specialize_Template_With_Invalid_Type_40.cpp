//header file
#pragma once
#include "Mutator_base.h"

/**
 * specialize_template_with_invalid_type_40
 */ 
class MutatorFrontendAction_40 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(40)

private:
    class MutatorASTConsumer_40 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_40(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/specialize_template_with_invalid_type_40.h"

// ========================================================================================================
#define MUT40_OUTPUT 1

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->getTemplatedDecl()->hasBody()) {
            auto funcName = FD->getTemplatedDecl()->getNameAsString();
            std::string mutation = funcName + "<void*>(nullptr);";
            auto funcBody = FD->getTemplatedDecl()->getBody();
            auto bodyStr = stringutils::rangetoStr(*(Result.SourceManager), funcBody->getSourceRange());
            size_t pos = bodyStr.find_last_of('}');
            if (pos != std::string::npos) {
                bodyStr.insert(pos, "\n/*mut40*/" + mutation + "\n");
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(funcBody->getSourceRange()), bodyStr);
            }
        }
    }
}

void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}