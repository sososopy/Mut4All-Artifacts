//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_NonDependent_Default_Arg_341
 */ 
class MutatorFrontendAction_341 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(341)

private:
    class MutatorASTConsumer_341 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_341(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_NonDependent_Default_Arg_341.h"

// ========================================================================================================
#define MUT341_OUTPUT 1

void MutatorFrontendAction_341::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *method = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method")) {
        if (!method || !Result.Context->getSourceManager().isWrittenInMainFile(method->getLocation()))
            return;

        const CXXRecordDecl *classDecl = method->getParent();
        if (!classDecl || !classDecl->isTemplated())
            return;

        const TemplateParameterList *tparams = classDecl->getTemplateParameterList();
        if (!tparams || tparams->size() == 0)
            return;
        const TemplateTypeParmDecl *tParam = tparams->getParam(0);
        std::string tName = tParam->getNameAsString();

        for (const ParmVarDecl *param : method->parameters()) {
            if (param->hasDefaultArg()) {
                SourceRange range = param->getDefaultArgRange();
                if (range.isInvalid())
                    continue;

                std::string replacement = "SomeFunction<" + tName + ">()";
                Rewrite.ReplaceText(range, replacement);
            }
        }
    }
}
  
void MutatorFrontendAction_341::MutatorASTConsumer_341::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
        isTemplated(),
        ofClass(cxxRecordDecl(isTemplated())),
        hasAnyParameter(hasDefaultArg())
    ).bind("method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}