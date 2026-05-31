//header file
#pragma once
#include "Mutator_base.h"

/**
 * LambdaReturnTypeWithOuterScopeDecltype_183
 */ 
class MutatorFrontendAction_183 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(183)

private:
    class MutatorASTConsumer_183 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_183(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::pair<clang::FunctionDecl*, std::vector<clang::VarDecl*>>> FunctionVariables;
    };
};

//source file
#include "../include/LambdaReturnTypeWithOuterScopeDecltype_183.h"

// ========================================================================================================
#define MUT183_OUTPUT 1

void MutatorFrontendAction_183::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(lambda->getBeginLoc()))
            return;

        auto *func = lambda->getFunctionParent();
        if (!func)
            return;

        FunctionVariables.clear();
        ASTContext &Context = *Result.Context;
        for (auto &decl : Context.getTranslationUnitDecl()->decls()) {
            if (auto *fdecl = dyn_cast<clang::FunctionDecl>(decl)) {
                if (fdecl != func && fdecl->isThisDeclarationADefinition()) {
                    std::vector<clang::VarDecl*> vars;
                    for (auto *stmt : fdecl->getBody()->children()) {
                        if (auto *var = dyn_cast<clang::VarDecl>(stmt)) {
                            vars.push_back(var);
                        }
                    }
                    if (!vars.empty()) {
                        FunctionVariables.emplace_back(fdecl, vars);
                    }
                }
            }
        }

        if (FunctionVariables.empty())
            return;

        auto selected = FunctionVariables[0];
        auto var = selected.second[0];
        std::string varName = var->getNameAsString();

        SourceLocation captureStart = lambda->getBeginLoc();
        SourceLocation captureEnd = lambda->getCaptureEndLoc();
        std::string captureText = Rewrite.getRewrittenText(SourceRange(captureStart, captureEnd));
        if (captureText == "[]") {
            Rewrite.ReplaceText(SourceRange(captureStart, captureEnd), "[");
            Rewrite.InsertTextAfter(captureEnd, varName + "]");
        } else {
            Rewrite.InsertTextAfter(captureEnd, ", " + varName + "]");
        }

        SourceLocation bodyStart = lambda->getBody()->getBeginLoc();
        Rewrite.InsertText(bodyStart, " -> decltype(" + varName + ")", true, true);
    }
}
  
void MutatorFrontendAction_183::MutatorASTConsumer_183::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(ignoringImplicit(), unless(isImplicit()))
        .bind("lambda") 
        .within(functionDecl(isVariadic(), isTemplateInstantiation()));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}