//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_specialization_type_deduction_40
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
#include "../include/template_specialization_type_deduction_40.h"

// ========================================================================================================
#define MUT40_OUTPUT 1

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionTemplate")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getTemplatedKind() == FunctionDecl::TK_FunctionTemplate) {
            auto *TemplateParams = FD->getDescribedFunctionTemplate()->getTemplateParameters();
            if (TemplateParams->size() == 1) {
                if (auto *ParamType = dyn_cast<TemplateTypeParmDecl>(TemplateParams->getParam(0))) {
                    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
                        auto ArgType = CallExpr->getArg(0)->getType();
                        if (ArgType.isConstQualified()) {
                            std::string newType = "Nested<const " + ArgType.getAsString() + ">";
                            std::string newDecl = "template<class T> struct Nested {};\n";
                            newDecl += "template<class T> struct Container: public Nested<const T> {};\n";
                            newDecl += FD->getReturnType().getAsString() + " " + FD->getNameAsString() + "(Container<const T> c);";
                            Rewrite.InsertTextBefore(FD->getBeginLoc(), "/*mut40*/" + newDecl + "\n");
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionTemplateMatcher = functionDecl(hasDescendant(callExpr(hasArgument(0, hasType(isConstQualified()))))).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionTemplateMatcher, &callback);
    matchFinder.matchAST(Context);
}