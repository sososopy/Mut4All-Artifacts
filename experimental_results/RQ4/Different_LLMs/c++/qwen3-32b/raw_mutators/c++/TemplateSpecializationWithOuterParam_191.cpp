//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateSpecializationWithOuterParam_191
 */ 
class MutatorFrontendAction_191 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(191)

private:
    class MutatorASTConsumer_191 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_191(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/TemplateSpecializationWithOuterParam_191.h"

// ========================================================================================================
#define MUT191_OUTPUT 1

void MutatorFrontendAction_191::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("templateDecl")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;

        TemplateParameterList *TPL = CT->getTemplateParameters();
        if (!TPL || TPL->size() < 1)
            return;

        TemplateTypeParmDecl *firstParam = dyn_cast<TemplateTypeParmDecl>(TPL->getParam(0));
        if (!firstParam)
            return;

        std::string paramName = firstParam->getNameAsString();
        std::string structName = CT->getNameAsString();

        std::string specializationCode = "template<>\nstruct " + structName + "<int> : Bar<" + paramName + "> {\n};\n";

        SourceLocation insertionLoc = CT->getEndLoc();
        Rewrite.InsertTextAfterToken(insertionLoc, specializationCode);
    }
}
  
void MutatorFrontendAction_191::MutatorASTConsumer_191::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(has(cxxRecordDecl())).bind("templateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}