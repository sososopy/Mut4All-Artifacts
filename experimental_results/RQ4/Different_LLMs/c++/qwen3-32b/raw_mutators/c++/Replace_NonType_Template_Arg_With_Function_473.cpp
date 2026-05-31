//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_NonType_Template_Arg_With_Function_473
 */ 
class MutatorFrontendAction_473 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(473)

private:
    class MutatorASTConsumer_473 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_473(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_NonType_Template_Arg_With_Function_473.h"

// ========================================================================================================
#define MUT473_OUTPUT 1

void MutatorFrontendAction_473::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *alias = Result.Nodes.getNodeAs<clang::TemplateAliasDecl>("alias")) {
        if (!alias || !Result.Context->getSourceManager().isWrittenInMainFile(alias->getLocation()))
            return;

        auto *templateDecl = alias->getTemplatedDecl();
        if (auto *tparams = templateDecl->getTemplateParameters()) {
            auto *tparam = tparams->getParam(0);
            std::string tParamName = tparam->getNameAsString();

            TemplateArgumentListInfo *argList = alias->getTemplateArgs();
            for (unsigned i = 0; i < argList->size(); ++i) {
                TemplateArgument arg = argList->getArg(i);
                if (arg.getKind() == TemplateArgument::Kind::Expression) {
                    auto *expr = arg.getAsExpr();
                    auto range = expr->getSourceRange();
                    std::string replacement = "f(" + tParamName + "::x)";
                    Rewrite.ReplaceText(range, replacement);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_473::MutatorASTConsumer_473::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateAliasDecl(
        hasDeclaration(
            templateDecl(
                hasTemplateParameters(
                    hasParameter(0, hasType(autoType()))
                )
            )
        )
    ).bind("alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}