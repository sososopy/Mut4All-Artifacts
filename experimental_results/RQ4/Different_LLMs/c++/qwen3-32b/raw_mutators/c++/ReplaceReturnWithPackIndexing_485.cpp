//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceReturnWithPackIndexing_485
 */ 
class MutatorFrontendAction_485 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(485)

private:
    class MutatorASTConsumer_485 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_485(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/ReplaceReturnWithPackIndexing_485.h"

// ========================================================================================================
#define MUT485_OUTPUT 1

void MutatorFrontendAction_485::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("functionTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;

        const TemplateParameterList *TPL = FTD->getTemplateParameters();
        if (!TPL || TPL->size() < 2)
            return;

        const NonTypeTemplateParmDecl *NTTP = dyn_cast<NonTypeTemplateParmDecl>(TPL->getParam(0));
        if (!NTTP)
            return;

        const TemplateTypeParmDecl *VariadicParam = dyn_cast<TemplateTypeParmDecl>(TPL->getParam(1));
        if (!VariadicParam || !VariadicParam->isParameterPack())
            return;

        const FunctionDecl *FD = FTD->getTemplatedDecl();
        if (!FD)
            return;

        QualType ReturnType = FD->getReturnType();
        if (!ReturnType.getTypePtr()->isTemplateTypeParmType())
            return;

        const TemplateTypeParmType *TTPType = ReturnType.getTypePtr()->getAs<TemplateTypeParmType>();
        if (!TTPType)
            return;

        const TemplateTypeParmDecl *TTPDecl = TTPType->getDecl();
        if (!TTPDecl || TTPDecl != VariadicParam)
            return;

        SourceLocation ReturnTypeBegin = FD->getTypeSourceInfo()->getTypeLoc().getBeginLoc();
        SourceLocation ReturnTypeEnd = FD->getTypeSourceInfo()->getTypeLoc().getEndLoc();

        std::string PackName = VariadicParam->getNameAsString();
        std::string IndexParamName = NTTP->getNameAsString();

        std::string ReplacementText = PackName + "...[" + IndexParamName + "]&";

        Rewrite.ReplaceText(SourceRange(ReturnTypeBegin, ReturnTypeEnd), ReplacementText);
    }
}
  
void MutatorFrontendAction_485::MutatorASTConsumer_485::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("functionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}