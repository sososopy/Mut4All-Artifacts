//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Requires_In_Lambda_Exception_Spec_121
 */ 
class MutatorFrontendAction_121 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(121)

private:
    class MutatorASTConsumer_121 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_121(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Nested_Requires_In_Lambda_Exception_Spec_121.h"

// ========================================================================================================
#define MUT121_OUTPUT 1

void MutatorFrontendAction_121::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method")) {
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(MD->getLocation()))
            return;

        TemplateParameterList *TPL = MD->getTemplateParameterList();
        if (!TPL || TPL->size() == 0)
            return;

        NamedDecl *FirstParam = dyn_cast<NamedDecl>(TPL->getParam(0));
        if (!FirstParam)
            return;
        std::string paramName = FirstParam->getNameAsString();

        if (!MD->isNoexcept())
            return;

        SourceRange exceptRange = MD->getExceptionSpecRange();
        if (exceptRange.isInvalid())
            return;

        std::string newNoexcept = "noexcept( requires { requires requires { typename " + paramName + "::nested_type; }; })";
        Rewrite.ReplaceText(exceptRange, newNoexcept);
    }
}

void MutatorFrontendAction_121::MutatorASTConsumer_121::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
        ofClass(isLambda()),
        hasName("operator()"),
        hasTemplateParameterList(),
        isNoexcept()
    ).bind("method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}