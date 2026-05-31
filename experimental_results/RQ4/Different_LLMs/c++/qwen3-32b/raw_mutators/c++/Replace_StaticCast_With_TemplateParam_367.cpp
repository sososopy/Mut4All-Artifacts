//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_StaticCast_With_TemplateParam_367
 */ 
class MutatorFrontendAction_367 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(367)

private:
    class MutatorASTConsumer_367 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_367(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_StaticCast_With_TemplateParam_367.h"

// ========================================================================================================
#define MUT367_OUTPUT 1

void MutatorFrontendAction_367::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ctorInit = Result.Nodes.getNodeAs<clang::CXXCtorInitializer>("ctorInit")) {
        if (!Result.Context->getSourceManager().isWrittenInMainFile(ctorInit->getSourceLocation()))
            return;

        auto *ctorDecl = ctorInit->getConstructor();
        auto *recordDecl = ctorDecl->getParent();

        if (recordDecl->getTemplateParameters()->size() == 0)
            return;

        auto *castExpr = ctorInit->getInit()->IgnoreImplicit()->getAs<clang::CXXStaticCastExpr>();
        if (!castExpr)
            return;

        auto typeLoc = castExpr->getTypeInfoAsWritten()->getTypeLoc();
        if (!typeLoc)
            return;

        auto typeRange = typeLoc.getSourceRange();
        std::string originalType = stringutils::getSourceText(*Result.SourceManager, typeRange, Result.Context->getLangOpts());

        auto *templateParam = recordDecl->getTemplateParameters()->getParam(0);
        if (auto *typeParam = dyn_cast<clang::TypeTemplateParmDecl>(templateParam)) {
            std::string paramName = typeParam->getNameAsString();
            std::string newType = paramName + "*";
            Rewrite.ReplaceText(typeRange, newType);
        }
    }
}
  
void MutatorFrontendAction_367::MutatorASTConsumer_367::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(
        hasAnyCXXCtorInitializer(
            cxxCtorInitializer(
                hasInitializer(
                    ignoringImplicit(
                        cxxStaticCastExpr(
                            hasType(
                                unless(
                                    templateTypeParmType()
                                )
                            )
                        )
                    )
                )
            )
        )
    ).bind("ctorInit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}