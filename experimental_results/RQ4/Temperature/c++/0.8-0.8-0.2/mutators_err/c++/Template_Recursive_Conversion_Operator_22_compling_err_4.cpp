//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_recursive_conversion_operator_22
 */ 
class MutatorFrontendAction_22 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(22)

private:
    class MutatorASTConsumer_22 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_22(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_recursive_conversion_operator_22.h"

// ========================================================================================================
#define MUT22_OUTPUT 1

void MutatorFrontendAction_22::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ConvOp = Result.Nodes.getNodeAs<clang::CXXConversionDecl>("convOp")) {
        if (!ConvOp || !Result.Context->getSourceManager().isWrittenInMainFile(
                ConvOp->getLocation()))
            return;

        if (ConvOp->isUserProvided() && ConvOp->getParent()->getTemplateSpecializationKind() != clang::TSK_Undeclared) {
            auto Body = ConvOp->getBody();
            if (!Body) return;

            std::string NewBody = "{ Wrapper<" + ConvOp->getParent()->getNameAsString() + "> temp = *this; return temp; }";
            
            Rewrite.ReplaceText(ConvOp->getBody()->getSourceRange(), NewBody);
        }
    }
}

void MutatorFrontendAction_22::MutatorASTConsumer_22::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConversionDecl(isUserProvided(), hasParent(cxxRecordDecl(has(clang::TemplateSpecializationKind(anyOf(clang::TSK_ImplicitInstantiation, clang::TSK_ExplicitInstantiation, clang::TSK_ExplicitSpecialization)))).bind("convOp")));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}